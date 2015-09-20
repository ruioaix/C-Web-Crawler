#ifndef __SAFE_H
#define __SAFE_H 

#include <stdio.h>
#include <stdbool.h>

void *malloc_safe(size_t size, const char *filename, const int lineNum);
void *calloc_safe(size_t num, size_t size, const char *filename, const int lineNum);
void *realloc_safe(void *p, size_t size, const char *filename, const int lineNum);
#define smalloc(n) malloc_safe(n, __FILE__, __LINE__)
#define scalloc(n, s) calloc_safe(n, s, __FILE__, __LINE__)
#define srealloc(p, n) realloc_safe(p, n, __FILE__, __LINE__)

FILE *fopen_safe(const char *openfilename, const char *mode, char *filename, const int lineNum);
#define sfopen(fn, mode) fopen_safe(fn, mode, __FILE__, __LINE__)

#endif
