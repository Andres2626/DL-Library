#ifndef _DL_H
#define _DL_H

#define DL_OK            0 
#define DL_ERROR         1
#define DL_INVALID_MEM  -1

/* types */
typedef void* dl_func_ptr;
typedef void* dl_handle_ptr;

/* This is the dynamic structure library interface */
struct dl_mod {
	const char* file; /* file of the module */
	int state; /* 1 if load fails */
	dl_handle_ptr handle; /* pointer to module */
};

/* 
 * NOTE: To ensure compatibility with Windows and POSIX systems, the third parameter, 
 * which belongs to the library loading mode, is not used. 
 */
int dl_open_module(struct dl_mod *mod, const char *file, int);
dl_func_ptr dl_sym_module(struct dl_mod* mod, const char *symbol);
int dl_close_module(struct dl_mod *mod);
const char *dl_get_error();

#endif /* !_DL_H */

