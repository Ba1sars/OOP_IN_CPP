#include <iostream>
#include <stdexcept>
#include <string>

#include "dialog.hpp"
#include "generator.hpp"

int main() {
  int mode;
  std::string struct_name, json_string;
  try {
    std::cout << "Выберите в каком варианте выполнить задание 0 (в "
                 "std::string), 1 (в const char *): "
              << std::endl;
    mode = int_input();
    std::cout << "Введите название структуры: " << std::endl;
    struct_name = str_input();
    std::cout << "Введите строку в формате json: " << std::endl;
    json_string = str_input();
    if (mode) {
      const char *res =
          json_struct_builder(struct_name.c_str(), json_string.c_str());
      std::cout << res << std::endl;
      delete[] res;
    } else {
      std::string res = json_struct_builder(struct_name, json_string);
      std::cout << res << std::endl;
    }
  } catch (std::runtime_error &e) {
    std::cout << "Program ended because of " << e.what() << std::endl;
    return -1;
  }
  return 0;
}
