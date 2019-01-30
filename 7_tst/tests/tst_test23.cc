#include "tst.h"
#include <cassert>

int
main()
{
  const TST<> tst1{ "catamorphism" };
  assert(tst1.size() == 12);

  const TST<> tst2{ tst1 + "functor" };
  assert(tst2.size() == 12 + 7);

  const TST<> tst3{ tst2 + "theory" };
  assert(tst3.size() == 12 + 7 + 6);

  const TST<> tst4{ tst3 + "fur" };
  assert(tst4.size() == 12 + 7 + 6 + (3 - 2));

  const TST<> tst5{ tst4 + "further" };
  assert(tst5.size() == 12 + 7 + 6 + (3 - 2) + (7 - 3));

  const TST<> tst6{ tst5 + "category" };
  assert(tst6.size() == 12 + 7 + 6 + (3 - 2) + (7 - 3) + (8 -3));
}
