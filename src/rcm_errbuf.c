#include <stdarg.h>
#include <stdio.h>

#include "rcm_errbuf.h"

#ifdef _MSC_VER
static int rcm_error_vsnprintf(char *str, size_t size, const char *format,
                               va_list ap)
{
  int count = -1;
  if (size != 0) {
    count = _vsnprintf_s(str, size, _TRUNCATE, format, ap);
  }
  if (count == -1) {
    count = _vscprintf(format, ap);
  }
  return count;
}
#endif

RCM_API void rcm_errbuf_set(char *err, const char *format, ...)
{
  va_list ap;
  if (!err) {
    return;
  }
  va_start(ap, format);
#ifdef _MSC_VER
  (void)rcm_error_vsnprintf(err, RCM_ERRBUF_SIZE, format, ap);
#else
  (void)vsnprintf(err, RCM_ERRBUF_SIZE, format, ap);
#endif
  va_end(ap);
}
