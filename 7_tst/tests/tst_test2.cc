#include "tst.h"
#include <cassert>
#include <string>

auto
append(const std::string& acc, char c)
{
  return std::move(acc) + c;
}

int
main()
{
  const std::string list{ "#42@" };

  assert(
    Detail::fold(list.begin(), list.end(), size_t{ 0 }, [](size_t i, char c) {
      static_cast<void>(c);
      return ++i;
    }) == list.size());

  assert(Detail::fold(
           list.begin(),
           list.end(),
           std::string{},
           [](const std::string& acc, char c) { return std::move(acc) + c; }) ==

         list);

  assert(Detail::fold(list.begin(), list.end(), std::string{}, append) == list);
}
