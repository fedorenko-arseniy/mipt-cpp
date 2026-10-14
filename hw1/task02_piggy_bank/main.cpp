#include <cstring>
#include <iomanip>
#include <iostream>

using Operation = void (*)(long long&);

struct Command {
  const char* name;
  Operation operation;
};

void Deposit(long long& balance) {
  long long amount = 0;
  if (!(std::cin >> amount) || amount <= 0) {
    std::cout << "error\n";
    return;
  }

  balance += amount;
}

void Withdraw(long long& balance) {
  long long amount = 0;
  if (!(std::cin >> amount) || amount <= 0 || amount > balance) {
    std::cout << "error\n";
    return;
  }

  balance -= amount;
}

void PrintBalance(long long& balance) {
  std::cout << balance << '\n';
}

void Reset(long long& balance) {
  balance = 0;
}

void IncreaseByPercent(long long& balance) {
  long long percent = 0;
  if (!(std::cin >> percent) || percent <= 0) {
    std::cout << "error\n";
    return;
  }

  balance += balance * percent / 100;
}

int main() {
  long long balance = 0;
  char command[32]{};

  const Command commands[] = {
      {"deposit", Deposit},
      {"withdraw", Withdraw},
      {"balance", PrintBalance},
      {"reset", Reset},
      {"percent", IncreaseByPercent},
  };

  constexpr int kCommandCount = 5;

  while (std::cin >> std::setw(sizeof(command)) >> command) {
    if (std::strcmp(command, "stop") == 0) {
      break;
    }

    Operation operation = nullptr;
    for (int i = 0; i < kCommandCount; ++i) {
      if (std::strcmp(command, commands[i].name) == 0) {
        operation = commands[i].operation;
        break;
      }
    }

    if (operation != nullptr) {
      operation(balance);
    } else {
      std::cout << "error\n";
    }

    std::cin.clear();
    char ch;
    while (std::cin.get(ch)) {
      if (ch == '\n') {
        break;
      }
    }
  }

  return 0;
}
