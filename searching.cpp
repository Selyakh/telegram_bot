// Copyright (c) 2024 Irina Selyakh
//
// Данное программное обеспечение распространяется на условиях лицензии MIT.
// Подробности смотрите в файле LICENSE

#include "searching.h"
#include <algorithm>

static bool IsLetter(char symbol) {
  return ('a' <= symbol && symbol <= 'z') || ('A' <= symbol && symbol <= 'Z');
}

bool CheckingInputValue(const std::string value) {  // определение ф-ции, проверяющая верный ввод числа
  if (value[0] == '0' || value.length() != 3) {
    return false;
  }
  for (size_t i = 0; i < value.length(); ++i) {
    if (IsLetter(value[i])) {
      return false;
    }
    for (size_t j = i + 1; j < value.length(); ++j) {
      if (value[i] == value[j]) {
        return false;
      }
    }
  }
  return true;
}

std::string ChangingType(const size_t value) {
  return std::to_string(value);
}

std::string Data::FindNumber(const class Data& data, const class Data& search_data) {
  std::vector<size_t> parsing_user;
  std::vector<size_t> parsing_computer;
  CounterVariables::bulls = 0;
  CounterVariables::cows = 0;
  if (CounterVariables::priority) {  // 1й блок - если игрок предлагает число
    parsing_user = TokenizeNumber(search_data.user_);                 // парсим значение игрока
    parsing_computer = TokenizeNumber(ChangingType(data.computer_));  // парсим значение бота
    CounterVariables::priority = false;
  } else {                                      // 2й блок - если бот предлагает число
    parsing_user = TokenizeNumber(data.user_);  // парсим значение игрока
    parsing_computer = TokenizeNumber(ChangingType(search_data.computer_));  // парсим значение бота
    CounterVariables::priority = true;
  }

  // поиск совпавших цифр
  for (size_t i = 0; i < parsing_user.size(); ++i) {
    for (size_t k = 0; k < parsing_computer.size(); ++k) {
      if (parsing_user[i] == parsing_computer[k] && i == k) {
        CounterVariables::bulls++;
      } else if (parsing_user[i] == parsing_computer[k] && i != k) {
        CounterVariables::cows++;
      }
    }
  }

  // если бот ничего не отгадал, то больше он эти цифры не будет предлагать
  // хоть немного увеличить шансы бота на выигрыш
  if (CounterVariables::priority) {  // проверка, что сейчас ход бота
    if (CounterVariables::bulls == 0 && CounterVariables::cows == 0) {
      for (size_t elem : parsing_computer) {
        CounterVariables::bot_memory.erase(
            std::remove(CounterVariables::bot_memory.begin(), CounterVariables::bot_memory.end(), elem),
            CounterVariables::bot_memory.end());
      }
    }

    // если бот отгадал 3 цифры, но в разном порядке
    if (CounterVariables::cows == 3 || (CounterVariables::cows == 1 && CounterVariables::bulls == 2) ||
        (CounterVariables::cows == 2 && CounterVariables::bulls == 1)) {
      CounterVariables::bot_memory.clear();  // убираем лишние цифры с памяти нашего бота
      CounterVariables::bot_memory = {parsing_computer};  // обновляем точными
    }
  }

  std::string result =
      "быки " + ChangingType(CounterVariables::bulls) + ", коровы " + ChangingType(CounterVariables::cows);
  return result;
}