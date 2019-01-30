#include "tst.h"
#include <cassert>

int
main()
{
  TST<>{ TST<>{ "#42@" }.right() };
  TST<char>{ TST<char>{ "#42@" }.right() };
  TST<wchar_t>{ TST<wchar_t>{ L"#42@" }.right() };
  TST<char16_t>{ TST<char16_t>{ u"#42@" }.right() };
  TST<char32_t>{ TST<char32_t>{ U"#42@" }.right() };
}
