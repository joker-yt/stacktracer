#include "Derive1.h"
#include "Derive2.h"
#include <iomanip>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void funcion_wo_param() { return; }

void funcion_w_param1(char *p) {
  printf("%s: param -> %s\n", __func__, p);
  return;
}

int main(int argc, char const *argv[]) {
  std::cout << __FUNCTION__ << ":start" << std::endl;
  Base1 *p1 = new Derive1();
  Base2 *p2 = new Derive2();
#if DEBUG
  printf("DEBUG::%s->p1 %p\n", __func__, p1);
  printf("DEBUG::%s->p2 %p\n", __func__, p2);
#endif
  // Segmentation fault!!!!
  // funcion_wo_param();
  // funcion_w_param1((char *)"this is a parameter");

  // p1->request(p2);
  char *p = (char *)malloc(strlen("sample string") + 1);
  strcpy(p, "sample string");
#if DEBUG
  printf("instance:%p\n", p1);
  printf("param:%p\n", p);
#endif
  p1->method(p);
  free(p);
#if DEBUG
  printf("DEBUG::return back to main\n");
#endif

  return 0;
}
