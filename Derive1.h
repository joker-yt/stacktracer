#ifndef __DERIVE1_H___
#define __DERIVE1_H___

#include "Derive2.h"
#include <iostream>

class Base1 {
private:
public:
  Base1(){};
  virtual ~Base1(){};
  virtual void request(Base2 *p) {
    std::cout << "Base1::"
              << "this is request" << std::endl;
    p->response();
  }
  virtual void method(char *str){};
};

class Derive1 : public Base1 {
private:
  void l2_method(char *str) { std::cout << __func__ << "\n"; };

public:
  Derive1(){};
  virtual ~Derive1(){};
  void request(Base2 *p) {
    std::cout << "Derive1::"
              << "this is request" << std::endl;
    p->response();
  }
  void method(char *str) {
    int a = 1;
    // unsigned long ad = (unsigned long)__builtin_frame_address(0);
    // std::cout << "method::" << ad << std::endl;
    std::cout << "method"
              << "::" << str << std::endl;
    std::cout << a << std::endl;
    l2_method(str);
  };
};

#endif /* end of include guard: __DERIVE1_H___ */
