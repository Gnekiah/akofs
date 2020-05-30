/*
 * getopt-like option parser
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: Xxiong <xxiong@cqu.edu.cn>
 */

#ifndef AKOFS_AKO_GET_OPT_H_
#define AKOFS_AKO_GET_OPT_H_

#ifdef __cplusplus
extern "C" {
#endif

struct ako_optparse {
    char** argv;
    int permute;
    int optind;
    int optopt;
    char* optarg;
    char errmsg[64];
    int subopt;
};

enum ako_optparse_argtype {
    AKO_OPTPARSE_NONE,
    AKO_OPTPARSE_REQUIRED,
    AKO_OPTPARSE_OPTIONAL
};

struct ako_optparse_long {
    const char* longname;
    int shortname;
    enum ako_optparse_argtype argtype;
};

/**
 * Initializes the parser state.
 */
extern void ako_optparse_init(struct ako_optparse* options, char** argv);

/**
 * Read the next option in the argv array.
 * @param optstring a getopt()-formatted option string.
 * @return the next option character, -1 for done, or '?' for error
 *
 * Just like getopt(), a character followed by no colons means no
 * argument. One colon means the option has a required argument. Two
 * colons means the option takes an optional argument.
 */
extern int ako_optparse(struct ako_optparse* options, const char* optstring);

/**
 * Handles GNU-style long options in addition to getopt() options.
 * This works a lot like GNU's getopt_long(). The last option in
 * longopts must be all zeros, marking the end of the array. The
 * longindex argument may be NULL.
 */
extern int ako_optparse_long(struct ako_optparse* options,
    const struct ako_optparse_long* longopts,
    int* longindex);

/**
 * Used for stepping over non-option arguments.
 * @return the next non-option argument, or NULL for no more arguments
 *
 * Argument parsing can continue with ako_optparse() after using this
 * function. That would be used to parse the options for the
 * subcommand returned by ako_optparse_arg(). This function allows you to
 * ignore the value of optind.
 */
extern char* ako_optparse_arg(struct ako_optparse* options);

#ifdef __cplusplus
}
#endif

#endif // AKOFS_AKO_GET_OPT_H_