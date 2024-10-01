// Copyright (c) 2024 Irina Selyakh
//
// Данное программное обеспечение распространяется на условиях лицензии MIT.
// Подробности смотрите в файле LICENSE

#include "parse.h"

static bool IsDigit(char symbol) {
  return '0' <= symbol && symbol <= '9';
}

static size_t ParseNumber(std::string input, size_t& pos) {
  size_t value = 0;
  while (IsDigit(input[pos])) {
    value = value * 10 + (input[pos] - '0');
    ++pos;
    break;
  }
  return value;
}

std::vector<size_t> TokenizeNumber(std::string input) {
  std::vector<size_t> tokens;
  size_t pos = 0;
  while (pos < input.size()) {
    const auto symbol = input[pos];
    if (IsDigit(symbol)) {
      tokens.emplace_back(ParseNumber(input, pos));
    }
  }
  return tokens;
}