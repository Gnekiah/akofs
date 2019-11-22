/*
 * Header definition for global macros.
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: Xxiong <xxiong@cqu.edu.cn>
 */

#ifndef SPARKLE_SPK_MACROS_H_
#define SPARKLE_SPK_MACROS_H_

#ifdef __cplusplus
extern "C" {
#endif

// A macro to disallow the copy constructor and operator= functions 
// This should be used in the priavte:declarations for a class
#ifndef DISALLOW_COPY_AND_ASSIGN
#define DISALLOW_COPY_AND_ASSIGN(TypeName)  \
    TypeName(const TypeName&);              \
    TypeName& operator=(const TypeName&)
#endif 

#ifdef __cplusplus
}
#endif

#endif  // SPARKLE_SPK_MACROS_H_
