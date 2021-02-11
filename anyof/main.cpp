#include "par_any_of.h"
#include "clock.h"
#include <future>
#include <atomic>
#include <chrono>
#include <iostream>
#include <algorithm>
#include <vector>
#include <iomanip>

/*Funkcija koja provjerava je li dani broj n paran, te u tom slucaju vraca true, inace false.*/
bool is_even(int n)
{
    return (n % 2 == 0);
}

/*Funkcija koja provjerava je li dani broj n neparan, te u tom slucaju vraca true, inace false.*/
bool is_odd(int n)
{
    return (n % 2 != 0);
}

/*Funkcija koja provjerava je li dani broj n jednak 10, te u tom slucaju baca izuzetak, inace true.*/
bool exception_p(int n){
    if(n == 10){
        throw std::runtime_error("Iznimka u funkciji exception_p!");
    }
    return true;
}


/*U prva dva primjera provjeravamo je li barem jedna komponenta vektora dimenzije N, cija je svaka komponenta broj M,
parna, a u drugom je li neparna. Treci primjer je primjer propagiranja izuzetka, gdje fja exception_p na ovakom vektoru sigurno
baca iznimku. Koristeci clock.h, zakljucujemo da paralelizacija nije bila isplativa.*/
int main()
{
    try
    {

        int N = 10000;
        int M = 10;
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "vektor dimenzije " << N << " cija je svaka komponenta broj " << M << "." << std::endl;
        std::vector<int> v(N, M);
        std::cout << "----------------------------------------" << std::endl;

        std::cout << "Je li barem jedna komponenta vektora parna?" << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        Clock ser_myClock;
        std::cout << "std::any_of: " << any_of(v.begin(), v.end(), is_even) << std::endl;
        auto ser_time = ser_myClock.stop(Clock::microsec);
        std::cout << "std::any_of time = " << std::setprecision(10) << ser_time << " us" << std::endl;

        Clock par_myClock;
        std::cout << "parany_of: " << parany_of(v.begin(), v.end(), is_even) << std::endl;
        auto par_time = par_myClock.stop(Clock::microsec);
        std::cout << "parany_of time = " << std::setprecision(10) << par_time << " us" << std::endl;
        std::cout << "----------------------------------------" << std::endl;

        std::cout << "Je li barem jedna komponenta vektora neparna?" << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        Clock ser_myClock1;
        std::cout << "std::any_of: " << any_of(v.begin(), v.end(), is_odd) << std::endl;
        auto ser_time1 = ser_myClock1.stop(Clock::microsec);
        std::cout << "std::any_of time = " << std::setprecision(10) << ser_time1 << " us" << std::endl;

        Clock par_myClock1;
        std::cout << "parany_of: " << parany_of(v.begin(), v.end(), is_odd) << std::endl;
        auto par_time1 = par_myClock1.stop(Clock::microsec);
        std::cout << "parany_of time = " << std::setprecision(10) << par_time1 << " us" << std::endl;

        std::cout << "----------------------------------------" << std::endl;

        std::cout << "Primjer propagiranja izuzetka:" << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "parany_of: " << parany_of(v.begin(), v.end(), exception_p) << std::endl;
    }
    catch (std::exception const &e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
    catch (...)
    {
        std::cout << "Exception! " << std::endl;
    }

    return 0;
}
