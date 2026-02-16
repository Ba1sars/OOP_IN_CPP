#include "simple_class.hpp"
#include <algorithm>
#include <istream>
#include <stdexcept>

/**
 * @brief Класс, представляющий поезд как массив вагонов.
 *
 * Содержит динамический массив объектов wagon и количество вагонов.
 * Поддерживает копирование, перемещение, добавление и удаление вагонов,
 * перераспределение пассажиров и другие операции.
 */
class train {
private:
  int wagon_amount = 1; ///< Число вагонов в поезде
  wagon *wagon_chain;   ///< Указатель на динамический массив вагонов
  int max_train_lenght = 10; ///< Максимальное число вагонов в поезде

public:
  /**
   * @brief Конструктор по умолчанию.
   *
   * Создаёт поезд из 1 вагона по умолчанию.
   */
  train() { wagon_chain = new wagon[max_train_lenght]; }

  /**
   * @brief Конструктор с инициализацией массивом.
   *
   * @param wagons_number Количество вагонов.
   * @param wagon_arr Массив вагонов.
   *
   * @throws std::invalid_argument если количество <= 0.
   */
  train(int wagons_number, wagon *wagon_arr, int max_wagons_amount) {
    wagon_amount = wagons_number;
    max_train_lenght = max_wagons_amount;
    wagon_chain = new wagon[max_train_lenght];
    for (int i = 0; i < wagon_amount; i++) {
      wagon_chain[i] = wagon_arr[i];
    }
  }

  /**
   * @brief Копирующий конструктор.
   * @param from Поезд-источник.
   *
   * Создаёт копию массива вагонов.
   */
  train(const train &from) {
    wagon_amount = from.wagon_amount;
    max_train_lenght = from.max_train_lenght;
    wagon_chain = new wagon[max_train_lenght];
    for (int i = 0; i < wagon_amount; i++) {
      wagon_chain[i] = from.wagon_chain[i];
    }
  }

  /**
   * @brief Копирующее присваивание.
   * @param from Поезд-источник.
   * @returns Ссылку на this.
   */
  train &operator=(const train &from) {
    if (this != &from) {
      delete[] wagon_chain;
      wagon_amount = from.wagon_amount;
      max_train_lenght = from.max_train_lenght;
      wagon_chain = new wagon[max_train_lenght];
      for (int i = 0; i < wagon_amount; i++) {
        wagon_chain[i] = from.wagon_chain[i];
      }
    }
    return *this;
  }

  /**
   * @brief Перемещающий конструктор.
   * @param from Исходный поезд (будет "опустошен").
   */
  train(train &&from) noexcept {
    wagon_amount = from.wagon_amount;
    max_train_lenght = from.max_train_lenght;
    wagon_chain = from.wagon_chain;
    from.wagon_amount = 0;
    from.wagon_chain = nullptr;
  }

  /**
   * @brief Перемещающее присваивание.
   * @param from Исходный поезд (будет "опустошен").
   * @returns Ссылку на *this.
   */
  train &operator=(train &&from) noexcept {
    if (this != &from) {
      delete[] wagon_chain;
      wagon_amount = from.wagon_amount;
      max_train_lenght = from.max_train_lenght;
      wagon_chain = from.wagon_chain;
      from.wagon_amount = 0;
      from.wagon_chain = nullptr;
    }
    return *this;
  }

  /**
   * @brief Деструктор. Освобождает память.
   */
  ~train() { delete[] wagon_chain; }

  /**
   * @brief Получить вагон по индексу.
   * @param wagon_index Индекс (0..wagon_amount-1).
   * @returns Ссылку на вагон.
   * @throws std::invalid_argument если индекс вне диапазона.
   */
  wagon &operator[](int wagon_index) {
    if (wagon_index < 0 || wagon_index >= max_train_lenght)
      throw std::invalid_argument("Недопустимый номер вагона");
    return wagon_chain[wagon_index];
  }

  /**
   * @brief Добавить вагон в поезд.
   * @param to_add Вагон для добавления.
   */
  train& operator+=(wagon &to_add) {
    if(wagon_amount + 1 >= max_train_lenght){
      max_train_lenght += 5;
      wagon *new_wagon_chain = new wagon[max_train_lenght];
      for(int i = 0;i < wagon_amount;i++){
        new_wagon_chain[i] = wagon_chain[i];
      }
      delete [] wagon_chain;
      wagon_chain = new_wagon_chain;
    }
    wagon_chain[wagon_amount] = to_add;
    wagon_amount += 1;
    return *this;
  }

  /**
   * @brief Удалить вагон по индексу.
   * @param to_delete Индекс удаляемого вагона.
   * @throws std::invalid_argument если индекс неверен.
   */
  void operator-(int to_delete) {
    if (to_delete < 0 || to_delete >= wagon_amount)
      throw std::invalid_argument("В составе нет вагона с таким номером");
    wagon *new_wagon_chain = new wagon[max_train_lenght];
    int new_index_count = 0;
    for (int i = 0; i < wagon_amount; i++) {
      if (to_delete != i) {
        new_wagon_chain[new_index_count] = wagon_chain[i];
        ++new_index_count;
      }
    }
    delete[] wagon_chain;
    --wagon_amount;
    wagon_chain = new_wagon_chain;
  }

  /**
   * @brief Усадить пассажиров в наиболее свободный вагон указанного типа.
   *
   * @param to_put Число пассажиров.
   * @param wagon_type Тип вагона.
   *
   * @throws std::invalid_argument если тип не найден, число отрицательное или нет мест.
   */

  void operator() (int to_put, std::string wagon_type){
    if (wagon_type != "сидячий" && wagon_type != "люкс" &&
        wagon_type != "эконом" && wagon_type != "ресторан")
      throw std::invalid_argument("В составе нет вагона такого типа");
    if (to_put < 0)
      throw std::invalid_argument(
          "Нельзя усадить отрицательное число людей в вагон");
    int least_amount = wagon_chain[0].get_wagon_occupied();
    int found_index = 0;
    for (int i = 0; i < wagon_amount; i++) {
      if (wagon_chain[i].get_wagon_type() == wagon_type) {
        if (least_amount > wagon_chain[i].get_wagon_occupied()) {
          least_amount = wagon_chain[i].get_wagon_occupied();
        }
        found_index = i;
      }
    }
    if (to_put > wagon_chain[found_index].get_wagon_capacity() ||
        least_amount + to_put > wagon_chain[found_index].get_wagon_capacity())
      throw std::invalid_argument(
          "Нельзя усадить такое число пассажиров в вагон");
    wagon_chain[found_index].set_wagon_capacity(least_amount + to_put);
  }

   /**
   * @brief Перераспределить пассажиров по вагонам для выравнивания занятости.
   *
   * Алгоритм собирает по классам (сидячий, эконом, люкс) количество вагонов,
   * их вместимость и число пассажиров, затем пытается выровнять загрузку
   * между вагонами каждого класса. Лишние пассажиры "переливаются" вверх
   * (из сидячего в эконом, из эконома в люкс).
   *
   * @throws std::invalid_argument если часть людей невозможно пересадить
   *       (остались лишние при переполнении всех вагонов).
   */
  void operator!() {
    int overflow = 0;
    int sitted_occup = 0, sitted_cap = 0, sitted_amount = 0;
    int econom_occup = 0, econom_cap = 0, econom_amount = 0;
    int luxury_occup = 0, luxury_cap = 0, luxury_amount = 0;

    for (int i = 0; i < wagon_amount; i++) {
      sitted_amount += (wagon_chain[i].get_wagon_type() == "сидячий" ? 1 : 0);
      econom_amount += (wagon_chain[i].get_wagon_type() == "эконом" ? 1 : 0);
      luxury_amount += (wagon_chain[i].get_wagon_type() == "люкс" ? 1 : 0);

      sitted_cap += (wagon_chain[i].get_wagon_type() == "сидячий"
                         ? wagon_chain[i].get_wagon_capacity()
                         : 0);
      econom_cap += (wagon_chain[i].get_wagon_type() == "эконом"
                         ? wagon_chain[i].get_wagon_capacity()
                         : 0);
      luxury_cap += (wagon_chain[i].get_wagon_type() == "люкс"
                         ? wagon_chain[i].get_wagon_capacity()
                         : 0);

      sitted_occup += (wagon_chain[i].get_wagon_type() == "сидячий"
                           ? wagon_chain[i].get_wagon_occupied()
                           : 0);
      econom_occup += (wagon_chain[i].get_wagon_type() == "эконом"
                           ? wagon_chain[i].get_wagon_occupied()
                           : 0);
      luxury_occup += (wagon_chain[i].get_wagon_type() == "люкс"
                           ? wagon_chain[i].get_wagon_occupied()
                           : 0);
    }

    for (int i = 0; i < 3; i++) { // 0 - сидячий , 1 - эконом, 2 - люкс
      int base = 0, rem = 0, allocated = 0, total_occup = 0, total_cap = 0,
          total_count = 0;

      if (i == 0) {
        total_cap = sitted_cap;
        total_occup = sitted_occup + overflow;
        total_count = sitted_amount;
      } else if (i == 1) {
        total_cap = econom_cap;
        total_occup = econom_occup + overflow;
        total_count = econom_amount;
      } else if (i == 2) {
        total_cap = luxury_cap;
        total_occup = luxury_occup + overflow;
        total_count = luxury_amount;
      }

      allocated = std::min(total_occup, total_cap);
      overflow = total_occup - allocated;

      if (total_count > 0) {
        base = allocated / total_count;
        rem = allocated % total_count;
      }

      int distributed = 0;
      for (int j = 0; j < wagon_amount; j++) {
        if ((i == 0 && wagon_chain[j].get_wagon_type() == "сидячий") ||
            (i == 1 && wagon_chain[j].get_wagon_type() == "эконом") ||
            (i == 2 && wagon_chain[j].get_wagon_type() == "люкс")) {
          int assign = base + (distributed < rem ? 1 : 0);
          distributed++;

          int curr_occup = wagon_chain[j].get_wagon_occupied();
          if (assign > curr_occup) {
            wagon_chain[j] + (assign - curr_occup);
          } else if (assign < curr_occup) {
            wagon_chain[j] - (curr_occup - assign);
          }
        }
      }
    }
    if (overflow > 0)
      throw std::invalid_argument("Не удалось распределить людей");
  }

  /**
   * @brief Сжать состав: убрать пустые или ненужные вагоны.
   *
   * Алгоритм проверяет вагоны каждого класса, пересаживает пассажиров
   * в доступные места и освобождает ненужные пустые вагоны, удаляя их.
   */
  void operator--() {
    int sitted_occup = 0, sitted_amount = 0;
    int econom_occup = 0, econom_amount = 0;
    int luxury_occup = 0, luxury_amount = 0;

    for (int i = 0; i < wagon_amount; i++) {
      sitted_amount += (wagon_chain[i].get_wagon_type() == "сидячий" ? 1 : 0);
      econom_amount += (wagon_chain[i].get_wagon_type() == "эконом" ? 1 : 0);
      luxury_amount += (wagon_chain[i].get_wagon_type() == "люкс" ? 1 : 0);

      sitted_occup += (wagon_chain[i].get_wagon_type() == "сидячий"
                           ? wagon_chain[i].get_wagon_occupied()
                           : 0);
      econom_occup += (wagon_chain[i].get_wagon_type() == "эконом"
                           ? wagon_chain[i].get_wagon_occupied()
                           : 0);
      luxury_occup += (wagon_chain[i].get_wagon_type() == "люкс"
                           ? wagon_chain[i].get_wagon_occupied()
                           : 0);
    }

    for (int i = 0; i < 3; i++) { // 0 - сидячий, 1 - эконом, 2 - люкс
      int total_occup = 0, total_count = 0;

      if (i == 0) {
        total_occup = sitted_occup;
        total_count = sitted_amount;
      } else if (i == 1) {
        total_occup = econom_occup;
        total_count = econom_amount;
      } else if (i == 2) {
        total_occup = luxury_occup;
        total_count = luxury_amount;
      }
      if (total_count <= 1 && total_occup > 0)
        continue;
      for (int j = 0; j < wagon_amount; j++) {
        if ((i == 0 && wagon_chain[j].get_wagon_type() == "сидячий") ||
            (i == 1 && wagon_chain[j].get_wagon_type() == "эконом") ||
            (i == 2 && wagon_chain[j].get_wagon_type() == "люкс")) {
          if (total_occup == 0) {
            *this - j;
            j--;
          } else if (total_occup > wagon_chain[j].get_wagon_capacity()) {
            wagon_chain[j].set_wagon_capacity(
                wagon_chain[j].get_wagon_capacity());
            total_occup -= wagon_chain[j].get_wagon_capacity();
          } else if (total_occup < wagon_chain[j].get_wagon_capacity()) {
            wagon_chain[j].set_wagon_occupied(total_occup);
            total_occup = 0;
          }
        }
      }
    }
  }

  /**
   * @brief Вставить вагон-ресторан в «оптимальное место».
   *
   * Алгоритм подсчитывает общее число пассажиров (кроме вагона люкс и вагона ресторан),
   * находит точку где примерно "середина" пассажиропотока и вставляет туда
   * новый вагон-ресторан.
   */
  void operator++(int) {
    int total_occup = 0;
    for (int i = 0; i < wagon_amount; i++) {
      if (wagon_chain[i].get_wagon_type() != "люкс" &&
          wagon_chain[i].get_wagon_type() != "ресторан") {
        total_occup += wagon_chain[i].get_wagon_occupied();
      }
    }
    int to_put_index = -1, half_summ = 0;
    if(wagon_amount + 1 >= max_train_lenght){ max_train_lenght += 5; }
    wagon *new_wagon_chain = new wagon[max_train_lenght];
    for (int i = 0; i < wagon_amount; i++) {
      new_wagon_chain[i] = wagon_chain[i];
      if (wagon_chain[i].get_wagon_type() != "люкс" &&
          wagon_chain[i].get_wagon_type() != "ресторан") {
        half_summ += wagon_chain[i].get_wagon_occupied();
      }
      if (half_summ * 2 >= total_occup / 2 - 10) {
        to_put_index = i + 1;
        break;
      }
    }
    if (to_put_index == -1)
      to_put_index = wagon_amount;
    wagon new_restaurant("ресторан");
    new_wagon_chain[to_put_index] = new_restaurant;
    for (int i = to_put_index + 1; i < wagon_amount; i++) {
      new_wagon_chain[i] = wagon_chain[i - 1];
    }
    delete[] wagon_chain;
    wagon_chain = new_wagon_chain;
    wagon_amount++;
  } 

  /**
   * @brief Вывод состава в поток.
   * @param out Выходной поток.
   * @param train Поезд.
   * @returns out.
   */
  friend std::ostream &operator<<(std::ostream &out, const train &train);

  /**
   * @brief Ввод состава из потока.
   * Сначала вводится количество вагонов, затем каждый вагон.
   * 
   * @param in Входной поток.
   * @param train Поезд для заполнения.
   * @returns in.
   */
  friend std::istream &operator>>(std::istream &in, train &train);
};
