#include "simple_class.hpp"
#include "complex_class.hpp"
#include <catch2/catch_all.hpp>
#include <sstream>
#include <stdexcept>

TEST_CASE("Тест простого класса (вагон)", "[Wagon]") {
  SECTION("Пустой конструктор") {
    wagon a;
    REQUIRE(a.get_wagon_occupied() == 0);
    REQUIRE(a.get_wagon_type() == "сидячий");
    REQUIRE(a.get_wagon_capacity() == 100);
  }

  SECTION("Инициализирующий конструктор с ручной инициализацией каждого поля") {
    wagon a(20, 3, "люкс");
    REQUIRE(a.get_wagon_occupied() == 3);
    REQUIRE(a.get_wagon_capacity() == 20);
    REQUIRE(a.get_wagon_type() == "люкс");
  }

  SECTION("Инициализирующий конструктор с ручной инициализацией поля type и "
          "автоматической инициализацией остальных полей") {
    wagon a("сидячий");
    REQUIRE(a.get_wagon_occupied() == 0);
    REQUIRE(a.get_wagon_capacity() == 100);
    REQUIRE(a.get_wagon_type() == "сидячий");
  }

  SECTION("Ввод экземпляра простого класса из входного потока") {
    wagon a;
    std::stringstream in("люкс/40/20");
    in >> a;
    REQUIRE(a.get_wagon_occupied() == 20);
    REQUIRE(a.get_wagon_capacity() == 40);
    REQUIRE(a.get_wagon_type() == "люкс");
  }

  SECTION("Вывод экземпляра простого класса в выходной поток") {
    wagon a;
    std::stringstream out;
    out << a;
    REQUIRE(out.str() == "Тип вагона: сидячий Занятость вагона: 0/100");
  }

  SECTION("Перемещение пассажиров из одного вагона в другой в почти равной "
          "пропорции") {
    wagon a(30, 5, "сидячий");
    wagon b("люкс");
    a >> b;
    REQUIRE(b.get_wagon_occupied() > 0);
  }
}

TEST_CASE("Тест простого класса (вагон) на правильный вывод ошибок",
          "[Wagon_error]") {

  SECTION("Полностью ручная инициализация") {
    SECTION("Вместимость меньше, чем занятые места") {
      REQUIRE_THROWS_AS(wagon(1, 2, "сидячий"), std::invalid_argument);
    }

    SECTION("Отрицательное количество занятых мест") {
      REQUIRE_THROWS_AS(wagon(50, -2, "люкс"), std::invalid_argument);
    }

    SECTION("Отрицательное количество вместимости") {
      REQUIRE_THROWS_AS(wagon(-50, 8, "люкс"), std::invalid_argument);
    }

    SECTION("Несуществующий тип вагона") {
      REQUIRE_THROWS_AS(wagon(30, 2, "локомотив"), std::invalid_argument);
    }

    SECTION("Вагон-ресторан с ненулевой вместимостью") {
      REQUIRE_THROWS_AS(wagon(10, 3, "ресторан"), std::invalid_argument);
    }
  }

  SECTION("Ручная инициализация поля type") {
    SECTION("Инициализация несуществующим типом вагона") {
      REQUIRE_THROWS_AS(wagon("локомотив"), std::invalid_argument);
    }
  }

  SECTION("Проверка сеттеров класса") {
    wagon a;

    SECTION("Вместимость меньше, чем занятые места") {
      REQUIRE_THROWS_AS(a.set_wagon_occupied(110), std::invalid_argument);
    }

    SECTION("Отрицательное количество занятых мест") {
      REQUIRE_THROWS_AS(a.set_wagon_occupied(-12), std::invalid_argument);
    }

    SECTION("Отрицательное количество вместимости") {
      REQUIRE_THROWS_AS(a.set_wagon_capacity(-23), std::invalid_argument);
    }

    SECTION("Несуществующий тип вагона") {
      REQUIRE_THROWS_AS(a.set_wagon_type("локомотив"), std::invalid_argument);
    }

    SECTION("Ненулевая вместимость при выборе вагона ресторана") {
      REQUIRE_THROWS_AS(a.set_wagon_type("ресторан"), std::invalid_argument);
    }
  }

  SECTION("Проверка функции для пересаживания пассажиров") {
    wagon from(40, 0, "люкс");
    wagon to(20, 3, "люкс");
    wagon rest("ресторан");

    SECTION("Нельзя пересаживать пассажиров в/из вагон/-а ресторан") {
      REQUIRE_THROWS_AS(rest >> to, std::invalid_argument);
    }

    /* SECTION("Не получилось поровну распределить людей"){
       REQUIRE_THROWS_AS(from >> to, std::invalid_argument);
     } */
  }
}


TEST_CASE("Тест сложного класса (поезд)", "[Train]") {

    SECTION("Конструкторы") {
        SECTION("Пустой конструктор") {
            train t;
            REQUIRE(t[0].get_wagon_type() == "сидячий");
        }

        SECTION("Конструктор с массивом вагонов") {
            wagon arr[2] = {wagon(100, 50, "сидячий"), wagon(30, 10, "люкс")};
            train t(2, arr, 10);
            REQUIRE(t[0].get_wagon_type() == "сидячий");
            REQUIRE(t[1].get_wagon_type() == "люкс");
        }

        SECTION("Копирующий конструктор") {
            wagon arr[2] = {wagon("эконом"), wagon("люкс")};
            train t1(2, arr, 10);
            train t2(t1);
            REQUIRE(t1[0].get_wagon_type() == t2[0].get_wagon_type());
        }

        SECTION("Перемещающий конструктор") {
            wagon arr[1] = {wagon("эконом")};
            train t1(1, arr, 5);
            train t2(std::move(t1));
            REQUIRE(t2[0].get_wagon_type() == "эконом");
        }
    }

    SECTION("Операторы присваивания") {
        SECTION("Копирующее присваивание") {
            wagon arr[2] = {wagon("сидячий"), wagon("люкс")};
            train t1(2, arr, 10), t2;
            t2 = t1;
            REQUIRE(t2[1].get_wagon_type() == "люкс");
        }

        SECTION("Перемещающее присваивание") {
            wagon arr[1] = {wagon("эконом")};
            train t1(1, arr, 5), t2;
            t2 = std::move(t1);
            REQUIRE(t2[0].get_wagon_type() == "эконом");
        }
    }

    SECTION("Добавление и удаление вагонов") {
        train t;
        wagon w("люкс");
        t += w;
        REQUIRE(t[1].get_wagon_type() == "люкс");

        t - 1;
        REQUIRE_THROWS_AS(t[1].get_wagon_type(), std::invalid_argument);
    }

    SECTION("Посадка пассажиров (operator())") {
        wagon arr[2] = {wagon(100, 50, "сидячий"), wagon(70, 20, "эконом")};
        train t(2, arr, 10);

        t(10, "сидячий");
        REQUIRE(t[0].get_wagon_occupied() >= 50);

        REQUIRE_THROWS_AS(t(-5, "сидячий"), std::invalid_argument);
        REQUIRE_THROWS_AS(t(200, "сидячий"), std::invalid_argument);
        REQUIRE_THROWS_AS(t(5, "локомотив"), std::invalid_argument);
    }

    SECTION("Перегрузка ! (распределение пассажиров)") {
        wagon arr[2] = {wagon(100, 90, "сидячий"), wagon(100, 10, "сидячий")};
        train t(2, arr, 10);
        !t;
        int sum = t[0].get_wagon_occupied() + t[1].get_wagon_occupied();
        REQUIRE(sum == 100);
        REQUIRE(abs(t[0].get_wagon_occupied() - t[1].get_wagon_occupied()) <= 1);
    }

    SECTION("Перегрузка -- (удаление пустых вагонов)") {
        wagon arr[3] = {wagon(100, 0, "сидячий"), wagon(70, 10, "эконом"), wagon(30, 0, "люкс")};
        train t(3, arr, 10);
        --t;
        REQUIRE_THROWS_AS(t[2].get_wagon_type(), std::invalid_argument);
    }

    SECTION("Перегрузка ++ (вставка ресторана)") {
        wagon arr[2] = {wagon(100, 40, "сидячий"), wagon(70, 20, "эконом")};
        train t(2, arr, 10);
        t++;
        bool has_restaurant = false;
        for (int i = 0; i < 3; i++) {
            if (t[i].get_wagon_type() == "ресторан") has_restaurant = true;
        }
        REQUIRE(has_restaurant);
    }

    SECTION("Ввод/Вывод") {
        std::stringstream input;
        input << "2\nсидячий/100/50\nлюкс/30/10\n";
        train t;
        input >> t;
        REQUIRE(t[0].get_wagon_type() == "сидячий");
        REQUIRE(t[1].get_wagon_type() == "люкс");
    }
}
