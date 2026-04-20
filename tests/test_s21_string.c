#include <check.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "../s21_string.h"

// ========== Тесты из test_part1.c (переписаны на Check) ==========
START_TEST(test_strlen) {
  ck_assert_int_eq(s21_strlen(""), strlen(""));
  ck_assert_int_eq(s21_strlen("hello"), strlen("hello"));
  ck_assert_int_eq(s21_strlen("12345"), strlen("12345"));
  ck_assert_int_eq(s21_strlen("a\nb\tc"), strlen("a\nb\tc"));
}
END_TEST

START_TEST(test_strchr) {
  char str[] = "Hello, World!";
  ck_assert_ptr_eq(s21_strchr(str, 'H'), strchr(str, 'H'));
  ck_assert_ptr_eq(s21_strchr(str, 'o'), strchr(str, 'o'));
  ck_assert_ptr_eq(s21_strchr(str, '!'), strchr(str, '!'));
  ck_assert_ptr_eq(s21_strchr(str, 'z'), strchr(str, 'z'));
  ck_assert_ptr_eq(s21_strchr(str, '\0'), strchr(str, '\0'));
}
END_TEST

START_TEST(test_strrchr) {
  char str[] = "Hello, World! Hello!";
  ck_assert_ptr_eq(s21_strrchr(str, 'H'), strrchr(str, 'H'));
  ck_assert_ptr_eq(s21_strrchr(str, 'o'), strrchr(str, 'o'));
  ck_assert_ptr_eq(s21_strrchr(str, '!'), strrchr(str, '!'));
  ck_assert_ptr_eq(s21_strrchr(str, 'z'), strrchr(str, 'z'));
}
END_TEST

START_TEST(test_strpbrk) {
  char str[] = "Hello, World!";
  ck_assert_ptr_eq(s21_strpbrk(str, "H"), strpbrk(str, "H"));
  ck_assert_ptr_eq(s21_strpbrk(str, "lo"), strpbrk(str, "lo"));
  ck_assert_ptr_eq(s21_strpbrk(str, "!"), strpbrk(str, "!"));
  ck_assert_ptr_eq(s21_strpbrk(str, "xyz"), strpbrk(str, "xyz"));
}
END_TEST

START_TEST(test_strstr) {
  char str[] = "Hello, World!";
  ck_assert_ptr_eq(s21_strstr(str, "Hello"), strstr(str, "Hello"));
  ck_assert_ptr_eq(s21_strstr(str, "World"), strstr(str, "World"));
  ck_assert_ptr_eq(s21_strstr(str, "!"), strstr(str, "!"));
  ck_assert_ptr_eq(s21_strstr(str, "xyz"), strstr(str, "xyz"));
  ck_assert_ptr_eq(s21_strstr(str, ""), strstr(str, ""));
}
END_TEST

START_TEST(test_strcspn) {
  char str[] = "Hello, World!";
  ck_assert_int_eq(s21_strcspn(str, "H"), strcspn(str, "H"));
  ck_assert_int_eq(s21_strcspn(str, "lo"), strcspn(str, "lo"));
  ck_assert_int_eq(s21_strcspn(str, "!"), strcspn(str, "!"));
  ck_assert_int_eq(s21_strcspn(str, "xyz"), strcspn(str, "xyz"));
}
END_TEST

START_TEST(test_memchr) {
  char str[] = "Hello, World!";
  ck_assert_ptr_eq(s21_memchr(str, 'H', 13), memchr(str, 'H', 13));
  ck_assert_ptr_eq(s21_memchr(str, 'o', 13), memchr(str, 'o', 13));
  ck_assert_ptr_eq(s21_memchr(str, '!', 13), memchr(str, '!', 13));
  ck_assert_ptr_eq(s21_memchr(str, 'z', 13), memchr(str, 'z', 13));
}
END_TEST

// Специальные функции
START_TEST(test_to_upper) {
  char *test1 = "hello";
  char *result1 = s21_to_upper(test1);
  ck_assert_str_eq(result1, "HELLO");
  free(result1);

  char *test2 = "Hello123";
  char *result2 = s21_to_upper(test2);
  ck_assert_str_eq(result2, "HELLO123");
  free(result2);

  char *test3 = "";
  char *result3 = s21_to_upper(test3);
  ck_assert_str_eq(result3, "");
  free(result3);
}
END_TEST

START_TEST(test_to_lower) {
  char *test1 = "HELLO";
  char *result1 = s21_to_lower(test1);
  ck_assert_str_eq(result1, "hello");
  free(result1);

  char *test2 = "Hello123";
  char *result2 = s21_to_lower(test2);
  ck_assert_str_eq(result2, "hello123");
  free(result2);
}
END_TEST

START_TEST(test_insert) {
  char *src = "Hello World!";
  char *result1 = s21_insert(src, "Say ", 0);
  ck_assert_str_eq(result1, "Say Hello World!");
  free(result1);

  char *result2 = s21_insert(src, "my ", 6);
  ck_assert_str_eq(result2, "Hello my World!");
  free(result2);

  char *result3 = s21_insert(src, "!!!", 12);
  ck_assert_str_eq(result3, "Hello World!!!!");
  free(result3);
}
END_TEST

START_TEST(test_trim) {
  char *test1 = "   Hello World!   ";
  char *result1 = s21_trim(test1, NULL);
  ck_assert_str_eq(result1, "Hello World!");
  free(result1);

  char *test2 = "***Hello World!***";
  char *result2 = s21_trim(test2, "*");
  ck_assert_str_eq(result2, "Hello World!");
  free(result2);

  char *test3 = "";
  char *result3 = s21_trim(test3, NULL);
  ck_assert_str_eq(result3, "");
  free(result3);
}
END_TEST

// ========== Тесты из test_s21_string.c (второй участник) ==========
START_TEST(test_s21_memcmp) {
  char str1[] = "Hello";
  char str2[] = "Hello";
  char str3[] = "Hellp";
  ck_assert_int_eq(s21_memcmp(str1, str2, 5), memcmp(str1, str2, 5));
  ck_assert_int_eq(s21_memcmp(str1, str3, 5), memcmp(str1, str3, 5));
  ck_assert_int_eq(s21_memcmp(str1, str2, 0), memcmp(str1, str2, 0));
}
END_TEST

START_TEST(test_s21_memcpy) {
  char src[] = "Hello, World!";
  char dest1[20];
  char dest2[20];
  s21_memcpy(dest1, src, 14);
  memcpy(dest2, src, 14);
  ck_assert_mem_eq(dest1, dest2, 14);
}
END_TEST

START_TEST(test_s21_memset) {
  char str1[20] = "Hello, World!";
  char str2[20] = "Hello, World!";
  s21_memset(str1, 'X', 5);
  memset(str2, 'X', 5);
  ck_assert_mem_eq(str1, str2, 20);
  s21_memset(str1, 0, 10);
  memset(str2, 0, 10);
  ck_assert_mem_eq(str1, str2, 20);
}
END_TEST

START_TEST(test_s21_strncmp) {
  char str1[] = "Hello";
  char str2[] = "Hello";
  char str3[] = "Hellp";
  ck_assert_int_eq(s21_strncmp(str1, str2, 5), strncmp(str1, str2, 5));
  ck_assert_int_eq(s21_strncmp(str1, str3, 5), strncmp(str1, str3, 5));
  ck_assert_int_eq(s21_strncmp(str1, str2, 0), strncmp(str1, str2, 0));
}
END_TEST

START_TEST(test_s21_strncpy) {
  char src[] = "Hello";
  char dest1[10];
  char dest2[10];
  s21_strncpy(dest1, src, 6);
  strncpy(dest2, src, 6);
  ck_assert_str_eq(dest1, dest2);
  char dest3[10];
  char dest4[10];
  s21_strncpy(dest3, src, 3);
  strncpy(dest4, src, 3);
  dest3[3] = '\0';
  dest4[3] = '\0';
  ck_assert_str_eq(dest3, dest4);
}
END_TEST

START_TEST(test_s21_strncat) {
  char dest1[20] = "Hello";
  char dest2[20] = "Hello";
  char src[] = ", World!";
  s21_strncat(dest1, src, 8);
  strncat(dest2, src, 8);
  ck_assert_str_eq(dest1, dest2);
  char dest3[20] = "Hello";
  char dest4[20] = "Hello";
  s21_strncat(dest3, src, 3);
  strncat(dest4, src, 3);
  ck_assert_str_eq(dest3, dest4);
  char dest5[20] = "Test";
  char dest6[20] = "Test";
  s21_strncat(dest5, src, 0);
  strncat(dest6, src, 0);
  ck_assert_str_eq(dest5, dest6);
  char dest7[30] = "Hello";
  char dest8[30] = "Hello";
  s21_strncat(dest7, src, 20);
  strncat(dest8, src, 20);
  ck_assert_str_eq(dest7, dest8);
}
END_TEST

// ==================== ТЕСТЫ ДЛЯ SPRINTF ====================
START_TEST(test_s21_sprintf_c) {
  char buffer1[100];
  char buffer2[100];
  int len1 = s21_sprintf(buffer1, "Char: %c", 'A');
  int len2 = sprintf(buffer2, "Char: %c", 'A');
  ck_assert_str_eq(buffer1, buffer2);
  ck_assert_int_eq(len1, len2);
}
END_TEST

START_TEST(test_s21_sprintf_d) {
  char buffer1[100];
  char buffer2[100];
  int len1 = s21_sprintf(buffer1, "Number: %d", 123);
  int len2 = sprintf(buffer2, "Number: %d", 123);
  ck_assert_str_eq(buffer1, buffer2);
  ck_assert_int_eq(len1, len2);
  len1 = s21_sprintf(buffer1, "Negative: %d", -456);
  len2 = sprintf(buffer2, "Negative: %d", -456);
  ck_assert_str_eq(buffer1, buffer2);
  ck_assert_int_eq(len1, len2);
}
END_TEST

START_TEST(test_s21_sprintf_i) {
  char buffer1[100];
  char buffer2[100];
  int len1 = s21_sprintf(buffer1, "Integer: %i", 123);
  int len2 = sprintf(buffer2, "Integer: %i", 123);
  ck_assert_str_eq(buffer1, buffer2);
  ck_assert_int_eq(len1, len2);
}
END_TEST

START_TEST(test_s21_sprintf_u) {
  char buffer1[100];
  char buffer2[100];
  int len1 = s21_sprintf(buffer1, "Unsigned: %u", 123456);
  int len2 = sprintf(buffer2, "Unsigned: %u", 123456);
  ck_assert_str_eq(buffer1, buffer2);
  ck_assert_int_eq(len1, len2);
}
END_TEST

START_TEST(test_s21_sprintf_s) {
  char buffer1[100];
  char buffer2[100];
  int len1 = s21_sprintf(buffer1, "String: %s", "Hello");
  int len2 = sprintf(buffer2, "String: %s", "Hello");
  ck_assert_str_eq(buffer1, buffer2);
  ck_assert_int_eq(len1, len2);
  len1 = s21_sprintf(buffer1, "Null: %s", NULL);
  ck_assert_str_eq(buffer1, "Null: (null)");
  len1 = s21_sprintf(buffer1, "String: %20s", "Test");
  len2 = sprintf(buffer2, "String: %20s", "Test");
  ck_assert_str_eq(buffer1, buffer2);
  ck_assert_int_eq(len1, len2);
  len1 = s21_sprintf(buffer1, "String: %-20s", "Test");
  len2 = sprintf(buffer2, "String: %-20s", "Test");
  ck_assert_str_eq(buffer1, buffer2);
  ck_assert_int_eq(len1, len2);
  len1 = s21_sprintf(buffer1, "String: %.3s", "Hello");
  len2 = sprintf(buffer2, "String: %.3s", "Hello");
  ck_assert_str_eq(buffer1, buffer2);
  ck_assert_int_eq(len1, len2);
}
END_TEST

START_TEST(test_s21_sprintf_f) {
  char buffer1[100];
  char buffer2[100];
  int len1 = s21_sprintf(buffer1, "Float: %f", 3.14159);
  int len2 = sprintf(buffer2, "Float: %f", 3.14159);
  double val1, val2;
  sscanf(buffer1, "Float: %lf", &val1);
  sscanf(buffer2, "Float: %lf", &val2);
  ck_assert(fabs(val1 - val2) < 1e-6);
  ck_assert_int_eq(len1, len2);
}
END_TEST

START_TEST(test_s21_sprintf_percent) {
  char buffer1[100];
  char buffer2[100];
  int len1 = s21_sprintf(buffer1, "Percent: %%");
  int len2 = sprintf(buffer2, "Percent: %%");
  ck_assert_str_eq(buffer1, buffer2);
  ck_assert_int_eq(len1, len2);
}
END_TEST

START_TEST(test_s21_sprintf_flags) {
  char buffer1[100];
  char buffer2[100];
  int len1 = s21_sprintf(buffer1, "Plus: %+d", 123);
  int len2 = sprintf(buffer2, "Plus: %+d", 123);
  ck_assert_str_eq(buffer1, buffer2);
  ck_assert_int_eq(len1, len2);
  len1 = s21_sprintf(buffer1, "Space: % d", 123);
  len2 = sprintf(buffer2, "Space: % d", 123);
  ck_assert_str_eq(buffer1, buffer2);
  ck_assert_int_eq(len1, len2);
  len1 = s21_sprintf(buffer1, "Minus: %-10d", 123);
  len2 = sprintf(buffer2, "Minus: %-10d", 123);
  ck_assert_str_eq(buffer1, buffer2);
  ck_assert_int_eq(len1, len2);
}
END_TEST

START_TEST(test_s21_sprintf_width_precision) {
  char buffer1[100];
  char buffer2[100];
  int len1 = s21_sprintf(buffer1, "Width: %10d", 123);
  int len2 = sprintf(buffer2, "Width: %10d", 123);
  ck_assert_str_eq(buffer1, buffer2);
  ck_assert_int_eq(len1, len2);
  len1 = s21_sprintf(buffer1, "Precision: %.5d", 123);
  len2 = sprintf(buffer2, "Precision: %.5d", 123);
  ck_assert_str_eq(buffer1, buffer2);
  ck_assert_int_eq(len1, len2);
  len1 = s21_sprintf(buffer1, "Float: %10.3f", 3.14159);
  len2 = sprintf(buffer2, "Float: %10.3f", 3.14159);
  double val1, val2;
  sscanf(buffer1, "Float: %lf", &val1);
  sscanf(buffer2, "Float: %lf", &val2);
  ck_assert(fabs(val1 - val2) < 1e-3);
}
END_TEST

START_TEST(test_s21_sprintf_length_modifiers) {
  char b1[100], b2[100];

  short hs = -123;
  long ll = 123456789;

  s21_sprintf(b1, "%hd", hs);
  sprintf(b2, "%hd", hs);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%ld", ll);
  sprintf(b2, "%ld", ll);
  ck_assert_str_eq(b1, b2);
}
END_TEST

START_TEST(test_s21_sprintf_precision_zero) {
  char b1[100], b2[100];

  s21_sprintf(b1, "%.0d", 0);
  sprintf(b2, "%.0d", 0);
  ck_assert_str_eq(b1, b2);

  s21_sprintf(b1, "%.0u", 0);
  sprintf(b2, "%.0u", 0);
  ck_assert_str_eq(b1, b2);
}
END_TEST

START_TEST(test_s21_sprintf_float_rounding) {
  char b1[100], b2[100];

  s21_sprintf(b1, "%.2f", 1.999);
  sprintf(b2, "%.2f", 1.999);

  ck_assert_str_eq(b1, b2);
}
END_TEST

START_TEST(test_s21_sprintf_negative_float) {
  char b1[100], b2[100];

  s21_sprintf(b1, "%+f", -3.14);
  sprintf(b2, "%+f", -3.14);

  ck_assert_str_eq(b1, b2);
}
END_TEST

START_TEST(test_s21_sprintf_width_precision_combo) {
  char b1[100], b2[100];

  s21_sprintf(b1, "%10.5d", 123);
  sprintf(b2, "%10.5d", 123);

  ck_assert_str_eq(b1, b2);
}
END_TEST

START_TEST(test_s21_sprintf_unknown_specifier) {
  char b1[100];

  s21_sprintf(b1, "test %q");

  ck_assert_str_eq(b1, "test %q");
}
END_TEST

START_TEST(test_s21_sprintf_unsigned_edge) {
  char b1[100], b2[100];

  unsigned int val = 0;

  s21_sprintf(b1, "%u", val);
  sprintf(b2, "%u", val);

  ck_assert_str_eq(b1, b2);
}
END_TEST

START_TEST(test_s21_sprintf_float_no_precision) {
  char b1[100], b2[100];

  s21_sprintf(b1, "%.0f", 3.7);
  sprintf(b2, "%.0f", 3.7);

  ck_assert_str_eq(b1, b2);
}
END_TEST

// ========== Тесты из test_part3.c (третий участник) ==========
START_TEST(test_sscanf_basic) {
  int a1 = 0, a2 = 0;
  int b1 = 0, b2 = 0;
  int r1 = sscanf("123 456", "%d %d", &a1, &a2);
  int r2 = s21_sscanf("123 456", "%d %d", &b1, &b2);
  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(a1, b1);
  ck_assert_int_eq(a2, b2);
}
END_TEST

START_TEST(test_sscanf_string) {
  char s1[20];
  char s2[20];
  int r1 = sscanf("hello world", "%s", s1);
  int r2 = s21_sscanf("hello world", "%s", s2);
  ck_assert_int_eq(r1, r2);
  ck_assert_str_eq(s1, s2);
}
END_TEST

START_TEST(test_sscanf_width) {
  int a1 = 0;
  int a2 = 0;
  sscanf("12345", "%3d", &a1);
  s21_sscanf("12345", "%3d", &a2);
  ck_assert_int_eq(a1, a2);
}
END_TEST

START_TEST(test_sscanf_char) {
  char c1;
  char c2;
  sscanf("abc", "%c", &c1);
  s21_sscanf("abc", "%c", &c2);
  ck_assert_int_eq(c1, c2);
}
END_TEST

START_TEST(test_sscanf_octal) {
  int a1 = 0, a2 = 0;
  int r1 = sscanf("123", "%o", &a1);
  int r2 = s21_sscanf("123", "%o", &a2);
  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(a1, a2);
}
END_TEST

START_TEST(test_sscanf_hex) {
  int a1 = 0, a2 = 0;
  int r1 = sscanf("2fA", "%x", &a1);
  int r2 = s21_sscanf("2fA", "%x", &a2);
  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(a1, a2);
}
END_TEST

START_TEST(test_sscanf_auto_base) {
  int a1 = 0, a2 = 0, a3 = 0, a4 = 0;
  int r1 = sscanf("0x10", "%i", &a1);
  int r2 = s21_sscanf("0x10", "%i", &a2);
  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(a1, a2);

  int r3 = sscanf("010", "%i", &a3);
  int r4 = s21_sscanf("010", "%i", &a4);
  ck_assert_int_eq(r3, r4);
  ck_assert_int_eq(a3, a4);
}
END_TEST

START_TEST(test_sscanf_float) {
  float f1 = 0, f2 = 0;
  int r1 = sscanf("123.456", "%f", &f1);
  int r2 = s21_sscanf("123.456", "%f", &f2);
  ck_assert_int_eq(r1, r2);
  ck_assert_float_eq(f1, f2);
}
END_TEST

START_TEST(test_sscanf_exponent) {
  float f1 = 0, f2 = 0;
  int r1 = sscanf("10e4", "%e", &f1);
  int r2 = s21_sscanf("10e4", "%e", &f2);
  ck_assert_int_eq(r1, r2);
  ck_assert_float_eq(f1, f2);
}
END_TEST

START_TEST(test_sscanf_tricky_exponent) {
  float f1 = 0, f2 = 0;
  int r1 = sscanf("1.6e-1", "%4f", &f1);
  int r2 = s21_sscanf("1.6e-1", "%4f", &f2);
  ck_assert_int_eq(r1, r2);
  ck_assert_float_eq(f1, f2);
}
END_TEST

START_TEST(test_sscanf_double) {
  double d1 = 0, d2 = 0;
  int r1 = sscanf("123.456", "%lf", &d1);
  int r2 = s21_sscanf("123.456", "%lf", &d2);
  ck_assert_int_eq(r1, r2);
  ck_assert_double_eq(d1, d2);
}
END_TEST

START_TEST(test_sscanf_long_double) {
  long double d1 = 0;
  long double d2 = 0;

  int r1 = sscanf("123.456", "%Lf", &d1);
  int r2 = s21_sscanf("123.456", "%Lf", &d2);

  ck_assert_int_eq(r1, r2);
  ck_assert_ldouble_eq_tol(d1, d2, 1e-6);
}
END_TEST

START_TEST(test_sscanf_short) {
  short a1 = 0, a2 = 0;
  int r1 = sscanf("123", "%hd", &a1);
  int r2 = s21_sscanf("123", "%hd", &a2);
  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(a1, a2);
}
END_TEST

START_TEST(test_sscanf_long) {
  long a1 = 0, a2 = 0;
  int r1 = sscanf("123456", "%ld", &a1);
  int r2 = s21_sscanf("123456", "%ld", &a2);
  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(a1, a2);
}
END_TEST

START_TEST(test_sscanf_plus) {
  int a1 = 0, a2 = 0;
  sscanf("+123", "%d", &a1);
  s21_sscanf("+123", "%d", &a2);
  ck_assert_int_eq(a1, a2);
}
END_TEST

START_TEST(test_sscanf_minus) {
  int a1 = 0, a2 = 0;
  sscanf("-123", "%d", &a1);
  s21_sscanf("-123", "%d", &a2);
  ck_assert_int_eq(a1, a2);
}
END_TEST

START_TEST(test_sscanf_spaces) {
  int a1 = 0, a2 = 0;
  sscanf("   123", "%d", &a1);
  s21_sscanf("   123", "%d", &a2);
  ck_assert_int_eq(a1, a2);
}
END_TEST

START_TEST(test_sscanf_empty) {
  int a1 = 5, a2 = 5;
  int r1 = sscanf("", "%d", &a1);
  int r2 = s21_sscanf("", "%d", &a2);
  ck_assert_int_eq(r1, r2);
}
END_TEST

START_TEST(test_sscanf_suppression) {
  int a1 = 0, a2 = 0;
  int r1 = sscanf("123 456", "%*d %d", &a1);
  int r2 = s21_sscanf("123 456", "%*d %d", &a2);
  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(a1, a2);
}
END_TEST

START_TEST(test_sscanf_multiple) {
  int a1, b1, a2, b2;
  int r1 = sscanf("123 456", "%d %d", &a1, &b1);
  int r2 = s21_sscanf("123 456", "%d %d", &a2, &b2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(a1, a2);
  ck_assert_int_eq(b1, b2);
}
END_TEST

START_TEST(test_sscanf_n) {
  int n1 = 0, n2 = 0;
  int a1 = 0, a2 = 0;

  sscanf("123 abc", "%d %n", &a1, &n1);
  s21_sscanf("123 abc", "%d %n", &a2, &n2);

  ck_assert_int_eq(a1, a2);
  ck_assert_int_eq(n1, n2);
}
END_TEST

START_TEST(test_sscanf_pointer) {
  void *p1 = NULL;
  void *p2 = NULL;

  int r1 = sscanf("0x1234", "%p", &p1);
  int r2 = s21_sscanf("0x1234", "%p", &p2);

  ck_assert_int_eq(r1, r2);
}
END_TEST

START_TEST(test_sscanf_string_width) {
  char s1[10], s2[10];

  sscanf("hello", "%3s", s1);
  s21_sscanf("hello", "%3s", s2);

  ck_assert_str_eq(s1, s2);
}
END_TEST

START_TEST(test_sscanf_string_length) {
  wchar_t s1[10];
  wchar_t s2[10];

  int r1 = sscanf("abc", "%ls", s1);
  int r2 = s21_sscanf("abc", "%ls", s2);

  ck_assert_int_eq(r1, r2);
  ck_assert_mem_eq(s1, s2, sizeof(wchar_t) * 4);

  wchar_t s3[10];
  wchar_t s4[10];

  int r3 = sscanf("abcde", "%3ls", s3);
  int r4 = s21_sscanf("abcde", "%3ls", s4);

  ck_assert_int_eq(r3, r4);
  ck_assert_mem_eq(s3, s4, sizeof(wchar_t) * 4);
}
END_TEST

START_TEST(test_sscanf_char_width) {
  char s1[4] = {0};
  char s2[4] = {0};

  sscanf("abcd", "%3c", s1);
  s21_sscanf("abcd", "%3c", s2);

  ck_assert_mem_eq(s1, s2, 3);
}
END_TEST

START_TEST(test_sscanf_char_length) {
  wchar_t s1 = 0;
  wchar_t s2 = 0;

  int r1 = sscanf("ab", "%lc", &s1);
  int r2 = s21_sscanf("ab", "%lc", &s2);

  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(s1, s2);

  wchar_t s3[2] = {0};
  wchar_t s4[2] = {0};

  int r3 = sscanf("ab", "%2lc", s3);
  int r4 = s21_sscanf("ab", "%2lc", s4);

  ck_assert_int_eq(r3, r4);
  ck_assert_mem_eq(s3, s4, sizeof(wchar_t) * 2);
}
END_TEST

START_TEST(test_strerror_known) {
  for (int i = 0; i < 140; i++) {
    ck_assert_str_eq(strerror(i), s21_strerror(i));
  }
}
END_TEST

START_TEST(test_strerror_unknown) {
  ck_assert_str_eq(strerror(999), s21_strerror(999));
  ck_assert_str_eq(strerror(-1), s21_strerror(-1));
}
END_TEST

START_TEST(test_strtok_basic) {
  char s1[] = "one two three";
  char s2[] = "one two three";
  char *t1 = strtok(s1, " ");
  char *t2 = s21_strtok(s2, " ");
  ck_assert_str_eq(t1, t2);
}
END_TEST

START_TEST(test_strtok_multiple) {
  char s1[] = "a b c";
  char s2[] = "a b c";
  char *t1 = strtok(s1, " ");
  char *t2 = s21_strtok(s2, " ");
  ck_assert_str_eq(t1, t2);
  t1 = strtok(NULL, " ");
  t2 = s21_strtok(NULL, " ");
  ck_assert_str_eq(t1, t2);
}
END_TEST

START_TEST(test_strtok_delims) {
  char s1[] = "a,b;c";
  char s2[] = "a,b;c";
  char *t1 = strtok(s1, ",;");
  char *t2 = s21_strtok(s2, ",;");
  ck_assert_str_eq(t1, t2);
}
END_TEST

START_TEST(test_strtok_empty) {
  char s1[] = "";
  char s2[] = "";
  ck_assert_ptr_eq(strtok(s1, ","), s21_strtok(s2, ","));
}
END_TEST

// ========== Создание общего suite ==========
Suite *s21_string_suite(void) {
  Suite *s = suite_create("s21_string");
  TCase *tc_part1 = tcase_create("Part1");
  tcase_add_test(tc_part1, test_strlen);
  tcase_add_test(tc_part1, test_strchr);
  tcase_add_test(tc_part1, test_strrchr);
  tcase_add_test(tc_part1, test_strpbrk);
  tcase_add_test(tc_part1, test_strstr);
  tcase_add_test(tc_part1, test_strcspn);
  tcase_add_test(tc_part1, test_memchr);
  suite_add_tcase(s, tc_part1);

  TCase *tc_special = tcase_create("Special");
  tcase_add_test(tc_special, test_to_upper);
  tcase_add_test(tc_special, test_to_lower);
  tcase_add_test(tc_special, test_insert);
  tcase_add_test(tc_special, test_trim);
  suite_add_tcase(s, tc_special);

  TCase *tc_memory = tcase_create("Memory");
  tcase_add_test(tc_memory, test_s21_memcmp);
  tcase_add_test(tc_memory, test_s21_memcpy);
  tcase_add_test(tc_memory, test_s21_memset);
  suite_add_tcase(s, tc_memory);

  TCase *tc_string2 = tcase_create("String2");
  tcase_add_test(tc_string2, test_s21_strncmp);
  tcase_add_test(tc_string2, test_s21_strncpy);
  tcase_add_test(tc_string2, test_s21_strncat);
  suite_add_tcase(s, tc_string2);

  TCase *tc_sprintf = tcase_create("Sprintf");
  tcase_add_test(tc_sprintf, test_s21_sprintf_c);
  tcase_add_test(tc_sprintf, test_s21_sprintf_d);
  tcase_add_test(tc_sprintf, test_s21_sprintf_i);
  tcase_add_test(tc_sprintf, test_s21_sprintf_u);
  tcase_add_test(tc_sprintf, test_s21_sprintf_s);
  tcase_add_test(tc_sprintf, test_s21_sprintf_f);
  tcase_add_test(tc_sprintf, test_s21_sprintf_percent);
  tcase_add_test(tc_sprintf, test_s21_sprintf_flags);
  tcase_add_test(tc_sprintf, test_s21_sprintf_width_precision);
  tcase_add_test(tc_sprintf, test_s21_sprintf_length_modifiers);
  tcase_add_test(tc_sprintf, test_s21_sprintf_precision_zero);
  tcase_add_test(tc_sprintf, test_s21_sprintf_float_rounding);
  tcase_add_test(tc_sprintf, test_s21_sprintf_negative_float);
  tcase_add_test(tc_sprintf, test_s21_sprintf_width_precision_combo);
  tcase_add_test(tc_sprintf, test_s21_sprintf_unknown_specifier);
  tcase_add_test(tc_sprintf, test_s21_sprintf_unsigned_edge);
  tcase_add_test(tc_sprintf, test_s21_sprintf_float_no_precision);
  suite_add_tcase(s, tc_sprintf);

  TCase *tc_sscanf = tcase_create("Sscanf");
  tcase_add_test(tc_sscanf, test_sscanf_basic);
  tcase_add_test(tc_sscanf, test_sscanf_string);
  tcase_add_test(tc_sscanf, test_sscanf_string_length);
  tcase_add_test(tc_sscanf, test_sscanf_width);
  tcase_add_test(tc_sscanf, test_sscanf_char);
  tcase_add_test(tc_sscanf, test_sscanf_char_length);
  tcase_add_test(tc_sscanf, test_sscanf_octal);
  tcase_add_test(tc_sscanf, test_sscanf_hex);
  tcase_add_test(tc_sscanf, test_sscanf_auto_base);
  tcase_add_test(tc_sscanf, test_sscanf_float);
  tcase_add_test(tc_sscanf, test_sscanf_exponent);
  tcase_add_test(tc_sscanf, test_sscanf_tricky_exponent);
  tcase_add_test(tc_sscanf, test_sscanf_double);
  tcase_add_test(tc_sscanf, test_sscanf_long_double);
  tcase_add_test(tc_sscanf, test_sscanf_short);
  tcase_add_test(tc_sscanf, test_sscanf_long);
  tcase_add_test(tc_sscanf, test_sscanf_plus);
  tcase_add_test(tc_sscanf, test_sscanf_minus);
  tcase_add_test(tc_sscanf, test_sscanf_spaces);
  tcase_add_test(tc_sscanf, test_sscanf_empty);
  tcase_add_test(tc_sscanf, test_sscanf_suppression);
  tcase_add_test(tc_sscanf, test_sscanf_multiple);
  tcase_add_test(tc_sscanf, test_sscanf_n);
  tcase_add_test(tc_sscanf, test_sscanf_pointer);
  tcase_add_test(tc_sscanf, test_sscanf_string_width);
  tcase_add_test(tc_sscanf, test_sscanf_char_width);
  suite_add_tcase(s, tc_sscanf);

  TCase *tc_strtok = tcase_create("Strtok");
  tcase_add_test(tc_strtok, test_strtok_basic);
  tcase_add_test(tc_strtok, test_strtok_multiple);
  tcase_add_test(tc_strtok, test_strtok_delims);
  tcase_add_test(tc_strtok, test_strtok_empty);
  suite_add_tcase(s, tc_strtok);

  TCase *tc_strerror = tcase_create("Strerror");
  tcase_add_test(tc_strerror, test_strerror_known);
  tcase_add_test(tc_strerror, test_strerror_unknown);
  suite_add_tcase(s, tc_strerror);

  return s;
}

int main(void) {
  int number_failed;
  Suite *s = s21_string_suite();
  SRunner *sr = srunner_create(s);
  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return number_failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
