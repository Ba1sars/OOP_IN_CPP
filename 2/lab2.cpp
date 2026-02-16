#include "simple_class.hpp"
#include "complex_class.hpp"
#include <iostream>
#include <stdexcept>

int main(){
  try {
    wagon a("люкс");
    std::cout << a << std::endl;
    int value = 23;
    a + value;
    std::cout << a << std::endl;
    int free = 6;
    wagon b("сидячий");
    std::cout << a.percentage_of_occupancy() << std::endl;
    a >> b;
    std::cout << a << '\n' << b << std::endl;
    b - free;
    std::cout << b << std::endl;

    std::cout << "\nПоезд" << std::endl;
    std::cout << "Пустой конструктор" << std::endl;
    train tr;
    std::cout << tr << std::endl;
    wagon a1("люкс");
    tr += a1;
    std::cout << tr << std::endl;

  }
  catch (std::invalid_argument &e){
    std::cout << e.what() << std::endl;
  }
}
