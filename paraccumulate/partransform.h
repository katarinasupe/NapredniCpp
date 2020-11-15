#include <thread>

// Paralelni transform algoritam
template <typename RandomAccessIterator, typename F>
void parallel_transform(RandomAccessIterator first, RandomAccessIterator last, F&& f){
    auto length = std::distance(first, last);
    int phard = std::thread::hardware_concurrency();
    if(!phard)
        phard = 2;  
    int n = 1000;  
    int pmax = (length > n) ? (int)length/n : 1;  
    int p = std::min(pmax, phard);  //broj dretvi
    std::vector<std::thread> threads; // paralelne programske niti

    auto begin = first;
    auto end = begin;
    length = length/p;

    for(int i=0; i < p  ; ++i){
        begin = end;
        if(i == (p - 1)) 
            end = last;
        else std::advance(end, length);
        threads.emplace_back([begin, end, &f](){
            std::transform(begin, end, begin, std::forward<F> (f));
        });
    }
    for(int i=0; i < p; ++i)
        threads[i].join();  
}

    
