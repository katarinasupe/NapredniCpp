#include <chrono>

// Klasa clock koja pojednostavljuje mjerenje vremena izvršavanja programa.
// Vrijeme se može mjeriti samo u sekundama, milisekundama ili mikrosekundama.
// Sat se pokreće u konstruktoru ili pozivom metode start(). Zaustavlja se 
// pozivom metode stop() koja vraća proteklo vrijeme od posljednjeg poziva 
// start() metode u traženim jedinicama.
class Clock{
    public:
        enum Interval{sec, millisec, microsec};
        Clock(){start();}
        // Pokreni sat.
        void start(){
            begin = mClock.now();
        }
        // Zaustavi sat i vrati proteklo vrijeme u unit jedinicama.
        double stop(Interval unit){
            end = mClock.now();
            switch(unit){
                case sec:
                    return (double)std::chrono::duration_cast<std::chrono::seconds>(end - begin).count();
                    break;
                case millisec:
                    return (double)std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
                    break;
                case microsec:
                    return (double)std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
                    break;
                default:
                    return -1;
                    break;
            }
        }
    private:
        std::chrono::high_resolution_clock mClock;
        std::chrono::time_point<std::chrono::high_resolution_clock> begin;
        std::chrono::time_point<std::chrono::high_resolution_clock> end;
};

// Primjer upotrebe:
//  
//    clock.start();
//    double sum = std::accumulate(niz, niz+SIZE, 0.0);
//    auto time = clock.stop(Clock::microsec);
//    std::cout << "ser. sum = " << std::setprecision(10) << sum << ", time = " << time << " us" << std::endl;

