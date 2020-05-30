#ifndef AKOFS_TEST_AKO_GET_OPT_HPP_
#define AKOFS_TEST_AKO_GET_OPT_HPP_

#include <gtest/gtest.h>
#include <exception>
#include <ako_getopt.h>

TEST(test_ako_getopt, get_option_as_GNU_getopt_case_1) {
    bool amend = false;
    bool brief = false;
    const char* color = "white";
    int delay = 0;

    char* arg;
    int option;
    struct ako_optparse options;

    const int argc = 10;
    const char* argv[] = {
        "./sparkle_test", "-a", "-b", "-c", "red", "-d22",
        "subcommand", "example.txt", "-a", "-b", NULL
    };

    char short_argv[10][16];
    char* short_argv_ptr[11];
    for (int i = 0; i < 10; i++) {
        strcpy(short_argv[i], argv[i]);
        short_argv_ptr[i] = short_argv[i];
    }
    short_argv_ptr[10] = NULL;
    
    ako_optparse_init(&options, short_argv_ptr);
    while ((option = ako_optparse(&options, "abc:d::")) != -1) {
        switch (option) {
        case 'a':
            amend = true;
            break;
        case 'b':
            brief = true;
            break;
        case 'c':
            color = options.optarg;
            break;
        case 'd':
            delay = options.optarg ? atoi(options.optarg) : 1;
            break;
        case '?':
            /* fprintf(stderr, "%s: %s\n", argv[0], options.errmsg); */
            EXPECT_TRUE(0);
        }
    }

    EXPECT_TRUE(amend);
    EXPECT_TRUE(brief);
    EXPECT_STREQ("red", color);
    EXPECT_EQ(22, delay);

    /* Print remaining arguments. */
    arg = ako_optparse_arg(&options);
    EXPECT_STREQ("subcommand", arg);
    arg = ako_optparse_arg(&options);
    EXPECT_STREQ("example.txt", arg); 
    arg = ako_optparse_arg(&options);
    EXPECT_EQ(NULL, arg);
}

TEST(test_ako_getopt, get_option_as_GNU_getopt_long_case_1) {
    bool amend = false;
    bool brief = false;
    const char* color = "white";
    int delay = 0;

    char* arg;
    int option, longindex;
    int cnt_flag = 0;
    struct ako_optparse options;

    const int argc = 8;
    const char* argv[] = {
        "./sparkle_test", "--amend", "-b", "--color", "red", "--delay=22",
        "subcommand", "example.txt", NULL
    };

    /* short argv */
    char short_argv[8][16];
    char* short_argv_ptr[9];
    for (int i = 0; i < 8; i++) {
        strcpy(short_argv[i], argv[i]);
        short_argv_ptr[i] = short_argv[i];
    }
    short_argv_ptr[8] = NULL;

    /* long argv */
    char long_argv[8][16];
    char* long_argv_ptr[9];
    for (int i = 0; i < 8; i++) {
        strcpy(long_argv[i], argv[i]);
        long_argv_ptr[i] = long_argv[i];
    }
    long_argv_ptr[8] = NULL;

    /* test for short argv parse */
    ako_optparse_init(&options, short_argv_ptr);

    while ((option = ako_optparse(&options, "abc:d::")) != -1) {
        if (option == '?')
            EXPECT_TRUE(0);
        cnt_flag++;
    }
    EXPECT_EQ(1, cnt_flag);
    EXPECT_EQ(2, options.optind);

    cnt_flag = 0;
    while ((arg = ako_optparse_arg(&options)))
        cnt_flag++;
    EXPECT_EQ(6, cnt_flag);

    /* test for long argv parse */
    ako_optparse_init(&options, long_argv_ptr);

    struct ako_optparse_long longopts[] = {
        {"amend", 'a', AKO_OPTPARSE_NONE},
        {"brief", 'b', AKO_OPTPARSE_NONE},
        {"color", 'c', AKO_OPTPARSE_REQUIRED},
        {"delay", 'd', AKO_OPTPARSE_OPTIONAL},
        {NULL, 0, AKO_OPTPARSE_NONE}
    };

    while ((option = ako_optparse_long(&options, longopts, &longindex)) != -1) {
        switch (option) {
        case 'a':
            amend = true;
            break;
        case 'b':
            brief = true;
            break;
        case 'c':
            color = options.optarg;
            break;
        case 'd':
            delay = options.optarg ? atoi(options.optarg) : 1;
            break;
        case '?':
            /* fprintf(stderr, "%s: %s\n", argv[0], options.errmsg); */
            EXPECT_TRUE(0);
        }
    }

    EXPECT_EQ(6, options.optind);
    EXPECT_TRUE(amend);
    EXPECT_TRUE(brief);
    EXPECT_STREQ("red", color);
    EXPECT_EQ(22, delay);

    /* Print remaining arguments. */
    arg = ako_optparse_arg(&options);
    EXPECT_STREQ("subcommand", arg);
    arg = ako_optparse_arg(&options);
    EXPECT_STREQ("example.txt", arg);
    arg = ako_optparse_arg(&options);
    EXPECT_EQ(NULL, arg);
}

TEST(test_ako_getopt, get_option_as_GNU_getopt_long_case_2) {
    const char* amend = NULL;
    const char* brief = NULL;
    const char* color = NULL;
    int delay = 0;

    char* arg;
    int option, longindex;
    struct ako_optparse options;

    const int argc = 8;
    const char* argv[] = {
        "./sparkle_test", "--amend=emmm", "-b2019", "--color", "red", "--delay",
        "subcommand", "example.txt", NULL
    };

    /* long argv */
    char long_argv[8][16];
    char* long_argv_ptr[9];
    for (int i = 0; i < 8; i++) {
        strcpy(long_argv[i], argv[i]);
        long_argv_ptr[i] = long_argv[i];
    }
    long_argv_ptr[8] = NULL;

    /* test for long argv parse */
    ako_optparse_init(&options, long_argv_ptr);

    struct ako_optparse_long longopts[] = {
        {"amend", 'a', AKO_OPTPARSE_REQUIRED},
        {"brief", 'b', AKO_OPTPARSE_OPTIONAL},
        {"color", 'c', AKO_OPTPARSE_REQUIRED},
        {"delay", 'd', AKO_OPTPARSE_OPTIONAL},
        {NULL, 0, AKO_OPTPARSE_NONE}
    };

    while ((option = ako_optparse_long(&options, longopts, &longindex)) != -1) {
        switch (option) {
        case 'a':
            amend = options.optarg;
            break;
        case 'b':
            brief = options.optarg;
            break;
        case 'c':
            color = options.optarg;
            break;
        case 'd':
            delay = options.optarg ? atoi(options.optarg) : 1;
            break;
        case '?':
            /* fprintf(stderr, "%s: %s\n", argv[0], options.errmsg); */
            EXPECT_TRUE(0);
        }
    }

    EXPECT_EQ(6, options.optind);
    EXPECT_STREQ("emmm", amend);
    EXPECT_STREQ("2019", brief);
    EXPECT_STREQ("red", color);
    EXPECT_EQ(1, delay);

    /* Print remaining arguments. */
    arg = ako_optparse_arg(&options);
    EXPECT_STREQ("subcommand", arg);
    arg = ako_optparse_arg(&options);
    EXPECT_STREQ("example.txt", arg);
    arg = ako_optparse_arg(&options);
    EXPECT_EQ(NULL, arg);
}

/* 
 * TODO: write more test unit
 */

#endif // AKOFS_TEST_AKO_GET_OPT_HPP_
