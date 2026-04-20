#include <ctype.h>

#include "s21_string.h"

void *s21_to_upper(const char *str) {
  if (!str) return S21_NULL;
  s21_size_t len = s21_strlen(str);
  char *result = (char *)malloc(len + 1);
  if (!result) return S21_NULL;
  for (s21_size_t i = 0; i <= len; i++) {
    if (i < len) {
      result[i] = (char)toupper((unsigned char)str[i]);
    } else {
      result[i] = '\0';
    }
  }
  return result;
}

void *s21_to_lower(const char *str) {
  if (!str) return S21_NULL;
  s21_size_t len = s21_strlen(str);
  char *result = (char *)malloc(len + 1);
  if (!result) return S21_NULL;
  for (s21_size_t i = 0; i <= len; i++) {
    if (i < len) {
      result[i] = (char)tolower((unsigned char)str[i]);
    } else {
      result[i] = '\0';
    }
  }
  return result;
}

void *s21_insert(const char *src, const char *str, s21_size_t start_index) {
  if (!src || !str) return S21_NULL;
  s21_size_t src_len = s21_strlen(src);
  s21_size_t str_len = s21_strlen(str);
  if (start_index > src_len) return S21_NULL;
  char *result = (char *)malloc(src_len + str_len + 1);
  if (!result) return S21_NULL;
  s21_size_t i = 0;
  for (; i < start_index; i++) {
    result[i] = src[i];
  }
  for (s21_size_t j = 0; j < str_len; j++, i++) {
    result[i] = str[j];
  }
  for (s21_size_t j = start_index; j <= src_len; j++, i++) {
    result[i] = src[j];
  }

  return result;
}

void *s21_trim(const char *src, const char *trim_chars) {
  if (!src) return S21_NULL;
  s21_size_t src_len = s21_strlen(src);
  if (src_len == 0) {
    char *result = (char *)malloc(1);
    if (result) result[0] = '\0';
    return result;
  }
  const char *default_chars = " \t\n\r\f\v";
  const char *chars_to_trim = trim_chars ? trim_chars : default_chars;
  s21_size_t start = 0;
  while (start < src_len) {
    int found = 0;
    for (const char *c = chars_to_trim; *c != '\0'; c++) {
      if (src[start] == *c) {
        found = 1;
        break;
      }
    }
    if (!found) break;
    start++;
  }
  s21_size_t end = src_len - 1;
  while (end > start) {
    int found = 0;
    for (const char *c = chars_to_trim; *c != '\0'; c++) {
      if (src[end] == *c) {
        found = 1;
        break;
      }
    }
    if (!found) break;
    end--;
  }
  s21_size_t result_len = (end >= start) ? (end - start + 1) : 0;
  char *result = (char *)malloc(result_len + 1);
  if (!result) return S21_NULL;
  for (s21_size_t i = 0; i < result_len; i++) {
    result[i] = src[start + i];
  }
  result[result_len] = '\0';
  return result;
}
