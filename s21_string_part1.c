#include "s21_string.h"

s21_size_t s21_strlen(const char *str) {
  s21_size_t len = 0;
  while (str && str[len] != '\0') {
    len++;
  }
  return len;
}

char *s21_strchr(const char *str, int c) {
  if (!str) return S21_NULL;
  while (*str != '\0') {
    if (*str == (char)c) {
      return (char *)str;
    }
    str++;
  }
  if ((char)c == '\0') {
    return (char *)str;
  }
  return S21_NULL;
}

char *s21_strrchr(const char *str, int c) {
  if (!str) return S21_NULL;
  const char *last = S21_NULL;
  while (*str != '\0') {
    if (*str == (char)c) {
      last = str;
    }
    str++;
  }
  if ((char)c == '\0') {
    return (char *)str;
  }
  return (char *)last;
}

char *s21_strpbrk(const char *str1, const char *str2) {
  if (!str1 || !str2) return S21_NULL;
  const char *s1 = str1;
  while (*s1 != '\0') {
    const char *s2 = str2;
    while (*s2 != '\0') {
      if (*s1 == *s2) {
        return (char *)s1;
      }
      s2++;
    }
    s1++;
  }
  return S21_NULL;
}

char *s21_strstr(const char *haystack, const char *needle) {
  if (!haystack || !needle) return S21_NULL;
  if (*needle == '\0') {
    return (char *)haystack;
  }
  for (; *haystack != '\0'; haystack++) {
    const char *h = haystack;
    const char *n = needle;
    while (*h != '\0' && *n != '\0' && *h == *n) {
      h++;
      n++;
    }
    if (*n == '\0') {
      return (char *)haystack;
    }
  }
  return S21_NULL;
}

s21_size_t s21_strcspn(const char *str1, const char *str2) {
  if (!str1 || !str2) return 0;
  s21_size_t count = 0;
  while (*str1 != '\0') {
    const char *s2 = str2;
    int found = 0;
    while (*s2 != '\0') {
      if (*str1 == *s2) {
        found = 1;
        break;
      }
      s2++;
    }
    if (found) {
      break;
    }
    count++;
    str1++;
  }
  return count;
}
void *s21_memchr(const void *str, int c, s21_size_t n) {
  if (!str) return S21_NULL;
  const unsigned char *s = (const unsigned char *)str;
  unsigned char ch = (unsigned char)c;
  for (s21_size_t i = 0; i < n; i++) {
    if (s[i] == ch) {
      return (void *)(s + i);
    }
  }
  return S21_NULL;
}
