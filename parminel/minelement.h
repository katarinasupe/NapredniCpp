/* Kreirati paralelni min_element() algoritam koristeći programske niti. 
 * Vidi https://en.cppreference.com/w/cpp/algorithm/min_element
 */
#include <thread>
#include <vector>
#include <algorithm>

template<typename ForwardIt, typename Compare>
void block_minimum(ForwardIt first, ForwardIt last, Compare comp, ForwardIt& minimum)
{
    minimum = std::min_element(first, last, comp);
}

template<typename ForwardIt, typename Compare>
ForwardIt par_min_element(ForwardIt first, ForwardIt last, Compare comp)
{
    auto length = std::distance(first, last);
    int phard = std::thread::hardware_concurrency();
    if(!phard)
        phard = 2;  
    int n = 1000;  
    int pmax = (length > n) ? (int)length/n : 1;  
    int p = std::min(pmax, phard);  //broj dretvi
    std::vector<std::thread> threads(p-1); // paralelne programske niti
    std::vector<ForwardIt> minimums(p); //vektor iteratora
   
    auto block_size = length/p;
    auto block_first = first;
    auto block_last = block_first;

    for(int i=0; i < p-1; ++i)
    {
        std::advance(block_last, block_size); //iterator block_last inkrementiraj za block_size elemenata
        threads[i] = std::thread(block_minimum<ForwardIt,Compare>, block_first, block_last, comp, std::ref(minimums[i]));
        block_first = block_last;
    }

    minimums[p-1] = std::min_element(block_first, last, comp);  

    for(int i=0; i < p-1; ++i)
    {
        threads[i].join();  
    }

    return *std::min_element(minimums.begin(), minimums.end(), [](const ForwardIt& a, const ForwardIt& b){ return *a < *b; });
    /* Kako je minimums vektor koji sadrzi elemente tipa ForwardIt, onda u argument fje min_element proslijedujemo lambdu
    koja govori fji min_element kako usporedivati dane ForwardIt iteratore (a nek ih usporeduje s obzirom na njihovu pravu vrijednost).
    Dereferenciramo na kraju, td se vraca tip ForwardIt */
}
   

