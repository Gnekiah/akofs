/*
 * AkoFS: Peer-to-Peer Distributed File System.
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: Xxiong <xxiong@cqu.edu.cn>
 */

#include <iostream>
#include <string>
#include <ako_compat.h>
#include <ako_setproctitle.h>
#include <ako_config.h>
#include <ako_logger.h>
#include <ako_getopt.h>
#include <ako_errno.h>
#include <ako_daemon.h>
#include <ako_slab.h>
#include <ako_eventd.h>
#include <ako_core.h>

#define AKO_VERSION_MAJOR       0
#define AKO_VERSION_MINOR       0
#define AKO_VERSION_REVISION    1

/*
 * Config cell defination
 * each configuration segment has section, name and value
 */
struct ako_config_item_t {
    const char* section;
    const char* name;
    const char* value;
};

/*
 * Default configuration
 * After parsing options from console, default config_map will be 
 * generated by the follows
 */
static struct ako_config_item_t ako_config_string_default[] = {
    /* Config Section */
    {"Config", "config-path", ""},
    /* Logger Section */
    {"Logger", "config-path", "sparkle-logger.conf"},
    /* DAS Section */
    {"Network", "backlog", "2048"},
    {"Network", "local", "0.0.0.0:20530"},
    {"Network", "pilot", "127.0.0.1:20530"},





    /* End of Config String */
    NULL
};

/* 
 * Long options defination
 */
static struct ako_optparse_long ako_long_opts_def[] = {
    /* Options for replacing configuration specificated or defaulted */
    {"server-port",             'p', AKO_OPTPARSE_REQUIRED},
    {"configuration-path",      'c', AKO_OPTPARSE_REQUIRED},
    /* Run target and return right now */
    {"check-configuration",     'C', AKO_OPTPARSE_REQUIRED},
    {"help",                    'h', AKO_OPTPARSE_NONE},
    {"usage",                   'h', AKO_OPTPARSE_NONE},
    {"version",                 'v', AKO_OPTPARSE_NONE},
    {NULL,                      NULL, AKO_OPTPARSE_NONE}
};

/*
 * Default options
 */
static struct ako_opts_default_t {
    int32_t server_port;
    char*  configuration_path;
} ako_opts_default = { 0 };

/*
 * Print current version
 */
static void ako_version() {
    std::cerr
        << "sparkle version " 
        << AKO_VERSION_MAJOR << "." 
        << AKO_VERSION_MINOR << "."
        << AKO_VERSION_REVISION << std::endl
        << "Copyright (C) 2019 CQU STARLab. All rights reserved." << std::endl;
}

/*
 * Full usage information
 * called on `-h` or `--help` options
 */
static void ako_usage() {
    std::cerr 
        << "usage: sparkle [--version][--help][--usage]" << std::endl
        << "               [--check-configuration=<path>]" << std::endl
        << "               [--configuration-path=<path>][--server-port=<port>]" << std::endl
        << "Options:" << std::endl
        << "  -p PORT, --server-port=PORT" << std::endl
        << "                            Port for listening and feeding back." << std::endl
        << "  -c PATH, --configuration-path=PATH" << std::endl
        << "                            Assign configuration files to init server." << std::endl
        << "  -C PATH, --check-configuration=PATH" << std::endl
        << "                            Check configuration files, alert on syntax error." << std::endl
        << "  -h, --help, --usage       Print this message and exit." << std::endl
        << "  -v, --version             Print the version number of make and exit." << std::endl
        << std::endl
        << "This program built for x86_64-linux-gnu, armv7-linux-gnu and x86_64-Windows-7+" << std::endl
        << "Report bugs to <bug@cqucs.ml>" << std::endl;
}

/* 
 * Brief usage information
 * called while `parse_opts()` error
 */
static void ako_usage_brief() {
    std::cerr
        << "usage: sparkle [--version][--help][--usage]" << std::endl
        << "               [--check-configuration=<path>]" << std::endl
        << "               [--configuration-path=<path>][--server-port=<port>]" << std::endl
        << std::endl
        << "`sparkle --usage` for more detail information." << std::endl;
}

/*
 * Alloc memory and generate default configuration map from default 
 * `ako_config_string_default`
 */
static std::map<std::string, std::string>* ako_config_map_init() {
    std::map<std::string, std::string> *config_map = 
        new std::map<std::string, std::string>();

    if (!config_map)
        return nullptr;

    struct ako_config_item_t* item = ako_config_string_default;
    while (item) {
        config_map->insert(std::pair<std::string, std::string>
            (std::string(item->section) + "=" + item->name, item->value));
        item++;
    }

    return config_map;
}

/*
 * Parse options and parameters from console
 */
static int ako_parse_opts(const int argc, const char **argv) {
    int ret = 0;
    char** long_argv = NULL;
    int option, longindex;
    struct ako_optparse options;

    memset(&ako_opts_default, 0, sizeof(ako_opts_default));

    long_argv = (char**)malloc(sizeof(char*) * (argc + 1));
    if (!long_argv)
        return -AKOE_NOMEM;

    for (int i = 0; i < argc; i++) {
        long_argv[i] = (char*)malloc(sizeof(char) * strlen(argv[i]) + 1);
        strcpy(long_argv[i], argv[i]);
    }
    long_argv[argc] = NULL;

    ako_optparse_init(&options, long_argv);
    while ((option = ako_optparse_long(&options, ako_long_opts_def, &longindex)) != -1) {
        switch (option) {
        case 'p':
            ako_opts_default.server_port = atoi(options.optarg);
            break;
        case 'c':
            ako_opts_default.configuration_path = options.optarg;
            break;
        case 'C':
            // TODO: Add configuration check, load configuration file and do syntax check
            //       Include sparkle.conf and logger.conf
            exit(0);
        case 'h':
            ako_usage();
            exit(0);
        case 'v':
            ako_version();
            exit(0);
        case '?':
            fprintf(stderr, "%s: %s\n", argv[0], options.errmsg);
            ako_usage_brief();
            exit(-1);
        }
    }

    char* arg = ako_optparse_arg(&options);
    if (arg) {
        fprintf(stderr, "%s: Unknown arguments!\n", argv[0]);
        do {
            printf("    %s\n", arg);
        } while (arg = ako_optparse_arg(&options));

        ako_usage_brief();
        exit(-1);
    }

    free(long_argv);
    return 0;
}

/*
 * Enable options and parameters from console
 */
static int ako_enable_opts() {
    int ret = 0;


    // TODO

    return ret;
}

/*
 * Macro for getting error position inner throwed file
 */
#define STDCERR_POSITION_IN_SRC_BEFORE_LOGGER   \
"FATAL ERROR: in file '" << __FILE__ << "', " \
<< "at line No " << __LINE__ << ", " \
<< "function '" << __FUNCTION__ << "', "


int main(int argc, const char **argv) 
{
    const char* ako_proc_name = "sparkle";
    const char* ako_main_thread_name = "sparkle-main";
    const char* ako_config_path = "sparkle.conf";

    int err = 0;
    std::map<std::string, std::string>* default_config_map = nullptr;
    struct ako_slab_chain *slab_chain = nullptr;
    const int slab_chain_size = 1024 * 64;  /* should specified by config file */
    struct ako_callback_ops* ops = nullptr;
    /* config unit */
    struct eventd_config_t *eventd_config = nullptr;

    ako_setproctitle(argv, ako_proc_name);
    ako_pthread_setname(pthread_self(), ako_main_thread_name);

    /* parsing options from command */
    err = ako_parse_opts(argc, argv);
    if (err) {
        /* on parse options error or get usage information */
        std::cerr << STDCERR_POSITION_IN_SRC_BEFORE_LOGGER 
            << "ako_parse_opts error" << std::endl;
        goto out;
    }

    /* init configuration mapping */
    default_config_map = ako_config_map_init();
    if (!default_config_map) {
        /* malloc failed */
        std::cerr << STDCERR_POSITION_IN_SRC_BEFORE_LOGGER 
            << "ako_config_map_init error" << std::endl;
        goto out;
    }

    /* init configuration, and load from config file */
    err = ako_config_init(ako_config_path, default_config_map);
    if (err) {
        /* error on config init, maybe its malloc failed */
        std::cerr << STDCERR_POSITION_IN_SRC_BEFORE_LOGGER
            << "ako_config_init failed" << std::endl;
        goto err_config;
    }

    /* enable options into configuration */
    err = ako_enable_opts();
    if (err) {
        std::cerr << STDCERR_POSITION_IN_SRC_BEFORE_LOGGER
            << "ako_enable_opts error." << std::endl;
        goto err_config;
    }

    /* init logger */
    err = ako_logger_init(ako_config_get_string("Logger", "config_path").c_str());
    if (err) {
        /* after logger inited, use logging `akolog_xxx` instead of `std::cerr` */
        std::cerr << STDCERR_POSITION_IN_SRC_BEFORE_LOGGER
            << "ako_logger_init failed." << std::endl;
        goto err_logger;
    }

    /* init slab allocator */
    slab_chain = new struct ako_slab_chain;
    if (!slab_chain) {
        akolog_fatal(LOGGING_POSITION, "failed to alloc slab allocator");
        goto err_slab;
    }
    ako_slab_init(slab_chain, slab_chain_size);

    /* init event driver and socket server */
    eventd_config = new struct eventd_config_t;
    if (!eventd_config) {
        akolog_fatal(LOGGING_POSITION, "failed to alloc eventd config unit");
        goto err_eventd;
    }
    /* dump eventd/css/server config */
    strcpy(eventd_config->css_conf.addr, 
        ako_config_get_string("CSS", "addr").c_str());
    eventd_config->css_conf.backlog = ako_config_get_int("CSS", "backlog");
    eventd_config->css_conf.port = ako_config_get_int("CSS", "port");
    /* dump eventd/das/server config */
    strcpy(eventd_config->das_conf.addr,
        ako_config_get_string("DAS", "addr").c_str());
    eventd_config->das_conf.backlog = ako_config_get_int("DAS", "backlog");
    eventd_config->das_conf.port = ako_config_get_int("DAS", "port");
    /* do eventd init */
    err = ako_eventd_init(eventd_config);
    if (err) {
        akolog_fatal(LOGGING_POSITION, "failed to init event driver and socket server");
        goto err_eventd;
    }

    /* init regional management service */
    ops = new struct ako_callback_ops;
    if (!ops) {
        akolog_fatal(LOGGING_POSITION, "failed to alloc `ako_rms_ops`");
        goto err_ako_init;
    }
    err = ako_init(ops);
    if (err) {
        akolog_fatal(LOGGING_POSITION, "failed to init regional management service");
        goto err_ako_init;
    }

    /* init sparkle daemon service */
    err = ako_daemon_init();
    if (err) {
        akolog_fatal(LOGGING_POSITION, "failed to init sparkle daemon service");
        goto err_daemon;
    }
    err = ako_daemon_start();

err_daemon:
    ako_daemon_exit();

err_ako_init:
    ako_exit();
    delete ops;
    ops = nullptr;

err_eventd:
    ako_eventd_exit();
    delete eventd_config;
    eventd_config = nullptr;

err_slab:
    ako_slab_destroy(slab_chain);
    delete slab_chain;
    slab_chain = nullptr;

err_logger:
    ako_logger_free();

err_config:
    ako_config_exit();
    delete default_config_map;
    default_config_map = nullptr;

out:
    return err;
}