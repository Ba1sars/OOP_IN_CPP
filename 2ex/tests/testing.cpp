#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include <sstream>

#include "../triple_signal/triple_signal.hpp"
#include "../triple_vector/triple_vector.hpp"

std::string get_vector_signals(Triple_signal::Triple_vector vec)
{
    return vec.get_signals();
}

Triple_signal::Triple_vector modify_and_return(Triple_signal::Triple_vector &&vec)
{
    vec.set_signals("11");

    return vec;
}

TEST_CASE("Тестирование простого класса")
{
    SECTION("Конструктор")
    {
        SECTION("Дефолтный конструктор")
        {
            Triple_signal::Triple_signal signal;
            REQUIRE(signal.get_signal_char() == 'X');
        }
        SECTION("Инициализирующий конструктор")
        {
            Triple_signal::Triple_signal signal_1('0');
            REQUIRE(signal_1.get_signal_char() == '0');

            Triple_signal::Triple_signal signal_2('1');
            REQUIRE(signal_2.get_signal_char() == '1');

            Triple_signal::Triple_signal signal_3('X');
            REQUIRE(signal_3.get_signal_char() == 'X');

            REQUIRE_THROWS_AS(Triple_signal::Triple_signal('4'), std::invalid_argument);
        }
        SECTION("Присваивающий конструктор")
        {
            Triple_signal::Triple_signal signal_1('1');
            Triple_signal::Triple_signal signal_2(signal_1);
            REQUIRE(signal_2.get_signal() == 1);
        }
    }
    SECTION("Методы")
    {
        Triple_signal::Triple_signal signal;

        signal.set_signal('1');
        REQUIRE(signal.get_signal() == 1);
        REQUIRE(signal.get_signal() == 1);

        REQUIRE_THROWS_AS(signal.set_signal('p'), std::invalid_argument);

        Triple_signal::Triple_signal signal_2('0');

        REQUIRE(signal_2.get_signal() == 0);
        signal_2.set_signal('x');
        REQUIRE(signal_2.get_signal() == 2);
    }
    SECTION("Операторы")
    {

        Triple_signal::Triple_signal signal_1('0');
        Triple_signal::Triple_signal signal_2('1');

        REQUIRE((signal_1 || signal_2).get_signal() == 1);
        REQUIRE((signal_1 && signal_2).get_signal() == 0);

        signal_2.set_signal('0');
        REQUIRE(signal_1 == signal_2);

        signal_2.set_signal('X');
        REQUIRE((!signal_1).get_signal_char() == '1');
        REQUIRE((!signal_2).get_signal_char() == 'X');
    }
    SECTION("Ввод")
    {
        Triple_signal::Triple_signal signal;
        std::stringstream input("1");
        input >> signal;

        REQUIRE(signal.get_signal_char() == '1');
        input.clear();

        input.str("12");
        REQUIRE_THROWS_AS(input >> signal, std::invalid_argument);
        input.clear();

        input.str("UNKNOWN");
        input >> signal;
        REQUIRE(signal.get_signal_char() == 'X');
        input.clear();

        input.str("0");
        signal.input(input);
        REQUIRE(signal.get_signal_char() == '0');
        input.clear();

        input.str("3");
        REQUIRE_THROWS_AS(signal.input(input), std::invalid_argument);
    }
    SECTION("Вывод")
    {
        Triple_signal::Triple_signal signal('0');
        std::stringstream out;
        out << signal;

        REQUIRE(out.str() == "0");
        out.str("");
        out.clear();

        signal.set_signal('1');
        signal.print(out);

        REQUIRE(out.str() == "1");
    }
}

TEST_CASE("Тестирование сложного класса")
{
    SECTION("Конструктор")
    {
        SECTION("Дефолтный конструктор")
        {
            Triple_signal::Triple_vector vector;

            REQUIRE(vector.get_signals() == "");
        }
        SECTION("Инициализирующий конструктор")
        {
            Triple_signal::Triple_vector vector(3);

            REQUIRE(vector.get_signals() == "XXX");

            REQUIRE_THROWS_AS(Triple_signal::Triple_vector(-123), std::invalid_argument);

            Triple_signal::Triple_vector vector_2("110X1");

            REQUIRE(vector_2.get_signals() == "110X1");

            REQUIRE_THROWS_AS(Triple_signal::Triple_vector("-1"), std::invalid_argument);
        }
        SECTION("Копирующий конструктор")
        {
            Triple_signal::Triple_vector vector_1("110X1");

            std::string old = get_vector_signals(vector_1);

            vector_1.set_signals("001");

            REQUIRE(old == "110X1");
            REQUIRE(vector_1.get_signals() == "001");
        }

        SECTION("Перемещающий конструктор")
        {
            Triple_signal::Triple_vector temp("110");

            Triple_signal::Triple_vector vector = modify_and_return(std::move(temp));

            REQUIRE(vector.get_signals() == "11");
            REQUIRE(temp.get_signals() == "");
        }
    }

    SECTION("Методы")
    {
        Triple_signal::Triple_vector vector;

        vector.set_signals("1101XX1");

        REQUIRE(vector.get_signals() == "1101XX1");

        REQUIRE_THROWS(vector.set_signals("21413432x"));

        Triple_signal::Triple_vector vector_2("1101x");

        int size = 0;
        Triple_signal::Triple_signal *array = vector_2.get_signals(size);

        REQUIRE(size == 5);
        REQUIRE((array[0]).get_signal_char() == '1');

        REQUIRE(!(vector.is_known()));

        vector.set_signals("11011");
        REQUIRE(vector.is_known());
    }

    SECTION("Операторы")
    {
        SECTION("Логические операторы")
        {
            Triple_signal::Triple_vector vector_1("1101100xX");
            Triple_signal::Triple_vector vector_2("0101Xx1");

            REQUIRE((vector_1 & vector_2) == (Triple_signal::Triple_vector("0101X00")));

            REQUIRE((vector_1 | vector_2) == (Triple_signal::Triple_vector("11011X1")));

            REQUIRE((~vector_1) == (Triple_signal::Triple_vector("0010011XX")));
        }
        SECTION("Копирующий оператор")
        {
            Triple_signal::Triple_vector vector_1;
            Triple_signal::Triple_vector vector_2("0101Xx1");

            vector_1 = vector_2;
            vector_2.set_signals("110");

            REQUIRE(vector_1.get_signals() == "0101XX1");
        }
        SECTION("Перемещающий оператор")
        {
            Triple_signal::Triple_vector vector_1;
            Triple_signal::Triple_vector vector_2("0101X00");

            vector_1 = std::move(vector_2);

            REQUIRE(vector_1.get_signals() == "0101X00");
            REQUIRE(vector_2.get_signals() == "");
        }
        SECTION("Другие операторы")
        {
            Triple_signal::Triple_vector vector_1("1101100xX");
            Triple_signal::Triple_vector vector_2("0101Xx1");
            Triple_signal::Triple_signal signal_1('x');
            Triple_signal::Triple_signal signal_2('1');
            REQUIRE(vector_1 != vector_2);

            Triple_signal::Triple_vector vector_3 = vector_1 + vector_2 + vector_2 + signal_1 + signal_2;

            REQUIRE(vector_3.get_signals() == "1101100XX0101XX10101XX1X1");

            REQUIRE(vector_1[2] == Triple_signal::Triple_signal('0'));
            REQUIRE_THROWS_AS(vector_1[10], std::out_of_range);

            vector_1[0] = Triple_signal::Triple_signal('0');
            REQUIRE(vector_1[0] == Triple_signal::Triple_signal('0'));

            Triple_signal::Triple_vector sub_vec = vector_2[{1, 6}];
            REQUIRE(sub_vec.get_signals() == "101XX1");

            REQUIRE_THROWS_AS(vector_2[std::make_pair(5, 1)], std::invalid_argument);
            REQUIRE_THROWS_AS(vector_2[std::make_pair(1, 10)], std::out_of_range);

            REQUIRE((!vector_1) == 0);

            vector_1.set_signals("11101");

            REQUIRE(!(vector_1) == 1);
        }
    }

    SECTION("Ввод")
    {
        SECTION("Оператор >>")
        {
            std::stringstream input("1101");

            Triple_signal::Triple_vector vector;

            input >> vector;

            REQUIRE(vector.get_signals() == "1101");
            input.clear();

            input.str("432");

            REQUIRE_THROWS_AS(input >> vector, std::invalid_argument);

            input.clear();
        }
        SECTION("Метод input()")
        {
            std::stringstream input("1101");

            Triple_signal::Triple_vector vector;

            vector.input(input);

            REQUIRE(vector.get_signals() == "1101");
            input.clear();

            input.str("432");

            REQUIRE_THROWS_AS(vector.input(input), std::invalid_argument);

            input.clear();
        }
    }

    SECTION("Вывод")
    {
        SECTION("Оператор <<")
        {
            std::stringstream out;

            Triple_signal::Triple_vector vector("1101x");

            out << vector;

            REQUIRE(out.str() == "1101X");
        }
        SECTION("Метод print()")
        {
            std::stringstream out;

            Triple_signal::Triple_vector vector("1101x");

            vector.print(out);

            REQUIRE(out.str() == "1101X");
        }
    }
}