#ifndef TEST_H
#define TEST_H

#include <stdexcept>

template<typename F>
bool
exception_test(F f)
{
  bool result{ false };
  try {
    f();
  } catch (const std::logic_error&) {
    result = true;
  }
  return result;
}

#endif
