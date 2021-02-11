**Zadatak**.  

Potrebno je konstruirati asinhroni rječnik na bazi otvorenog haširanja. Naša će _hash_ tablica imati fiksni broj
pretinaca koji se određuje pri konstrukciji i stoga ne treba implementirati redimenzioniranje tablice.
Odabrat ćemo pristup baziran na otvorenom haširanju tako da će se cijela struktura sastojati od jednog 
vektora pretinaca (fiksne dimenzije), a svaki pretinac će biti povezana lista elemenata tablice. 
Za povezanu listu koristimo standardnu listu `std::list<>`, a element tablice je tipa `std::pair<Key, Value>`.

Zadatak se sastoji od dva dijela:  1) Treba konstruirati serijski rječnik namijenjen korištenju u jednoj programskoj niti.
Rječnik treba testirati jediničnim testovima. 2) Konstruirani rječnik treba učiniti sigurnim za asinhroni pristup 
i u main funkciji napisati test program. 


**1. dio**

Sučelje rječnika i neki djelovi implementacije dani su u datoteci `lookup-seq.h`. Za pretince koristimo 
unutarnju klasu `Bucket` koja sadrži listu elemenata. Svaki je element rječnika par ključa (`Key`) i 
vrijednosti (`Value`).  Za haširanje se kao dodijeljeni tip koristi `std::hash<Key>`, koji korisnik može 
uvijek zamijeniti sa svojom klasom. 

Sučelje klase  `HashTable` je znatno reducirano i dano je u datoteci `lookup-seq.h`. 
Tu su sve metode koje treba implementirati,
s time da se može dodati proizvoljan broj privatnih metoda. 

Metode `value()` i `find_if()` vraćaju `std::optional<>` objekt jer je moguće da nema objekta kojeg 
trebaju vratiti. Metoda `find_all_if()` signalizira da nije našla element tako što vraća praznu listu. 

Napravite jedinične testove u `test` direktoriju (kreirajte sami) pomoću google test biblioteke.
Google test smjestiti kao subproject u direktorij `external`  (kreirajte sami). Modificirati na odgovarajući 
način sustav za izgradnju. Testirajte sve metode osim metode `print()` koja služi vama za verifikaciju 
strukture. 

**2. dio**

Modificirati prethodno napravljenu klasu  `HashTable` tako da bude sigurna za asinhroni pristup. Pri tome svaki pretinac
štititi jednim _mutexom_ tipa `std::shared_mutex` kako bi se iskoristila činjenica da je tablica predviđena 
za često čitanje i rijetko upisivanje. 

Sučelje paralelne `HashTable` se nije promijenilo u odnosu na serijsku verziju. Ono je ponovljeno u datoteci
`ts-lookup.h` s dodatnim implementacijskim detaljem (_mutexom_).

Konačno u metodi `m̀ain()` (datoteka `main.cpp`) treba testirati samo paralelnu verziju koda. 
Napraviti smisleni test program koji će testirati cijelu klasu, ili barem njen veći dio. Serijski dio se testira
samo u jediničnim testovima u `test` direktoriju. Za paralelnu verziju ne treba pisati nikakve jedinične testove.

**Bodovanje**

Prvi i drugi dio zadaće bit će bodovani posebno, tako da se ovdje radi o dva zadatka u jednom.
