#ifndef __TRACER_DOT_H__
#define __TRACER_DOT_H__

#include "tracer_util.h"
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define OUTPUT_DOT_FILENAME "output.dot"
FILE *g_output_fp = NULL;

void output_to_file_enter_dot(const char *func_name, const char *demangled,
                              const char *caller, const char *demangled_caller,
                              void *param) {
  if (!g_output_fp) {
    g_output_fp = fopen(OUTPUT_DOT_FILENAME, "w+");
  }

  char *caller_object = NULL;
  if (!caller || !strcmp(caller, "main")) {
    caller_object = (char *)caller;
  } else {
    demangled_split((char *)demangled_caller);
    caller_object = split_caller_object(demangled_caller);
  }

  demangled_split((char *)demangled);

  fprintf(g_output_fp, "\t%s => %s [label = \"%s\", return = \"return\"]",
          caller_object, split_caller_object(demangled),
          split_caller_method(demangled));

  fprintf(g_output_fp, "{\n");
}
void output_to_file_exit_dot() {
  if (g_output_fp)
    fprintf(g_output_fp, "\t}\n");
}

#endif /* end of include guard: __TRACER_DOT_H__ */
