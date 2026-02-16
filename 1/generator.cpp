
#include "generator.hpp"
#include <cstring>

json_type determine_json_type(std::string &json_string) {
  if (json_string.empty())
    return json_type::INVALID;
  if (json_string.size() >= 2 && json_string[0] == '"' &&
      json_string[json_string.size() - 1] == '"')
    return json_type::STRING;
  if (json_string == "false" || json_string == "true")
    return json_type::BOOL;
  if (json_string == "null")
    return json_type::VOID;
  int sign_ind = json_string.find_first_of("+-");
  bool has_sign = (sign_ind == 0 || sign_ind == std::string::npos);
  bool has_dot = (std::count(json_string.begin(), json_string.end(), '.') == 1);
  bool has_digit =
      (std::any_of(json_string.begin(), json_string.end(),
                   [](char digit) { return std::isdigit(digit); }));
  if (!has_sign || !has_digit)
    return json_type::INVALID;
  if (has_dot)
    return json_type::DOUBLE;
  return json_type::LONG;
}

std::string from_json_type_to_c_type(const json_type &determined) {
  switch (determined) {
  case json_type::LONG:
    return "long";
  case json_type::DOUBLE:
    return "double";
  case json_type::BOOL:
    return "bool";
  case json_type::STRING:
    return "char *";
  case json_type::VOID:
    return "void *";
  default:
    throw std::runtime_error("Unsuported type");
  }
}

std::string json_struct_builder(std::string &struct_name,
                                std::string &json_string) {
    std::string start_of_sentence = std::format("struct {}", struct_name);
    start_of_sentence += " {";
    if (json_string.front() != '{' || json_string.back() != '}') {
        throw std::runtime_error("Invalid JSON: must start with { and end with }");
    }
    std::string inner = json_string.substr(1, json_string.size() - 2);
    inner.erase(0, inner.find_first_not_of(" \t"));
    inner.erase(inner.find_last_not_of(" \t") + 1);
    std::istringstream temp_json_string(inner);
    std::string token;
    while (std::getline(temp_json_string, token, ',')) {
        token.erase(0, token.find_first_not_of(" \t"));
        token.erase(token.find_last_not_of(" \t") + 1);
        size_t colon_pos = token.find(':');
        if (colon_pos == std::string::npos) {
            throw std::runtime_error("Invalid JSON: missing colon in key-value pair");
        }
        std::string key_part = token.substr(0, colon_pos);
        std::string value_part = token.substr(colon_pos + 1);
        key_part.erase(0, key_part.find_first_not_of(" \t"));
        key_part.erase(key_part.find_last_not_of(" \t") + 1);
        value_part.erase(0, value_part.find_first_not_of(" \t"));
        value_part.erase(value_part.find_last_not_of(" \t") + 1);
        if (key_part.empty() || key_part.front() != '"' || key_part.back() != '"') {
            throw std::runtime_error("Invalid JSON key: must be quoted");
        }
        std::string key = key_part.substr(1, key_part.size() - 2);
        json_type type = determine_json_type(value_part);
        if (type == json_type::INVALID) {
            throw std::runtime_error("Invalid JSON value type");
        }
        std::string c_type = from_json_type_to_c_type(type);
        start_of_sentence += std::format("\n\t{} {};", c_type, key);
    }
    start_of_sentence += "\n};";
    return start_of_sentence;
}

const char *json_struct_builder(const char *struct_name,
                                const char *json_string) {
  std::string temp_struct_name(struct_name);
  std::string temp_json_string(json_string);
  std::string temp_res =
      json_struct_builder(temp_struct_name, temp_json_string);
  char *res = new char[temp_res.size() + 1];
  std::strcpy(res, temp_res.c_str());
  return res;
}
