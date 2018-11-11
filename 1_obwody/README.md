Authors: Piotr Krzywicki, Kamil Dubil

## Zadanie obwody elektroniczne

Należy napisać program, który na podstawie tekstowego opisu obwodu
elektronicznego wytworzy listę typów elementów (ang. bill of materials)
potrzebnych do zmontowania odpowiedniego układu. Dla uproszczenia zakładamy, że
obwody elektroniczne składają się z tranzystorów, diod, rezystorów,
kondensatorów i źródeł napięcia.

Program czyta opis obwodu ze standardowego wejścia. Każdy wiersz zawiera opis
jednego elementu obwodu. Wiersz zawiera oznaczenie elementu, typ elementu
i listę węzłów, do których podłączone są końcówki tego elementu. Oznaczenie
elementu składa się z wielkiej litery `T`, `D`, `R`, `C` lub `E` (oznaczającej rodzaj
elementu, odpowiednio tranzystor, diodę, rezystor, kondensator, źródło napięcia)
i numeru elementu, będącego liczbą całkowitą z zakresu od 0 do 999999999, bez
zer wiodących. Typ elementu jest napisem rozpoczynającym się wielką literą
alfabetu angielskiego lub cyfrą dziesiętną, po czym mogą wystąpić wielkie i małe
litery alfabetu angielskiego, cyfry dziesiętne, przecinek, łącznik lub ukośnik.
Numer węzła jest liczbą całkowitą z zakresu od 0 do 999999999, bez zer
wiodących. Tranzystor ma trzy końcówki (emiter, baza, kolektor lub źródło,
bramka, dren), a pozostałe elementy – dwie końcówki (dla diody są to anoda
i katoda, dla kondensatora spolaryzowanego – dodatnia i ujemna okładka, a dla
źródła napięcia stałego – biegun dodatni i ujemy). Składowe opisu elementu
odseparowane są białymi znakami, które mogą też znajdować się na początku
i końcu wiersza. Oznaczenia elementów w obwodzie nie mogą się powtarzać.
Wszystkie końcówki danego elementu nie mogą być podłączone do jednego węzła.
Patrz przykład danych wejściowych.

Program analizuje dane wejściowe wiersz po wierszu. Program ignoruje puste
wiersze. Dla każdego niepustego wiersza program decyduje, czy wiersz ten jest
poprawny. Jeśli wiersz nie jest poprawny, to informacje w nim zawarte nie są
brane pod uwagę przy dalszym przetwarzaniu, a program wypisuje na standardowe
wyjście diagnostyczne wiersz z informacją o błędzie, zawierającą numer błędnego
wiersza i dokładną, oryginalną jego postać. Wiersze numeruje się od 1. Należy
zastosować format dokładnie zgodny z podanym przykładem danych wyjściowych.

Po przeanalizowaniu wszystkich danych wejściowych program wypisuje na
standardowe wyjście listę typów elementów. Każdy typ elementu ma być opisany
w jednym wierszu, który ma zawierać listę oznaczeń elementów tego typu oraz typ
elementu. Lista typów elementów powinna być posortowana w następującej
kolejności: tranzystory, diody, rezystory, kondensatory, źródła napięcia.
W obrębie tego samego rodzaju elementów lista oznaczeń elementów i wiersze
powinny być posortowane rosnąco według numerów elementów. Należy zastosować
format dokładnie zgodny z podanym przykładem danych wyjściowych.

Na koniec, jeśli w obwodzie występuje węzeł, do którego nie są podłączone co
najmniej dwa elementy, program wypisuje na standardowe wyjście diagnostyczne
jeden wiersz z ostrzeżeniem zawierającym posortowaną rosnąco listę numerów
takich węzłów. W obwodzie zawsze występuje węzeł o numerze 0 (masa), nawet gdy
nie został jawnie wyspecyfikowany w danych wejściowych. Należy zastosować
format dokładnie zgodny z podanym przykładem danych wyjściowych.

Program będzie kompilowany na maszynie students poleceniem

```
g++ -Wall -Wextra -O2 -std=c++17 obwody.cc -o obwody
```

Oczekiwane rozwiązanie nie powinno zawierać definicji własnych struktur i klas,
a przynajmniej takich, które zawierają dane. Zamiast tego należy intensywnie
korzystać z kontenerów i algorytmów dostarczanych przez standardową bibliotekę
języka C++. Obsługę wejścia i wyjścia należy zrealizować za pomocą strumieni.

Rozwiązanie należy umieścić w pliku `obwody.cc`, który należy umieścić
w repozytorium SVN w katalogu

`grupaN/zadanie1/ab123456+cd123456`

lub

`grupaN/zadanie1/ab123456+cd123456+ef123456`

gdzie `N` jest numerem grupy, a `ab123456`, `cd123456`, `ef123456` są identyfikatorami
członków zespołu umieszczającego to rozwiązanie. Katalog z rozwiązaniem nie
powinien zawierać innych plików, ale może zawierać podkatalog prywatne, gdzie
można umieszczać różne pliki, np. swoje testy. Pliki umieszczone w tym
podkatalogu nie będą oceniane. Nie wolno umieszczać w repozytorium plików
dużych, binarnych, tymczasowych (np. `*.o`) ani innych zbędnych.
