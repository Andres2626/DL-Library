#include "dl.h"

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

/*
 * on Windows systems, LoadLibrary is used to load a library at runtime, 
 * unlike POSIX, the dlfcn.h header is used with the dl**(...) functions.
*/
#if defined (_WIN32) && !defined (__CYGWIN__)
#include <Windows.h> /* LoadLibraryA function */

/* windows macros */
#define OPEN(file) LoadLibraryA(file)
#define CLOSE(handle) FreeLibrary(handle)
#define SYM(handle, symbol) GetProcAddress(handle, symbol)
#define ERR GetLastError()
#else
#include <dlfcn.h> /* posix dl functions */

/* posix macros */
#define OPEN(file) dlopen(file, RTLD_LAZY) /* Enable LAZY mode as default */
#define CLOSE(handle) dlclose(handle)
#define SYM(handle, symbol) dlsym(handle, symbol)
#define ERR dlerror()
#endif

int dl_open_module(struct dl_mod* mod, const char* file, int mode) 
{
    (void)mode; /* POSIX compatibility */

    /* check valid dl pointer */
    if (!mod)
        return DL_INVALID_MEM;

    mod->file = file;

    /* load module */
    dl_func_ptr h = OPEN(mod->file);
    if (!h) {
        mod->state = DL_ERROR; /* error state */
        return DL_ERROR;
    }

    /* yes! the module has loaded correcly */
    mod->state = DL_OK; /* loaded state */
    mod->handle = h;
    return DL_OK;
}

dl_func_ptr dl_sym_module(struct dl_mod* mod, const char* symbol)
{
    dl_func_ptr ptr = SYM(mod->handle, symbol);
    if (!ptr)
        return NULL; /* function not found in module */

    /* return the function pointer of loaded module */
    return ptr;
}

int dl_close_module(struct dl_mod *mod)
{
    return CLOSE(mod->handle);
}

const char *dl_get_error() 
{
#if defined (_WIN32) && !defined (__CYGWIN__)
	static char buff[64]; /* error buffer */
	int code = ERR;
	
	/* detect if the code is valid */
	if (!code)
		return NULL; /* no error */
	
	/* create error message */
	FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, 
	               code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buff, 
				   sizeof(buff), NULL);
	return buff;
#else
	return ERR; /* posix dlerror() */
#endif
}