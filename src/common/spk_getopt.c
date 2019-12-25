/*
 * getopt-like option parser
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: Xxiong <xxiong@cqu.edu.cn>
 */

#include <spk_getopt.h>

#define SPK_OPTPARSE_MSG_INVALID "invalid option"
#define SPK_OPTPARSE_MSG_MISSING "option requires an argument"
#define SPK_OPTPARSE_MSG_TOOMANY "option takes no arguments"

static int spk_optparse_error(struct spk_optparse* options, const char* msg, const char* data)
{
    unsigned p = 0;
    const char* sep = " -- '";
    while (*msg)
        options->errmsg[p++] = *msg++;
    while (*sep)
        options->errmsg[p++] = *sep++;
    while (p < sizeof(options->errmsg) - 2 && *data)
        options->errmsg[p++] = *data++;
    options->errmsg[p++] = '\'';
    options->errmsg[p++] = '\0';
    return '?';
}

void spk_optparse_init(struct spk_optparse* options, char** argv)
{
    options->argv = argv;
    options->permute = 1;
    options->optind = 1;
    options->subopt = 0;
    options->optarg = 0;
    options->errmsg[0] = '\0';
}

static int spk_optparse_is_dashdash(const char* arg)
{
    return arg != 0 && arg[0] == '-' && arg[1] == '-' && arg[2] == '\0';
}

static int spk_optparse_is_shortopt(const char* arg)
{
    return arg != 0 && arg[0] == '-' && arg[1] != '-' && arg[1] != '\0';
}

static int spk_optparse_is_longopt(const char* arg)
{
    return arg != 0 && arg[0] == '-' && arg[1] == '-' && arg[2] != '\0';
}

static void spk_optparse_permute(struct spk_optparse* options, int index)
{
    char* nonoption = options->argv[index];
    int i;
    for (i = index; i < options->optind - 1; i++)
        options->argv[i] = options->argv[i + 1];
    options->argv[options->optind - 1] = nonoption;
}

static int spk_optparse_argtype(const char* optstring, char c)
{
    int count = SPK_OPTPARSE_NONE;
    if (c == ':')
        return -1;
    for (; *optstring && c != *optstring; optstring++);
    if (!*optstring)
        return -1;
    if (optstring[1] == ':')
        count += optstring[2] == ':' ? 2 : 1;
    return count;
}

int spk_optparse(struct spk_optparse* options, const char* optstring)
{
    int type;
    char* next;
    char* option = options->argv[options->optind];
    options->errmsg[0] = '\0';
    options->optopt = 0;
    options->optarg = 0;
    if (option == 0) {
        return -1;
    }
    else if (spk_optparse_is_dashdash(option)) {
        options->optind++; /* consume "--" */
        return -1;
    }
    else if (!spk_optparse_is_shortopt(option)) {
        if (options->permute) {
            int index = options->optind++;
            int r = spk_optparse(options, optstring);
            spk_optparse_permute(options, index);
            options->optind--;
            return r;
        }
        else {
            return -1;
        }
    }
    option += options->subopt + 1;
    options->optopt = option[0];
    type = spk_optparse_argtype(optstring, option[0]);
    next = options->argv[options->optind + 1];
    switch (type) {
    case -1: {
        char str[2] = { 0, 0 };
        str[0] = option[0];
        options->optind++;
        return spk_optparse_error(options, SPK_OPTPARSE_MSG_INVALID, str);
    }
    case SPK_OPTPARSE_NONE:
        if (option[1]) {
            options->subopt++;
        }
        else {
            options->subopt = 0;
            options->optind++;
        }
        return option[0];
    case SPK_OPTPARSE_REQUIRED:
        options->subopt = 0;
        options->optind++;
        if (option[1]) {
            options->optarg = option + 1;
        }
        else if (next != 0) {
            options->optarg = next;
            options->optind++;
        }
        else {
            char str[2] = { 0, 0 };
            str[0] = option[0];
            options->optarg = 0;
            return spk_optparse_error(options, SPK_OPTPARSE_MSG_MISSING, str);
        }
        return option[0];
    case SPK_OPTPARSE_OPTIONAL:
        options->subopt = 0;
        options->optind++;
        if (option[1])
            options->optarg = option + 1;
        else
            options->optarg = 0;
        return option[0];
    }
    return 0;
}

char* spk_optparse_arg(struct spk_optparse* options)
{
    char* option = options->argv[options->optind];
    options->subopt = 0;
    if (option != 0)
        options->optind++;
    return option;
}

static int spk_optparse_longopts_end(const struct spk_optparse_long* longopts, int i)
{
    return !longopts[i].longname && !longopts[i].shortname;
}

static void spk_optparse_from_long(const struct spk_optparse_long* longopts, char* optstring)
{
    char* p = optstring;
    int i;
    for (i = 0; !spk_optparse_longopts_end(longopts, i); i++) {
        if (longopts[i].shortname) {
            int a;
            *p++ = longopts[i].shortname;
            for (a = 0; a < (int)longopts[i].argtype; a++)
                * p++ = ':';
        }
    }
    *p = '\0';
}

/* Unlike strcmp(), handles options containing "=". */
static int spk_optparse_longopts_match(const char* longname, const char* option)
{
    const char* a = option, * n = longname;
    if (longname == 0)
        return 0;
    for (; *a && *n && *a != '='; a++, n++)
        if (*a != *n)
            return 0;
    return *n == '\0' && (*a == '\0' || *a == '=');
}

/* Return the part after "=", or NULL. */
static char* spk_optparse_longopts_arg(char* option)
{
    for (; *option && *option != '='; option++);
    if (*option == '=')
        return option + 1;
    else
        return 0;
}

static int spk_optparse_long_fallback(struct spk_optparse* options,
    const struct spk_optparse_long* longopts,
    int* longindex)
{
    int result;
    char optstring[96 * 3 + 1]; /* 96 ASCII printable characters */
    spk_optparse_from_long(longopts, optstring);
    result = spk_optparse(options, optstring);
    if (longindex != 0) {
        *longindex = -1;
        if (result != -1) {
            int i;
            for (i = 0; !spk_optparse_longopts_end(longopts, i); i++)
                if (longopts[i].shortname == options->optopt)
                    * longindex = i;
        }
    }
    return result;
}

int spk_optparse_long(struct spk_optparse* options,
    const struct spk_optparse_long* longopts,
    int* longindex)
{
    int i;
    char* option = options->argv[options->optind];
    if (option == 0) {
        return -1;
    }
    else if (spk_optparse_is_dashdash(option)) {
        options->optind++; /* consume "--" */
        return -1;
    }
    else if (spk_optparse_is_shortopt(option)) {
        return spk_optparse_long_fallback(options, longopts, longindex);
    }
    else if (!spk_optparse_is_longopt(option)) {
        if (options->permute) {
            int index = options->optind++;
            int r = spk_optparse_long(options, longopts, longindex);
            spk_optparse_permute(options, index);
            options->optind--;
            return r;
        }
        else {
            return -1;
        }
    }

    /* Parse as long option. */
    options->errmsg[0] = '\0';
    options->optopt = 0;
    options->optarg = 0;
    option += 2; /* skip "--" */
    options->optind++;
    for (i = 0; !spk_optparse_longopts_end(longopts, i); i++) {
        const char* name = longopts[i].longname;
        if (spk_optparse_longopts_match(name, option)) {
            char* arg;
            if (longindex)
                * longindex = i;
            options->optopt = longopts[i].shortname;
            arg = spk_optparse_longopts_arg(option);
            if (longopts[i].argtype == SPK_OPTPARSE_NONE && arg != 0) {
                return spk_optparse_error(options, SPK_OPTPARSE_MSG_TOOMANY, name);
            } if (arg != 0) {
                options->optarg = arg;
            }
            else if (longopts[i].argtype == SPK_OPTPARSE_REQUIRED) {
                options->optarg = options->argv[options->optind];
                if (options->optarg == 0)
                    return spk_optparse_error(options, SPK_OPTPARSE_MSG_MISSING, name);
                else
                    options->optind++;
            }
            return options->optopt;
        }
    }
    return spk_optparse_error(options, SPK_OPTPARSE_MSG_INVALID, option);
}
