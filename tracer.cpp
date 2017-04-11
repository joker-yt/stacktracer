// output format type
// #define FORMAT_DOT
#define FORMAT_PLANTUML

#ifdef FORMAT_DOT
#include "tracer_dot.h"
#elif defined FORMAT_PLANTUML
#include "tracer_plantuml.h"
#endif
#include "tracer_util.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
// http://llvm.org/test-doxygen/api/namespacellvm_1_1dwarf.html
// https://github.com/ikegam/ftrace/blob/master/prototype.c
// https://manned.org/dwarf_siblingof

extern "C" {
void __cyg_profile_func_enter(void *func_address, void *call_site);
void __cyg_profile_func_exit(void *func_address, void *call_site);
}

inline void output_to_file_enter(const char *func_name, const char *demangled,
                                 const char *caller,
                                 const char *demangled_caller, char *param) {
#ifdef FORMAT_DOT
  printf("%s\n", __FUNCTION__);
  output_to_file_enter_dot(func_name, demangled, caller, demangled_caller,
                           param);
#elif defined FORMAT_PLANTUML
  output_to_file_enter_plantuml(func_name, demangled, caller, demangled_caller,
                                param);
#endif
}

void output_to_file_exit(const char *func_name, const char *demangled,
                         const char *caller, const char *demangled_caller) {
  printf("%s\n", __FUNCTION__);
#ifdef FORMAT_DOT
  output_to_file_exit_dot();
#elif defined FORMAT_PLANTUML
  output_to_file_exit_plantuml(func_name, demangled, caller, demangled_caller);
#endif
}

inline void debug_print(const char *func_name, const char *demangled,
                        const char *caller, const char *demangled_caller,
                        void *param) {
#ifdef DEBUG
  printf("---------------\n");
  printf("%s: [func_name]-> %s\n", __func__, func_name);

  printf("%s: [demangled]-> %s\n", __func__, demangled);

  printf("%s: [caller] -> %s\n", __func__, caller);
  printf("%s: [caller(demangled)] -> %s\n", __func__, demangled_caller);

  printf("%s: [param] -> %p\n", __func__,
         (param == 0) ? "(null)" : (char *)param);
  printf("---------------\n");
#endif
}

void __cyg_profile_func_enter(void *func_address, void *call_site) {
  // see the stack
  // can't move to a new function because not to change a new frame....
  // | upper frame pointer
  // |-----------------------
  // | return address(+4)(+8)
  // |-----------------------
  // | instance address(for C++, +8)(???)
  // |-----------------------
  // | variable 1(+12)(+16)
  // |-----------------------

  const char *func_name = addr2name(func_address);
  if (!func_name)
    return;

  const char *demangled = (char *)demangle(func_name);
  if (!demangled)
    return;

  const char *caller = addr2name(call_site);
  if (!caller)
    return;

  const char *demangled_caller = (char *)demangle(caller);
  if (!caller)
    return;

  const char *param_address = 0;

  if (func_name && strcmp(func_name, "main") &&
      !IsConst_Destructor(demangled)) {
#ifdef __i386__
    param_address = *(char **)(((char *)__builtin_frame_address(1)) + 12);
#elif __x86_64__
    param_address = *(char **)(((char *)__builtin_frame_address(1)) + 16);
// char *param_address1 =
//     *(char **)(((char *)__builtin_frame_address(1)) + 16);
// printf("param : %p\n", param_address);
// printf("%lx\n", *((unsigned long int *)param_address));
// printf("param1 : %p\n", param_address1);
// printf("%lx\n", *((unsigned long int *)param_address1));
#endif
  }
  debug_print(func_name, demangled, caller, demangled_caller,
              (char *)param_address);
  output_to_file_enter(func_name, demangled, caller, demangled_caller,
                       (char *)param_address);

  if (demangled)
    demangle_free(demangled);

  // print_msc_sequence(func_address, call_site, param_address);
  return;
  // test(func_address);
}

void __cyg_profile_func_exit(void *func_address, void *call_site) {
  const char *func_name = addr2name(func_address);
  if (!func_name)
    return;

  const char *demangled = (char *)demangle(func_name);
  if (!demangled)
    return;

  const char *caller = addr2name(call_site);
  if (!caller)
    return;

  const char *demangled_caller = (char *)demangle(caller);
  if (!caller)
    return;

  output_to_file_exit(func_name, demangled, caller, demangled_caller);

  if (demangled)
    demangle_free(demangled);
}
