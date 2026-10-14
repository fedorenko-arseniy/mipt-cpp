#include <cstring>
#include <iomanip>
#include <iostream>

bool IsInside(int x, int y) {
  return x >= 0 && x < 10 && y >= 0 && y < 5;
}

void Clear(char board[5][10]) {
  for (int y = 0; y < 5; ++y) {
    for (int x = 0; x < 10; ++x) {
      board[y][x] = ' ';
    }
  }
}

bool Put(char board[5][10], int x, int y, char symbol) {
  if (!IsInside(x, y)) {
    return false;
  }

  board[y][x] = symbol;
  return true;
}

bool Text(char board[5][10], int x, int y, const char* word) {
  if (!IsInside(x, y)) {
    return false;
  }

  const int length = std::strlen(word);
  if (length > 10 - x) {
    return false;
  }

  for (int i = 0; i < length; ++i) {
    board[y][x + i] = word[i];
  }
  return true;
}

void Print(const char board[5][10]) {
  for (int y = 0; y < 5; ++y) {
    for (int x = 0; x < 10; ++x) {
      std::cout << board[y][x];
    }
    std::cout << '\n';
  }
}

int main() {
  char board[5][10]{};
  char command[32]{};

  Clear(board);

  while (std::cin >> std::setw(sizeof(command)) >> command) {
    if (std::strcmp(command, "quit") == 0) {
      break;
    }

    if (std::strcmp(command, "clear") == 0) {
      Clear(board);
    } else if (std::strcmp(command, "print") == 0) {
      Print(board);
    } else if (std::strcmp(command, "put") == 0) {
      int x = 0;
      int y = 0;
      char symbol = '\0';
      if (!(std::cin >> x >> y >> symbol) || !Put(board, x, y, symbol)) {
        std::cout << "error\n";
      }
    } else if (std::strcmp(command, "text") == 0) {
      int x = 0;
      int y = 0;
      char word[12]{};
      if (!(std::cin >> x >> y >> std::setw(sizeof(word)) >> word) ||
          !Text(board, x, y, word)) {
        std::cout << "error\n";
      }
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
