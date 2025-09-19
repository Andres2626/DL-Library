#ifndef _DL_H
#define _DL_H

/* types */
typedef void* dl_func_ptr;
typedef void* dl_handle_ptr;

/* This is the dynamic structure library interface */
typedef struct dl_mod {
	const char* file; /* file of the module */
	int state; /* 1 if load fails */
	dl_handle_ptr handle; /* pointer to module */
};

/* 
 * NOTE: To ensure compatibility with Windows and POSIX systems, the third parameter, 
 * which belongs to the library loading mode, is not used. 
 */
int open_module(struct dl_mod *mod, const char *file, int);
dl_func_ptr sym_module(struct dl_mod* mod, const char *symbol);
int close_module(dl_handle_ptr handle);

#endif /* !_DL_H */

