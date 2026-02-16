#include "input.hpp"
#include <string>
#include <functional>
#include <limits>
#include <stdexcept>
#include <climits>

void process_error(std::istream &in)
{
    if (in.bad())
    {
        throw std::runtime_error("Corrupted input stream");
    }
    if (in.eof())
    {
        throw std::runtime_error("EOF");
    }
    if (in.fail())
    {
        in.clear();
        in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

int is_number(std::string str)
{
    for (size_t i = 0; i < str.size(); i++)
    {
        if (i == 0 && str[i] == '-')
        {
            continue;
        }
        if (str[i] < '0' || str[i] > '9')
        {
            return 0;
        }
    }

    return 1;
}

int readint(std::istream &in, int minumum)
{
    int res;
    std::string str;
    std::getline(in, str);

    while (1)
    {
        process_error(in);
        try
        {
            res = std::stol(str);
            if (!is_number(str))
            {
                std::cout << "Введено не число. Попробуйте снова: ";
                std::getline(in, str);
            }
            else if (res <= minumum)
            {
                std::cout << "Введите число больше " << minumum << ". Попробуйте снова: ";
                std::getline(in, str);
            }
            else
            {
                break;
            }
        }
        catch (std::exception &e)
        {
            std::cout << "Введено не число. Попробуйте снова: ";
            std::getline(in, str);
        }
    }
    return res;
}

std::string readline(std::istream &in)
{
    std::string res;
    std::getline(in, res);
    while (!in.good())
    {
        process_error(in);
        std::getline(in, res);
    }
    return res;
}
