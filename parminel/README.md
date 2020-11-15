**Zadatak**.

1. Potrebno je kreirati paralelnu verziju  `std::min_element()` algoritma koristeći programske niti. 
Signatura algoritma je

````
template<typename ForwardIt, typename Compare>
ForwardIt par_min_element(ForwardIt first, ForwardIt last, Compare comp);
````

Radi se stoga o verziji algoritma koja uzima objekt `comp` za uspoređivanje elemenata. 

2. Potrebno je testirati korektnost algorima i provjeriti njegovu efikasnost usporedbom 
sa serijskim `std::min_element()` algoritmom. Pri mjerenju vremena koristite vašu `Clock` 
klasu. 

Testirati na nizovima slučajno generiranih brojeva tipa `int`. 
U tu svrhu napisati klasu `RandomInt` koja predstavlja generator slučajnih brojeva tipa `int` uniformno distribuiranih 
u zadanom rasponu.  Sučelje klase je dano ovdje:

````
class RandomInt{
  public:
     RandomInt(int a, int b);
     RandomInt(RandomInt const & rhs);
     RandomInt & operator=(RandomInt const & rhs) = delete;
     int operator()();
};
````

- a, b su granice distribuiranih int-ova;
- konstruktor mora postaviti svaki puta različit _seed_ kako bi se uvijek generirao drugi 
  slučajni niz. Hint: koristiti std::random_device za generiranje _seed_ vrijednosti. 
- konstruktor kopije mora postaviti novi _seed_  kako kopija ne bi generirala isti niz kao 
  i original;
- operator() generira novu pseudoslučajnu vrijednost.

Klasa se mora moći koristiti na sljedeći način: 

````
  RandomInt ir(1,100);  // konstrukcija
  std::vector<int> AA(5);
  std::generate(AA.begin(), AA.end(), ir);  // kopiranje u algoritam generate()
  std::copy(AA.begin(), AA.end(), std::ostream_iterator<int>(std::cout, ",")); // ispis
  std::cout << "\n";
  std::generate(AA.begin(), AA.end(), ir);
  std::copy(AA.begin(), AA.end(), std::ostream_iterator<int>(std::cout, ","));
  std::cout << "\n";
  std::generate(AA.begin(), AA.end(), ir);
  std::copy(AA.begin(), AA.end(), std::ostream_iterator<int>(std::cout, ","));
  std::cout << "\n";

  for(int i=0; i<10; ++i)
      std::cout << ir() << ((i ==9) ? "\n" : ",");
````

Svaki novi poziv funkcije `std::generate()` mora generirati novi slučajni niz. U 
implementaciji koristiti `<chrono>` biblioteku.

