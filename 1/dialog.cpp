
#include "dialog.hpp"

bool check_str_input(std::string &to_input) {
  std::getline(std::cin, to_input);
  if (std::cin.eof()) {
    throw std::runtime_error("EOF");
  } else if (std::cin.bad()) {
    throw std::runtime_error("Broke input");
  } else if (std::cin.fail()) {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return false;
  }
  return true;
}

bool check_int_input(int &to_input) {
  std::cin >> to_input;
  if (std::cin.eof()) {
    throw std::runtime_error("EOF");
  } else if (std::cin.bad()) {
    throw std::runtime_error("Broke input");
  } else if (std::cin.fail()) {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return false;
  }
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  return true;
}

int int_input() {{
}
  int res;
  while (!check_int_input(res) || res > 1 || res < 0)
    ;
  return res;
}

std::string str_input() {
  std::string res;
  while (!check_str_input(res))
    ;
  return res;
}
