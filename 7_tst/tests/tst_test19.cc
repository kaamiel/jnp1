#include "tst.h"
#include <cassert>
#include <string>

int
main()
{
  const TST<> tst{ TST<>{ "catamorphism" } + "functor" + "theory" + "category" +
                   "lambda" + "cat" };

  assert(tst.exist(std::string{ "catamorphism" }));
  assert(tst.exist(std::string{ "functor" }));
  assert(tst.exist(std::string{ "theory" }));
  assert(tst.exist(std::string{ "category" }));
  assert(tst.exist(std::string{ "lambda" }));
  assert(tst.exist(std::string{ "cat" }));
  assert(!tst.exist(std::string{ "dog" }));
}
