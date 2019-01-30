#include "tst.h"

int
main()
{
  TST<>{ "#42@" };
  TST<char>{ "#42@" };
  TST<wchar_t>{ L"#42@" };
  TST<char16_t>{ u"#42@" };
  TST<char32_t>{ U"#42@" };
}
