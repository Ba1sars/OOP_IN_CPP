#include "entities.hpp"
#include <stdexcept>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <ctime>

namespace ent {

Entity::Entity(const std::string& entity_name, int max_hp, int max_tp)
    : name(entity_name), current_health(max_hp), position_x(0), position_y(0) {
    if(max_hp <= 0) {
        throw std::invalid_argument("Нельзя создать сущность с таким здоровьем");
    }
    max_health = max_hp;

    if(max_tp <= 0) {
        throw std::invalid_argument("Нельзя создать сущность с таким временем");
    }
    max_time_points = max_tp;
    current_time_points = max_tp;
}

bool Entity::is_alive() const {
    return current_health > 0;
}

void Entity::reset_time_points() {
    current_time_points = max_time_points;
}

void Entity::set_current_time_points(int t_points) {
    if(t_points < 0 || t_points > max_time_points) {
        throw std::invalid_argument("Нельзя задать сущности такое количество времени");
    }
    current_time_points = t_points;
}

void Entity::set_current_health_points(int h_points) {
    if(h_points < 0 || h_points > max_health) {
        throw std::invalid_argument("Нельзя задать сущности такое количество здоровья");
    }
    current_health = h_points;
}

void Entity::set_position(int x, int y) {
    position_x = x;
    position_y = y;
}

Monster::Monster(const std::string& entity_name, int max_hp, int max_tp,
                 int vision, int move_cost)
    : Entity(entity_name, max_hp, max_tp)
    , vision_radius(vision)
    , movement_cost(move_cost) {
    if(vision < 0) {
        throw std::invalid_argument("Радиус обзора не может быть отрицательным");
    }
    if(move_cost <= 0) {
        throw std::invalid_argument("Стоимость движения должна быть положительной");
    }
}

bool Monster::move(int x, int y) {
    if(current_time_points < movement_cost) {
        return false;
    }
    position_x = x;
    position_y = y;
    current_time_points -= movement_cost;
    return true;
}

bool Monster::can_see(const Entity* target) const {
    if(!target) return false;

    int dx = target->get_x() - position_x;
    int dy = target->get_y() - position_y;
    double distance = std::sqrt(dx * dx + dy * dy);

    return distance <= vision_radius;
}

void Monster::take_damage(int damage) {
    if(damage < 0) return;
    current_health -= damage;
    if(current_health < 0) {
        current_health = 0;
    }
}

void Monster::die() {
    current_health = 0;
}

bool Monster::is_alive() const {
    return current_health > 0;
}

Operative::Operative(const std::string& name, int max_hp, int max_tp,
                     int str, int acc, int vision)
    : Entity(name, max_hp, max_tp)
    , inventory(20)
    , active_weapon(nullptr)
    , secondary_weapon(nullptr)
    , hand_slots(2)
    , strength(str)
    , accuracy(acc)
    , vision_radius(vision)
    , movement_cost(1)
    , reload_cost(2) {
    if(str <= 0 || acc <= 0 || vision <= 0) {
        throw std::invalid_argument("Характеристики оперативника должны быть положительными");
    }
}

void Operative::heal(int amount) {
    if(amount <= 0) return;
    current_health += amount;
    if(current_health > max_health) {
        current_health = max_health;
    }
}

bool Operative::equip_weapon(itm::Weapon* weapon) {
    if(!weapon) return false;

    if(active_weapon) {
        inventory.add(active_weapon);
    }

    active_weapon = weapon;
    return true;
}

bool Operative::equip_secondary_weapon(itm::Weapon* weapon) {
    if(!weapon || hand_slots < 2) return false;

    if(secondary_weapon) {
        inventory.add(secondary_weapon);
    }

    secondary_weapon = weapon;
    return true;
}

bool Operative::reload_weapon() {
    if(!active_weapon) return false;
    if(current_time_points < reload_cost) return false;

    for(int i = 0; i < inventory.get_count(); ++i) {
        itm::IItem* item = inventory.get_item(i);
        itm::AmmoContainer* container = dynamic_cast<itm::AmmoContainer*>(item);

        if(container) {
            if(active_weapon->reload(container)) {
                current_time_points -= reload_cost;
                return true;
            }
        }
    }

    return false;
}

bool Operative::switch_weapons() {
    if(!secondary_weapon) return false;

    itm::Weapon* temp = active_weapon;
    active_weapon = secondary_weapon;
    secondary_weapon = temp;

    return true;
}

void Operative::update() {
}

bool Operative::move(int x, int y) {
    if(current_time_points < movement_cost) {
        return false;
    }
    position_x = x;
    position_y = y;
    current_time_points -= movement_cost;
    return true;
}

int Operative::get_movement_cost() const {
    return movement_cost;
}

int Operative::attack(Entity* target) {
    if(!target || !active_weapon) return 0;
    if(!active_weapon->is_loaded()) return 0;

    int fire_cost = 1;
    if(current_time_points < fire_cost) return 0;

    int damage = active_weapon->fire();
    current_time_points -= fire_cost;

    IMortal* mortal = dynamic_cast<IMortal*>(target);
    if(mortal) {
        mortal->take_damage(damage);
    }

    return damage;
}

int Operative::get_attack_range() const {
    if(!active_weapon) return 0;
    return 10;
}

itm::Inventory& Operative::get_inventory() {
    return inventory;
}

bool Operative::can_carry(int weight) const {
    return inventory.get_total_weight() + weight <= strength * 10;
}

int Operative::get_vision_radius() const {
    return vision_radius;
}

bool Operative::can_see(const Entity* target) const {
    if(!target) return false;

    int dx = target->get_x() - position_x;
    int dy = target->get_y() - position_y;
    double distance = std::sqrt(dx * dx + dy * dy);

    return distance <= vision_radius;
}

void Operative::take_damage(int damage) {
    if(damage < 0) return;
    current_health -= damage;
    if(current_health < 0) {
        current_health = 0;
    }
}

void Operative::die() {
    current_health = 0;
}

bool Operative::is_alive() const {
    return current_health > 0;
}

WildMonster::WildMonster(const std::string& name, int max_hp, int max_tp,
                         int vision, int move_cost, int damage, int acc)
    : Monster(name, max_hp, max_tp, vision, move_cost)
    , melee_damage(damage)
    , melee_accuracy(acc)
    , attack_range(1)
    , attack_cost(1) {
    if(damage <= 0 || acc <= 0) {
        throw std::invalid_argument("Урон и точность должны быть положительными");
    }
}

void WildMonster::decide_action() {
}

void WildMonster::update() {
}

int WildMonster::attack(Entity* target) {
    if(!target) return 0;
    if(current_time_points < attack_cost) return 0;

    int dx = std::abs(target->get_x() - position_x);
    int dy = std::abs(target->get_y() - position_y);
    int distance = static_cast<int>(std::sqrt(dx*dx + dy*dy));
    if(distance > attack_range) return 0;

    current_time_points -= attack_cost;
    IMortal* mortal = dynamic_cast<IMortal*>(target);
    if(mortal) {
        mortal->take_damage(melee_damage);
    }

    return melee_damage;
}

IntelligentMonster::IntelligentMonster(const std::string& name, int max_hp,
                                       int max_tp, int vision, int move_cost, int acc)
    : Monster(name, max_hp, max_tp, vision, move_cost)
    , carried_weapon(nullptr)
    , accuracy(acc)
    , attack_range(3)
    , melee_damage(10)
    , attack_cost(2) {
    if(acc <= 0) {
        throw std::invalid_argument("Точность должна быть положительной");
    }
}

bool IntelligentMonster::pickup_weapon(itm::Weapon* weapon) {
    if(!weapon) return false;

    if(carried_weapon) {
        drop_weapon();
    }

    carried_weapon = weapon;
    return true;
}

void IntelligentMonster::drop_weapon() {
    carried_weapon = nullptr;
}

void IntelligentMonster::decide_action() {
}

void IntelligentMonster::update() {
}

int IntelligentMonster::attack(Entity* target) {
    if(!target) return 0;
    if(current_time_points < attack_cost) return 0;

    int dx = std::abs(target->get_x() - position_x);
    int dy = std::abs(target->get_y() - position_y);
    int distance = static_cast<int>(std::sqrt(dx*dx + dy*dy));
    
    int damage = 0;
    
    if(carried_weapon) {
        if(distance > attack_range) return 0;
        damage = 15;
    } else {
        if(distance > 1) return 0;
        damage = melee_damage;
    }
    
    current_time_points -= attack_cost;
    IMortal* mortal = dynamic_cast<IMortal*>(target);
    if(mortal) {
        mortal->take_damage(damage);
    }

    return damage;
}

int IntelligentMonster::get_attack_range() const {
    if(!carried_weapon) return 1;
    return attack_range;
}

Forager::Forager(const std::string& name, int max_hp, int max_tp,
                 int vision, int move_cost)
    : Monster(name, max_hp, max_tp, vision, move_cost)
    , inventory(15)
    , max_inventory_size(15)
    , max_carry_weight(50) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

void Forager::collect_items() {
    if(inventory.get_count() >= max_inventory_size) return;
}

void Forager::go_to_storage() {
    if(storage_points.empty()) return;
    
    Point nearest = storage_points[0];
    int min_dist = std::abs(nearest.x - position_x) + std::abs(nearest.y - position_y);
    
    for(const auto& point : storage_points) {
        int dist = std::abs(point.x - position_x) + std::abs(point.y - position_y);
        if(dist < min_dist) {
            min_dist = dist;
            nearest = point;
        }
    }
}

void Forager::add_storage_point(int x, int y) {
    for(const auto& point : storage_points) {
        if(point.x == x && point.y == y) return;
    }
    
    storage_points.push_back(Point(x, y));
}

void Forager::decide_action() {
}

void Forager::update() {
}

itm::Inventory& Forager::get_inventory() {
    return inventory;
}

bool Forager::can_carry(int weight) const {
    return inventory.get_total_weight() + weight <= max_carry_weight;
}

} // namespace ent
