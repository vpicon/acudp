#ifndef _ACUDP_PYTHON_EXTENSION_UTIL_H
#define _ACUDP_PYTHON_EXTENSION_UTIL_H

// Macro for nicer unused argument compiler flag
#ifdef __GNUC__
#  define UNUSED(x) UNUSED_ ## x __attribute__((__unused__))
#else
#  define UNUSED(x) UNUSED_ ## x
#endif


#endif  // _ACUDP_PYTHON_EXTENSION_UTIL_H
