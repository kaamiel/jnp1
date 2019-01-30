#include "tst.h"
#include <string>

int
main()
{
  TST<>{ std::string{ "#42@" } };
  TST<char>{ std::string{ "#42@" } };
  TST<wchar_t>{ std::wstring{ L"#42@" } };
  TST<char16_t>{ std::u16string{ u"#42@" } };
  TST<char32_t>{ std::u32string{ U"#42@" } };
}
