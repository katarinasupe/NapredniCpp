#include <random>

// Klasa koja predstavlja generator slučajnih brojeva tipa int uniformno distribuiranih 
// u zadanom rasponu.  Sučelje klase je dano ovdje:
class RandomInt
{
  public:
    RandomInt(int a, int b)
      : m_rd(std::random_device{}) //postavi novi seed
      , m_mt(m_rd())
      , m_dist(a, b)
    {}; 
    //konstruktor kopije
    RandomInt(RandomInt const& rhs)
      : m_rd(std::random_device{})
      , m_mt(m_rd())
      , m_dist(rhs.m_dist)
    {}; //postavi novi seed (koristi istu distribuciju)

    RandomInt& operator=(const RandomInt& rhs) = delete;

    int operator()()
    { 
       return m_dist(m_mt); //generiraj novu pseudoslucajnu vrijednost
    };

  private:
    //koristit cemo random_device koji ce biti seed za Mersenne Twister PRNG
    //random_device sam nije dovoljno dobar za uzastopno generiranje vise brojeva
    std::random_device m_rd; //generiramo jedan random broj koji je _seed_
    std::mt19937 m_mt; //Mersenne Twister PRNG
    std::uniform_int_distribution<int> m_dist; //dist(mt) producira random integer uniformno distribuiran na danom zatvorenom intervalu [a,b]
};
/*
- a, b su granice distribuiranih int-ova;
- konstruktor kopije (i konstruktor) mora postaviti novi _seed_ kako kopija ne bi 
  generirala isti niz kao i original;
- operator() generira novu pseudoslučajnu vrijednost.
*/


