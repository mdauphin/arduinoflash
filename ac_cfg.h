#include <windows.h>

#define PATH_MAX MAX_PATH

#define strncasecmp _strnicmp 
#define strcasecmp _stricmp 
#define WIN32NATIVE

int usleep(unsigned int usec);