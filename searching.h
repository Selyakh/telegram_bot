// Copyright (c) 2024 Irina Selyakh
//
// Данное программное обеспечение распространяется на условиях лицензии MIT.
// Подробности смотрите в файле LICENSE

#ifndef NUMBER_SEARCH_H
#define NUMBER_SEARCH_H
#include <unordered_set>
#include "parse.h"

constexpr int kMaximumAttempts = 8;  // максимальное количество попыток

bool CheckingInputValue(
    const std::string value);  // объявление ф-ции, которая проверяет корректность передаваемого на вход значени
std::string ChangingType(const size_t value);  // объявление ф-ции, которая меняет тип на string

struct CounterVariables {
  inline static int bulls = 0;
  inline static int cows = 0;
  inline static int counter = 0;
  inline static bool priority = true;  // приоритетность в игре
  inline static bool one_initialization_bot = true;  // для того, чтобы инициализация значения бота была разовая
  inline static std::vector<size_t> bot_memory;  // память бота из которой создается его значение
  inline static std::unordered_set<size_t> improved_bot_memory;  // улучшенная память)

  static bool Check(const size_t& value) {
    return improved_bot_memory.find(value) != improved_bot_memory.end();
  }

  [[nodiscard]] static size_t Calculate() {
    size_t value = 0;
    size_t first_digit = bot_memory[rand() % bot_memory.size()];
    size_t second_digit = bot_memory[rand() % bot_memory.size()];
    size_t third_digit = bot_memory[rand() % bot_memory.size()];
    return value = first_digit * 100 + second_digit * 10 + third_digit;
  }

  [[nodiscard]] static size_t SetRandom(size_t value) {
    if (bot_memory.empty()) {
      return value = rand() % 900 + 100;
    }

    value = Calculate();  // создание рандомного числа

    if (bot_memory.size() == 3) {  // если бот знает точные три числа, но не знает, как именно они стоят
      // данный блок не позволяет боту предоставить несколько раз одно и тоже число, что увеличивает его шансы на победу
      // поэтому была создана ранее переменная improved_bot_memory, которая в себе будет хранить повторяющиеся числа
      if (!Check(value)) {
        improved_bot_memory.insert(value);
        return value;
      }
      while (Check(value)) {
        value = Calculate();
      }
      improved_bot_memory.insert(value);
      return value;
    }
    return value;
  }
};

class Data {
  std::string user_;
  size_t computer_ = 0;

 public:
  Data() = default;

  explicit Data(const std::string user_value) : user_(user_value) {
    if (CounterVariables::one_initialization_bot) {  // разовая инициализация числа бота, только когда запускается новая
                                                     // игра
      computer_ = CounterVariables::SetRandom(computer_);
      while (!CheckingInputValue(ChangingType(computer_))) {
        computer_ = CounterVariables::SetRandom(computer_);
      }
      CounterVariables::one_initialization_bot = false;
    }
  }

  explicit Data(const size_t computer_value) : computer_(computer_value) {
    computer_ = CounterVariables::SetRandom(computer_);
    while (!CheckingInputValue(ChangingType(computer_))) {
      computer_ = CounterVariables::SetRandom(computer_);
    }
  }

  size_t GetComputer() const {
    return computer_;
  }

  std::string FindNumber(const class Data& data,
                         const class Data& search_data);  // основной метод поиска совпавшихся цифр
  ~Data() = default;
};

#endif  // NUMBER_SEARCH_H