#include <algorithm>
#include <atomic>
#include <thread>
#include <vector>
#include <future>
#include <chrono>

std::atomic<bool> is_any_true = {false}; //globalna zajednicka varijabla

/*any_of() algoritam za svaki blok (dretvu) koji postavlja atomic<bool> is_any_true na true, cim je neki element true i signalizira drugim
dretvama da prekinu radom. Tada postavlja vrijednost promisea takoder na true. Ukoliko se desila iznimka, dretve zavrsavaju s radom, 
a iznimka se propagira u glavnu dretvu.*/
template <typename InputIt, typename UnaryPredicate>
void block_any_of(InputIt first, InputIt last, UnaryPredicate p, std::promise<bool> &promise)
{
  try
  {
    for (; first != last; ++first)
    {
      if (is_any_true)
      {
        break;
      }

      if (p(*first))
      {
        is_any_true = true;
        promise.set_value(true);
      }
    }
  }
  catch (...)
  {
    try
    {
      is_any_true = true;
      promise.set_exception(std::current_exception());
    }
    catch (...)
    {
    }
  }
}

/* Paralelni any_of() algoritam baziran na promise mehanizmu. */
template <typename InputIt, typename UnaryPredicate>
bool parany_of(InputIt first, InputIt last, UnaryPredicate p)
{
  is_any_true = {false};
  std::promise<bool> promise;
  std::future<bool> future = promise.get_future();

  auto length = std::distance(first, last);
  int phard = std::thread::hardware_concurrency();
  if (!phard)
    phard = 2;
  int n = 1000;
  int pmax = (length > n) ? (int)length / n : 1;
  int p_thread = std::min(pmax, phard);       //broj dretvi
  std::vector<std::thread> threads(p_thread); // paralelne programske niti

  auto block_size = length / p_thread;
  if (block_size < 10)
  {
    throw std::runtime_error("Svaka nit mora dobiti minimalno 10 elemenata!");
  }
  auto block_first = first;
  auto block_last = block_first;

  for (int i = 0; i < p_thread; ++i)
  {
    std::advance(block_last, block_size);
    threads[i] = std::thread(block_any_of<InputIt, UnaryPredicate>, block_first, block_last, p, std::ref(promise));
    block_first = block_last;
  }

  //pricekaj da dretve zavrse s radom
  for (int i = 0; i < p_thread; ++i)
  {
    threads[i].join();
  }

  //ako is_any_true nije postavljen na true te se nije desila iznimka, vrati false
  if (!is_any_true)
  {
    return false;
  }
  else //inace dohvati vrijednost postavljenu u promise (a to je true) i vrati je glavnoj dretvi
  {
    return future.get();
  }
}
