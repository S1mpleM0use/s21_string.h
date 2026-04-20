#include "s21_string.h"

int is_delimiter(char c, const char *delim);
char *find_token_start(char *str, const char *delim);
char *find_token_end(char *start, const char *delim, char **next_ptr);

typedef struct {
  int width;
  int suppress;
  char length;
  char specifier;
} Format;

int sign_finder(const char **str_ptr);
double my_pow(double num, int power);
int is_digit(char c);
int is_valid_digit_char(char c);
int is_A_to_F_letter(char c);
int is_specifier(char c);
int is_whitespace(char c);
void skip_whitespaces(const char **str_ptr, const char **format_ptr);

void str_parser(const char **str_ptr, char **final_str, int width);
void wide_str_parser(const char **str_ptr, wchar_t **final_str, int width);
void char_parser(const char **str_ptr, char **record_char, int width);
void wide_char_parser(const char **str_ptr, wchar_t **record_char, int width);

void digit_str_parser(const char **str_ptr, char **final_str, int width);
int digit_parser(const char **str_ptr, int base, int width);
int base_detector(const char **str_ptr);
int number_parser(const char **str_ptr, char specifier, int width);

Format format_parser(const char **format_ptr);

int exponent_finder(char **str_ptr, int *success_flag);
void float_str_parser(const char **str_ptr, char **final_str, int width);
double float_parser(const char **str_ptr, int width, int *exponent_parsed_flag);

#define S21_LINUX_ERR_COUNT 134
#define S21_LINUX_ERRORS                                \
  {"Success",                                           \
   "Operation not permitted",                           \
   "No such file or directory",                         \
   "No such process",                                   \
   "Interrupted system call",                           \
   "Input/output error",                                \
   "No such device or address",                         \
   "Argument list too long",                            \
   "Exec format error",                                 \
   "Bad file descriptor",                               \
   "No child processes",                                \
   "Resource temporarily unavailable",                  \
   "Cannot allocate memory",                            \
   "Permission denied",                                 \
   "Bad address",                                       \
   "Block device required",                             \
   "Device or resource busy",                           \
   "File exists",                                       \
   "Invalid cross-device link",                         \
   "No such device",                                    \
   "Not a directory",                                   \
   "Is a directory",                                    \
   "Invalid argument",                                  \
   "Too many open files in system",                     \
   "Too many open files",                               \
   "Inappropriate ioctl for device",                    \
   "Text file busy",                                    \
   "File too large",                                    \
   "No space left on device",                           \
   "Illegal seek",                                      \
   "Read-only file system",                             \
   "Too many links",                                    \
   "Broken pipe",                                       \
   "Numerical argument out of domain",                  \
   "Numerical result out of range",                     \
   "Resource deadlock avoided",                         \
   "File name too long",                                \
   "No locks available",                                \
   "Function not implemented",                          \
   "Directory not empty",                               \
   "Too many levels of symbolic links",                 \
   "Unknown error 41",                                  \
   "No message of desired type",                        \
   "Identifier removed",                                \
   "Channel number out of range",                       \
   "Level 2 not synchronized",                          \
   "Level 3 halted",                                    \
   "Level 3 reset",                                     \
   "Link number out of range",                          \
   "Protocol driver not attached",                      \
   "No CSI structure available",                        \
   "Level 2 halted",                                    \
   "Invalid exchange",                                  \
   "Invalid request descriptor",                        \
   "Exchange full",                                     \
   "No anode",                                          \
   "Invalid request code",                              \
   "Invalid slot",                                      \
   "Unknown error 58",                                  \
   "Bad font file format",                              \
   "Device not a stream",                               \
   "No data available",                                 \
   "Timer expired",                                     \
   "Out of streams resources",                          \
   "Machine is not on the network",                     \
   "Package not installed",                             \
   "Object is remote",                                  \
   "Link has been severed",                             \
   "Advertise error",                                   \
   "Srmount error",                                     \
   "Communication error on send",                       \
   "Protocol error",                                    \
   "Multihop attempted",                                \
   "RFS specific error",                                \
   "Bad message",                                       \
   "Value too large for defined data type",             \
   "Name not unique on network",                        \
   "File descriptor in bad state",                      \
   "Remote address changed",                            \
   "Can not access a needed shared library",            \
   "Accessing a corrupted shared library",              \
   ".lib section in a.out corrupted",                   \
   "Attempting to link in too many shared libraries",   \
   "Cannot exec a shared library directly",             \
   "Invalid or incomplete multibyte or wide character", \
   "Interrupted system call should be restarted",       \
   "Streams pipe error",                                \
   "Too many users",                                    \
   "Socket operation on non-socket",                    \
   "Destination address required",                      \
   "Message too long",                                  \
   "Protocol wrong type for socket",                    \
   "Protocol not available",                            \
   "Protocol not supported",                            \
   "Socket type not supported",                         \
   "Operation not supported",                           \
   "Protocol family not supported",                     \
   "Address family not supported by protocol",          \
   "Address already in use",                            \
   "Cannot assign requested address",                   \
   "Network is down",                                   \
   "Network is unreachable",                            \
   "Network dropped connection on reset",               \
   "Software caused connection abort",                  \
   "Connection reset by peer",                          \
   "No buffer space available",                         \
   "Transport endpoint is already connected",           \
   "Transport endpoint is not connected",               \
   "Cannot send after transport endpoint shutdown",     \
   "Too many references: cannot splice",                \
   "Connection timed out",                              \
   "Connection refused",                                \
   "Host is down",                                      \
   "No route to host",                                  \
   "Operation already in progress",                     \
   "Operation now in progress",                         \
   "Stale file handle",                                 \
   "Structure needs cleaning",                          \
   "Not a XENIX named type file",                       \
   "No XENIX semaphores available",                     \
   "Is a named type file",                              \
   "Remote I/O error",                                  \
   "Disk quota exceeded",                               \
   "No medium found",                                   \
   "Wrong medium type",                                 \
   "Operation canceled",                                \
   "Required key not available",                        \
   "Key has expired",                                   \
   "Key has been revoked",                              \
   "Key was rejected by service",                       \
   "Owner died",                                        \
   "State not recoverable",                             \
   "Operation not possible due to RF-kill",             \
   "Memory page has hardware error",                    \
   "Unknown error"}

#define S21_APPLE_ERR_COUNT 134
#define S21_APPLE_ERRORS                                \
  {"Success",                                           \
   "Operation not permitted",                           \
   "No such file or directory",                         \
   "No such process",                                   \
   "Interrupted system call",                           \
   "Input/output error",                                \
   "No such device or address",                         \
   "Argument list too long",                            \
   "Exec format error",                                 \
   "Bad file descriptor",                               \
   "No child processes",                                \
   "Resource temporarily unavailable",                  \
   "Cannot allocate memory",                            \
   "Permission denied",                                 \
   "Bad address",                                       \
   "Block device required",                             \
   "Device or resource busy",                           \
   "File exists",                                       \
   "Invalid cross-device link",                         \
   "No such device",                                    \
   "Not a directory",                                   \
   "Is a directory",                                    \
   "Invalid argument",                                  \
   "Too many open files in system",                     \
   "Too many open files",                               \
   "Inappropriate ioctl for device",                    \
   "Text file busy",                                    \
   "File too large",                                    \
   "No space left on device",                           \
   "Illegal seek",                                      \
   "Read-only file system",                             \
   "Too many links",                                    \
   "Broken pipe",                                       \
   "Numerical argument out of domain",                  \
   "Numerical result out of range",                     \
   "Resource deadlock avoided",                         \
   "File name too long",                                \
   "No locks available",                                \
   "Function not implemented",                          \
   "Directory not empty",                               \
   "Too many levels of symbolic links",                 \
   "Unknown error 41",                                  \
   "No message of desired type",                        \
   "Identifier removed",                                \
   "Channel number out of range",                       \
   "Level 2 not synchronized",                          \
   "Level 3 halted",                                    \
   "Level 3 reset",                                     \
   "Link number out of range",                          \
   "Protocol driver not attached",                      \
   "No CSI structure available",                        \
   "Level 2 halted",                                    \
   "Invalid exchange",                                  \
   "Invalid request descriptor",                        \
   "Exchange full",                                     \
   "No anode",                                          \
   "Invalid request code",                              \
   "Invalid slot",                                      \
   "Unknown error 58",                                  \
   "Bad font file format",                              \
   "Device not a stream",                               \
   "No data available",                                 \
   "Timer expired",                                     \
   "Out of streams resources",                          \
   "Machine is not on the network",                     \
   "Package not installed",                             \
   "Object is remote",                                  \
   "Link has been severed",                             \
   "Advertise error",                                   \
   "Srmount error",                                     \
   "Communication error on send",                       \
   "Protocol error",                                    \
   "Multihop attempted",                                \
   "RFS specific error",                                \
   "Bad message",                                       \
   "Value too large for defined data type",             \
   "Name not unique on network",                        \
   "File descriptor in bad state",                      \
   "Remote address changed",                            \
   "Can not access a needed shared library",            \
   "Accessing a corrupted shared library",              \
   ".lib section in a.out corrupted",                   \
   "Attempting to link in too many shared libraries",   \
   "Cannot exec a shared library directly",             \
   "Invalid or incomplete multibyte or wide character", \
   "Interrupted system call should be restarted",       \
   "Streams pipe error",                                \
   "Too many users",                                    \
   "Socket operation on non-socket",                    \
   "Destination address required",                      \
   "Message too long",                                  \
   "Protocol wrong type for socket",                    \
   "Protocol not available",                            \
   "Protocol not supported",                            \
   "Socket type not supported",                         \
   "Operation not supported",                           \
   "Protocol family not supported",                     \
   "Address family not supported by protocol",          \
   "Address already in use",                            \
   "Cannot assign requested address",                   \
   "Network is down",                                   \
   "Network is unreachable",                            \
   "Network dropped connection on reset",               \
   "Software caused connection abort",                  \
   "Connection reset by peer",                          \
   "No buffer space available",                         \
   "Transport endpoint is already connected",           \
   "Transport endpoint is not connected",               \
   "Cannot send after transport endpoint shutdown",     \
   "Too many references: cannot splice",                \
   "Connection timed out",                              \
   "Connection refused",                                \
   "Host is down",                                      \
   "No route to host",                                  \
   "Operation already in progress",                     \
   "Operation now in progress",                         \
   "Stale file handle",                                 \
   "Structure needs cleaning",                          \
   "Not a XENIX named type file",                       \
   "No XENIX semaphores available",                     \
   "Is a named type file",                              \
   "Remote I/O error",                                  \
   "Disk quota exceeded",                               \
   "No medium found",                                   \
   "Wrong medium type",                                 \
   "Operation canceled",                                \
   "Required key not available",                        \
   "Key has expired",                                   \
   "Key has been revoked",                              \
   "Key was rejected by service",                       \
   "Owner died",                                        \
   "State not recoverable",                             \
   "Operation not possible due to RF-kill",             \
   "Memory page has hardware error",                    \
   "Unknown error"}
