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
#else
#include <dlfcn.h> /* posix dl functions */

/* posix macros */
#define OPEN(file) dlopen(file, RTLD_LAZY) /* Enable LAZY mode as default */
#define CLOSE(handle) dlclose(handle)
#define SYM(handle, symbol) dlsym(handle, symbol)
#endif

int open_module(struct dl_mod* mod, const char* file, int mode) 
{
    (void)mode; /* POSIX compatibility */

    /* check valid dl pointer */
    if (!mod)
        return NULL;

    mod->file = file;

    /* load module */
    dl_func_ptr h = OPEN(mod->file);
    if (!h) {
        mod->state = 1; /* error state */
        return 1;
    }

    /* yes! the module has loaded correcly */
    mod->state = 0; /* loaded state */
    mod->handle = h;
    return mod->handle;
}

dl_func_ptr sym_module(struct dl_mod* mod, const char* symbol)
{
    dl_func_ptr ptr = SYM(mod->handle, symbol);
    if (!ptr)
        return NULL; /* function not found in module */

    /* return the function pointer of loaded module */
    return ptr;
}

int close_module(dl_handle_ptr handle) 
{
    return CLOSE(handle);
}