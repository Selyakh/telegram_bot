// Copyright (c) 2024 Irina Selyakh
//
// Данное программное обеспечение распространяется на условиях лицензии MIT.
// Подробности смотрите в файле LICENSE

#include <tgbot/tgbot.h>
#include "searching.h"

bool flag = true;  // для вынужденной остановки процесса / выхода из него

class Bot {
  TgBot::Bot bot_;
  Data data_;
  Data find_data_;
  size_t bot_value_;

  void ActionStartCommand(TgBot::Message::Ptr msg_ptr) {
    size_t chat_id = msg_ptr->chat->id;
    TgBot::InlineKeyboardMarkup::Ptr keyboard(new TgBot::InlineKeyboardMarkup);
    std::vector<TgBot::InlineKeyboardButton::Ptr> row0;

    TgBot::InlineKeyboardButton::Ptr button1(new TgBot::InlineKeyboardButton);
    button1->text = "Новая игра";
    button1->callbackData = "new_game";
    row0.push_back(button1);

    TgBot::InlineKeyboardButton::Ptr button2(new TgBot::InlineKeyboardButton);
    button2->text = "Правила игры";
    button2->callbackData = "rules";
    row0.push_back(button2);
    
    keyboard->inlineKeyboard.push_back(row0);
    bot_.getApi().sendAnimation(chat_id, TgBot::InputFile::fromFile("image.gif", "image/gif"));
    bot_.getApi().sendMessage(msg_ptr->chat->id, "Я рад приветствовать тебя в игре Быки и коровы!", nullptr, nullptr,
                              keyboard);
  }

  void ActionPlayCommand(TgBot::Message::Ptr msg_ptr) {
    TgBot::InlineKeyboardMarkup::Ptr keyboard(new TgBot::InlineKeyboardMarkup);
    std::vector<TgBot::InlineKeyboardButton::Ptr> row0;

    TgBot::InlineKeyboardButton::Ptr button1(new TgBot::InlineKeyboardButton);
    button1->text = "Приступить к игре";
    button1->callbackData = "new_game";
    row0.push_back(button1);

    keyboard->inlineKeyboard.push_back(row0);

    bot_.getApi().sendMessage(msg_ptr->chat->id, "Погнали!", nullptr, nullptr, keyboard);
  }

  void ActionOtherCommand(TgBot::Message::Ptr msg_ptr) {
    TgBot::InlineKeyboardMarkup::Ptr keyboard(new TgBot::InlineKeyboardMarkup);
    std::vector<TgBot::InlineKeyboardButton::Ptr> row0;

    TgBot::InlineKeyboardButton::Ptr button1(new TgBot::InlineKeyboardButton);
    button1->text = "Продолжить";
    button1->callbackData = "next";
    row0.push_back(button1);

    TgBot::InlineKeyboardButton::Ptr button2(new TgBot::InlineKeyboardButton);
    button2->text = "Новая игра";
    button2->callbackData = "new_game";
    row0.push_back(button2);

    TgBot::InlineKeyboardButton::Ptr button3(new TgBot::InlineKeyboardButton);
    button3->text = "Выйти из игры";
    button3->callbackData = "exit";
    row0.push_back(button3);

    keyboard->inlineKeyboard.push_back(row0);
    bot_.getApi().sendMessage(msg_ptr->chat->id, "Выберите команду", nullptr, nullptr, keyboard);
  }

  void ActionCallbackQuery(TgBot::CallbackQuery::Ptr query) {
    auto chat_id = query->message->chat->id;
    auto msg = query->message;
    if (query->data == "rules") {
      bot_.getApi().sendMessage(
          chat_id,
          "Правила игры Быки и коровы просты. Игрок задумывает трехзначное число, так чтобы все цифры числа "
          "были разные (ноль в игре используется, но на первом месте стоять не может). Цель бота — отгодать это число. "
          "Каждый ход, отгадывающий называет число, тоже "
          "трехзначное и с разными цифрами. Если цифра из названного числа есть в отгадываемом числе, то эта ситуация "
          "называется корова. Если цифра из названного числа есть в отгадываемом числе и стоит в том же месте, то эта "
          "ситуация называется бык. ");
      bot_.getApi().sendMessage(
          chat_id,
          "Выигрывает тот, кто первым угадает число противника. Максимальное количество попыток отгадать число - 8");
      ActionPlayCommand(msg);
    } else if (query->data == "new_game") {
      bot_.getApi().sendMessage(chat_id, "Введите свое трехзначное число:");
      CounterVariables::counter = 0;  // обновляем счетчик попыток
      CounterVariables::bot_memory = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};  // инициализируем память бота
      CounterVariables::priority = true;  // обновляем очередность хода
      CounterVariables::one_initialization_bot =
          true;  // обновляем флаг для возможности первой инициализации значения бота
      flag = true;
    } else if (query->data == "next") {
      bot_.getApi().sendMessage(chat_id, "Отлично, продолжаем!");
      bot_.getApi().sendMessage(chat_id, "Введите число для следующей попытки:");
      CounterVariables::counter = 0;
    } else if (query->data == "exit") {
      bot_.getApi().sendMessage(chat_id, "Спасибо за игру!");
      ActionStartCommand(msg);
    }
  }

  void DataLaunch(int64_t chat_id, TgBot::Message::Ptr msg_ptr) {  // запуск игры
    auto arg = msg_ptr->text;
    if (CheckingInputValue(arg) && flag) {  // разовый вход при запуске новой игры
      data_ = Data{arg};                    // инициализация значений игрока и бота
      bot_value_ = data_.GetComputer();     // сохраняем значение бота
      bot_.getApi().sendMessage(chat_id, "Введите число для попытки отгадать число бота:");
      flag = false;
    } else if (CheckingInputValue(arg) && CounterVariables::counter < kMaximumAttempts) {  // игрок отгадывает
      find_data_ = Data{arg};  // инициализация отгадываемого значения игрока
      auto result = data_.FindNumber(data_, find_data_);  // поиск совпадений
      if (CounterVariables::bulls == 3) {
        bot_.getApi().sendMessage(chat_id, "Поздравляю, ты победил!");
        bot_.getApi().sendAnimation(chat_id, TgBot::InputFile::fromFile("tenor.gif", "image/gif"));
        CounterVariables::bot_memory.clear();
        ActionStartCommand(msg_ptr);
        return;
      }
      bot_.getApi().sendMessage(chat_id, "Твой результат: " + result);
      CounterVariables::counter++;

      // бот отгадывет
      size_t bot_guess = 0;
      find_data_ = Data{bot_guess};  // инициализация отгадываемого значения бота
      bot_.getApi().sendMessage(chat_id, "Бот предполагает число: " + ChangingType(find_data_.GetComputer()));
      std::string bot_result = data_.FindNumber(data_, find_data_);  // поиск совпадений
      if (CounterVariables::bulls == 3) {
        bot_.getApi().sendMessage(chat_id, "Жаль, но ты проиграл.");
        bot_.getApi().sendMessage(chat_id, "Значение бота было " + ChangingType(bot_value_));
        CounterVariables::bot_memory.clear();
        ActionStartCommand(msg_ptr);
        return;
      }
      bot_.getApi().sendMessage(chat_id, "Результат бота: " + bot_result);
      if (CounterVariables::counter >= kMaximumAttempts) {
        bot_.getApi().sendMessage(chat_id, "Попытки отгадать число бота закончились.");
        ActionOtherCommand(msg_ptr);
        return;
      }
      bot_.getApi().sendMessage(chat_id, "Введите число для следующей попытки:");
    } else {
      bot_.getApi().sendMessage(chat_id, "Неверный ввод!");
    }
  }

 public:
  explicit Bot(std::string api_token) : bot_(api_token) {
    srand(time(nullptr));
    bot_.getEvents().onCommand("start", [this](TgBot::Message::Ptr msg_ptr) { ActionStartCommand(msg_ptr); });
    bot_.getEvents().onCallbackQuery([this](TgBot::CallbackQuery::Ptr query) { ActionCallbackQuery(query); });
    bot_.getEvents().onAnyMessage([this](TgBot::Message::Ptr msg_ptr) {
      int64_t chat_id = msg_ptr->chat->id;
      auto arg = msg_ptr->text;
      if (!CounterVariables::bot_memory.empty()) {
        DataLaunch(chat_id, msg_ptr);
      }
    });
  }

  void Run() {
    auto& api = bot_.getApi();
    api.deleteWebhook();
    auto long_poll = TgBot::TgLongPoll(bot_);
    while (true) {
      try {
        long_poll.start();
      } catch (const TgBot::TgException& err) {
        std::cerr << "TgBot error: " << err.what() << '\n';
        continue;
      }
    }
  }
};

int main() {
  Bot bot("PLACE YOUR TOKEN HERE");
  bot.Run();

  return 0;
}