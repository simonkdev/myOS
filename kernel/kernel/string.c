#include <stddef.h>

// Simple strcmp for freestanding use
int strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++; s2++;
    }
    return (unsigned char)*s1 - (unsigned char)*s2;
}

// Simple memset for freestanding use
void *memset(void *dest, int c, size_t n) {
    unsigned char *p = dest;
    while (n--) {
        *p++ = (unsigned char)c;
    }
    return dest;
}
