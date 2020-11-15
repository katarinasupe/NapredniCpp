#include "clock.h"
#include "paraccumulate.h"
#include "partransform.h"
#include <iostream>
#include <vector>
#include <iomanip>
#include <string>  
#include <algorithm>
#include <cmath>

//pomocna fja koja prima vektor koji ce transformirati i funkciju koju ce primijeniti na svaku komponentu danog vektora
//vraca transformirani vektor (transformiran paralelnim transform algoritmom)
template <typename T, typename F>
std::vector<T> calculate(std::vector<T> v, F&& f){
    parallel_transform(std::begin(v), std::end(v), std::forward<F>(f)); //lokalno promijeni v
    return v;
}

// Glavni program koji testira paralelnu verziju accumulate() algoritma.
// Potrebno je na par primjera usporediti serijsku i paralelnu verziju 
// algoritma i usporediti njihova vremena izvršavanja kao funkciju veličine
// spremnika na kojem rade
int main(){

    /*Uzela sam za primjer 2-dim do 200000000-dim vektor cija je svaka komponenta jednaka 10. Vidimo da je za manje dimenzije
    serijski accumulate algoritam brzi od paralelnog (za 200000-dim vektor je serijski jos malo brzi). Kada dodemo na 2000000-dim vektor, 
    tada paralelni algoritam postaje brzi te vidimo da sto je veca dimenzija vektora, paralelni algoritam postaje sve brzi. 
    Za 200000000-dim vektor nas paralelni algoritam je gotovo 19 puta brzi od serijskog.*/

    /*Za transform paralelni algoritam primjecujemo da postaje brzi od serijskog za dimenziju vektora 20000. 
    Ukoliko pokusamo s kompleksnijom transformacijom (npr. sqrt(x)), tada ćemo vidjeti da je benefit paralelizacije transform
    algoritma manji.*/

    std::vector<int> par_result;
    std::vector<int> ser_result; 

    for(int i = 0; i < 9; ++i){

        int n =  2 * (int)pow(10, i); //dimenzija vektora
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "vector dimension = " << n << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        std::vector<int> v(n, 10); //popuni desetkama
        std::cout << std::endl;

        //mjerenje paralelnog accumulate u mikrosekundama
        Clock par_myClock;
        auto par_sum = parallel_accumulate(v.begin(), v.end(), 0);
        auto par_time = par_myClock.stop(Clock::microsec); //zaustavi sat
        std::cout << "parallel_accumulate sum = " << std::setprecision(10) << par_sum << ", time = " << par_time << " us" << std::endl;

        //mjerenje serijskog accumulate u mikrosekundama
        Clock ser_myClock;
        auto ser_sum = std::accumulate(v.begin(), v.end(), 0.0); 
        auto ser_time = ser_myClock.stop(Clock::microsec);
        std::cout << "serial_accumulate sum = " << std::setprecision(10) << ser_sum << ", time = " << ser_time << " us" << std::endl;
        std::cout << std::endl;

        //mjerenje paralelnog transform u mikrosekundama
        Clock par_myClock_transf;
        par_result = calculate(v, [](int x) { return x * 3; }); //primjer: fja koja utrostrucuje svaku komponentu vektora v
        auto par_time_transf = par_myClock_transf.stop(Clock::microsec);
        std::cout << "parallel_transform time = " << std::setprecision(10) << par_time_transf << " us" << std::endl;
    
        //mjerenje serijskog transform u mikrosekundama
        Clock ser_myClock_transf;
        std::transform(v.begin(), v.end(), std::back_inserter(ser_result), [](int x) { return x * 3; });  
        auto ser_time_transf = ser_myClock_transf.stop(Clock::microsec);
        std::cout << "serial_transform time = " << std::setprecision(10) << ser_time_transf << " us" << std::endl;
        std::cout << std::endl;

        //mjerenje paralelnog transform u mikrosekundama
        Clock par_myClock_transf_compl;
        par_result = calculate(v, [](int x) { return (int)std::sqrt(x); }); //primjer: korijen 
        auto par_time_transf_compl = par_myClock_transf_compl.stop(Clock::microsec);
        std::cout << "parallel_transform_compl time = " << std::setprecision(10) << par_time_transf_compl << " us" << std::endl;
    
        //mjerenje serijskog transform u mikrosekundama
        Clock ser_myClock_transf_compl;
        std::transform(v.begin(), v.end(), std::back_inserter(ser_result), [](int x) { return (int)std::sqrt(x); });  
        auto ser_time_transf_compl = ser_myClock_transf_compl.stop(Clock::microsec);
        std::cout << "serial_transform_compl time = " << std::setprecision(10) << ser_time_transf_compl << " us" << std::endl;
        std::cout << std::endl;

    }

//provjera rada transform metode - nisam htjela vracati izlaze iznad, jer su ti vektori prevelike dimenzije
/*
    std::vector<int> v = { 1,2,3 };  
    std::vector<int> par_result;
    std::vector<int> ser_result;  

    par_result = calculate(v, [](int x) { return x * 3; }); //fja koja utrostrucuje vektor
    std::for_each(par_result.begin(), par_result.end(),  [](const int& s) { std::cout << s << std::endl; }); 
  
    std::transform(v.begin(), v.end(), std::back_inserter(ser_result), [](int x) { return x * 3; });  
    std::for_each(ser_result.begin(), ser_result.end(),  [](const int& s) { std::cout << s << std::endl; }); */

    return 0;
}
