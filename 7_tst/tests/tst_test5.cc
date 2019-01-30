#include "tst.h"
#include <string>

int
main()
{
  const TST<> tst_default{};
  tst_default + std::string{ "#42@" };

  const TST<char> tst_char{};
  tst_char + std::string{ "#42@" };

  const TST<wchar_t> tst_wchar_t{};
  tst_wchar_t + std::wstring{ L"#42@" };

  const TST<char16_t> tst_char16_t{};
  tst_char16_t + std::u16string{ u"#42@" };

  const TST<char32_t> tst_char32_t{};
  tst_char32_t + std::u32string{ U"#42@" };
}
