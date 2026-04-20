#include "s21_string.h"

// ==================== ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ====================

static double s21_pow10(int n) {
  double result = 1.0;
  if (n >= 0) {
    for (int i = 0; i < n; i++) {
      result *= 10.0;
    }
  } else {
    for (int i = 0; i < -n; i++) {
      result /= 10.0;
    }
  }
  return result;
}

// Проверка на NaN (Not a Number)
static int s21_isnan(double x) {
  // В IEEE 754 NaN не равен самому себе
  return (x != x);
}

// Проверка на бесконечность
static int s21_isinf(double x) {
  // Очень большие числа считаем бесконечностью (упрощённая проверка)
  volatile double tmp = x;
  if (tmp > 1e308 || tmp < -1e308) {
    return 1;
  }
  return 0;
}

// ==================== СТРУКТУРА ДЛЯ ПАРАМЕТРОВ SPRINTF ====================

typedef struct {
  int minus;         // флаг '-'
  int plus;          // флаг '+'
  int space;         // флаг пробел
  int width;         // минимальная ширина поля
  int precision;     // точность
  int is_precision;  // установлена ли точность
  char length;       // модификатор длины (h, l, L)
  char specifier;    // спецификатор (c, d, i, f, s, u, %)
} s21_sprintf_params;

// ==================== ПАРСИНГ ПАРАМЕТРОВ ====================

static void parse_flags(const char **format, s21_sprintf_params *params) {
  while (1) {
    switch (**format) {
      case '-':
        params->minus = 1;
        break;
      case '+':
        params->plus = 1;
        break;
      case ' ':
        params->space = 1;
        break;
      default:
        return;
    }
    (*format)++;
  }
}

static void parse_width(const char **format, s21_sprintf_params *params) {
  params->width = 0;
  while (**format >= '0' && **format <= '9') {
    params->width = params->width * 10 + (**format - '0');
    (*format)++;
  }
}

static void parse_precision(const char **format, s21_sprintf_params *params) {
  if (**format == '.') {
    (*format)++;
    params->is_precision = 1;
    params->precision = 0;

    while (**format >= '0' && **format <= '9') {
      params->precision = params->precision * 10 + (**format - '0');
      (*format)++;
    }
  }
}

static void parse_length(const char **format, s21_sprintf_params *params) {
  switch (**format) {
    case 'h':
    case 'l':
    case 'L':
      params->length = **format;
      (*format)++;
      break;
  }
}

// ==================== ФУНКЦИИ ДЛЯ ДОБАВЛЕНИЯ ПРОБЕЛОВ/НУЛЕЙ
// ====================

static void add_padding(char **str, int width, char pad_char) {
  for (int i = 0; i < width; i++) {
    *(*str)++ = pad_char;
  }
}

static void add_sign(char **str, s21_sprintf_params *params, int is_negative) {
  if (is_negative) {
    *(*str)++ = '-';
  } else if (params->plus) {
    *(*str)++ = '+';
  } else if (params->space) {
    *(*str)++ = ' ';
  }
}

// ==================== ОБРАБОТКА СПЕЦИФИКАТОРОВ ====================

static void process_char(char **str, s21_sprintf_params *params, va_list args) {
  char c = (char)va_arg(args, int);

  int padding = params->width - 1;
  if (padding > 0) {
    if (!params->minus) {
      add_padding(str, padding, ' ');
    }
    *(*str)++ = c;
    if (params->minus) {
      add_padding(str, padding, ' ');
    }
  } else {
    *(*str)++ = c;
  }
}

static void process_string(char **str, s21_sprintf_params *params,
                           va_list args) {
  char *s = va_arg(args, char *);
  const char *null_str = "(null)";

  if (s == S21_NULL) {
    s = (char *)null_str;
  }

  // Вычисляем длину строки с учетом точности
  int len = 0;
  while (s[len] != '\0') {
    if (params->is_precision && len >= params->precision) {
      break;
    }
    len++;
  }

  int padding = params->width - len;
  if (padding > 0 && !params->minus) {
    add_padding(str, padding, ' ');
  }

  for (int i = 0; i < len; i++) {
    *(*str)++ = s[i];
  }

  if (padding > 0 && params->minus) {
    add_padding(str, padding, ' ');
  }
}

static void process_int(char **str, s21_sprintf_params *params, va_list args) {
  long long num;

  // Определяем тип в зависимости от модификатора длины
  if (params->length == 'h') {
    num = (short)va_arg(args, int);
  } else if (params->length == 'l') {
    num = va_arg(args, long);
  } else {
    num = va_arg(args, int);
  }

  char buffer[32];
  int is_negative = (num < 0);
  unsigned long long abs_num;

  if (is_negative) {
    abs_num = (unsigned long long)(-num);
  } else {
    abs_num = (unsigned long long)num;
  }

  // Преобразуем число в строку
  int num_len = 0;
  if (abs_num == 0) {
    if (!(params->is_precision && params->precision == 0)) {
      buffer[0] = '0';
      buffer[1] = '\0';
      num_len = 1;
    }
  } else {
    char temp_buffer[32];
    int temp_len = 0;

    while (abs_num > 0) {
      temp_buffer[temp_len++] = '0' + (abs_num % 10);
      abs_num /= 10;
    }

    for (int i = 0; i < temp_len; i++) {
      buffer[i] = temp_buffer[temp_len - i - 1];
    }
    buffer[temp_len] = '\0';
    num_len = temp_len;
  }

  // Определяем необходимую точность
  int actual_precision = 0;
  if (params->is_precision) {
    if (params->precision == 0 && num == 0) {
      num_len = 0;  // Не выводим ничего для нуля с точностью 0
    } else if (params->precision > num_len) {
      actual_precision = params->precision - num_len;
    }
  }

  // Определяем длину знака
  int sign_len = (is_negative || params->plus || params->space) ? 1 : 0;

  // Общая длина вывода
  int total_len = num_len + actual_precision + sign_len;

  // Вычисляем padding
  int padding = params->width - total_len;

  // Добавляем пробелы слева (если не флаг минус)
  if (padding > 0 && !params->minus) {
    add_padding(str, padding, ' ');
  }

  // Добавляем знак
  add_sign(str, params, is_negative);

  // Добавляем ведущие нули для точности
  if (actual_precision > 0) {
    add_padding(str, actual_precision, '0');
  }

  // Добавляем само число
  for (int i = 0; i < num_len; i++) {
    *(*str)++ = buffer[i];
  }

  // Добавляем пробелы справа (если флаг минус)
  if (padding > 0 && params->minus) {
    add_padding(str, padding, ' ');
  }
}

static void process_unsigned(char **str, s21_sprintf_params *params,
                             va_list args) {
  unsigned long long num;

  if (params->length == 'h') {
    num = (unsigned short)va_arg(args, unsigned int);
  } else if (params->length == 'l') {
    num = va_arg(args, unsigned long);
  } else {
    num = va_arg(args, unsigned int);
  }

  char buffer[32];
  int num_len = 0;

  if (num == 0) {
    if (!(params->is_precision && params->precision == 0)) {
      buffer[0] = '0';
      buffer[1] = '\0';
      num_len = 1;
    }
  } else {
    char temp_buffer[32];
    int temp_len = 0;

    while (num > 0) {
      temp_buffer[temp_len++] = '0' + (num % 10);
      num /= 10;
    }

    for (int i = 0; i < temp_len; i++) {
      buffer[i] = temp_buffer[temp_len - i - 1];
    }
    buffer[temp_len] = '\0';
    num_len = temp_len;
  }

  // Определяем необходимую точность
  int actual_precision = 0;
  if (params->is_precision) {
    if (params->precision == 0 && num == 0) {
      num_len = 0;  // Не выводим ничего для нуля с точностью 0
    } else if (params->precision > num_len) {
      actual_precision = params->precision - num_len;
    }
  }

  // Общая длина вывода
  int total_len = num_len + actual_precision;

  // Вычисляем padding
  int padding = params->width - total_len;

  // Добавляем пробелы слева (если не флаг минус)
  if (padding > 0 && !params->minus) {
    add_padding(str, padding, ' ');
  }

  // Добавляем ведущие нули для точности
  if (actual_precision > 0) {
    add_padding(str, actual_precision, '0');
  }

  // Добавляем само число
  for (int i = 0; i < num_len; i++) {
    *(*str)++ = buffer[i];
  }

  // Добавляем пробелы справа (если флаг минус)
  if (padding > 0 && params->minus) {
    add_padding(str, padding, ' ');
  }
}

static void process_float(char **str, s21_sprintf_params *params,
                          va_list args) {
  double num;

  if (params->length == 'L') {
    num = (double)va_arg(args, long double);
  } else {
    num = va_arg(args, double);
  }

  // Определяем точность (по умолчанию 6 для f)
  int precision = params->is_precision ? params->precision : 6;

  // Проверка на специальные значения
  if (s21_isnan(num)) {
    const char *nan_str = "nan";
    int len = 3;
    int padding = params->width - len;

    if (padding > 0 && !params->minus) {
      add_padding(str, padding, ' ');
    }

    for (int i = 0; i < len; i++) {
      *(*str)++ = nan_str[i];
    }

    if (padding > 0 && params->minus) {
      add_padding(str, padding, ' ');
    }
    return;
  }

  if (s21_isinf(num)) {
    const char *inf_str = "inf";
    int len = 3;

    // Проверяем, нужно ли добавлять знак
    int has_sign = (num < 0 || params->plus || params->space) ? 1 : 0;
    if (has_sign) {
      len += 1;  // Добавляем место для знака
    }

    int padding = params->width - len;

    if (padding > 0 && !params->minus) {
      add_padding(str, padding, ' ');
    }

    // Добавляем знак
    if (num < 0) {
      *(*str)++ = '-';
    } else if (params->plus) {
      *(*str)++ = '+';
    } else if (params->space) {
      *(*str)++ = ' ';
    }

    for (int i = 0; i < 3; i++) {
      *(*str)++ = inf_str[i];
    }

    if (padding > 0 && params->minus) {
      add_padding(str, padding, ' ');
    }
    return;
  }

  // Обработка обычных чисел
  int is_negative = (num < 0);
  if (is_negative) {
    num = -num;
  }

  // Округление с заданной точностью
  double round_factor = s21_pow10(precision);
  double rounded = num * round_factor;
  long long int_part = (long long)num;
  long long frac_part =
      (long long)((rounded - (int_part * round_factor)) + 0.5);

  // Проверка переноса из дробной части в целую
  if (frac_part >= (long long)round_factor) {
    int_part++;
    frac_part = 0;
  }

  // Преобразование целой части в строку
  char int_buffer[32];
  int int_len = 0;

  if (int_part == 0) {
    int_buffer[0] = '0';
    int_buffer[1] = '\0';
    int_len = 1;
  } else {
    long long temp = int_part;
    char temp_buffer[32];
    int temp_len = 0;

    while (temp > 0) {
      temp_buffer[temp_len++] = '0' + (temp % 10);
      temp /= 10;
    }

    for (int i = 0; i < temp_len; i++) {
      int_buffer[i] = temp_buffer[temp_len - i - 1];
    }
    int_buffer[temp_len] = '\0';
    int_len = temp_len;
  }

  // Преобразование дробной части в строку
  char frac_buffer[32];
  int frac_len = 0;

  if (precision > 0) {
    if (frac_part == 0) {
      for (int i = 0; i < precision; i++) {
        frac_buffer[i] = '0';
      }
      frac_len = precision;
    } else {
      char temp_buffer[32];
      int temp_len = 0;
      long long temp = frac_part;

      while (temp > 0) {
        temp_buffer[temp_len++] = '0' + (temp % 10);
        temp /= 10;
      }

      // Дополняем нулями слева если нужно
      while (temp_len < precision) {
        temp_buffer[temp_len++] = '0';
      }

      for (int i = 0; i < temp_len; i++) {
        frac_buffer[i] = temp_buffer[temp_len - i - 1];
      }
      frac_len = temp_len;
    }
  }

  // Расчет длины вывода
  int sign_len = (is_negative || params->plus || params->space) ? 1 : 0;
  int dot_len = (precision > 0) ? 1 : 0;
  int total_len = int_len + frac_len + dot_len + sign_len;

  // Вычисляем padding
  int padding = params->width - total_len;

  // Вывод с учетом флагов
  if (padding > 0 && !params->minus) {
    add_padding(str, padding, ' ');
  }

  add_sign(str, params, is_negative);

  // Целая часть
  for (int i = 0; i < int_len; i++) {
    *(*str)++ = int_buffer[i];
  }

  // Дробная часть
  if (precision > 0) {
    *(*str)++ = '.';
    for (int i = 0; i < frac_len; i++) {
      *(*str)++ = frac_buffer[i];
    }
  }

  if (padding > 0 && params->minus) {
    add_padding(str, padding, ' ');
  }
}

static void process_percent(char **str, s21_sprintf_params *params) {
  (void)params;  // Неиспользуемый параметр
  *(*str)++ = '%';
}

// ==================== ОСНОВНАЯ ФУНКЦИЯ SPRINTF ====================

int s21_sprintf(char *str, const char *format, ...) {
  va_list args;
  va_start(args, format);

  char *ptr = str;
  const char *fmt = format;

  while (*fmt) {
    if (*fmt != '%') {
      *ptr++ = *fmt++;
      continue;
    }

    fmt++;  // Пропускаем '%'

    // Инициализация параметров
    s21_sprintf_params params = {0};

    // Парсинг параметров
    parse_flags(&fmt, &params);
    parse_width(&fmt, &params);
    parse_precision(&fmt, &params);
    parse_length(&fmt, &params);

    // Получение спецификатора
    if (*fmt) {
      params.specifier = *fmt++;

      switch (params.specifier) {
        case 'c':
          process_char(&ptr, &params, args);
          break;
        case 's':
          process_string(&ptr, &params, args);
          break;
        case 'd':
        case 'i':
          process_int(&ptr, &params, args);
          break;
        case 'u':
          process_unsigned(&ptr, &params, args);
          break;
        case 'f':
          process_float(&ptr, &params, args);
          break;
        case '%':
          process_percent(&ptr, &params);
          break;
        default:
          // Неизвестный спецификатор - выводим как есть
          *ptr++ = '%';
          *ptr++ = params.specifier;
          break;
      }
    }
  }

  *ptr = '\0';
  va_end(args);

  return (int)(ptr - str);
}