**Zadatak**. 

1. Potrebno je implementirati paralelni `accumulate` algoritam 
(vidjeti [stranicu kolegija](https://web.math.pmf.unizg.hr/nastava/napcpp/programske_niti.html#_paralelni_stdaccumulate_algoritam)). 
U main() funkciji treba testirati implementaciju uspoređivanjem sa serijskim kodom 
(`std::accumulate` algoritmom). Usporedite vremena izvršavanja paralelnog i serijskog koda u ovisnosti o
broju elemenata u spremniku. Kada je paralelni kod sporiji, brži i koliko?

2. Implementirati paralelni `transform` algoritam. Algoritam uzima unarni operator i za razliku
   od standardnog algoritma ima tip `void`. Testirati algoritam uspoređivanjem sa serijskim `std::transform` algoritmom.
   Usporedite vremena izvršavanja paralelnog i serijskog koda u ovisnosti o
   broju elemenata u spremniku i kompleksnosti transformacije. 

3. Za lakše mjerenje vremena treba implementirati klasu `Clock` prema specifikaciji u datoteci 
`clock.h`. 
