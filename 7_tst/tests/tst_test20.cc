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

  assert(tst_default.prefix(std::string{ "#42" }) == "#42");
  assert(tst_char.prefix(std::basic_string<char>{ "#42" }) == "#42");
  assert(tst_wchar_t.prefix(std::basic_string<wchar_t>{ L"#42" }) == L"#42");
  assert(tst_char16_t.prefix(std::basic_string<char16_t>{ u"#42" }) == u"#42");
  assert(tst_char32_t.prefix(std::basic_string<char32_t>{ U"#42" }) == U"#42");
}
