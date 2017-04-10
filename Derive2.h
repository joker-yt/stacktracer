#ifndef __DERIVE2_H__
#define __DERIVE2_H__

#include <iostream>

class Base2 {
private:
public:
  Base2(){};
  virtual ~Base2(){};
  virtual void response() {
    std::cout << "Base2::"
              << "this is response" << std::endl;
  }
};

class Derive2 : public Base2 {
private:
public:
  Derive2(){};
  virtual ~Derive2(){};
  void response() {
    std::cout << "Derive2::"
              << "this is response" << std::endl;
  }
};

#endif /* end of include guard: __DERIVE2_H__ */
