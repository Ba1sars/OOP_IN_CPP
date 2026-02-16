#ifndef ITEMS_HPP
#define ITEMS_HPP

#include "item_interfaces.hpp"
#include <string>
#include <vector>
#include <memory>

namespace itm {

/**
 * @brief Типы боеприпасов, поддерживаемые оружием в игре.
 */
enum class AmmoType {
    PISTOL_9MM,   ///< Патроны 9мм для пистолета.
    RIFLE_556,    ///< Боеприпасы 5.56×45mm NATO для винтовки.
    SHOTGUN_12G,  ///< Дроби 12-го калибра для дробовика.
    SNIPER_762    ///< Патроны 7.62×51mm NATO для снайперской винтовки.
};

// Предварительное объявление
class Entity;

/**
 * @brief Представляет контейнер, содержащий боеприпасы определенного типа.
 */
class AmmoContainer;

/**
 * @brief Предмет-оружие, способное стрелять и перезаряжаться.
 */
class Weapon : public IItem {
private:
    std::string name;
    int damage;
    AmmoType ammo_type;
    int current_ammo;
    int max_ammo;
    int fire_cost;
    int reload_cost;
    int weight;

public:
    /**
     * @brief Создает оружие с заданными свойствами.
     * @param weapon_name Человекочитаемое имя оружия.
     * @param dmg Базовый урон за выстрел.
     * @param type Тип боеприпасов, используемых оружием.
     * @param max_ammo_capacity Максимальное количество патронов, которое может вместить оружие.
     * @param fire_time_cost Очки времени, потребляемые за выстрел.
     * @param reload_time_cost Очки времени, потребляемые для полной или частичной перезарядки.
     * @param w Вес оружия в произвольных единицах.
     */
    Weapon(const std::string& weapon_name, int dmg, AmmoType type, int max_ammo_capacity, int fire_time_cost, int reload_time_cost, int w);

    /**
     * @brief Стреляет из оружия, если оно заряжено.
     * Потребляет один патрон и очки времени.
     * @return Нанесенный урон если произведен выстрел; 0 если оружие пусто.
     */
    int fire();

    /**
     * @brief Перезаряжает оружие из контейнера с боеприпасами.
     * @param container Указатель на контейнер с боеприпасами для извлечения.
     * @return true если был загружен хотя бы один патрон.
     */
    bool reload(AmmoContainer* container);

    /**
     * @brief Проверяет, заряжено ли оружие в данный момент.
     * @return true если current_ammo > 0.
     */
    bool is_loaded() const;

    /**
     * @brief Получает базовое значение урона оружия.
     * @return Урон за выстрел.
     */
    int get_damage() const { return damage; }

    /**
     * @brief Получает вес оружия.
     * @return Вес в произвольных единицах.
     */
    int get_weight() const override { return weight; }

    /**
     * @brief Получает отображаемое имя оружия.
     * @return Имя оружия в виде строки.
     */
    std::string get_name() const override { return name; }
};

/**
 * @brief Контейнер, хранящий определенный тип боеприпасов.
 */
class AmmoContainer : public IItem {
private:
    AmmoType ammo_type;
    int current_ammo;
    int max_ammo;
    int weight;

public:
    /**
     * @brief Создает контейнер для боеприпасов с заданной вместимостью и весом.
     * @param type Тип хранимых боеприпасов.
     * @param max_capacity Максимальное количество патронов, которое может вместить этот контейнер.
     * @param w Вес контейнера (пустой или полный, как определено в дизайне).
     */
    AmmoContainer(AmmoType type, int max_capacity, int w);

    /**
     * @brief Пытается извлечь заданное количество боеприпасов из контейнера.
     * @param amount Желаемое количество патронов для извлечения.
     * @return Фактическое количество извлеченных патронов (может быть меньше при недостатке).
     */
    int extract_ammo(int amount);

    /**
     * @brief Получает общий вес контейнера с боеприпасами.
     * @return Вес в произвольных единицах.
     */
    int get_weight() const override { return weight; }

    /**
     * @brief Получает человекочитаемое имя, описывающее тип и количество боеприпасов.
     * @return Строка вида "Патроны 9мм (12/20)".
     */
    std::string get_name() const override;
};

/**
 * @brief Расходный предмет, восстанавливающий здоровье сущности.
 */
class MedKit : public IItem {
private:
    std::string name;
    int heal_amount;
    int use_cost;
    int weight;

public:
    /**
     * @brief Создает аптечку с параметрами лечения и времени.
     * @param kit_name Отображаемое имя аптечки.
     * @param heal Количество восстанавливаемого здоровья при использовании.
     * @param use_time_cost Очки времени, потребляемые при использовании аптечки.
     * @param w Вес аптечки.
     */
    MedKit(const std::string& kit_name, int heal, int use_time_cost, int w);

    /**
     * @brief Применяет лечение к целевой сущности.
     * @param target Указатель на сущность для лечения.
     */
    void use(Entity* target);

    /**
     * @brief Получает вес аптечки.
     * @return Вес в произвольных единицах.
     */
    int get_weight() const override { return weight; }

    /**
     * @brief Получает отображаемое имя аптечки.
     * @return Имя в виде строки.
     */
    std::string get_name() const override { return name; }
};

/**
 * @brief Коллекция предметов, переносимых сущностью.
 */
class Inventory {
private:
    std::vector<IItem*> items;
    int max_capacity;
    int current_count;

public:
    /**
     * @brief Создает инвентарь с заданной вместимостью.
     * @param initial_capacity Максимальное количество предметов, которое может вместить инвентарь (по умолчанию: 10).
     */
    Inventory(int initial_capacity = 10);

    /**
     * @brief Деструктор; НЕ удаляет принадлежащие предметы (контейнер без владения по дизайну).
     */
    ~Inventory();

    /**
     * @brief Добавляет предмет в инвентарь, если есть место.
     * @param item Указатель на предмет для добавления.
     * @return true если предмет был успешно добавлен.
     */
    bool add(IItem* item);

    /**
     * @brief Удаляет предмет из инвентаря.
     * @param item Указатель на предмет для удаления.
     * @return true если предмет был найден и удален.
     */
    bool remove(IItem* item);

    /**
     * @brief Извлекает предмет по индексу.
     * @param index Индекс предмета (начиная с 0).
     * @return Указатель на предмет, или nullptr если индекс выходит за границы.
     */
    IItem* get_item(int index);

    /**
     * @brief Вычисляет общий вес всех предметов в инвентаре.
     * @return Сумма весов всех предметов.
     */
    int get_total_weight() const;

    /**
     * @brief Получает текущее количество предметов в инвентаре.
     * @return Количество предметов.
     */
    int get_count() const { return current_count; }

    /**
     * @brief Удаляет и возвращает все предметы из инвентаря.
     * Инвентарь становится пустым после этого вызова.
     * @return Вектор, содержащий указатели на все предметы.
     */
    std::vector<IItem*> drop_all_items();
};

} // namespace itm

#endif // ITEMS_HPP
