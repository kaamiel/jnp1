#include "tst.h"
#include <cassert>

int
main()
{
  char c_default{ TST<>{ "#42@" }.value() };
  assert(c_default == '#');

  char c_char{ TST<char>{ "#42@" }.value() };
  assert(c_char == '#');

  wchar_t c_wchar_t{ TST<wchar_t>{ L"#42@" }.value() };
  assert(c_wchar_t == L'#');

  char16_t c_char16_t{ TST<char16_t>{ u"#42@" }.value() };
  assert(c_char16_t == u'#');

  char32_t c_char32_t{ TST<char32_t>{ U"#42@" }.value() };
  assert(c_char32_t == U'#');
}
