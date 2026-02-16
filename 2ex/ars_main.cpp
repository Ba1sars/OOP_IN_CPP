#include <iostream>
#include <string>
#include <iomanip>
#include "input/input.hpp"
#include "triple_signal/triple_signal.hpp"
#include "triple_vector/triple_vector.hpp"

void print_menu()
{
    std::cout << "\n╔════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║          РАБОТА С ТРОИЧНЫМИ СИГНАЛАМИ И ВЕКТОРАМИ             ║\n";
    std::cout << "╠════════════════════════════════════════════════════════════════╣\n";
    std::cout << "║ 1.  Создать и работать с одиночным сигналом                   ║\n";
    std::cout << "║ 2.  Создать и работать с вектором сигналов                    ║\n";
    std::cout << "║ 3.  Логические операции с сигналами (AND, OR, NOT)            ║\n";
    std::cout << "║ 4.  Побитовые операции с векторами (&, |, ~)                  ║\n";
    std::cout << "║ 5.  Конкатенация векторов                                     ║\n";
    std::cout << "║ 6.  Доступ к элементам вектора по индексу                     ║\n";
    std::cout << "║ 7.  Срез вектора (подвектор)                                  ║\n";
    std::cout << "║ 8.  Проверка определенности вектора                           ║\n";
    std::cout << "║ 9.  Сравнение векторов                                        ║\n";
    std::cout << "║ 10. Демонстрация всех возможностей                            ║\n";
    std::cout << "║ 0.  Выход                                                     ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════════╝\n";
    std::cout << "Выберите пункт меню: ";
}

void work_with_signal()
{
    std::cout << "\n--- РАБОТА С ОДИНОЧНЫМ СИГНАЛОМ ---\n";
    std::cout << "Введите сигнал (0, 1, X, x или UNKNOWN): ";
    
    try
    {
        Triple_signal::Triple_signal signal;
        signal.input(std::cin);
        
        std::cout << "Создан сигнал: " << signal << "\n";
        std::cout << "Значение как char: " << signal.get_signal_char() << "\n";
        std::cout << "Инверсия (!signal): " << !signal << "\n";
        
        std::cout << "\nВведите второй сигнал для операций: ";
        Triple_signal::Triple_signal signal2;
        signal2.input(std::cin);
        
        std::cout << "\nРезультаты операций:\n";
        std::cout << "  " << signal << " AND " << signal2 << " = " << (signal && signal2) << "\n";
        std::cout << "  " << signal << " OR  " << signal2 << " = " << (signal || signal2) << "\n";
        std::cout << "  " << signal << " == " << signal2 << " ? " << (signal == signal2 ? "Да" : "Нет") << "\n";
    }
    catch (const std::exception &e)
    {
        std::cout << "Ошибка: " << e.what() << "\n";
    }
}

void work_with_vector()
{
    std::cout << "\n--- РАБОТА С ВЕКТОРОМ СИГНАЛОВ ---\n";
    std::cout << "Выберите способ создания вектора:\n";
    std::cout << "1. Из строки (например: 01X10)\n";
    std::cout << "2. Заданное количество неопределенных сигналов\n";
    std::cout << "Ваш выбор: ";
    
    int choice = readint(std::cin, 0);
    
    try
    {
        Triple_signal::Triple_vector vec;
        
        if (choice == 1)
        {
            std::cout << "Введите строку сигналов (0/1/X/x): ";
            std::string signals = readline(std::cin);
            vec.set_signals(signals);
        }
        else if (choice == 2)
        {
            std::cout << "Введите количество неопределенных сигналов: ";
            int count = readint(std::cin, 0);
            vec = Triple_signal::Triple_vector(count);
        }
        else
        {
            std::cout << "Неверный выбор!\n";
            return;
        }
        
        std::cout << "Создан вектор: " << vec << "\n";
        std::cout << "Вектор определен? " << (vec.is_known() ? "Да" : "Нет") << "\n";
        
        int size;
        Triple_signal::Triple_signal *signals = vec.get_signals(size);
        std::cout << "Размер вектора: " << size << "\n";
        std::cout << "Элементы: ";
        for (int i = 0; i < size; i++)
        {
            std::cout << signals[i] << " ";
        }
        std::cout << "\n";
    }
    catch (const std::exception &e)
    {
        std::cout << "Ошибка: " << e.what() << "\n";
    }
}

void logical_operations()
{
    std::cout << "\n--- ЛОГИЧЕСКИЕ ОПЕРАЦИИ С СИГНАЛАМИ ---\n";
    std::cout << "\nТаблица истинности для AND:\n";
    std::cout << "  0 AND 0 = " << (Triple_signal::Triple_signal('0') && Triple_signal::Triple_signal('0')) << "\n";
    std::cout << "  0 AND 1 = " << (Triple_signal::Triple_signal('0') && Triple_signal::Triple_signal('1')) << "\n";
    std::cout << "  0 AND X = " << (Triple_signal::Triple_signal('0') && Triple_signal::Triple_signal('X')) << "\n";
    std::cout << "  1 AND 0 = " << (Triple_signal::Triple_signal('1') && Triple_signal::Triple_signal('0')) << "\n";
    std::cout << "  1 AND 1 = " << (Triple_signal::Triple_signal('1') && Triple_signal::Triple_signal('1')) << "\n";
    std::cout << "  1 AND X = " << (Triple_signal::Triple_signal('1') && Triple_signal::Triple_signal('X')) << "\n";
    std::cout << "  X AND X = " << (Triple_signal::Triple_signal('X') && Triple_signal::Triple_signal('X')) << "\n";
    
    std::cout << "\nТаблица истинности для OR:\n";
    std::cout << "  0 OR 0 = " << (Triple_signal::Triple_signal('0') || Triple_signal::Triple_signal('0')) << "\n";
    std::cout << "  0 OR 1 = " << (Triple_signal::Triple_signal('0') || Triple_signal::Triple_signal('1')) << "\n";
    std::cout << "  0 OR X = " << (Triple_signal::Triple_signal('0') || Triple_signal::Triple_signal('X')) << "\n";
    std::cout << "  1 OR 0 = " << (Triple_signal::Triple_signal('1') || Triple_signal::Triple_signal('0')) << "\n";
    std::cout << "  1 OR 1 = " << (Triple_signal::Triple_signal('1') || Triple_signal::Triple_signal('1')) << "\n";
    std::cout << "  1 OR X = " << (Triple_signal::Triple_signal('1') || Triple_signal::Triple_signal('X')) << "\n";
    std::cout << "  X OR X = " << (Triple_signal::Triple_signal('X') || Triple_signal::Triple_signal('X')) << "\n";
    
    std::cout << "\nТаблица инверсии (NOT):\n";
    std::cout << "  NOT 0 = " << !Triple_signal::Triple_signal('0') << "\n";
    std::cout << "  NOT 1 = " << !Triple_signal::Triple_signal('1') << "\n";
    std::cout << "  NOT X = " << !Triple_signal::Triple_signal('X') << "\n";
}

void bitwise_operations()
{
    std::cout << "\n--- ПОБИТОВЫЕ ОПЕРАЦИИ С ВЕКТОРАМИ ---\n";
    std::cout << "Введите первый вектор (например: 01X10): ";
    std::string vec1_str = readline(std::cin);
    
    std::cout << "Введите второй вектор: ";
    std::string vec2_str = readline(std::cin);
    
    try
    {
        Triple_signal::Triple_vector vec1(vec1_str);
        Triple_signal::Triple_vector vec2(vec2_str);
        
        std::cout << "\nВектор 1: " << vec1 << "\n";
        std::cout << "Вектор 2: " << vec2 << "\n";
        std::cout << "\nРезультаты операций:\n";
        std::cout << "  vec1 & vec2  = " << (vec1 & vec2) << "  (побитовое И)\n";
        std::cout << "  vec1 | vec2  = " << (vec1 | vec2) << "  (побитовое ИЛИ)\n";
        std::cout << "  ~vec1        = " << ~vec1 << "  (побитовое НЕ)\n";
        std::cout << "  ~vec2        = " << ~vec2 << "  (побитовое НЕ)\n";
    }
    catch (const std::exception &e)
    {
        std::cout << "Ошибка: " << e.what() << "\n";
    }
}

void concatenation()
{
    std::cout << "\n--- КОНКАТЕНАЦИЯ ВЕКТОРОВ ---\n";
    std::cout << "Введите первый вектор: ";
    std::string vec1_str = readline(std::cin);
    
    std::cout << "Введите второй вектор: ";
    std::string vec2_str = readline(std::cin);
    
    try
    {
        Triple_signal::Triple_vector vec1(vec1_str);
        Triple_signal::Triple_vector vec2(vec2_str);
        Triple_signal::Triple_vector result = vec1 + vec2;
        
        std::cout << "\n" << vec1 << " + " << vec2 << " = " << result << "\n";
        
        std::cout << "\nТакже можно добавить одиночный сигнал:\n";
        std::cout << "Введите сигнал для добавления: ";
        Triple_signal::Triple_signal sig;
        sig.input(std::cin);
        
        Triple_signal::Triple_vector result2 = vec1 + sig;
        std::cout << vec1 << " + " << sig << " = " << result2 << "\n";
    }
    catch (const std::exception &e)
    {
        std::cout << "Ошибка: " << e.what() << "\n";
    }
}

void index_access()
{
    std::cout << "\n--- ДОСТУП К ЭЛЕМЕНТАМ ПО ИНДЕКСУ ---\n";
    std::cout << "Введите вектор: ";
    std::string vec_str = readline(std::cin);
    
    try
    {
        Triple_signal::Triple_vector vec(vec_str);
        std::cout << "Вектор: " << vec << "\n";
        
        int size;
        vec.get_signals(size);
        std::cout << "Размер: " << size << "\n";
        
        std::cout << "Введите индекс для доступа (0-" << size - 1 << "): ";
        int index = readint(std::cin, -1);
        
        Triple_signal::Triple_signal &sig = vec[index];
        std::cout << "Элемент с индексом " << index << ": " << sig << "\n";
        
        std::cout << "Хотите изменить этот элемент? (1-Да, 0-Нет): ";
        int change = readint(std::cin, -1);
        
        if (change == 1)
        {
            std::cout << "Введите новое значение (0/1/X): ";
            Triple_signal::Triple_signal new_sig;
            new_sig.input(std::cin);
            vec[index] = new_sig;
            std::cout << "Вектор после изменения: " << vec << "\n";
        }
    }
    catch (const std::exception &e)
    {
        std::cout << "Ошибка: " << e.what() << "\n";
    }
}

void slice_vector()
{
    std::cout << "\n--- СРЕЗ ВЕКТОРА ---\n";
    std::cout << "Введите вектор: ";
    std::string vec_str = readline(std::cin);
    
    try
    {
        Triple_signal::Triple_vector vec(vec_str);
        std::cout << "Вектор: " << vec << "\n";
        
        int size;
        vec.get_signals(size);
        
        std::cout << "Введите начальный индекс: ";
        size_t start = readint(std::cin, -1);
        
        std::cout << "Введите конечный индекс: ";
        size_t end = readint(std::cin, -1);
        
        Triple_signal::Triple_vector slice = vec[std::make_pair(start, end)];
        std::cout << "Подвектор [" << start << ":" << end << "]: " << slice << "\n";
    }
    catch (const std::exception &e)
    {
        std::cout << "Ошибка: " << e.what() << "\n";
    }
}

void check_known()
{
    std::cout << "\n--- ПРОВЕРКА ОПРЕДЕЛЕННОСТИ ВЕКТОРА ---\n";
    std::cout << "Введите вектор: ";
    std::string vec_str = readline(std::cin);
    
    try
    {
        Triple_signal::Triple_vector vec(vec_str);
        std::cout << "Вектор: " << vec << "\n";
        
        bool known1 = vec.is_known();
        bool known2 = !vec;
        
        std::cout << "Метод is_known(): " << (known1 ? "Определен" : "Не определен") << "\n";
        std::cout << "Оператор !vec:    " << (known2 ? "Определен" : "Не определен") << "\n";
        
        if (!known1)
        {
            std::cout << "\nВектор содержит неопределенные сигналы (X)\n";
        }
        else
        {
            std::cout << "\nВсе сигналы определены (только 0 и 1)\n";
        }
    }
    catch (const std::exception &e)
    {
        std::cout << "Ошибка: " << e.what() << "\n";
    }
}

void compare_vectors()
{
    std::cout << "\n--- СРАВНЕНИЕ ВЕКТОРОВ ---\n";
    std::cout << "Введите первый вектор: ";
    std::string vec1_str = readline(std::cin);
    
    std::cout << "Введите второй вектор: ";
    std::string vec2_str = readline(std::cin);
    
    try
    {
        Triple_signal::Triple_vector vec1(vec1_str);
        Triple_signal::Triple_vector vec2(vec2_str);
        
        std::cout << "\nВектор 1: " << vec1 << "\n";
        std::cout << "Вектор 2: " << vec2 << "\n";
        
        bool equal = (vec1 == vec2);
        bool not_equal = (vec1 != vec2);
        
        std::cout << "\nvec1 == vec2: " << (equal ? "Да" : "Нет") << "\n";
        std::cout << "vec1 != vec2: " << (not_equal ? "Да" : "Нет") << "\n";
    }
    catch (const std::exception &e)
    {
        std::cout << "Ошибка: " << e.what() << "\n";
    }
}

void full_demo()
{
    std::cout << "\n╔════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║            ПОЛНАЯ ДЕМОНСТРАЦИЯ ВОЗМОЖНОСТЕЙ                    ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════════╝\n";
    
    std::cout << "\n1. Создание одиночных сигналов:\n";
    Triple_signal::Triple_signal s0('0'), s1('1'), sX('X');
    std::cout << "   s0 = " << s0 << ", s1 = " << s1 << ", sX = " << sX << "\n";
    
    std::cout << "\n2. Логические операции:\n";
    std::cout << "   s0 AND s1 = " << (s0 && s1) << "\n";
    std::cout << "   s0 OR s1  = " << (s0 || s1) << "\n";
    std::cout << "   NOT s1    = " << !s1 << "\n";
    
    std::cout << "\n3. Создание векторов:\n";
    Triple_signal::Triple_vector v1("10X01");
    Triple_signal::Triple_vector v2("11001");
    Triple_signal::Triple_vector v3(3);
    std::cout << "   v1 = " << v1 << "\n";
    std::cout << "   v2 = " << v2 << "\n";
    std::cout << "   v3 = " << v3 << " (3 неопределенных)\n";
    
    std::cout << "\n4. Побитовые операции с векторами:\n";
    std::cout << "   v1 & v2 = " << (v1 & v2) << "\n";
    std::cout << "   v1 | v2 = " << (v1 | v2) << "\n";
    std::cout << "   ~v1     = " << ~v1 << "\n";
    
    std::cout << "\n5. Конкатенация:\n";
    Triple_signal::Triple_vector v4 = v1 + v2;
    std::cout << "   v1 + v2 = " << v4 << "\n";
    Triple_signal::Triple_vector v5 = v1 + s1;
    std::cout << "   v1 + s1 = " << v5 << "\n";
    
    std::cout << "\n6. Доступ к элементам:\n";
    std::cout << "   v1[0] = " << v1[0] << "\n";
    std::cout << "   v1[2] = " << v1[2] << "\n";
    
    std::cout << "\n7. Срез вектора:\n";
    Triple_signal::Triple_vector slice = v1[std::make_pair(1, 3)];
    std::cout << "   v1[1:3] = " << slice << "\n";
    
    std::cout << "\n8. Проверка определенности:\n";
    std::cout << "   v1 определен? " << (v1.is_known() ? "Нет" : "Нет") << " (содержит X)\n";
    std::cout << "   v2 определен? " << (v2.is_known() ? "Да" : "Нет") << "\n";
    
    std::cout << "\n9. Сравнение векторов:\n";
    std::cout << "   v1 == v2? " << (v1 == v2 ? "Да" : "Нет") << "\n";
    std::cout << "   v1 != v2? " << (v1 != v2 ? "Да" : "Нет") << "\n";
    
    std::cout << "\n10. Копирование и перемещение:\n";
    Triple_signal::Triple_vector v6 = v1;
    std::cout << "   v6 (копия v1) = " << v6 << "\n";
    Triple_signal::Triple_vector v7 = std::move(v2);
    std::cout << "   v7 (перемещен из v2) = " << v7 << "\n";
    
    std::cout << "\n╔════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                   ДЕМОНСТРАЦИЯ ЗАВЕРШЕНА                       ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════════╝\n";
}

int main()
{
    std::cout << "╔════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║     ИНТЕРАКТИВНАЯ ПРОГРАММА ДЛЯ ТРОИЧНЫХ СИГНАЛОВ             ║\n";
    std::cout << "║                                                                ║\n";
    std::cout << "║  Троичная логика: 0 (ноль), 1 (единица), X (неопределенность) ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════════╝\n";
    
    int choice;
    do
    {
        print_menu();
        choice = readint(std::cin, -1);
        
        switch (choice)
        {
        case 1:
            work_with_signal();
            break;
        case 2:
            work_with_vector();
            break;
        case 3:
            logical_operations();
            break;
        case 4:
            bitwise_operations();
            break;
        case 5:
            concatenation();
            break;
        case 6:
            index_access();
            break;
        case 7:
            slice_vector();
            break;
        case 8:
            check_known();
            break;
        case 9:
            compare_vectors();
            break;
        case 10:
            full_demo();
            break;
        case 0:
            std::cout << "\nДо свидания!\n";
            break;
        default:
            std::cout << "\nНеверный выбор! Попробуйте снова.\n";
            break;
        }
        
        if (choice != 0 && choice >= 1 && choice <= 10)
        {
            std::cout << "\nНажмите Enter для продолжения...";
            std::cin.get();
        }
        
    } while (choice != 0);
    
    return 0;
}
