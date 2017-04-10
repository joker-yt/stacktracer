#ifndef __TRACER_UTIL_H__
#define __TRACER_UTIL_H__

#include <cxxabi.h>
#include <dlfcn.h>
#include <stdlib.h>

// IsConst_Destructor(demangled)
// demangled -> XXX::YYY
// demangled_method -> address of "YYY"
// demangled_split -> XXX\0:YYY
// split_caller_object -> XXX
// split_caller_method -> YYY
// dladdrを用いて関数アドレスから関数シンボルへ変換
const char *addr2name(void *address) {
  Dl_info dli;
  if (0 != dladdr(address, &dli)) {
    return dli.dli_sname;
  }
  return 0;
}

const char *addr2fname(void *address) {
  Dl_info dli;
  if (0 != dladdr(address, &dli)) {
    return dli.dli_fname;
  }

  return 0;
}

char *demangle(const char *mangled_name) {
  char *p = (char *)malloc(32);
  size_t sz;
  int st;

  abi::__cxa_demangle((const char *)mangled_name, p, &sz, &st);
  if (st < 0 || !p) {
    free(p);
    return 0;
  }

  return p;
}

void demangle_free(const char *p) { free((void *)p); }

// demangled_method -> address of "YYY"
char *demangled_method(const char *demangled) {
  char *p = (char *)demangled;
  while (*p)
    if (*p == ':')
      return p + 2;
    else
      p++;

  return 0;
}

// demangled_split -> XXX\0:YYY
char *demangled_split(char *demangled) {
  char *p = demangled_method(demangled);
  if (p)
    *(p - 2) = '\0';
  return p ? demangled : 0;
}

// split_caller_object -> XXX
char *split_caller_object(const char *split_name) { return (char *)split_name; }

// split_caller_method -> YYY
char *split_caller_method(const char *split_name) {
  char *p = (char *)split_name;
  while (*p++)
    ;

  return (*p == ':') ? p + 1 : 0;
}

bool IsConst_Destructor(const char *demangled) {
  char *p = demangled_method(demangled);
  if (*p == '\0')
    return false;

  const char *cl = demangled;
  const char *mt = p;

  while (*cl && *mt && *cl != ':' && *mt != '\0' && *mt != '(') {
    if (*cl != *mt)
      return false;
    cl++;
    mt++;
  }

  return true;
}

#endif /* end of include guard: __TRACER_UTIL_H__ */
