#ifndef ITEM_INTERFACES_HPP
#define ITEM_INTERFACES_HPP

#include <string>

namespace itm {

/**
 * @brief Абстрактный базовый интерфейс для всех типов предметов в игре.
 * 
 * Любой конкретный предмет (оружие, расходник и т.д.) должен реализовать этот интерфейс,
 * чтобы предоставить основные свойства, такие как вес и имя.
 */
class IItem {
public:
    /**
     * @brief Виртуальный деструктор для безопасного полиморфного удаления.
     */
    virtual ~IItem() = default;

    /**
     * @brief Получает вес предмета.
     * @return Вес в произвольных единицах (например, килограммы или игровые очки).
     */
    virtual int get_weight() const = 0;

    /**
     * @brief Получает отображаемое имя предмета.
     * @return Человекочитаемое имя в виде строки.
     */
    virtual std::string get_name() const = 0;
};

} // namespace itm

#endif // ITEM_INTERFACES_HPP
