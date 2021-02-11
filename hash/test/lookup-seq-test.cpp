#include "gtest/gtest.h"
#include "lookup-seq.h"

class HashTableTest : public ::testing::Test {
 protected:
  void SetUp() override {
     hash_table.add_or_update("Goran", 919191);
     hash_table.add_or_update("Katarina", 888888);
     hash_table.add_or_update("Darko", 989898);
     hash_table.add_or_update("Kata", 989898);
     hash_table.add_or_update("Goran", 919192);
  }

    HashTable<std::string, int> hash_table_0;
    HashTable<std::string, int> hash_table;
};

/*provjera je li pocetna velicina neke HashTable hash_table_0 == 0*/
TEST_F(HashTableTest, IsEmptyInitially) {
    EXPECT_EQ(hash_table_0.size(), 0);
}

/*provjera je li testni HashTable neprazan*/
TEST_F(HashTableTest, IsNonEmpty)
{
    EXPECT_GT((int)hash_table.size(), 0); //size_t
}

/*provjera je li velicina HashTable bas == 4*/
TEST_F(HashTableTest, IsSize4)
{
    EXPECT_EQ(hash_table.size(), 4);
}

/*provjera je li dodavanje ili updatenanje parova kljuc-vrijednost dobro napravljeno te value funkcije*/
TEST_F(HashTableTest, AddingOrUpdating)
{
    //Goran postoji (assert td ne ide dalje ako ne postoji)
    ASSERT_TRUE(hash_table.value("Goran").has_value());
    //ocekujemo da je value od key-a "Goran" == 919192, sto je updateana vrijednost
    EXPECT_NE(hash_table.value("Goran"), 919191);
    EXPECT_EQ(hash_table.value("Goran"), 919192);

    //provjera jesu li svi parovi dobro dodani ili updateani
    ASSERT_TRUE(hash_table.value("Katarina").has_value());
    EXPECT_EQ(hash_table.value("Katarina"), 888888);

    ASSERT_TRUE(hash_table.value("Darko").has_value());
    EXPECT_EQ(hash_table.value("Darko"), 989898);

    ASSERT_TRUE(hash_table.value("Kata").has_value());
    EXPECT_EQ(hash_table.value("Kata"), 989898);
}

/*provjera da u HashTable ne postoji element s key-em "Anita"*/
TEST_F(HashTableTest, ValueOfNonExisting)
{
    EXPECT_FALSE(hash_table.value("Anita").has_value());
}

/*testiranje funkcije find_if*/
TEST_F(HashTableTest, FindIf)
{
    //nadi prvi element s key-em "Katarina" -> postoji 
    ASSERT_TRUE(hash_table.find_if([](std::pair<std::string, int> bucket_value){return bucket_value.first == "Katarina";}).has_value());
    EXPECT_EQ(hash_table.find_if([](std::pair<std::string, int> bucket_value){return bucket_value.first == "Katarina";}).value().first, "Katarina");
    EXPECT_EQ(hash_table.find_if([](std::pair<std::string, int> bucket_value){return bucket_value.first == "Katarina";}).value().second, 888888);

    //nadi prvi element s key-em "Marina" -> ne postoji 
    EXPECT_FALSE(hash_table.find_if([](std::pair<std::string, int> bucket_value){return bucket_value.first == "Marina";}).has_value());

    //nađi prvi element s value-em 989898 -> postoje 2, ali naci ce 'prvi' (ovisno o hash fji) -> znam da je "Kata" koristeci pomocni ispis (print() fja iz klase HashTable)
    ASSERT_TRUE(hash_table.find_if([](std::pair<std::string, int> bucket_value){return bucket_value.second == 989898;}).has_value());
    EXPECT_NE(hash_table.find_if([](std::pair<std::string, int> bucket_value){return bucket_value.second == 989898;}).value().first, "Darko");
    EXPECT_EQ(hash_table.find_if([](std::pair<std::string, int> bucket_value){return bucket_value.second == 989898;}).value().first, "Kata");
    EXPECT_EQ(hash_table.find_if([](std::pair<std::string, int> bucket_value){return bucket_value.second == 989898;}).value().second, 989898);

    //nađi prvi element s value-em 123456 -> ne postoji 
    EXPECT_FALSE(hash_table.find_if([](std::pair<std::string, int> bucket_value){return bucket_value.second == 123456;}).has_value());
}

/*testiranje funkcije find_all_if*/
TEST_F(HashTableTest, FindAllIf)
{
    ASSERT_TRUE(!hash_table.find_all_if([](std::pair<std::string, int> bucket_value){return bucket_value.second == 989898;}).empty());
    //znamo da je prvi element liste s key-em "Kata", jer se tako hashiralo -> bilo je testirano pomocu print() fje iz HashTable klase
    EXPECT_EQ(hash_table.find_all_if([](std::pair<std::string, int> bucket_value){return bucket_value.second == 989898;}).front().first, "Kata");
    //znamo da postoje takva dva elementa u listi, pa je drugi zapravo zadnji -> back() i on ima key "Darko"
    EXPECT_EQ(hash_table.find_all_if([](std::pair<std::string, int> bucket_value){return bucket_value.second == 989898;}).back().first, "Darko");

    //testiranje na nepostojecem key-u
    ASSERT_TRUE(hash_table.find_all_if([](std::pair<std::string, int> bucket_value){return bucket_value.first == "Anita";}).empty());
}

/*testiranje funkcije remove*/
TEST_F(HashTableTest, Remove)
{
    //uklanjanje elementa s key-em "Kata"
    EXPECT_TRUE(hash_table.remove("Kata"));
    //ocekujemo da vise ne postoji pair s key-em "Kata"
    EXPECT_FALSE(hash_table.find_if([](std::pair<std::string, int> bucket_value){return bucket_value.first == "Kata";}).has_value());
    //ne moze ukloniti element koji ne postoji
    EXPECT_FALSE(hash_table.remove("Anita"));
}