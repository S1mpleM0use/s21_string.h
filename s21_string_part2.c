#include "s21_string.h"

int s21_memcmp(const void *str1, const void *str2, s21_size_t n) {
  const unsigned char *s1 = (const unsigned char *)str1;
  const unsigned char *s2 = (const unsigned char *)str2;

  for (s21_size_t i = 0; i < n; i++) {
    if (s1[i] != s2[i]) {
      return (int)s1[i] - (int)s2[i];
    }
  }
  return 0;
}

void *s21_memcpy(void *dest, const void *src, s21_size_t n) {
  unsigned char *d = (unsigned char *)dest;
  const unsigned char *s = (const unsigned char *)src;

  for (s21_size_t i = 0; i < n; i++) {
    d[i] = s[i];
  }

  return dest;
}

void *s21_memset(void *str, int c, s21_size_t n) {
  unsigned char *ptr = (unsigned char *)str;
  unsigned char value = (unsigned char)c;

  for (s21_size_t i = 0; i < n; i++) {
    ptr[i] = value;
  }

  return str;
}

int s21_strncmp(const char *str1, const char *str2, s21_size_t n) {
  if (n == 0) {
    return 0;
  }

  while (n-- > 0) {
    if (*str1 != *str2) {
      return (unsigned char)*str1 - (unsigned char)*str2;
    }
    if (*str1 == '\0') {
      return 0;
    }
    str1++;
    str2++;
  }

  return 0;
}

char *s21_strncpy(char *dest, const char *src, s21_size_t n) {
  char *start = dest;

  while (n > 0 && *src != '\0') {
    *dest++ = *src++;
    n--;
  }

  while (n > 0) {
    *dest++ = '\0';
    n--;
  }

  return start;
}

char *s21_strncat(char *dest, const char *src, s21_size_t n) {
  char *start = dest;

  while (*dest != '\0') {
    dest++;
  }

  while (n > 0 && *src != '\0') {
    *dest++ = *src++;
    n--;
  }

  *dest = '\0';

  return start;
}