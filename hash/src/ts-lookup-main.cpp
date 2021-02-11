#include "ts-lookup.h"

#include <string>
#include <iostream>
#include <thread>
#include <array>


bool predicate_function_value(std::pair<std::string, int> bucket_value)
{
    return bucket_value.second == 777; 
}

/*promijeni vrijednost elementu s odredenim key-em u hash_table*/
void change_value(HashTable<std::string, int> *hash_table, std::string key, int value)
{
    hash_table->add_or_update(key, value);
}


/*ukloni iz hash_table*/
void remove_bucket_value(HashTable<std::string, int> *hash_table, std::string key)
{
    hash_table->remove(key);
}

/*ocekivamo da ce u hash_table biti dodana <Marina, 777> i <Anita, 777>, vrijednost od "Katarina" ce se promijeniti, ali poslije svejedno
ukloniti te ce se vrijednost od "Kata" updateati na 1234.*/
void add_and_remove(HashTable<std::string, int> *hash_table)
{
    hash_table->add_or_update("Marina", 777); //add
    hash_table->add_or_update("Katarina", 1234); //update
    hash_table->add_or_update("Kata", 1234); //update
    hash_table->remove("Katarina"); //remove
    hash_table->add_or_update("Anita", 777); //add
}

// TODO samo testiranje paralelne verzije koda.
int main(){
    HashTable<std::string, int> *hash_table = new HashTable<std::string, int>();
    hash_table->add_or_update("Goran", 0);
    hash_table->add_or_update("Katarina", 123);
    hash_table->add_or_update("Kata", 12345);
    std::cout << "Pocetna hash table: " << std::endl;
    hash_table->print(std::cout);
    std::cout << "Velicina hash_table: " << hash_table->size() << std::endl;
    std::array<std::thread, 100> threads;

    for(int i = 0; i < 100; i++)
    {
        threads[i] = std::thread( [=] { change_value( hash_table, "Goran", i); } );
        //nova vrijednost ce biti id zadnje dretve koja je promijenila vrijednost
    }

    for(int i = 0; i < 100; i++)
    {
        threads[i].join();
    }

    std::cout << "Hash table nakon updateanja vrijednosti: " << std::endl;
    hash_table->print(std::cout);
    std::cout << "Velicina hash_table: " << hash_table->size() << std::endl;


    for(int i = 0; i < 100; i++)
    {
        threads[i] = std::thread( [=] { remove_bucket_value( hash_table, "Goran"); } );
    }

    for(int i = 0; i < 100; i++)
    {
        threads[i].join();
    }

    std::cout << "Hash table nakon uklanjanja elementa: " << std::endl;
    hash_table->print(std::cout);
    std::cout << "Velicina hash_table: " << hash_table->size() << std::endl;


    for(int i = 0; i < 100; i++)
    {
        threads[i] = std::thread( [=] { add_and_remove( hash_table ); } );
    }

    for(int i = 0; i < 100; i++)
    {
        threads[i].join();
    }

    std::cout << "Hash table nakon dodavanja, updateanja i uklanjanja elemenata: " << std::endl;
    hash_table->print(std::cout);
    std::cout << "Velicina hash_table: " << hash_table->size() << std::endl;

    std::cout << "Testiranje predikata za find if na value: " << std::endl;
    if(hash_table->find_if(predicate_function_value).has_value())
    {
        std::pair<std::string, int> bucket_value = hash_table->find_if(predicate_function_value).value();
        std::cout <<"Key: " << bucket_value.first << std::endl;
        std::cout <<"Value: " << bucket_value.second << std::endl;
    }
    else{
        std::cout << "Za nijedan element predikat ne vraca true." << std::endl;
    }

    std::cout << "Testiranje predikata za find all_if na key: " << std::endl;
    if(!hash_table->find_all_if(predicate_function_value).empty())
    {
        //ispisi listu parova
        std::list<std::pair<std::string,int>> bucket_values = hash_table->find_all_if(predicate_function_value);
        for(std::pair<std::string,int> pair : bucket_values)
        {
            std::cout << "Key: " << pair.first << std::endl;
            std::cout << "Value: " << pair.second << std::endl;
        }
    }
    else{
        std::cout << "Za nijedan element predikat ne vraca true." << std::endl;
    }

    return 0;
}

