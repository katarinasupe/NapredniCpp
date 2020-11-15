#include <thread>
#include <numeric>

// Serijski accumulate() algoritam koji se poziva u svakoj programskoj niti.
template <typename T, typename Iterator>
void block_accumulate(Iterator first, Iterator last, T & result){
    T init = 0;
    result = std::accumulate(first, last, init);
}

// Paralelni accumulate algoritam
template <typename T, typename Iterator>
T parallel_accumulate(Iterator first, Iterator last, T init){
    auto length = std::distance(first, last);
    if(!length)
        return init; 

    int phard = std::thread::hardware_concurrency();
    if(!phard)
        phard = 2;  
    int n = 1000;  
    int pmax = (length > n) ? (int)length/n : 1;  
    int p = std::min(pmax, phard);  

    std::vector<T> results(p); // mjesto za rezultat izračunavanja
    std::vector<std::thread> threads(p-1); // paralelne programske niti

    auto block_size = length/p;
    auto block_first = first;
    auto block_last = block_first;
    for(int i=0; i < p-1; ++i){
        std::advance(block_last, block_size);
        threads[i] = std::thread(block_accumulate<T,Iterator>,
                                block_first, block_last, std::ref(results[i]));
        block_first = block_last;
    }
    // Ostatak sumiramo u glavnoj programskoj niti.
    results[p-1] = std::accumulate(block_first, last, T());  

    for(int i=0; i < p-1; ++i)
        threads[i].join();  
    // Kad sve dretve završe glavna će obaviti konačno sumiranje.
    return std::accumulate(results.begin(), results.end(), init); 
}
