#pragma once
// https://github.com/martinmoene/string-view-lite#configuration

#define nssv_CONFIG_NO_EXCEPTIONS 1

// #define nssv_CPLUSPLUS 199711L
// Define this macro to override the auto-detection of the supported C++ standard, 
// if your compiler does not set the __cpluplus macro correctly.

#define nssv_CONFIG_STD_SV_OPERATOR 0
// Define this to 1 to provide literal operator sv to create a string_view from a literal string. Default is 0. 
// Note that literal operators without leading underscore are reserved for the C++ standard.

#define nssv_CONFIG_USR_SV_OPERATOR 1
// Define this to 0 to omit literal operator _sv to create a string_view from a literal string. 
// Default is 1.

#undef nssv_CONFIG_CONVERSION_STD_STRING
// Define this to 1 to provide std::string– nonstd::string_view interoperability via methods 
// of nonstd::basic_string_view and free functions, define it to 0 to omit all said methods and functions. 
// Default is undefined.

#define nssv_CONFIG_CONVERSION_STD_STRING_CLASS_METHODS 0
// Define this to 1 to provide std::string– nonstd::string_view interoperability via methods 
// of nonstd::basic_string_view, define it to 0 to omit all said methods. 
// Default is undefined.

#undef nssv_CONFIG_CONVERSION_STD_STRING_FREE_FUNCTIONS
// Define this to 1 to provide std::string– nonstd::string_view interoperability via free functions, 
// define it to 0 to omit all said functions. This also controls the presence of these function if std::string_view is used. 
// Default is undefined.

#define nssv_CONFIG_NO_STREAM_INSERTION 0
// Define this to 1 to omit the use of standard streams. Default is undefined