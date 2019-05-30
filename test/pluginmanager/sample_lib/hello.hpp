#ifndef HELLO_H
#define HELLO_H

#include "HelloBase.hpp"

///
/// \brief TODO
///
class Hello : public HelloBase {
 public:
  Hello(const std::string& msg) : msg(msg), i(0) {}
  std::string Hi() const override;
  void Inc() override;
  int GetI() const override;

 private:
  std::string msg;
  int i;
};

extern "C" HelloBase* create(const std::string& msg) { return new Hello{msg}; }

extern "C" void destroy(HelloBase* p) { delete p; }

#endif  // HELLO_H
