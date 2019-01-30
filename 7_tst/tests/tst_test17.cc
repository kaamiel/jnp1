#include "tst.h"
#include <cassert>

int
main()
{
  const TST<> tst_default_empty{};
  const TST<char> tst_char_empty{};
  const TST<wchar_t> tst_wchar_t_empty{};
  const TST<char16_t> tst_char16_t_empty{};
  const TST<char32_t> tst_char32_t_empty{};

  const TST<> tst_default_nonempty{ "#42@" };
  const TST<char> tst_char_nonempty{ "#42@" };
  const TST<wchar_t> tst_wchar_t_nonempty{ L"#42@" };
  const TST<char16_t> tst_char16_t_nonempty{ u"#42@" };
  const TST<char32_t> tst_char32_t_nonempty{ U"#42@" };

  assert(tst_default_empty.empty() && !tst_default_nonempty.empty());
  assert(tst_char_empty.empty() && !tst_char_nonempty.empty());
  assert(tst_wchar_t_empty.empty() && !tst_wchar_t_nonempty.empty());
  assert(tst_char16_t_empty.empty() && !tst_char16_t_nonempty.empty());
  assert(tst_char32_t_empty.empty() && !tst_char32_t_nonempty.empty());
}
