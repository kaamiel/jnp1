#include "tst.h"
#include <cassert>

int
main()
{
  const TST<> tst_default{ "#42@" };
  const TST<char> tst_char{ "#42@" };
  const TST<wchar_t> tst_wchar_t{ L"#42@" };
  const TST<char16_t> tst_char16_t{ u"#42@" };
  const TST<char32_t> tst_char32_t{ U"#42@" };

  assert(tst_default.size() == 4);
  assert(tst_char.size() == 4);
  assert(tst_wchar_t.size() == 4);
  assert(tst_char16_t.size() == 4);
  assert(tst_char32_t.size() == 4);
}
