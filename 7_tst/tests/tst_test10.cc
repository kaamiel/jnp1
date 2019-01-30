#include "tst.h"
#include <cassert>

int
main()
{
  assert(TST<>{ "#" }.word() && !TST<>{ "#42@" }.word());
  assert(TST<char>{ "#" }.word() && !TST<char>{ "#42@" }.word());
  assert(TST<wchar_t>{ L"#" }.word() && !TST<wchar_t>{ L"#42@" }.word());
  assert(TST<char16_t>{ u"#" }.word() && !TST<char16_t>{ u"#42@" }.word());
  assert(TST<char32_t>{ U"#" }.word() && !TST<char32_t>{ U"#42@" }.word());
}
