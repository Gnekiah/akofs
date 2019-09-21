#ifndef SPARKLE_MACROS_
#define SPARKLE_MACROS_

// A macro to disallow the copy constructor and operator= functions 
// This should be used in the priavte:declarations for a class
#ifndef DISALLOW_COPY_AND_ASSIGN
#define DISALLOW_COPY_AND_ASSIGN(TypeName)  \
    TypeName(const TypeName&);              \
    TypeName& operator=(const TypeName&)
#endif 

#endif  // SPARKLE_MACROS_
