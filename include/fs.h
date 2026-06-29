#ifndef FS_H
#define FS_H 1
#include <stdint.h>

int fs_init(void);

int fs_create(const char *name);

int fs_open(const char *name);

int fs_close(int fd);

int fs_read(int fd, void *buffer, uint32_t size);

int fs_write(int fd, const void *buffer, uint32_t size);

int fs_delete(const char *name);

int cluster_alloc(void);

#endif