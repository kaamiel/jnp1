#include "test.h"
#include "tst.h"
#include <cassert>

int
main()
{
  assert(exception_test([]() {
    const TST<> tst{};
    tst.word();
  }));
  assert(exception_test([]() {
    const TST<char> tst{};
    tst.word();
  }));
  assert(exception_test([]() {
    const TST<wchar_t> tst{};
    tst.word();
  }));
  assert(exception_test([]() {
    const TST<char16_t> tst{};
    tst.word();
  }));
  assert(exception_test([]() {
    const TST<char32_t> tst{};
    tst.word();
  }));
}
