#include "s21_string.h"
#include "s21_string_part3_helper_functions.h"

char *s21_strerror(int errnum) {
  static char buffer[256];

#ifdef __linux__
  static const char *errors[] = S21_LINUX_ERRORS;
  const int max_err = S21_LINUX_ERR_COUNT;
#elif __APPLE__
  static const char *errors[] = S21_APPLE_ERRORS;
  const int max_err = S21_APPLE_ERR_COUNT;
#endif

  if (errnum >= 0 && errnum < max_err) {
    return (char *)errors[errnum];
  }

  s21_sprintf(buffer, "Unknown error %d", errnum);
  return buffer;
}

char *s21_strtok(char *str, const char *delim) {
  static char *buffer = S21_NULL;
  if (!str && buffer) {
    str = buffer;
  }

  if (!str) {
    return S21_NULL;
  }

  // find token start
  char *token_start = find_token_start(str, delim);
  if (!token_start) {
    buffer = S21_NULL;
    return S21_NULL;
  }

  // find token end
  char *next_ptr = S21_NULL;
  char *token_end = find_token_end(token_start, delim, &next_ptr);
  *token_end = '\0';

  buffer = next_ptr;

  return token_start;
}

int s21_sscanf(const char *str, const char *format, ...) {
  int success_count = 0;

  if (*str == '\0') {
    success_count = -1;
    return success_count;
  }

  va_list args;
  va_start(args, format);

  const char *str_ptr = str;
  const char *format_ptr = format;
  const char *start_for_n = str;
  while (*format_ptr != '\0' && *str_ptr != '\0') {
    if (is_whitespace(*format_ptr)) {
      skip_whitespaces(&str_ptr, &format_ptr);
      continue;
    }

    if (*format_ptr != '%') {
      if (*format_ptr != *str_ptr) break;

      format_ptr++;
      str_ptr++;
      continue;
    }

    Format current_format = format_parser(&format_ptr);
    if (current_format.specifier != 'c' && current_format.specifier != 'n') {
      while (is_whitespace(*str_ptr) && *str_ptr != '\0') {
        str_ptr++;
      }
    }

    switch (current_format.specifier) {
      //%d parser
      case 'd':
      //%i parser
      case 'i':
      //%o parser
      case 'o':
      //%u parser
      case 'u':
      //%x and %X parser
      case 'x':
      case 'X':
        int sign = sign_finder(&str_ptr);
        int value = number_parser(&str_ptr, current_format.specifier,
                                  current_format.width);
        int result = sign * value;

        if (!current_format.suppress) {
          if (current_format.specifier == 'd' ||
              current_format.specifier == 'i') {
            switch (current_format.length) {
              case 'h': {
                short *record_int = va_arg(args, short *);
                *record_int = (short)(result);
                break;
              }
              case 'l': {
                long *record_int = va_arg(args, long *);
                *record_int = (long)(result);
                break;
              }
              default: {
                int *record_int = va_arg(args, int *);
                *record_int = result;
                break;
              }
            }
          } else if (current_format.specifier == 'o' ||
                     current_format.specifier == 'u' ||
                     current_format.specifier == 'X' ||
                     current_format.specifier == 'x') {
            switch (current_format.length) {
              case 'h': {
                unsigned short *record_int = va_arg(args, unsigned short *);
                *record_int = (unsigned short)(result);
                break;
              }
              case 'l': {
                unsigned long *record_int = va_arg(args, unsigned long *);
                *record_int = (unsigned long)(result);
                break;
              }
              default: {
                unsigned int *record_int = va_arg(args, unsigned int *);
                *record_int = (unsigned int)result;
                break;
              }
            }
          }
          success_count++;
        }
        break;

      //%e %E %f %g %G parser
      case 'e':
      case 'E':
      case 'f':
      case 'g':
      case 'G':
        int sign_float = sign_finder(&str_ptr);
        int exponent_parsed_flag = 1;
        double value_float =
            float_parser(&str_ptr, current_format.width, &exponent_parsed_flag);
        double result_float = sign_float * value_float;
        if (!exponent_parsed_flag) break;
        if (!current_format.suppress) {
          switch (current_format.length) {
            case 'L': {
              long double *record_float = va_arg(args, long double *);
              *record_float = (long double)(result_float);
              break;
            }
            case 'l': {
              double *record_double = va_arg(args, double *);
              *record_double = (double)result_float;
              break;
            }
            default: {
              float *record_float = va_arg(args, float *);
              *record_float = (float)(result_float);
              break;
            }
          }
          success_count++;
        }
        break;
      //%s parser
      case 's':
        if (current_format.length == 'l') {
          wchar_t *record_str = va_arg(args, wchar_t *);
          wide_str_parser(&str_ptr, &record_str, current_format.width);
          success_count++;
        } else {
          char *record_str = va_arg(args, char *);
          str_parser(&str_ptr, &record_str, current_format.width);
          success_count++;
        }
        break;
      //%c parser
      case 'c':
        if (current_format.length == 'l') {
          wchar_t *record_char = va_arg(args, wchar_t *);
          wide_char_parser(&str_ptr, &record_char, current_format.width);
          success_count++;
        } else {
          char *record_char = va_arg(args, char *);
          char_parser(&str_ptr, &record_char, current_format.width);
          success_count++;
        }
        break;
      //%p parser
      case 'p':
        void **record_pointer = va_arg(args, void **);
        if (*str_ptr == '0' &&
            (*(str_ptr + 1) == 'x' || *(str_ptr + 1) == 'X')) {
          str_ptr += 2;
        }

        unsigned long value_p =
            digit_parser(&str_ptr, 16, current_format.width);

        *record_pointer = (void *)value_p;
        success_count++;
        break;
      //%n parser
      case 'n':
        int *record_n = va_arg(args, int *);
        *record_n = str_ptr - start_for_n;
        break;
      case 'Q':
        // ERROR MESSAGE HERE
        // printf("EEE");
        va_end(args);
        return success_count;
        break;
      default:
        break;
    }
  }
  va_end(args);
  return success_count;
}
