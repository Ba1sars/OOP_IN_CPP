#ifndef SIMPLE_CLASS
#define SIMPLE_CLASS

#include <iostream>
#include <istream>
#include <ostream>
#include <sstream>
#include <stdexcept>

/**
 * @brief Класс, описывающий вагон поезда.
 *
 * Содержит сведения о типе вагона, вместимости и числе пассажиров.
 * Предоставляет методы для управления параметрами и пересадки пассажиров.
 */
class wagon {
private:
  int max_capacity = 100;           ///< Максимальная вместительность вагона
  int occupied = 0;                 ///< Текущее количество пассажиров
  std::string type = "сидячий";     ///< Тип вагона

  /**
   * @brief Проверка корректности типа вагона.
   * @param type Тип (строкой).
   * @returns true если тип корректен, иначе false.
   */
  static bool is_valid_type(std::string &type) {
    if (type == "сидячий" || type == "эконом" || type == "люкс" ||
        type == "ресторан") {
      return true;
    }
    return false;
  }

  /**
   * @brief Получить стандартную вместимость для указанного типа.
   * @param type Строка с типом.
   * @returns Вместимость.
   * @throws std::invalid_argument если тип неверен.
   */
  static int capacity_of_type(std::string &type) {
    if (type == "сидячий") {
      return 100;
    } else if (type == "эконом") {
      return 70;
    } else if (type == "люкс") {
      return 30;
    } else if (type == "ресторан") {
      return 0;
    } else {
      throw std::invalid_argument("Несуществующий тип вагона");
    }
  }

public:
  /**
   * @brief Конструктор по умолчанию. Сидячий вагон вместимостью 100.
   */
  wagon() {}

  /**
   * @brief Конструктор с параметрами.
   * @param capacity Вместимость.
   * @param occup Текущее количество пассажиров.
   * @param wagon_type Тип вагона.
   * @throws std::invalid_argument если тип некорректен или вместимость/пассажиры заданы неверно.
   */
  wagon(int capacity, int occup, std::string wagon_type)
      : max_capacity(capacity), occupied(occup), type(wagon_type) {
    if (!is_valid_type(wagon_type))
      throw std::invalid_argument("Несуществующий тип вагона");
    if (capacity < 0 || occup < 0)
      throw std::invalid_argument(
          "Количество пассажиров не может быть отрицательным");
    if (capacity < occup)
      throw std::invalid_argument("Максимальная вместимость вагона не может "
                                  "быть меньше, чем количество занятых мест");
    if (wagon_type == "ресторан" && capacity != 0)
      throw std::invalid_argument("У вагона ресторана вместимость всегда 0");
  }

  /**
   * @brief Конструктор по типу.
   * @param wagon_type Тип вагона.
   * @throws std::invalid_argument если тип некорректен.
   */
  wagon(std::string wagon_type)
      : max_capacity(capacity_of_type(wagon_type)), type(wagon_type) {}

  /**
   * @brief Возвращает процент занятости.
   * @returns Процент занятости (0–100).
   */
  int percentage_of_occupancy() const { return occupied * 100 / max_capacity; }

  /**
   * @brief Посадить заданное число пассажиров.
   * @param to_seat Число пассажиров.
   * @throws std::invalid_argument если число отрицательное или нет мест.
   */
  void operator+(int to_seat) {
    if (to_seat < 0)
      throw std::invalid_argument(
          "Нельзя посадить отрицательное число пассажиров в вагон");
    if (to_seat > max_capacity || occupied + to_seat > max_capacity)
      throw std::invalid_argument(
          "Нет столько свободных мест в вагоне для этого числа пассажиров");
    occupied = occupied + to_seat;
  }

  /**
   * @brief Установить вместимость вагона.
   * @param capacity Новая вместимость.
   * @throws std::invalid_argument если вместимость отрицательна или для ресторана > 0.
   */
  void set_wagon_capacity(int capacity) {
    if (capacity < 0)
      throw std::invalid_argument(
          "Вместимость вагона не может быть отрицательной");
    if (type == "ресторан" && capacity > 0)
      throw std::invalid_argument("У вагона ресторана нет вместимости");
    max_capacity = capacity;
  }

  /**
   * @brief Изменить тип вагона.
   * @param wagon_type Новый тип.
   * @throws std::invalid_argument при некорректном типе или для ресторана с вместимостью != 0.
   */
  void set_wagon_type(std::string wagon_type) {
    if (!is_valid_type(wagon_type))
      throw std::invalid_argument("Несуществующий тип вагона");
    if (wagon_type == "ресторан" && max_capacity != 0)
      throw std::invalid_argument(
          "У вагона ресторана может быть только 0 вместимость");
    type = wagon_type;
  }

  /**
   * @brief Установить количество занятых мест.
   * @param occup Число пассажиров.
   * @throws std::invalid_argument если отрицательно или больше вместимости.
   */
  void set_wagon_occupied(int occup) {
    if (occup < 0 || occup > max_capacity)
      throw std::invalid_argument(
          "Нельзя усадить такое число пассажиров в вагон");
    occupied = occup;
  }

  /**
   * @brief Высадить пассажиров.
   * @param set_free Количество высаживаемых.
   * @throws std::invalid_argument если число отрицательное, больше занятости или вместимости.
   */
  void operator-(int set_free) {
    if (set_free < 0)
      throw std::invalid_argument(
          "Нельзя выпустить отрицательное число пассажиров");
    if (set_free > occupied)
      throw std::invalid_argument(
          "Нельзя выпустить больше людей, чем сидит в вагоне");
    if (set_free > max_capacity)
      throw std::invalid_argument(
          "Нельзя выпустить больше людей, чем вместимость вагона");
    occupied = occupied - set_free;
  }

  /**
   * @brief Получить текущее число пассажиров.
   * @returns Текущее число занятых мест.
   */
  int get_wagon_occupied() const { return occupied; }

  /**
   * @brief Получить вместимость.
   * @returns Максимальная вместимость.
   */
  int get_wagon_capacity() const { return max_capacity; }

  /**
   * @brief Получить тип.
   * @returns Тип вагона ("сидячий", "эконом", "люкс", "ресторан").
   */
  std::string get_wagon_type() const { return type; }

  /**
   * @brief Переместить пассажиров между двумя вагонами.
   * @param to Вагон назначения.
   * @throws std::invalid_argument если один из вагонов ресторан или распределение невозможно.
   */
  void operator>>(wagon &to) {
    if (type == "ресторан" || to.type == "ресторан")
      throw std::invalid_argument(
          "Нельзя пересадить пассажиров в/из вагон ресторана");
    int sum_capacity = max_capacity + to.max_capacity;
    int sum_occupied = occupied + to.occupied;
    int approximate_percent = sum_occupied * 100 / sum_capacity;
    occupied = (approximate_percent * max_capacity / 100);
    to.occupied = sum_occupied - occupied;
    if (occupied > max_capacity || to.occupied > to.max_capacity)
      throw std::invalid_argument("Не получилось поровну распределить людей");
  }

  /**
   * @brief Ввод вагона из потока.
   * @param in Входной поток.
   * @param wagon Объект для заполнения.
   * @returns in (поток).
   */
  friend std::istream &operator>>(std::istream &in, wagon &wagon);

  /**
   * @brief Вывод вагона в поток.
   * @param out Выходной поток.
   * @param wagon Объект для вывода.
   * @returns out (поток).
   */
  friend std::ostream &operator<<(std::ostream &out, wagon &wagon);
};

#endif // SIMPLE_CLASS
