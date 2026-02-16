#include "complex_class.hpp"
#include <istream>
#include <limits>
#include <stdexcept>
#include <vector>

std::istream &operator>>(std::istream &in, train &train) {
  int wagons_number;
  int max_wagons_amount;
  std::cout << "Введите максимальное количество вагонов в поезде";
  if (!(in >> max_wagons_amount)) {
    throw std::invalid_argument(
        "Некорректный ввод максимального числа вагонов");
  }
  if (max_wagons_amount <= 0) {
    throw std::invalid_argument("Поезд должен содержать хотя бы один вагон !");
  }
  in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  std::cout << "Введите количество вагонов в поезде: ";
  if (!(in >> wagons_number)) {
    throw std::invalid_argument("Некорректный ввод числа вагонов!");
  }
  if (wagons_number <= 0) {
    throw std::invalid_argument("Поезд должен содержать хотя бы один вагон!");
  }
  if(wagons_number >= max_wagons_amount){
    throw std::invalid_argument("Поезд не такой длинный!");
  }
  in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  delete[] train.wagon_chain;
  train.max_train_lenght = max_wagons_amount;
  train.wagon_amount = wagons_number;
  train.wagon_chain = new wagon[max_wagons_amount];
  std::cout << "Введите данные для каждого вагона:" << std::endl;
  for (int i = 0; i < wagons_number; i++) {
    std::cout << "Вагон #" << (i + 1) << ": ";
    std::string line;
    if (!std::getline(in, line)) {
      throw std::invalid_argument("Ошибка при вводе данных для вагона!");
    }
    std::istringstream iss(line);
    iss >> train.wagon_chain[i];
    if (!iss) {
      throw std::invalid_argument("Некорректный формат данных: " + line);
    }
  }
  return in;
}

std::ostream &operator<<(std::ostream &out, const train &train) {
  std::vector<std::string> topRow;
  std::vector<std::string> midRow;
  std::vector<std::string> botRow;
  for (int i = 0; i < train.wagon_amount; ++i) {
    const wagon &w = train.wagon_chain[i];
    std::string type = w.get_wagon_type();
    char fill;
    if (type == "сидячий")
      fill = '-';
    else if (type == "эконом")
      fill = '=';
    else if (type == "люкс")
      fill = '~';
    else if (type == "ресторан")
      fill = '*';
    else
      fill = '#';
    std::ostringstream content;
    content << w.get_wagon_occupied() << "/" << w.get_wagon_capacity();
    std::string text = content.str();
    int width = std::max((int)text.size() + 2, 8);
    std::string top(width, fill);
    std::string bottom(width - 2, fill);
    int padding = width - (int)text.size() - 2;
    int leftPad = padding / 2;
    int rightPad = padding - leftPad;
    std::ostringstream mid;
    mid << "|" << std::string(leftPad, ' ') << text
        << std::string(rightPad, ' ') << "|";
    topRow.push_back(top);
    midRow.push_back(mid.str());
    botRow.push_back("O" + bottom + "O");
  }
  for (size_t i = 0; i < topRow.size(); i++) {
    out << topRow[i];
    if (i != topRow.size() - 1)
      out << "   ";
  }
  out << "\n";
  for (size_t i = 0; i < midRow.size(); i++) {
    out << midRow[i];
    if (i != midRow.size() - 1)
      out << "---";
  }
  out << "\n";
  for (size_t i = 0; i < botRow.size(); i++) {
    out << botRow[i];
    if (i != botRow.size() - 1)
      out << "   ";
  }
  return out;
}
