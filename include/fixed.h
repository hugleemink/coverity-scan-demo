#ifndef FIXED_H
#define FIXED_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define BUFFER_SIZE     64
#define MAX_RESULT_SIZE 128

/* Phiên bản đã sửa — an toàn để dùng */
char* fixed_null_pointer(int condition);
void  fixed_buffer_overflow(const char *input);
int   fixed_memory_leak(int n);
void  fixed_use_after_free(void);
int   fixed_integer_overflow(int a, int b);
int   fixed_resource_leak(const char *filename);
int   fixed_uninitialized_var(int condition);

#endif /* FIXED_H */
