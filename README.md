#Szeregowanie zadań równoległych posiadających czas gotowości

##Kompilacja
Kompilowane z użyciem gcc w wersji 7.4.0
w zupełności powinno wystarczyć:

`g++ -O3 main.cpp -o program`

w związku z użyciem nowszych rozwiązań najstarszy
zgodny standard to c++11

##Uruchomienie
Program przyjmuje 3 argumenty:

`./program parametry nazwapliku.swf [ilość zadań]`

Ilość zadań jest to wartośc która określa ilość zczytanych popranych zadań,
to znaczy zadania niepoprawne nie są liczone. Gdzie w przypadku braku podania
ilości linii program pracuje na całym pliku. Program nie sprawdza poprawności
nazwy pliku.

##Parametry
Parametry są czytane i wykonywane w zadanej kolejności
Znaczenie parametrów:
h - oblicz rozwiazanie zachłanne
m - oblicz rozwiazanie tabu search
v - oblicz "głośno" rozwiązanie tabu search - program bedzie relacjonował na
bieżąco co się dzieje
p - wypisz ostanie obliczone rozwiazanie
l - wypisz długość ostatniego obliczonego rozwiązania
t - wypisz czas wykonywania programu - przy uruchomieniu m/v czas sie resetuje
d - wypisz długość idalną instancji

##Przykłądy
przykłady:

wypisz rozwiazanie tabu search
`./program mp tmp.swf 10000`

wypisz czas rozwiazania greedy oraz tabu search
`./program htmt tmp.swf 10000`

wypisz długość rozwiązania greedu oraz tabu search wraz z długością idealną
`./program dhlml tmp.swf 10000`
