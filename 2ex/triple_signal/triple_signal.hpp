/**
 * @file triple_signal.hpp
 * @author Alexey Parfenov
 * @brief Заголовочный файл, содержащий определение простого класса Triple_signal
 * @version 0.1
 * @date 2025-09-19
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef TRIPLE_SIGNAL_H
#define TRIPLE_SIGNAL_H

namespace Triple_signal
{
    /**
     * @brief Константы обозначающие значения сигнала
     * ZERO - 0
     * ONE - 1
     * UNKNOWN - неопределенность (также обозначается, как X или x)
     */
    enum Signal
    {
        ZERO,   ///< Сигнал 0
        ONE,    ///< Сигнал 1
        UNKNOWN ///< Сигнал неопределенность (X)
    };

    /**
     * @brief Класс представляющий собой троичный сигнал
     *
     */
    class Triple_signal
    {
        Signal value = UNKNOWN;

    public:
        /**
         * @brief Конструктор по умолчанию
         *
         * Создает сигнал со значением UNKNOWN (неопределенность)
         *
         */
        Triple_signal() = default;

        /**
         * @brief Инициализирующий конструктор
         *
         * @param value Значение сигнала в виде константы Triple_signal::Signal
         *
         */
        Triple_signal(Signal value);

        /**
         * @brief Инициализирующий конструктор
         *
         * @param value Значение сигнала в виде символа (0/1/x/X)
         *
         * @throw std::invalid_argument Если передать символ кроме 0/1/x/X
         */
        Triple_signal(char value);

        /**
         * @brief Присваивающий конструктор
         *
         * @param signal Троичный сигнал, значение которого присвоится к новому сигналу
         *
         * @throw std::invalid_argument Если передать что-то кроме 0/1/x/X
         */
        Triple_signal(const Triple_signal &signal);

        /**
         * @brief Геттер значения сигнала
         *
         * @return Возвращает значение сигнала в виде константы Triple_signal::Signal
         */
        Signal get_signal() const;

        /**
         * @brief Геттер значения сигнала
         * Возвращает значение сигнала в виде символа
         * @return char (0/1/X)
         */
        char get_signal_char() const;

        /**
         * @brief Сеттер значения сигнала
         *
         * @param value Значения сигнала в виде константы Triple_signal::Signal
         */
        void set_signal(Signal value);

        /**
         * @brief Сеттер значения сигнала
         *
         * @param value Значение сигнала: 0/1/x/X
         *
         * @throw std::invalid_argument Если передать символ кроме 0/1/x/X
         */
        void set_signal(char value);

        /**
         * @brief Функция вывода сигнала
         *
         * Выводится 0/1/X
         *
         * @param out Поток вывода
         */
        void print(std::ostream &out);

        /**
         * @brief Функция ввода сигнала
         * Принимает 0/1/UNKNOWN/x/X
         * @param in Поток ввода
         *
         * @throw std::invalid_argument Если ввести что-то кроме 0/1/UNKNOWN/x/X
         */
        void input(std::istream &in);

        /**
         * @brief Оператор логического ИЛИ между двумя сигналами
         *
         * @param signal_2 Второй сигнал
         *
         * @return Возвращает сигнал
         */
        Triple_signal operator||(const Triple_signal &signal_2) const;

        /**
         * @brief Оператор логического И между двумя сигналами
         *
         * @param signal_2 Второй сигнал
         *
         * @return Возвращает сигнал
         */
        Triple_signal operator&&(const Triple_signal &signal_2) const;

        /**
         * @brief Оператор сравнения сигналов
         *
         * @param signal_2 Второй сигнал
         *
         * @return true При равенстве
         * @return false При неравенстве
         */
        bool operator==(const Triple_signal &signal_2) const;

        /**
         * @brief Префиксный оператор логического НЕ
         *
         * @return Возвращает сигнал
         */
        Triple_signal operator!() const;

        /**
         * @brief Оператор потока вывода
         *
         * Выводится 0/1/X
         *
         * @param out Поток вывода
         * @param signal Выводимый сигнал
         *
         * @return std::ostream& Ссылка на выходной поток
         */
        friend std::ostream &operator<<(std::ostream &out, const Triple_signal &signal);

        /**
         * @brief Оператор потока ввода
         *
         * Принимает 0/1/UNKNOWN/x/X
         *
         * @param in Поток ввода
         * @param signal Вводимый сигнал
         *
         * @return std::istream& Ссылка на входной поток
         *
         * @throw std::invalid_argument Если ввести что-то кроме 0/1/UNKNOWN/x/X
         */
        friend std::istream &operator>>(std::istream &in, Triple_signal &signal);
    };

}

#endif
