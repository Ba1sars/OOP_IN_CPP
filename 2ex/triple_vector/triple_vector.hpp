/**
 * @file triple_vector.hpp
 * @author Alexey Parfenov
 * @brief Заголовочный файл, содержащий определение сложного класса Triple_vector
 * @version 0.1
 * @date 2025-09-22
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef TRIPLE_VECTOR_H
#define TRIPLE_VECTOR_H

#include <iostream>
#include <algorithm>
#include <string>

#include "../triple_signal/triple_signal.hpp"

namespace Triple_signal
{
    /**
     * @brief Класс представляющий вектор троичных сигналов
     *
     */
    class Triple_vector
    {
        size_t size = 0;
        Triple_signal *array = nullptr;

    public:
        /**
         * @brief Конструктор по умолчанию
         *
         * Создает пустой вектор
         *
         */
        Triple_vector() = default;

        /**
         * @brief Инициализирующий конструктор
         *
         * Создает вектор с заданным количеством неопределенных сигналов
         *
         * @param cnt_unknow Количество неопределенных сигналов
         *
         * @throw std::invalid_argument число сигналов меньше 0
         */
        Triple_vector(int cnt_unknow);

        /**
         * @brief Инициализирующий конструктор
         *
         * Создает вектор сигналов из строки
         *
         * @param signals Срока состоящая из 0/1/x/X
         *
         * @throw std::invalid_argument в строке есть символы кроме 0/1/x/X
         */
        Triple_vector(std::string signals);

        /**
         * @brief Копирующий конструктор
         *
         * Создает вектор скопировав другой
         *
         * @param vector Вектор сигналов
         */
        Triple_vector(const Triple_vector &vector) noexcept;

        /**
         * @brief Перемещающий конструктор
         *
         * @param vector
         */
        Triple_vector(Triple_vector &&vector) noexcept;

        /**
         * @brief Деструктор
         *
         */
        ~Triple_vector()
        {
            delete[] array;
        }

        /**
         * @brief Сеттер
         *
         * @param signals строка сигналов состоящая из 0/1/x/X
         *
         * @throw std::invalid_argument в строке есть что-то кроме 0/1/x/X
         */
        void set_signals(std::string signals);

        /**
         * @brief Геттер
         *
         * @return std::string Строка сигналов состоящая из 0/1/x/X
         */
        std::string get_signals() const;

        /**
         * @brief Геттер
         *
         * @param size размер полученного массива
         * @return Triple_signal* Массив сигналов
         */
        Triple_signal *get_signals(int &size) const;

        /**
         * @brief
         * Проверка на определенность вектора
         * @return true Все сигналы определенны
         * @return false Есть хотя бы один неопределенный сигнал
         */
        bool is_known() const;

        /**
         * @brief Ввод вектора
         *
         * @param in Поток ввода
         * @throw std::invalid_argument при вводе что-то кроме 0/1/x/X
         */
        void input(std::istream &in);

        /**
         * @brief Вывод вектора
         *
         * @param out Поток вывода
         */
        void print(std::ostream &out);

        /**
         * @brief Перегрузка копирующего оператора
         *
         * @param vector
         * @return Triple_vector&
         */
        Triple_vector &operator=(const Triple_vector &vector) noexcept;

        /**
         * @brief Перегрузка перемещающего оператора
         *
         * @param vector
         * @return Triple_vector&
         */
        Triple_vector &operator=(Triple_vector &&vector) noexcept;

        /**
         * @brief Перегрузка оператора сравнения на равенство
         *
         * @param vector_2
         * @return bool
         */
        bool operator==(const Triple_vector &vector_2) const;

        /**
         * @brief Перегрузка оператора сравнения на неравенство
         *
         * @param vector_2
         * @return bool
         */
        bool operator!=(const Triple_vector &vector_2) const;
        /**
         * @brief Оператор поразрядного логического ИЛИ
         *
         * @param vector_2
         * @return Triple_vector
         */
        Triple_vector operator|(const Triple_vector &vector_2) const;

        /**
         * @brief Оператор поразрядного логического И
         *
         * @param vector_2
         * @return Triple_vector
         */
        Triple_vector operator&(const Triple_vector &vector_2) const;

        /**
         * @brief Оператор поразрядного логического НЕ
         *
         * @return Triple_vector
         */
        Triple_vector operator~() const;

        /**
         * @brief Сложение векторов
         *
         * @param vector_2
         * @return Triple_vector
         */
        Triple_vector operator+(const Triple_vector &vector_2) const;

        /**
         * @brief Сложение сигнала с вектором
         *
         * @param signal
         * @return Triple_vector
         */
        Triple_vector operator+(const Triple_signal &signal) const;
        /**
         * @brief Оператор потока ввода
         *
         * Принимает 0/1/x/X
         *
         * @param in Поток ввода
         * @param vector Вводимый вектор
         *
         * @return std::istream& Ссылка на входной поток
         *
         * @throw std::invalid_argument Если ввести что-то кроме 0/1/x/X
         */
        friend std::istream &operator>>(std::istream &in, Triple_vector &vector);

        /**
         * @brief Оператор потока вывода
         *
         * Выводится 0/1/X
         *
         * @param out Поток вывода
         * @param vector Выводимый вектор
         *
         * @return std::ostream& Ссылка на выходной поток
         */
        friend std::ostream &operator<<(std::ostream &out, const Triple_vector &vector);

        /**
         * @brief
         *
         * @param index
         * @return Triple_signal&
         * @throw std::out_of_range индекс больше размера вектора
         */
        Triple_signal &operator[](size_t index);

        /**
         * @brief Оператор среза
         *
         * @param indices левый и правый индексы подвектора
         * @return Triple_vector Подвектор
         * @throw std::out_of_range индекс больше размера вектора
         * @throw std::invalid_argument левый индекс больше правого
         */
        Triple_vector operator[](const std::pair<size_t, size_t> &indices) const;

        /**
         * @brief Префиксный оператор проверки на определенность вектора
         *
         * @return true Все сигналы определенны
         * @return false Есть хотя бы один неопределенный сигнал
         */
        bool operator!() const;
    };

}

#endif
