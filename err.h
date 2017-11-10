#ifndef ERR_H
#define ERR_H
#include <stdio.h>
#include <signal.h>
#include <errno.h>


#define FATAL(...) fprintf(stderr,__VA_ARGS__)

#define MEM_NOT_AVAIL(ptr,file_name) fprintf(stderr,"%s:bad memory allocation for %s pointer\n",file_name,ptr)

#define file_err(A) if (!(A)) {printf("%s\n",strerror(EIO));}

#endif
