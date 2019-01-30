#include "tst.h"

int
main()
{
  const TST<> tst_default{};
  tst_default + "#42@";

  const TST<char> tst_char{};
  tst_char + "#42@";

  const TST<wchar_t> tst_wchar_t{};
  tst_wchar_t + L"#42@";

  const TST<char16_t> tst_char16_t{};
  tst_char16_t + u"#42@";

  const TST<char32_t> tst_char32_t{};
  tst_char32_t + U"#42@";
}
