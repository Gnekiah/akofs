/*
 * getopt-like option parser
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: Xxiong <xxiong@cqu.edu.cn>
 */

#ifndef SPARKLE_SPK_GET_OPT_H_
#define SPARKLE_SPK_GET_OPT_H_

#ifdef __cplusplus
extern "C" {
#endif

struct spk_optparse {
    char** argv;
    int permute;
    int optind;
    int optopt;
    char* optarg;
    char errmsg[64];
    int subopt;
};

enum spk_optparse_argtype {
    SPK_OPTPARSE_NONE,
    SPK_OPTPARSE_REQUIRED,
    SPK_OPTPARSE_OPTIONAL
};

struct spk_optparse_long {
    const char* longname;
    int shortname;
    enum spk_optparse_argtype argtype;
};

/**
 * Initializes the parser state.
 */
extern void spk_optparse_init(struct spk_optparse* options, char** argv);

/**
 * Read the next option in the argv array.
 * @param optstring a getopt()-formatted option string.
 * @return the next option character, -1 for done, or '?' for error
 *
 * Just like getopt(), a character followed by no colons means no
 * argument. One colon means the option has a required argument. Two
 * colons means the option takes an optional argument.
 */
extern int spk_optparse(struct spk_optparse* options, const char* optstring);

/**
 * Handles GNU-style long options in addition to getopt() options.
 * This works a lot like GNU's getopt_long(). The last option in
 * longopts must be all zeros, marking the end of the array. The
 * longindex argument may be NULL.
 */
extern int spk_optparse_long(struct spk_optparse* options,
    const struct spk_optparse_long* longopts,
    int* longindex);

/**
 * Used for stepping over non-option arguments.
 * @return the next non-option argument, or NULL for no more arguments
 *
 * Argument parsing can continue with spk_optparse() after using this
 * function. That would be used to parse the options for the
 * subcommand returned by spk_optparse_arg(). This function allows you to
 * ignore the value of optind.
 */
extern char* spk_optparse_arg(struct spk_optparse* options);

#ifdef __cplusplus
}
#endif

#endif // SPARKLE_SPK_GET_OPT_H_