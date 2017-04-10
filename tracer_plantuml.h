#ifndef __TRACER_PLANTUML_H__
#define __TRACER_PLANTUML_H__

#include "tracer_util.h"
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define OUTPUT_PLANTUML_FILENAME "output.txt"
const char Main[] = "main";
FILE *g_output_fp = NULL;

void output_to_file_enter_plantuml(const char *func_name, const char *demangled,
                                   const char *caller,
                                   const char *demangled_caller, void *param) {
  if (!g_output_fp) {
    g_output_fp = fopen(OUTPUT_PLANTUML_FILENAME, "w+");
    fprintf(g_output_fp, "@startuml %s.png\n", OUTPUT_PLANTUML_FILENAME);
  }

  char *callee_object = NULL;
  char *p = (char *)malloc(strlen(demangled) + 1);
  strcpy(p, demangled);
  demangled_split(p);
  callee_object = split_caller_object(p);

  char *p2 = 0;
  char *caller_object = NULL;
  if (!strcmp(Main, caller)) {
    caller_object = (char *)&Main[0];
  } else {
    p2 = (char *)malloc(strlen(demangled_caller) + 1);
    strcpy(p2, demangled_caller);
    demangled_split(p2);
    caller_object = split_caller_object(p);
  }

  fprintf(g_output_fp, "%s -> %s : %s [%s]\n", caller_object, callee_object,
          split_caller_method(p), (char *)param);

  if (strcmp(caller_object, callee_object))
    fprintf(g_output_fp, "activate %s\n", callee_object);

  if (p)
    free(p);
  if (p2)
    free(p2);
}

void output_to_file_exit_plantuml(const char *func_name, const char *demangled,
                                  const char *caller,
                                  const char *demangled_caller) {
  if (g_output_fp) {
    char *callee_object = NULL;
    char *p = (char *)malloc(strlen(demangled) + 1);
    strcpy(p, demangled);
    demangled_split(p);
    callee_object = split_caller_object(p);

    char *p2 = 0;
    char *caller_object = NULL;
    if (!strcmp(Main, caller)) {
      caller_object = (char *)&Main[0];
    } else {
      p2 = (char *)malloc(strlen(demangled_caller) + 1);
      strcpy(p2, demangled_caller);
      demangled_split(p2);
      caller_object = split_caller_object(p);
    }

    if (strcmp(caller_object, callee_object)) {
      fprintf(g_output_fp, "%s <-- %s : return\n", caller_object,
              callee_object);
      fprintf(g_output_fp, "deactivate %s\n", callee_object);
    }
    if (p)
      free(p);
    if (p2)
      free(p2);
  }
}

#endif /* end of include guard: __TRACER_PLANTUML_H__ */
