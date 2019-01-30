#include "tst.h"
#include <cassert>
#include <string>

int
main()
{
  const TST<> tst{ TST<>{ "catamorphism" } + "functor" + "theory" + "category" +
                   "lambda" + "cat" };

  assert(tst.prefix(std::string{ "catastrophe" }) == "cata");
  assert(tst.prefix(std::string{ "fur" }) == "fu");
  assert(tst.prefix(std::string{ "the" }) == "the");
  assert(tst.prefix(std::string{ "cat" }) == "cat");
  assert(tst.prefix(std::string{ "lookup" }) == "l");
  assert(tst.prefix(std::string{ "dog" }) == "");
}
