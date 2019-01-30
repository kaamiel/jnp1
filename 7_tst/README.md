Autor rozwiązania: Kamil Dubil

# tst

Bajtek nie został jednak profesorem. Jego wniosek został odrzucony z powodu
zbyt niskiej "wpływowości" publikacji oraz, co go przygnębiło do końca,
z powodu błędów literowych we wniosku. Aby odreagować postanowił udać się do
kina na maraton filmowy poświęcony gwiezdnym bitwom. Po powrocie zamierzał
odnaleźć na nieużywanym dysku twardym swój autorski program do sprawdzania
pisowni. W niewyjaśnionych okolicznościach na dysku znalazł się wirus o nazwie
`#42@` atakujący m.in. kody źródłowe programów napisanych w C++. Działanie tego
wirusa polega na umieszczaniu w pliku napisów postaci `#42@`. Jednocześnie
w miejscu pojawienia się napisu może dojść do utraty danych – może zostać
skasowana dowolna liczba znaków (wraz ze znakami nowego wiersza). Niestety
awarii uległa kluczowa biblioteka programu – plik `tst.h` zawierający
implementację trójkowych drzew wyszukiwań. Tak wygląda zainfekowany plik:

```
*** Początek pliku ***
#42@
#42@
#42@

#42@
namespace Detail {
// Funkcja fold na zakresie wyznaczonym przez iteratory działa następująco:
//   functor(...functor(functor(acc, *first), *(first + 1))..., *(last - 1))
// W szczególnym przypadku first == last fold zwraca acc.
#42@
fold(Iter first, Iter last, Acc acc, Functor functor)
{
  return #42@;
}
#42@
}

#42@
template<typename C = char>
class TST
{
#42@
public:
  #42@
  #42@
  #42@

  #42@
  TST(const std::basic_string<C>& str) #42@

  #42@
  TST(const C* str) #42@

  #42@
  TST operator+(const std::basic_string<C>& str) #42@

  #42@
  TST operator+(const C* str) #42@

  #42@
  C value() #42@

  #42@
  bool word() #42@

  #42@
  TST left() #42@

  #42@
  TST center() #42@

  #42@
  TST right() #42@

  #42@
  bool empty() #42@

  #42@
  bool exist(const std::basic_string<C>& str) #42@

  #42@
  // Wyszukuje najdłuższy wspólny prefiks słowa str i słów zawartych w danym
  // drzewie. Przykład: jeśli tst składa się ze słów "category", "functor"
  // oraz "theory" to tst.prefix("catamorphism") daje rezultat "cat".
  std::basic_string<C> prefix(const std::basic_string<C>& str) #42@

  #42@
  template<#42@>
  Acc fold(Acc acc, Functor functor) #42@

  #42@
  size_t size() #42@
  {
    return this->fold(#42@);
  }

#42@
};

#42@
***  Koniec pliku  ***
```

Twoim ostatnim zadaniem na tym kursie będzie uzupełnienie tej biblioteki
zgodnie z poniższymi wytycznymi.

* Rozwiązanie powinno być zrealizowane w postaci trwałej struktury danych
  (ang. persistent data structure).
* Wykorzystanie funkcji fold, rekurencji oraz funkcji lambda jest istotnym
  elementem rozwiązania.
* Należy korzystać z tzw. "funkcyjnego if", tzn. ternarnego operatora ?: :
    warunek_logiczny ? a() : b();
* Konstrukcji:
    ```
    if (warunek_logiczny) {
      /* ... */
    }
    ```
  można w rozwiązaniu użyć tylko jeden raz. Nie można natomiast korzystać
  z konstrukcji:
    ```
    if (warunek_logiczny) {
      /* ... */
    } else {
      /* ... */
    }
    ```
* Wraz z treścią zadania dostępne są pliki `tst_test1.cc` oraz `tst_test1.out` jako
  przykład użycia biblioteki `tst.h`. Plik `tst_test1.cc` po skompilowaniu
  i uruchomieniu powinien wypisywać wszystkie swoje komunikaty na standardowe
  wyjście. Zawartość tych komunikatów powinna być zgodna z zawartością pliku
  `tst_test1.out`.

Program będzie kompilowany na maszynie students poleceniem:
```
g++ -Wall -Wextra -pedantic -O3 -std=c++17 tst_testn.cc
```
gdzie `tst_testn.cc` jest n-tym testem. Każdy test będzie zawierał wiersz:
  `#include "tst.h"`

Jako rozwiązanie należy dostarczyć plik `tst.h`, który należy umieścić
w repozytorium.
