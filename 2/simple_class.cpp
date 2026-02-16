#include "simple_class.hpp"
#include <ostream>

std::istream &operator>>(std::istream &in, wagon &wagon) {
  std::string line;
  if (!std::getline(in, line))
    return in;
  std::istringstream iss(line);
  std::string type_str, capas_str, occup_str;
  if (!std::getline(iss, type_str, '/') || !std::getline(iss, capas_str, '/') ||
      !std::getline(iss, occup_str, '/')) {
    in.setstate(std::ios::failbit);
    return in;
  }
  int cap = 0, occ = 0;
  try {
    cap = std::stoi(capas_str);
    occ = std::stoi(occup_str);
  } catch (const std::exception &) {
    in.setstate(std::ios::failbit);
    return in;
  }
  if (!wagon::is_valid_type(type_str) || cap < 0 || occ < 0 || occ > cap ||
      (type_str == "ресторан" && cap != 0)) {
    in.setstate(std::ios::failbit);
    return in;
  }
  wagon.type = std::move(type_str);
  wagon.max_capacity = cap;
  wagon.occupied = occ;
  return in;
}

std::ostream &operator<<(std::ostream &out, wagon &wagon) {
  out << "Тип вагона: " << wagon.type << " "
      << "Занятость вагона: " << wagon.occupied << '/' << wagon.max_capacity;
  return out;
}
