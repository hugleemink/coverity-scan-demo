#ifndef VULNERABLE_H
#define VULNERABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ============================================================
 * vulnerable.h — Khai báo các hàm CÓ LỖI (để Coverity phát hiện)
 * Mục đích: DEMO — KHÔNG dùng trong production!
 * ============================================================ */

#define BUFFER_SIZE 64
#define MAX_INPUT   256

/* Demo: Null Pointer Dereference */
char* demo_null_pointer(int condition);

/* Demo: Buffer Overflow */
void demo_buffer_overflow(const char *input);

/* Demo: Memory Leak */
int demo_memory_leak(int n);

/* Demo: Use After Free */
void demo_use_after_free(void);

/* Demo: Integer Overflow */
int demo_integer_overflow(int a, int b);

/* Demo: Resource Leak (file handle) */
int demo_resource_leak(const char *filename);

/* Demo: Uninitialized Variable */
int demo_uninitialized_var(int condition);

#endif /* VULNERABLE_H */
