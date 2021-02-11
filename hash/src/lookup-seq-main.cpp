
#include "lookup-seq.h"

#include <string>
#include <iostream>

bool predicate_function_key(std::pair<std::string, int> bucket_value)
{
    return bucket_value.first == "Katarina"; 
}

bool predicate_function_value(std::pair<std::string, int> bucket_value)
{
    return bucket_value.second == 989898; 
}

//testiranje pomocu print() funkcije za provjeru valjanosti testova
int main(){

    HashTable<std::string, int> *hash_table = new HashTable<std::string, int>();
    hash_table->add_or_update("Goran", 919191);
    hash_table->add_or_update("Katarina", 888888);
    hash_table->add_or_update("Darko", 989898);
    hash_table->add_or_update("Kata", 989898);
    hash_table->add_or_update("Goran", 919192);

    std::cout << "HashTable: " << std::endl;
    hash_table->print(std::cout);
    
    std::cout << "Testiranje value: " << std::endl;
    if(hash_table->value("Goran").has_value()){
        std::cout << "Vrijednost od 'Goran': " << hash_table->value("Goran").value() <<std::endl;
    }
    else{
        std::cout << "Ne postoji Goran u imeniku!" << std::endl;
    }
    if(hash_table->value("Katarina").has_value()){
        std::cout <<  "Vrijednost od 'Katarina': " << hash_table->value("Katarina").value() <<std::endl;
    }
    else{
        std::cout << "Ne postoji Katarina u imeniku!" << std::endl;
    }

    std::cout << "Velicina hash tablice: " << hash_table->size() << std::endl;

    std::cout << "Testiranje predikata za find if na key (ako je key == 'Katarina'): " << std::endl;
    if(hash_table->find_if(predicate_function_key).has_value())
    {
        std::pair<std::string, int> bucket_value = hash_table->find_if(predicate_function_key).value();
        std::cout <<"Key: " << bucket_value.first << std::endl;
        std::cout <<"Value: " << bucket_value.second << std::endl;
    }
    else{
        std::cout << "Za nijedan element predikat ne vraca true." << std::endl;
    }
    std::cout << "Testiranje predikata za find if na value (ako je value == '989898'): " << std::endl;
    if(hash_table->find_if(predicate_function_value).has_value())
    {
        std::pair<std::string, int> bucket_value = hash_table->find_if(predicate_function_value).value();
        std::cout <<"Key: " << bucket_value.first << std::endl;
        std::cout <<"Value: " << bucket_value.second << std::endl;
    }
    else{
        std::cout << "Za nijedan element predikat ne vraca true." << std::endl;
    }

    std::cout << "Testiranje predikata za find all_if (ako je value == '989898'): " << std::endl;
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
  
    hash_table->remove("Kata");
    std::cout << "Nova HashTable (nakon uklanjanja elementa s key-em 'Kata'): " << std::endl;
    hash_table->print(std::cout);
    return 0;
}

