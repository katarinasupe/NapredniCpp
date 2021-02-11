#pragma once

#include <vector>
#include <list>
#include <ostream>
#include <optional>
#include <shared_mutex>

// Rječnik na bazi hash tabele

template <typename Key, typename Value, typename Hash = std::hash<Key>>
class HashTable{
    private:
    class Bucket{ 
        public:
        using BucketValue = std::pair<Key, Value>; 
        Bucket()
        {
            mData = std::list<BucketValue>();
        }

        int index_of_key(Key key)
        {
            std::list<BucketValue>::iterator it;
            it = std::find_if(mData.begin(), mData.end(), [&key](BucketValue bucket_value){ return bucket_value.first == key; });
            if (it != mData.end())
            {                
                return std::distance(mData.begin(), it);
            } 
            else{
                return -1;
            }
                
        }

        std::optional<Value> get_value_by_key(Key key)
        {
            std::shared_lock<std::shared_mutex> lk(mMutex);
            std::optional<Value> optional_value;
            int index = index_of_key(key);
            if(index != -1)
            {
                std::list<BucketValue>::iterator it = mData.begin();
                std::advance (it, index);
                optional_value = it->second;
                // return std::optional<Value>{it->second};
            }
            //return std::nullopt;
            return optional_value;
        }

        void add_or_update_bucket_value(Key key, Value value)
        {
            std::lock_guard<std::shared_mutex> lk(mMutex);
            //ako bucket_value.first (key) već postoji u listi BucketValuea mData, onda promijeni bucket_value.second (value)
            int index = index_of_key(key);
            if(index != -1)
            {
                std::list<BucketValue>::iterator it = mData.begin();
                std::advance (it, index);
                it->second = value;
            }
            else //inace samo dodaj u listu BucketValuea
            {            
                BucketValue bucket_value = std::make_pair(key, value);
                mData.push_back(bucket_value);
            } 
        }

        bool remove_from_bucket(Key key)
        {               
            std::lock_guard<std::shared_mutex> lk(mMutex);
            int index = index_of_key(key);
            if(index != -1)
            {
                std::list<BucketValue>::iterator it = mData.begin();
                std::advance (it, index);
                mData.erase(it);
                return true;
            }
            else{
                return false;
            }
        }

        void print_bucket_value(std::ostream & out)
        {
            std::shared_lock<std::shared_mutex> lk(mMutex);
            for( BucketValue pair : mData ){
                out << "Key = " << pair.first << std::endl;
                out << "Value = " << pair.second << std::endl;
                out << std::endl;
            }
        }

        // Nadi prvi element na koje predikat vraca "true".
        template <typename Predicat>
        std::optional<BucketValue> find_if_bucket(Predicat p) const
        {
            std::shared_lock<std::shared_mutex> lk(mMutex);
            for( BucketValue pair : mData ){
                if(p(pair))
                    return std::optional<BucketValue>{pair};
            }
            return std::nullopt;
        }

        // Vrati listu svih elemenata na kojima predikat vraca "true".
        template <typename Predicat>
        std::list<BucketValue> find_all_if_bucket(Predicat p) const
        {
            std::shared_lock<std::shared_mutex> lk(mMutex);
            std::list<BucketValue> *list_of_bucket_value = new std::list<BucketValue>();
            for( BucketValue pair : mData ){
                if(p(pair))
                    list_of_bucket_value->push_back(pair);
            }

            return *list_of_bucket_value;
        }

        size_t size_bucket() const
        {
            std::shared_lock<std::shared_mutex> lk(mMutex);
            size_t size = 0;
            for( BucketValue pair : mData ){
                size++;
            }
            return size;
        }
        private:
        std::list<BucketValue> mData;
        mutable std::shared_mutex mMutex;
    };
    // Podaci.
    std::vector<Bucket> mBuckets;
    using  BucketValue = typename Bucket::BucketValue; 
    Hash mHasher;

    public:
        HashTable(size_t N = 101, Hash hasher = Hash()) : mBuckets(N), mHasher(hasher)
        {
        }
        HashTable(HashTable const &) = delete;
        HashTable & operator=(HashTable const &) = delete;

        //  Vrati vrijednost koja odgovara kljucu
        std::optional<Value> value(Key key)
        {
            std::optional<Value> optional_value; //prazan
            int hashed_key = mHasher(key) % 101;   
            optional_value = mBuckets[hashed_key].get_value_by_key(key);

            return optional_value;
            //return std::optional<Value>{mBuckets[hashed_key].get_value_by_key(key)};
        }

        // Ukloni kljuc ako je prisutan i vrati true ako je uklonjen i false inace.
        bool remove(Key key)
        {
            int hashed_key = mHasher(key) % 101;   
            return mBuckets[hashed_key].remove_from_bucket(key);
        }

        // Dodaj par "(key, value)" ako nije u spremniku.
        // U suprotnom kljucu "key" daj vrijednost "value".
        void add_or_update(Key key, Value value)
        {
            int hashed_key = mHasher(key) % 101;   
            mBuckets[hashed_key].add_or_update_bucket_value(key, value);
        }

        // Ispis citave strukture (pretinci i povezane liste) radi kontrole.
        void print(std::ostream & out)
        {
                for(auto it = mBuckets.begin(); it != mBuckets.end(); it++){
                    it->print_bucket_value(out);
                }
        }

        // Broj elemenata u tabeli.
        size_t size() const
        {
            size_t size = 0;
            for(auto it = mBuckets.begin(); it != mBuckets.end(); it++){
                size += it->size_bucket();
            }
            return size;
        }

        // Nadi prvi element na koje predikat vraca "true".
        template <typename Predicat>
        std::optional<BucketValue> find_if(Predicat p) const
        {
            std::optional<BucketValue> optional_find;
            for( auto it = mBuckets.begin(); it != mBuckets.end(); it++){
                optional_find = it->find_if_bucket(p);
                if(optional_find.has_value())
                    return optional_find;
            }
            return std::nullopt; //tada optional<BucketValue> nema value -> has_value() == false
        }

        // Vrati listu svih elemenata na kojima predikat vraca "true".
        template <typename Predicat>
        std::list<BucketValue> find_all_if(Predicat p) const
        {
            std::list<BucketValue> *list_of_bucket_values = new std::list<BucketValue>();
            for( auto it = mBuckets.begin(); it != mBuckets.end(); it++){

                std::list<BucketValue> temp = it->find_all_if_bucket(p);
                if(!temp.empty())
                {
                    for(auto it1 = temp.begin(); it1 != temp.end(); it1++){
                        BucketValue bucket_value = *it1;
                        list_of_bucket_values->push_back(bucket_value);
                    }
                }    
            }
            return *list_of_bucket_values;
        }
};
