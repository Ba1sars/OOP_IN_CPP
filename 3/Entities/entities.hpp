#ifndef ENTITIES_HPP
#define ENTITIES_HPP

#include "entity_interfaces.hpp"
#include "../Items/items.hpp"
#include <string>
#include <vector>

namespace ent {

/**
 * @brief Представляет точку в 2D пространстве с целочисленными координатами.
 */
struct Point {
    int x, y;
    /**
     * @brief Создает точку с заданными координатами.
     * @param px X-координата.
     * @param py Y-координата.
     */
    Point(int px, int py) : x(px), y(py) {}
};

/**
 * @brief Базовый класс для всех сущностей в игре.
 */
class Entity {
protected:
    std::string name;
    int current_health;
    int max_health;
    int current_time_points;
    int max_time_points;
    int position_x;
    int position_y;

public:
    /**
     * @brief Создает сущность с именем, здоровьем и очками времени.
     * @param entity_name Имя сущности.
     * @param max_hp Максимальное количество здоровья.
     * @param max_tp Максимальное количество очков времени.
     */
    Entity(const std::string& entity_name, int max_hp, int max_tp);

    virtual ~Entity() = default;

    /**
     * @brief Обновляет состояние сущности (должен быть реализован в производных классах).
     */
    virtual void update() = 0;

    /**
     * @brief Проверяет, жива ли сущность.
     * @return true если текущее здоровье > 0, иначе false.
     */
    bool is_alive() const;

    /**
     * @brief Сбрасывает текущие очки времени сущности до максимума.
     */
    void reset_time_points();

    /**
     * @brief Получает текущее количество здоровья сущности.
     * @return Текущее значение здоровья.
     */
    int get_current_health() const { return this->current_health; }

    /**
     * @brief Получает максимальное количество здоровья сущности.
     * @return Максимальное значение здоровья.
     */
    int get_max_health() const { return this->max_health; }

    /**
     * @brief Получает текущие очки времени сущности.
     * @return Текущее значение очков времени.
     */
    int get_current_time_points() const { return this->current_time_points; }

    /**
     * @brief Получает максимальные очки времени сущности.
     * @return Максимальное значение очков времени.
     */
    int get_max_time_points() const { return this->max_time_points; }

    /**
     * @brief Получает имя сущности.
     * @return Имя сущности.
     */
    std::string get_name() const { return this->name; }

    /**
     * @brief Получает X-координату позиции сущности.
     * @return X позиция.
     */
    int get_x() const { return this->position_x; }

    /**
     * @brief Получает Y-координату позиции сущности.
     * @return Y позиция.
     */
    int get_y() const { return this->position_y; }

    /**
     * @brief Устанавливает позицию сущности.
     * @param x Новая X-координата.
     * @param y Новая Y-координата.
     */
    void set_position(int x, int y);

    /**
     * @brief Устанавливает текущие очки времени сущности.
     * @param t_points Новое значение очков времени.
     */
    void set_current_time_points(int t_points);

    /**
     * @brief Устанавливает текущее количество здоровья сущности.
     * @param h_points Новое значение здоровья.
     */
    void set_current_health_points(int h_points);
};

/**
 * @brief Абстрактный базовый класс для монстров.
 */
class Monster : public Entity, public IMovable, public IVisible, public IMortal {
protected:
    int vision_radius;
    int movement_cost;

public:
    /**
     * @brief Создает монстра с параметрами зрения и движения.
     * @param entity_name Имя монстра.
     * @param max_hp Максимальное количество здоровья.
     * @param max_tp Максимальное количество очков времени.
     * @param vision Радиус зрения в клетках.
     * @param move_cost Стоимость движения в очках времени за одну клетку.
     */
    Monster(const std::string& entity_name, int max_hp, int max_tp, int vision, int move_cost);

    virtual ~Monster() = default;

    /**
     * @brief Решает, какое действие предпримет монстр (должен быть реализован).
     */
    virtual void decide_action() = 0;

    /**
     * @brief Пытается переместить монстра в указанные координаты.
     * @param x Целевая X-координата.
     * @param y Целевая Y-координата.
     * @return true если перемещение успешно.
     */
    bool move(int x, int y) override;

    /**
     * @brief Получает стоимость движения этого монстра.
     * @return Стоимость движения в очках времени.
     */
    int get_movement_cost() const override { return this->movement_cost; }

    /**
     * @brief Получает радиус зрения этого монстра.
     * @return Радиус зрения в клетках.
     */
    int get_vision_radius() const override { return this->vision_radius; }

    /**
     * @brief Проверяет, может ли этот монстр видеть данную цель.
     * @param target Указатель на целевую сущность.
     * @return true если цель в пределах радиуса зрения.
     */
    bool can_see(const Entity* target) const override;

    /**
     * @brief Наносит урон монстру.
     * @param damage Количество урона для нанесения.
     */
    void take_damage(int damage) override;

    /**
     * @brief Обрабатывает смерть монстра.
     */
    void die() override;

    /**
     * @brief Проверяет, жив ли монстр.
     * @return true если текущее здоровье > 0.
     */
    bool is_alive() const override;
};

/**
 * @brief Представляет управляемого игроком оперативника с оружием и инвентарем.
 */
class Operative : public Entity, public IMovable, public IAttacker, public IInventoryHolder, public IVisible, public IMortal {
private:
    itm::Inventory inventory;
    itm::Weapon* active_weapon;
    itm::Weapon* secondary_weapon;
    int hand_slots;
    int strength;
    int accuracy;
    int vision_radius;
    int movement_cost;
    int reload_cost;

public:
    /**
     * @brief Создает оперативника с основными характеристиками.
     * @param name Имя оперативника.
     * @param max_hp Максимальное количество здоровья.
     * @param max_tp Максимальное количество очков времени.
     * @param str Характеристика силы.
     * @param acc Характеристика точности.
     * @param vision Радиус зрения в клетках.
     */
    Operative(const std::string& name, int max_hp, int max_tp, int str, int acc, int vision);

    /**
     * @brief Лечит оперативника на заданное количество.
     * @param amount Количество здоровья для восстановления.
     */
    void heal(int amount);

    /**
     * @brief Экипирует оружие как активное.
     * @param weapon Указатель на оружие для экипировки.
     * @return true если успешно.
     */
    bool equip_weapon(itm::Weapon* weapon);

    /**
     * @brief Экипирует оружие как вторичное.
     * @param weapon Указатель на оружие для экипировки.
     * @return true если успешно.
     */
    bool equip_secondary_weapon(itm::Weapon* weapon);

    /**
     * @brief Перезаряжает активное оружие.
     * @return true если перезарядка успешна.
     */
    bool reload_weapon();

    /**
     * @brief Меняет местами активное и вторичное оружие.
     * @return true если смена успешна.
     */
    bool switch_weapons();

    /**
     * @brief Обновляет состояние оперативника (например, перезарядки, эффекты).
     */
    void update() override;

    /**
     * @brief Перемещает оперативника в заданные координаты.
     * @param x Целевая X-координата.
     * @param y Целевая Y-координата.
     * @return true если перемещение успешно.
     */
    bool move(int x, int y) override;

    /**
     * @brief Получает стоимость движения для этого оперативника.
     * @return Стоимость движения в очках времени.
     */
    int get_movement_cost() const override;

    /**
     * @brief Атакует целевую сущность.
     * @param target Указатель на целевую сущность.
     * @return Нанесенный урон.
     */
    int attack(Entity* target) override;

    /**
     * @brief Получает дальность атаки оперативника (на основе активного оружия).
     * @return Дальность атаки в клетках.
     */
    int get_attack_range() const override;

    /**
     * @brief Получает ссылку на инвентарь оперативника.
     * @return Ссылка на объект инвентаря.
     */
    itm::Inventory& get_inventory() override;

    /**
     * @brief Проверяет, может ли оперативник нести дополнительный вес.
     * @param weight Вес для проверки.
     * @return true если общий вес не превысит грузоподъемность.
     */
    bool can_carry(int weight) const override;

    /**
     * @brief Получает радиус зрения оперативника.
     * @return Радиус зрения в клетках.
     */
    int get_vision_radius() const override;

    /**
     * @brief Проверяет, может ли оперативник видеть целевую сущность.
     * @param target Указатель на целевую сущность.
     * @return true если в пределах дальности зрения.
     */
    bool can_see(const Entity* target) const override;

    /**
     * @brief Наносит урон оперативнику.
     * @param damage Количество урона.
     */
    void take_damage(int damage) override;

    /**
     * @brief Обрабатывает смерть оперативника.
     */
    void die() override;

    /**
     * @brief Проверяет, жив ли оперативник.
     * @return true если текущее здоровье > 0.
     */
    bool is_alive() const override;

    /**
     * @brief Получает указатель на текущее активное оружие.
     * @return Указатель на активное оружие, или nullptr если его нет.
     */
    itm::Weapon* get_active_weapon() { return active_weapon; }

    /**
     * @brief Получает характеристику силы оперативника.
     * @return Значение силы.
     */
    int get_strength() const { return strength; }

    /**
     * @brief Получает характеристику точности оперативника.
     * @return Значение точности.
     */
    int get_accuracy() const { return accuracy; }
};

/**
 * @brief Монстр, который атакует в ближнем бою.
 */
class WildMonster : public Monster, public IAttacker {
private:
    int melee_damage;
    int melee_accuracy;
    int attack_range;
    int attack_cost;

public:
    /**
     * @brief Создает дикого монстра с характеристиками ближнего боя.
     * @param name Имя монстра.
     * @param max_hp Максимальное количество здоровья.
     * @param max_tp Максимальное количество очков времени.
     * @param vision Радиус зрения в клетках.
     * @param move_cost Стоимость движения в очках времени.
     * @param damage Базовый урон в ближнем бою.
     * @param accuracy Точность ближнего боя в процентах.
     */
    WildMonster(const std::string& name, int max_hp, int max_tp, int vision, int move_cost, int damage, int accuracy);

    /**
     * @brief Решает действие монстра (например, преследование или атака).
     */
    void decide_action() override;

    /**
     * @brief Обновляет внутреннее состояние монстра.
     */
    void update() override;

    /**
     * @brief Атакует целевую сущность в ближнем бою.
     * @param target Указатель на целевую сущность.
     * @return Нанесенный урон.
     */
    int attack(Entity* target) override;

    /**
     * @brief Получает дальность атаки (всегда 1 для ближнего боя).
     * @return Дальность атаки (1 клетка).
     */
    int get_attack_range() const override { return 1; }
};

/**
 * @brief Монстр, способный использовать дальнее или экипированное оружие.
 */
class IntelligentMonster : public Monster, public IAttacker {
private:
    itm::Weapon* carried_weapon;
    int accuracy;
    int attack_range;
    int melee_damage;
    int attack_cost;

public:
    /**
     * @brief Создает разумного монстра.
     * @param name Имя монстра.
     * @param max_hp Максимальное количество здоровья.
     * @param max_tp Максимальное количество очков времени.
     * @param vision Радиус зрения в клетках.
     * @param move_cost Стоимость движения в очках времени.
     * @param acc Характеристика точности.
     */
    IntelligentMonster(const std::string& name, int max_hp, int max_tp, int vision, int move_cost, int acc);

    /**
     * @brief Подбирает оружие из окружения.
     * @param weapon Указатель на оружие для подбора.
     * @return true если подбор успешен.
     */
    bool pickup_weapon(itm::Weapon* weapon);

    /**
     * @brief Бросает текущее носим оружие.
     */
    void drop_weapon();

    /**
     * @brief Решает действие монстра на основе логики ИИ.
     */
    void decide_action() override;

    /**
     * @brief Обновляет внутреннее состояние монстра.
     */
    void update() override;

    /**
     * @brief Атакует цель, используя носим оружие (если есть).
     * @param target Указатель на целевую сущность.
     * @return Нанесенный урон.
     */
    int attack(Entity* target) override;

    /**
     * @brief Получает дальность атаки на основе носимого оружия.
     * @return Дальность атаки в клетках.
     */
    int get_attack_range() const override;

    /**
     * @brief Получает текущее носим оружие (если есть).
     * @return Указатель на оружие, или nullptr.
     */
    itm::Weapon* get_weapon() { return carried_weapon; }
};

/**
 * @brief Монстр, который собирает и хранит предметы.
 */
class Forager : public Monster, public IInventoryHolder {
private:
    itm::Inventory inventory;
    std::vector<Point> storage_points;
    int max_inventory_size;
    int max_carry_weight;

public:
    /**
     * @brief Создает собирателя-монстра.
     * @param name Имя собирателя.
     * @param max_hp Максимальное количество здоровья.
     * @param max_tp Максимальное количество очков времени.
     * @param vision Радиус зрения в клетках.
     * @param move_cost Стоимость движения в очках времени.
     */
    Forager(const std::string& name, int max_hp, int max_tp, int vision, int move_cost);

    /**
     * @brief Собирает ближайшие предметы и добавляет их в инвентарь.
     */
    void collect_items();

    /**
     * @brief Двигается к ближайшей точке хранения для сдачи предметов.
     */
    void go_to_storage();

    /**
     * @brief Добавляет новую точку хранения в список собирателя.
     * @param x X-координата точки хранения.
     * @param y Y-координата точки хранения.
     */
    void add_storage_point(int x, int y);

    /**
     * @brief Решает следующее действие собирателя (собирать, хранить, двигаться).
     */
    void decide_action() override;

    /**
     * @brief Обновляет состояние собирателя.
     */
    void update() override;

    /**
     * @brief Получает ссылку на инвентарь собирателя.
     * @return Ссылка на инвентарь.
     */
    itm::Inventory& get_inventory() override;

    /**
     * @brief Проверяет, может ли собиратель нести дополнительный вес.
     * @param weight Вес для проверки.
     * @return true если в пределах грузоподъемности.
     */
    bool can_carry(int weight) const override;
};

} // namespace ent

#endif // ENTITIES_HPP
