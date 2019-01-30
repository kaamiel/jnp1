#include "tst.h"
#include <cassert>
#include <string>

int
main()
{
  const TST<> tst_default{ "#42@" };
  const TST<char> tst_char{ "#42@" };
  const TST<wchar_t> tst_wchar_t{ L"#42@" };
  const TST<char16_t> tst_char16_t{ u"#42@" };
  const TST<char32_t> tst_char32_t{ U"#42@" };

  assert(tst_default.exist(std::string{ "#42@" }));
  assert(tst_char.exist(std::basic_string<char>{ "#42@" }));
  assert(tst_wchar_t.exist(std::basic_string<wchar_t>{ L"#42@" }));
  assert(tst_char16_t.exist(std::basic_string<char16_t>{ u"#42@" }));
  assert(tst_char32_t.exist(std::basic_string<char32_t>{ U"#42@" }));
}
