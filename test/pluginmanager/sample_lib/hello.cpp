#include "hello.hpp"

std::string Hello::Hi() const { return this->msg; }

void Hello::Inc() { this->i++; }

int Hello::GetI() const { return this->i; }
