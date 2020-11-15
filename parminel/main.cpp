#include "minelement.h"
#include "random_int.h"
#include "clock.h"
#include <iostream>
#include <vector>
#include <iomanip>

//compare funkcija koja vraca true ako je a manji od b, inace false
bool cmp(int a, int b)
{
   return a < b;
}

/*Uzela sam za primjer 1-dim do 100000000-dim vektor cije su komponente random brojevi uniformno distribuirani na intervalu [37, 3000].
 Vidimo da je serijski accumulate algoritam brzi od paralelnog sve do N = 100000. Kada dodemo do 100000-dim vektora, tada paralelni algoritam
postaje brzi.*/
int main(int argc, char * argv[])
{
  int N = 1000000;
  if(argc > 1)
      int N = std::atoi(argv[1]);

  /*std::cout << "Testni vektor:";
  for (std::vector<int>::iterator it=source.begin(); it!=source.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';*/
  
  for(int i = 0; i < 9; ++i){
    N = (int)pow(10, i); //dimenzija vektora
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "vector dimension = " << N << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    RandomInt ri(37,3000);  
    std::vector<int> source(N); 
    std::generate(std::begin(source), std::end(source), ri); //novi vektor
    Clock ser_myClock; //pokreni sat
    const auto ser_min = std::min_element(std::begin(source), std::end(source), cmp);
    auto ser_time = ser_myClock.stop(Clock::microsec); //zaustavi sat
    std::cout << "serial minimum = " << std::setprecision(10) << *ser_min << ", time = " << ser_time << " us" << std::endl;

    Clock par_myClock; //pokreni sat
    const auto par_min = par_min_element(std::begin(source), std::end(source), cmp);
    auto par_time = par_myClock.stop(Clock::microsec); //zaustavi sat
    std::cout << "parallel minimum = " << std::setprecision(10) << *par_min << ", time = " << par_time << " us" << std::endl;
  }

  return 0;
}
