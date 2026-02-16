#ifndef GENERATOR
#define GENERATOR

#include <algorithm>
#include <cctype>
#include <format>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

enum json_type { STRING, LONG, DOUBLE, BOOL, VOID, INVALID };

json_type determine_json_type(std::string &json_string);
std::string from_json_type_to_c_type(const json_type &determined);
std::string json_struct_builder(std::string &struct_name,
                                std::string &json_string);
const char *json_struct_builder(const char *struct_name,
                                const char *json_string);

#endif //! GENERATOR
