#include "s21_string_part3_helper_functions.h"

int is_delimiter(char c, const char *delim) {
  for (s21_size_t i = 0; delim[i] != '\0'; i++) {
    if (c == delim[i]) {
      return 1;
    }
  }
  return 0;
}

char *find_token_start(char *str, const char *delim) {
  while (1) {
    if (*str == '\0') {
      return S21_NULL;
    }

    if (is_delimiter(*str, delim)) {
      str++;
    } else {
      return str;
    }
  }
}

char *find_token_end(char *start, const char *delim, char **next_ptr) {
  char *str = start;

  while (1) {
    if (*str == '\0') {
      *next_ptr = S21_NULL;
      return str;
    }

    if (is_delimiter(*str, delim)) {
      *next_ptr = str + 1;
      return str;
    }

    str++;
  }
}

int sign_finder(const char **str_ptr) {
  int sign = 1;
  if (**str_ptr == '-') {
    sign = -1;
    (*str_ptr)++;
  } else if (**str_ptr == '+') {
    (*str_ptr)++;
  }
  return sign;
}

double my_pow(double num, int power) {
  double result = 1.0;
  int pow = power;

  if (pow < 0) {
    pow = -pow;
    num = 1.0 / num;
  }

  for (int i = 0; i < pow; i++) {
    result *= num;
  }

  return result;
}

int is_digit(char c) {
  if (c >= '0' && c <= '9') {
    return 1;
  } else {
    return 0;
  }
}

int is_whitespace(char c) {
  if (c == ' ' || c == '\n' || c == '\t' || c == '\r') {
    return 1;
  } else {
    return 0;
  }
}

int is_A_to_F_letter(char c) {
  if ((c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f')) {
    return 1;
  } else {
    return 0;
  }
}

int is_valid_digit_char(char c) {
  if (is_digit(c) || c == '-' || c == '+' || is_A_to_F_letter(c)) {
    return 1;
  } else {
    return 0;
  }
}

int is_specifier(char c) {
  if ((c >= 'c' && c <= 'g') || c == 'E' || c == 'G' || c == 'i' || c == 'o' ||
      c == 's' || c == 'u' || c == 'x' || c == 'X' || c == 'p' || c == 'n') {
    return 1;
  } else {
    return 0;
  }
}

int base_detector(const char **str_ptr) {
  int base;
  if (**str_ptr == '0') {
    if (*(*str_ptr + 1) == 'x' || *(*str_ptr + 1) == 'X') {
      base = 16;
      (*str_ptr) += 2;
    } else {
      base = 8;
      (*str_ptr)++;
    }
  } else {
    base = 10;
  }

  return base;
}

void skip_whitespaces(const char **str_ptr, const char **format_ptr) {
  while (is_whitespace(**format_ptr) && **format_ptr != '\0') {
    (*format_ptr)++;
  }
  while ((is_whitespace(**str_ptr)) && **str_ptr != '\0') {
    (*str_ptr)++;
  }
}

void str_parser(const char **str_ptr, char **final_str, int width) {
  int final_str_len = 0;
  if (!width) {
    while (**str_ptr != ' ' && **str_ptr != '\n' && **str_ptr != '\t' &&
           **str_ptr != '\r' && **str_ptr != '\0') {
      (*final_str)[final_str_len] = **str_ptr;
      // DEBUG
      // printf("char_1: %c\n", (*final_str)[final_str_len]);
      //
      (*str_ptr)++;
      final_str_len++;
    }
  } else {
    while ((**str_ptr != ' ' && **str_ptr != '\n' && **str_ptr != '\t' &&
            **str_ptr != '\r' && **str_ptr != '\0') &&
           final_str_len < width) {
      (*final_str)[final_str_len] = **str_ptr;
      // DEBUG
      // printf("char_w: %c\n", (*final_str)[final_str_len]);
      // DEBUG
      (*str_ptr)++;
      final_str_len++;
    }
  }

  (*final_str)[final_str_len] = '\0';
}

void wide_str_parser(const char **str_ptr, wchar_t **final_str, int width) {
  int final_str_len = 0;
  if (!width) {
    while (**str_ptr != ' ' && **str_ptr != '\n' && **str_ptr != '\t' &&
           **str_ptr != '\r' && **str_ptr != '\0') {
      (*final_str)[final_str_len] = (wchar_t)(unsigned char)**str_ptr;
      (*str_ptr)++;
      final_str_len++;
    }
  } else {
    while ((**str_ptr != ' ' && **str_ptr != '\n' && **str_ptr != '\t' &&
            **str_ptr != '\r' && **str_ptr != '\0') &&
           final_str_len < width) {
      (*final_str)[final_str_len] = (wchar_t)(unsigned char)**str_ptr;
      (*str_ptr)++;
      final_str_len++;
    }
  }

  (*final_str)[final_str_len] = L'\0';
}

void char_parser(const char **str_ptr, char **record_char, int width) {
  if (width) {
    int i = 0;
    while (**str_ptr != '\0' && i < width) {
      (*record_char)[i] = **str_ptr;
      i++;
      (*str_ptr)++;
    }
  } else {
    **record_char = **str_ptr;
    (*str_ptr)++;
  }
}

void wide_char_parser(const char **str_ptr, wchar_t **record_char, int width) {
  if (width) {
    int i = 0;
    while (**str_ptr != '\0' && i < width) {
      (*record_char)[i] = (wchar_t)(unsigned char)**str_ptr;
      i++;
      (*str_ptr)++;
    }
  } else {
    **record_char = (wchar_t)(unsigned char)**str_ptr;
    (*str_ptr)++;
  }
}

void digit_str_parser(const char **str_ptr, char **final_str, int width) {
  int final_str_len = 0;
  if (!width) {
    while ((!is_whitespace(**str_ptr) && **str_ptr != '\0') &&
           is_valid_digit_char(**str_ptr)) {
      (*final_str)[final_str_len] = **str_ptr;
      // DEBUG
      // printf("char_1: %c\n", (*final_str)[final_str_len]);
      //
      (*str_ptr)++;
      final_str_len++;
    }
  } else {
    while (final_str_len < width &&
           (!is_whitespace(**str_ptr) && **str_ptr != '\0') &&
           is_valid_digit_char(**str_ptr)) {
      (*final_str)[final_str_len] = **str_ptr;
      // DEBUG
      // printf("char_w: %c\n", (*final_str)[final_str_len]);
      //
      (*str_ptr)++;
      final_str_len++;
    }
  }

  (*final_str)[final_str_len] = '\0';
}

int digit_parser(const char **str_ptr, int base, int width) {
  int value = 0;
  int digit;

  char digit_str[128];
  char *ptr = digit_str;
  digit_str_parser(str_ptr, &ptr, width);
  // //DEBUG
  // printf("str_ptr: %s\n", *str_ptr);
  // printf("digit ptr: %s\n", ptr);

  if (base == 10 || base == 8) {
    char limit = (base - 1) + '0';
    while (*ptr >= '0' && *ptr <= limit) {
      value = value * base + (*ptr - '0');
      ptr++;
    }
  } else if (base == 16) {
    while (is_digit(*ptr) || is_A_to_F_letter(*ptr)) {
      if (is_digit(*ptr))
        digit = *ptr - '0';
      else if (*ptr >= 'A' && *ptr <= 'F')
        digit = *ptr - 'A' + 10;
      else if (*ptr >= 'a' && *ptr <= 'f')
        digit = *ptr - 'a' + 10;
      value = value * base + digit;
      ptr++;
    }
  }
  return value;
}

int number_parser(const char **str_ptr, char specifier, int width) {
  int base;
  int value;
  if (specifier == 'd' || specifier == 'u') {
    base = 10;
    value = digit_parser(str_ptr, base, width);
  } else if (specifier == 'i') {
    base = base_detector(str_ptr);
    value = digit_parser(str_ptr, base, width);
  } else if (specifier == 'o') {
    base = 8;
    value = digit_parser(str_ptr, base, width);
  } else if (specifier == 'X' || specifier == 'x') {
    base = 16;
    if (**str_ptr == '0' &&
        (*(*str_ptr + 1) == 'x' || *(*str_ptr + 1) == 'X')) {
      *str_ptr += 2;
    }
    value = digit_parser(str_ptr, base, width);
  }
  return value;
}

Format format_parser(const char **format_ptr) {
  Format current_format = {0};
  if (**format_ptr == '%' && !is_whitespace(*(*format_ptr + 1)) &&
      *(*format_ptr + 1) != '\0') {
    (*format_ptr)++;

    if (is_digit(**format_ptr)) {
      int value = 0;
      while (is_digit(**format_ptr)) {
        value = value * 10 + (**format_ptr - '0');
        (*format_ptr)++;
      }
      current_format.width = value;
    }

    if (**format_ptr == '*') {
      current_format.suppress = 1;
      (*format_ptr)++;
    }
    if (**format_ptr == 'l' || **format_ptr == 'L' || **format_ptr == 'h') {
      current_format.length = **format_ptr;
      (*format_ptr)++;
    }
    if (is_specifier(**format_ptr)) {
      current_format.specifier = **format_ptr;
      (*format_ptr)++;
    }
  } else {
    current_format.specifier = 'Q';
  }
  return current_format;
}

int exponent_finder(char **str_ptr, int *success_flag) {
  int exponent = 0;
  if (**str_ptr == 'e' || **str_ptr == 'E') {
    (*str_ptr)++;
    // if e has +- and number OR just number = execute the code
    if ((is_digit(**str_ptr)) || ((**str_ptr == '-' || **str_ptr == '+') &&
                                  (is_digit(*(*str_ptr + 1))))) {
      int sign = 1;
      if (**str_ptr == '-') {
        sign = -1;
        (*str_ptr)++;
      } else if (**str_ptr == '+') {
        (*str_ptr)++;
      }
      while (is_digit(**str_ptr)) {
        exponent = exponent * 10 + (**str_ptr - '0');
        (*str_ptr)++;
      }
      exponent *= sign;
    } else {
      *success_flag = 0;
    }
  } else {
    exponent = 0;
  }
  return exponent;
}

void float_str_parser(const char **str_ptr, char **final_str, int width) {
  int final_str_len = 0;
  if (!width) {
    while ((!is_whitespace(**str_ptr) && **str_ptr != '\0') &&
           (is_digit(**str_ptr) || **str_ptr == 'e' || **str_ptr == 'E' ||
            **str_ptr == '.' || **str_ptr == '-' || **str_ptr == '+')) {
      (*final_str)[final_str_len] = **str_ptr;
      // DEBUG
      // printf("char_1: %c\n", (*final_str)[final_str_len]);
      //
      (*str_ptr)++;
      final_str_len++;
    }
  } else {
    while (final_str_len < width &&
           (!is_whitespace(**str_ptr) && **str_ptr != '\0') &&
           (is_digit(**str_ptr) || **str_ptr == 'e' || **str_ptr == 'E' ||
            **str_ptr == '.' || **str_ptr == '-' || **str_ptr == '+')) {
      (*final_str)[final_str_len] = **str_ptr;
      // DEBUG
      // printf("char_w: %c\n", (*final_str)[final_str_len]);
      //
      (*str_ptr)++;
      final_str_len++;
    }
  }

  (*final_str)[final_str_len] = '\0';
}

double float_parser(const char **str_ptr, int width,
                    int *exponent_parsed_flag) {
  double value;
  int int_part = 0;
  double double_part = 0;
  double power = -1;
  int exponent = 0;
  char float_str[128];
  char *ptr = float_str;
  float_str_parser(str_ptr, &ptr, width);
  // DEBUG
  // printf("ptr: %s\n", ptr);
  // DEBUG
  char *backup = ptr;

  while ((is_digit(*ptr)) && *ptr != '.') {
    int_part = int_part * 10 + (*ptr - '0');
    ptr++;
  }

  if (*ptr == '.') {
    ptr++;
  }

  while (is_digit(*ptr)) {
    double_part = double_part + (*ptr - '0') * my_pow(10, power);
    power--;
    ptr++;
  }

  exponent = exponent_finder(&ptr, exponent_parsed_flag);
  if (*exponent_parsed_flag == 0) {
    ptr = backup;
    return 0;
  }
  value = (int_part + double_part) * my_pow(10, exponent);
  return value;
}
