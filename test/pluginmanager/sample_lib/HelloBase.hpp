#ifndef HELLOBASE_H
#define HELLOBASE_H

#include <string>

class HelloBase {
 public:
  virtual ~HelloBase() {}
  virtual std::string Hi() const = 0;
  virtual void Inc() = 0;
  virtual int GetI() const = 0;
};

#endif  // HELLOBASE_H
