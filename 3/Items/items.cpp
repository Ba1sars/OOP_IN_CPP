#include "items.hpp"
#include <stdexcept>
#include <algorithm>

namespace itm {

Weapon::Weapon(const std::string& weapon_name, int dmg, AmmoType type,
               int max_ammo_capacity, int fire_time_cost,
               int reload_time_cost, int w)
    : name(weapon_name)
    , damage(dmg)
    , ammo_type(type)
    , current_ammo(0)
    , max_ammo(max_ammo_capacity)
    , fire_cost(fire_time_cost)
    , reload_cost(reload_time_cost)
    , weight(w) {
    if(dmg <= 0) {
        throw std::invalid_argument("Урон оружия должен быть положительным");
    }
    if(max_ammo_capacity <= 0) {
        throw std::invalid_argument("Ёмкость магазина должна быть положительной");
    }
    if(w <= 0) {
        throw std::invalid_argument("Вес должен быть положительным");
    }
}

int Weapon::fire() {
    if(current_ammo <= 0) {
        return 0;
    }

    current_ammo--;
    return damage;
}

bool Weapon::reload(AmmoContainer* container) {
    if(!container) return false;

    if(container->get_name().find("9MM") != std::string::npos && ammo_type != AmmoType::PISTOL_9MM) return false;
    if(container->get_name().find("556") != std::string::npos && ammo_type != AmmoType::RIFLE_556) return false;
    if(container->get_name().find("12G") != std::string::npos && ammo_type != AmmoType::SHOTGUN_12G) return false;
    if(container->get_name().find("762") != std::string::npos && ammo_type != AmmoType::SNIPER_762) return false;

    int needed = max_ammo - current_ammo;
    if(needed <= 0) return false;

    int extracted = container->extract_ammo(needed);
    current_ammo += extracted;

    return extracted > 0;
}

bool Weapon::is_loaded() const {
    return current_ammo > 0;
}

AmmoContainer::AmmoContainer(AmmoType type, int max_capacity, int w)
    : ammo_type(type)
    , current_ammo(max_capacity)
    , max_ammo(max_capacity)
    , weight(w) {
    if(max_capacity <= 0) {
        throw std::invalid_argument("Ёмкость контейнера должна быть положительной");
    }
    if(w <= 0) {
        throw std::invalid_argument("Вес должен быть положительным");
    }
}

int AmmoContainer::extract_ammo(int amount) {
    if(amount <= 0) return 0;

    int extracted = std::min(amount, current_ammo);
    current_ammo -= extracted;

    return extracted;
}

std::string AmmoContainer::get_name() const {
    switch(ammo_type) {
        case AmmoType::PISTOL_9MM:
            return "Контейнер патронов 9MM";
        case AmmoType::RIFLE_556:
            return "Контейнер патронов 5.56";
        case AmmoType::SHOTGUN_12G:
            return "Контейнер патронов 12G";
        case AmmoType::SNIPER_762:
            return "Контейнер патронов 7.62";
        default:
            return "Неизвестный тип патронов";
    }
}

MedKit::MedKit(const std::string& kit_name, int heal, int use_time_cost, int w)
    : name(kit_name)
    , heal_amount(heal)
    , use_cost(use_time_cost)
    , weight(w) {
    if(heal <= 0) {
        throw std::invalid_argument("Количество восстановленного здоровья должно быть положительным");
    }
    if(use_time_cost < 0) {
        throw std::invalid_argument("Стоимость использования не может быть отрицательной");
    }
    if(w <= 0) {
        throw std::invalid_argument("Вес должен быть положительным");
    }
}

void MedKit::use(Entity* target) {
    if(!target) return;
}

Inventory::Inventory(int initial_capacity)
    : max_capacity(initial_capacity)
    , current_count(0) {
    if(initial_capacity <= 0) {
        throw std::invalid_argument("Ёмкость инвентаря должна быть положительной");
    }
}

Inventory::~Inventory() {
    items.clear();
}

bool Inventory::add(IItem* item) {
    if(!item) return false;
    if(current_count >= max_capacity) return false;

    items.push_back(item);
    current_count++;
    return true;
}

bool Inventory::remove(IItem* item) {
    if(!item) return false;

    auto it = std::find(items.begin(), items.end(), item);
    if(it != items.end()) {
        items.erase(it);
        current_count--;
        return true;
    }

    return false;
}

IItem* Inventory::get_item(int index) {
    if(index < 0 || index >= current_count) {
        return nullptr;
    }
    return items[index];
}

int Inventory::get_total_weight() const {
    int total = 0;
    for(const auto& item : items) {
        if(item) {
            total += item->get_weight();
        }
    }
    return total;
}

std::vector<IItem*> Inventory::drop_all_items() {
    std::vector<IItem*> dropped = items;
    items.clear();
    current_count = 0;
    return dropped;
}

} // namespace itm
