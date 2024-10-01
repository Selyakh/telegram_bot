// Copyright (c) 2024 Irina Selyakh
//
// Данное программное обеспечение распространяется на условиях лицензии MIT.
// Подробности смотрите в файле LICENSE

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include "parse.h"
#include "searching.h"

TEST_CASE("TokenizeNumber correct number analysis", "[TokenizeNumber]") {
  REQUIRE(TokenizeNumber("123") == std::vector<size_t>{1, 2, 3});
  REQUIRE(TokenizeNumber("908") == std::vector<size_t>{9, 0, 8});
  REQUIRE(TokenizeNumber("001") == std::vector<size_t>{0, 0, 1});
  REQUIRE(TokenizeNumber("4567") == std::vector<size_t>{4, 5, 6, 7});
}

TEST_CASE("Checking the input value", "[CheckingInputValue]") {
  REQUIRE(CheckingInputValue("123") == true);
  REQUIRE(CheckingInputValue("908") == true);
  REQUIRE(CheckingInputValue("305") == true);
  REQUIRE(CheckingInputValue("012") == false);
  REQUIRE(CheckingInputValue("11") == false);
  REQUIRE(CheckingInputValue("112") == false);
  REQUIRE(CheckingInputValue("12a") == false);
  REQUIRE(CheckingInputValue("1234") == false);
}

TEST_CASE("ChangingType should convert size_t to string", "[ChangingType]") {
  REQUIRE(ChangingType(123) == "123");
  REQUIRE(ChangingType(908) == "908");
  REQUIRE(ChangingType(305) == "305");
}

TEST_CASE("SetRandom generates a random 3-digit number", "[CounterVariables]") {
  CounterVariables::bot_memory = {1, 2, 3, 4, 5, 6, 0, 7, 8, 9};
  size_t generated_number = CounterVariables::SetRandom(0);
  std::string number_str = ChangingType(generated_number);
  REQUIRE(number_str.size() == 3);
  REQUIRE(CheckingInputValue(number_str) == true);
}