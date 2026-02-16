#ifndef ENTITY_INTERFACES_HPP
#define ENTITY_INTERFACES_HPP

namespace itm {
    class Inventory;
}

namespace ent {

// Предварительные объявления
class Entity;

/**
 * @brief Интерфейс для сущностей, которые могут перемещаться по карте.
 */
class IMovable {
public:
    virtual ~IMovable() = default;

    /**
     * @brief Пытается переместить сущность в новую позицию.
     * @param x Целевая X-координата.
     * @param y Целевая Y-координата.
     * @return true если перемещение успешно.
     */
    virtual bool move(int x, int y) = 0;

    /**
     * @brief Получает стоимость времени, необходимую для перемещения на одну клетку.
     * @return Стоимость движения в очках времени.
     */
    virtual int get_movement_cost() const = 0;
};

/**
 * @brief Интерфейс для сущностей, способных атаковать других.
 */
class IAttacker {
public:
    virtual ~IAttacker() = default;

    /**
     * @brief Выполняет атаку на целевую сущность.
     * @param target Указатель на целевую сущность.
     * @return Количество нанесенного урона.
     */
    virtual int attack(Entity* target) = 0;

    /**
     * @brief Получает максимальную дальность, на которой эта сущность может атаковать.
     * @return Дальность атаки в клетках.
     */
    virtual int get_attack_range() const = 0;
};

/**
 * @brief Интерфейс для сущностей, которые могут содержать и управлять инвентарем.
 */
class IInventoryHolder {
public:
    virtual ~IInventoryHolder() = default;

    /**
     * @brief Возвращает ссылку на инвентарь сущности.
     * @return Ссылка на объект Inventory.
     */
    virtual itm::Inventory& get_inventory() = 0;

    /**
     * @brief Проверяет, может ли сущность нести дополнительный вес.
     * @param weight Вес (в произвольных единицах) для проверки.
     * @return true если общая нагрузка не превысит грузоподъемность.
     */
    virtual bool can_carry(int weight) const = 0;
};

/**
 * @brief Интерфейс для сущностей, которые могут воспринимать окружение.
 */
class IVisible {
public:
    virtual ~IVisible() = default;

    /**
     * @brief Получает радиус (в клетках), в пределах которого сущность может видеть.
     * @return Радиус зрения.
     */
    virtual int get_vision_radius() const = 0;

    /**
     * @brief Определяет, может ли эта сущность видеть заданную цель.
     * @param target Указатель на целевую сущность.
     * @return true если цель в пределах дальности зрения.
     */
    virtual bool can_see(const Entity* target) const = 0;
};

/**
 * @brief Интерфейс для сущностей, которые могут получать урон и умирать.
 */
class IMortal {
public:
    virtual ~IMortal() = default;

    /**
     * @brief Наносит урон сущности.
     * @param damage Количество урона для нанесения.
     */
    virtual void take_damage(int damage) = 0;

    /**
     * @brief Обрабатывает смерть сущности (например, изменение состояния, эффекты).
     */
    virtual void die() = 0;

    /**
     * @brief Проверяет, жива ли еще сущность.
     * @return true если здоровье больше нуля.
     */
    virtual bool is_alive() const = 0;
};

} // namespace ent

#endif // ENTITY_INTERFACES_HPP
