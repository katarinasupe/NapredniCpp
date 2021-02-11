**Zadatak.** Potrebno je napraviti paralelnu verziju  `std::any_of()` algoritma. 
Algoritam razbija ulazni niz na nekoliko podnizova i na svakom u posebnoj programskoj niti vrši potragu za 
elementom koji zadovoljava predikat. Najvažnije svojstvo paralelnog 
algoritma je da sve programske niti završavaju potragu kada jedna nit pronađe element.
Signalizaciju da je neka programska nit našla element treba izvršiti pomoću jedne varijable 
tipa `std::atomic<bool>` (zaglavlje `<atomic>`)  
Algoritam pokreće  maksimalno `std::thread::hardware_concurrency` programskih niti
pri čemu svaka nit dobiva minimalno 10 elemenata. 

Za prijenos vrijednosti iz programskih niti u pozivni program treba koristiti **jedan** 
`std::promise<>` objekt i pripadni `std::futute<>` objekt. Eventualno izbačeni izuzetak u
pretrazi mora biti korektno prenesen u pozivni program. 

Paziti na to da se na  `std::promise<>` objektu metoda `set_value()/set_exception()`  može pozvati
samo jednom, dok ostali pozivi izbacuju izuzetak koji moramo procesirati na neki način. 

U `main()` funkciji pokažite korektnost vašeg paralelnog algoritma na tri primjera 
usporedbom sa standardnim  `std::any_of()` algoritmom. U trećem primjeru demonstrirati 
propagiramje izuzetka.
