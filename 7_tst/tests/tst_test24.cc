#include "tst.h"
#include <cassert>

int
main()
{
  const TST<> tst{ "catamorphism" };
  const auto result{ tst + "cat" + "theory" + "function" + "catfish" };

  assert(result.value() == 'c');
  assert(!result.word());
  assert(!result.empty());
  assert(result.center().value() == 'a');
  assert(!result.center().word());
  assert(result.center().center().value() == 't');
  assert(result.center().center().word());

  assert(result.center().center().center().value() == 'a');
  assert(!result.center().center().center().word());
  assert(!result.center().center().center().empty());
  assert(result.center().center().center().center().value() == 'm');
  assert(!result.center().center().center().center().word());
  assert(result.center().center().center().center().center().value() == 'o');
  assert(!result.center().center().center().center().center().word());
  assert(result.center().center().center().center().center().center().value() ==
         'r');
  assert(!result.center().center().center().center().center().center().word());
  assert(result.center()
           .center()
           .center()
           .center()
           .center()
           .center()
           .center()
           .value() == 'p');
  assert(!result.center()
            .center()
            .center()
            .center()
            .center()
            .center()
            .center()
            .word());
  assert(result.center()
           .center()
           .center()
           .center()
           .center()
           .center()
           .center()
           .center()
           .value() == 'h');
  assert(!result.center()
            .center()
            .center()
            .center()
            .center()
            .center()
            .center()
            .center()
            .word());
  assert(result.center()
           .center()
           .center()
           .center()
           .center()
           .center()
           .center()
           .center()
           .center()
           .value() == 'i');
  assert(!result.center()
            .center()
            .center()
            .center()
            .center()
            .center()
            .center()
            .center()
            .center()
            .word());
  assert(result.center()
           .center()
           .center()
           .center()
           .center()
           .center()
           .center()
           .center()
           .center()
           .center()
           .value() == 's');
  assert(!result.center()
            .center()
            .center()
            .center()
            .center()
            .center()
            .center()
            .center()
            .center()
            .center()
            .word());
  assert(result.center()
           .center()
           .center()
           .center()
           .center()
           .center()
           .center()
           .center()
           .center()
           .center()
           .center()
           .value() == 'm');
  assert(result.center()
           .center()
           .center()
           .center()
           .center()
           .center()
           .center()
           .center()
           .center()
           .center()
           .center()
           .word());
  assert(result.center()
           .center()
           .center()
           .center()
           .center()
           .center()
           .center()
           .center()
           .center()
           .center()
           .center()
           .center()
           .empty());

  assert(result.center().center().center().right().value() == 'f');
  assert(!result.center().center().center().right().word());
  assert(!result.center().center().center().right().empty());
  assert(result.center().center().center().right().center().value() == 'i');
  assert(!result.center().center().center().right().center().word());
  assert(result.center().center().center().right().center().center().value() ==
         's');
  assert(!result.center().center().center().right().center().center().word());
  assert(result.center()
           .center()
           .center()
           .right()
           .center()
           .center()
           .center()
           .value() == 'h');
  assert(result.center()
           .center()
           .center()
           .right()
           .center()
           .center()
           .center()
           .word());
  assert(result.center()
           .center()
           .center()
           .right()
           .center()
           .center()
           .center()
           .center()
           .empty());

  assert(result.right().value() == 't');
  assert(!result.right().word());
  assert(!result.right().empty());
  assert(result.right().center().value() == 'h');
  assert(!result.right().center().word());
  assert(result.right().center().center().value() == 'e');
  assert(!result.right().center().center().word());
  assert(result.right().center().center().center().value() == 'o');
  assert(!result.right().center().center().center().word());
  assert(result.right().center().center().center().center().value() == 'r');
  assert(!result.right().center().center().center().center().word());
  assert(result.right().center().center().center().center().center().value() ==
         'y');
  assert(result.right().center().center().center().center().center().word());
  assert(result.right()
           .center()
           .center()
           .center()
           .center()
           .center()
           .center()
           .empty());

  assert(result.right().left().value() == 'f');
  assert(!result.right().left().word());
  assert(!result.right().left().empty());
  assert(result.right().left().center().value() == 'u');
  assert(!result.right().left().center().word());
  assert(result.right().left().center().center().value() == 'n');
  assert(!result.right().left().center().center().word());
  assert(result.right().left().center().center().center().value() == 'c');
  assert(!result.right().left().center().center().center().word());
  assert(result.right().left().center().center().center().center().value() ==
         't');
  assert(!result.right().left().center().center().center().center().word());
  assert(result.right()
           .left()
           .center()
           .center()
           .center()
           .center()
           .center()
           .value() == 'i');
  assert(
    !result.right().left().center().center().center().center().center().word());
  assert(result.right()
           .left()
           .center()
           .center()
           .center()
           .center()
           .center()
           .center()
           .value() == 'o');
  assert(!result.right()
            .left()
            .center()
            .center()
            .center()
            .center()
            .center()
            .center()
            .word());
  assert(result.right()
           .left()
           .center()
           .center()
           .center()
           .center()
           .center()
           .center()
           .center()
           .value() == 'n');
  assert(result.right()
           .left()
           .center()
           .center()
           .center()
           .center()
           .center()
           .center()
           .center()
           .word());
  assert(result.right()
           .left()
           .center()
           .center()
           .center()
           .center()
           .center()
           .center()
           .center()
           .center()
           .empty());
}
