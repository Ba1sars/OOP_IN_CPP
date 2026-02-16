#include "Map/map.hpp"
#include "Entities/entities.hpp"
#include "Items/items.hpp"
#include <iostream>
#include <vector>
#include <memory>

int main() {
    try {
        Level level(5, 5);
        
        std::vector<std::unique_ptr<ent::Entity>> entities;
        
        auto op = std::make_unique<ent::Operative>("Агент", 100, 10, 10, 10, 5);
        entities.push_back(std::move(op));
        level.add_operative(static_cast<ent::Operative*>(entities.back().get()));
        level.add_entity(entities.back().get(), 1, 1);
        
        auto wm = std::make_unique<ent::WildMonster>("Волк", 50, 8, 4, 2, 15, 80);
        entities.push_back(std::move(wm));
        level.add_monster(static_cast<ent::Monster*>(entities.back().get()));
        level.add_entity(entities.back().get(), 3, 2);
        
        auto im = std::make_unique<ent::IntelligentMonster>("Охотник", 70, 12, 5, 3, 75);
        entities.push_back(std::move(im));
        level.add_monster(static_cast<ent::Monster*>(entities.back().get()));
        level.add_entity(entities.back().get(), 4, 4);
        
        auto fg = std::make_unique<ent::Forager>("Сборщик", 30, 6, 3, 2);
        entities.push_back(std::move(fg));
        level.add_monster(static_cast<ent::Monster*>(entities.back().get()));
        level.add_entity(entities.back().get(), 0, 3);
        
        for (int y = 0; y < 5; ++y) {
            for (int x = 0; x < 5; ++x) {
                const Cell& cell = level.get_cell(x, y);
                ent::Entity* entity = cell.get_entity();
                
                if (entity) {
                    if (dynamic_cast<ent::Operative*>(entity)) std::cout << "* ";
                    else if (dynamic_cast<ent::WildMonster*>(entity)) std::cout << "W ";
                    else if (dynamic_cast<ent::IntelligentMonster*>(entity)) std::cout << "I ";
                    else if (dynamic_cast<ent::Forager*>(entity)) std::cout << "F ";
                    else std::cout << "? ";
                } else {
                    std::cout << ". ";
                }
            }
            std::cout << "\n";
        }
        
        std::cout << "\nЛегенда:\n";
        std::cout << "* - Оперативник\n";
        std::cout << "W - Дикий монстр\n";
        std::cout << "I - Умный монстр\n";
        std::cout << "F - Фуражир\n";
        std::cout << ". - Пустая клетка\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
