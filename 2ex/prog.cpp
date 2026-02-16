#include <iostream>
#include <string>
#include <format>

#include "input/input.hpp"
#include "triple_signal/triple_signal.hpp"

void menu(Triple_signal::Triple_signal &s_1, Triple_signal::Triple_signal &s_2)
{
    std::cout << "\n";

    std::cout
        << "Сигнал 1: " << s_1 << std::endl
        << "Сигнал 2: " << s_2 << std::endl;

    std::cout << "0: Выйти" << std::endl
              << "1: Назначить значение сигналу 1" << std::endl
              << "2: Назначить значение сигналу 2" << std::endl
              << "3: Ввести значение сигналу 1" << std::endl
              << "4: Ввести значение сигналу 2" << std::endl
              << "5: Логическое ИЛИ" << std::endl
              << "6: Логическое И" << std::endl
              << "7: Логическое НЕ для сигнала 1" << std::endl
              << "8: Логическое НЕ для сигнала 2" << std::endl
              << "9: Сравнение сигналов" << std::endl
              << "Выберите действие: ";
}

void solve()
{

    Triple_signal::Triple_signal signal_1;
    Triple_signal::Triple_signal signal_2;

    while (1)
    {
        menu(signal_1, signal_2);

        int operation;
        try
        {
            operation = readint(std::cin);
        }
        catch (std::exception &e)
        {
            return;
        }
        std::cout << "\n";

        std::string value;

        try
        {
            switch (operation)
            {
            case 0:
                return;
            case 1:
                std::cout << "Введите значение 1/0/X: ";

                value = readline(std::cin);
                try
                {
                    signal_1.set_signal(value[0]);
                }
                catch (std::exception &e)
                {
                    std::cout << "Введено неверное значение" << std::endl;
                }
                break;
            case 2:
                std::cout << "Введите значение 1/0/X: ";

                value = readline(std::cin);
                try
                {
                    signal_2.set_signal(value[0]);
                }
                catch (std::exception &e)
                {
                    std::cout << "Введено неверное значение" << std::endl;
                }
                break;
            case 3:
                std::cout << "Введите значение 1/0/X: ";

                try
                {
                    std::cin >> signal_1;
                }
                catch (std::invalid_argument &e)
                {
                    std::cout << "Введено неверное значение" << std::endl;
                }
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
            case 4:
                std::cout << "Введите значение 1/0/X: ";

                try
                {
                    std::cin >> signal_2;
                }
                catch (std::invalid_argument &e)
                {
                    std::cout << "Введено неверное значение" << std::endl;
                }
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
            case 5:

                std::cout
                    << signal_1 << " || " << signal_2 << " = " << (signal_1 || signal_2) << std::endl;
                break;
            case 6:

                std::cout
                    << signal_1 << " && " << signal_2 << " = " << (signal_1 && signal_2) << std::endl;
                break;
            case 7:
                std::cout << "Сигнал 1 = !" << signal_1 << " = ";

                std::cout << (!signal_1) << std::endl;
                break;
            case 8:
                std::cout << "Сигнал 2 = !" << signal_2 << " = ";

                std::cout << (!signal_2) << std::endl;
                break;
            case 9:
                std::cout << "Сигнал 1 " << (signal_1 == signal_2 ? "==" : "!=") << " Сигнал 2" << std::endl;
                break;
            default:
                std::cout << "Такого действия нет" << std::endl;
                break;
            }
        }
        catch (std::exception &e)
        {
        }
    }
}

int main()
{

    solve();

    std::cout << "Конец программы" << std::endl;

    return 0;
}
