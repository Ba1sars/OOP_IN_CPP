#include "map.hpp"
#include "matrix.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdexcept>

Cell::Cell()
    : type(CellType::EMPTY)
    , entity(nullptr) {
}

Cell::Cell(CellType cell_type)
    : type(cell_type)
    , entity(nullptr) {
}

void Cell::add_item(itm::IItem* item) {
    if(item) {
        items.push_back(item);
    }
}

void Cell::remove_item(itm::IItem* item) {
    if(!item) return;

    auto it = std::find(items.begin(), items.end(), item);
    if(it != items.end()) {
        items.erase(it);
    }
}

std::vector<itm::IItem*> Cell::take_all_items() {
    std::vector<itm::IItem*> result = items;
    items.clear();
    return result;
}

bool Cell::is_passable() const {
    switch(type) {
        case CellType::EMPTY:
        case CellType::STORAGE_POINT:
            return entity == nullptr;
        case CellType::WALL:
        case CellType::GLASS:
        case CellType::PARTITION:
            return false;
        default:
            return false;
    }
}

bool Cell::is_transparent() const {
    switch(type) {
        case CellType::EMPTY:
        case CellType::GLASS:
        case CellType::STORAGE_POINT:
            return true;
        case CellType::WALL:
        case CellType::PARTITION:
            return false;
        default:
            return false;
    }
}

bool Cell::is_shootable() const {
    switch(type) {
        case CellType::EMPTY:
        case CellType::STORAGE_POINT:
            return true;
        case CellType::WALL:
        case CellType::GLASS:
        case CellType::PARTITION:
            return false;
        default:
            return false;
    }
}

Level::Level(int width, int height)
    : map(height, width) {
    if(width <= 0 || height <= 0) {
        throw std::invalid_argument("Размеры уровня должны быть положительными");
    }

    for(int i = 0; i < height; ++i) {
        for(int j = 0; j < width; ++j) {
            map[i][j] = Cell(CellType::EMPTY);
        }
    }
}

Level::~Level() {
    operatives.clear();
    monsters.clear();
}

void Level::build_map(const MapConfig& config) {
    if(config.width != map.get_cols() || config.height != map.get_rows()) {
        map.resize(config.height, config.width);

        for(int i = 0; i < config.height; ++i) {
            for(int j = 0; j < config.width; ++j) {
                if(!map.is_valid_position(i, j) || map[i][j].get_type() == CellType::EMPTY) {
                    map[i][j] = Cell(CellType::EMPTY);
                }
            }
        }
    }

    if(!config.map_file_path.empty()) {
        load_from_file(config.map_file_path);
    }
}

void Level::load_from_file(const std::string& filepath) {
    std::ifstream file(filepath);
    if(!file.is_open()) {
        throw std::runtime_error("Не удалось открыть файл карты: " + filepath);
    }

    int height, width;
    file >> height >> width;


    if(height <= 0 || width <= 0) {
        throw std::runtime_error("Неверные размеры карты в файле");
    }

    map.resize(height, width);

    for(int i = 0; i < height; ++i) {
        for(int j = 0; j < width; ++j) {
            char cell_char;
            file >> cell_char;

            CellType cell_type;
            switch(cell_char) {
                case '.': cell_type = CellType::EMPTY; break;
                case '#': cell_type = CellType::WALL; break;
                case 'G': cell_type = CellType::GLASS; break;
                case 'P': cell_type = CellType::PARTITION; break;
                case 'S': cell_type = CellType::STORAGE_POINT; break;
                default: cell_type = CellType::EMPTY; break;
            }

            map[i][j] = Cell(cell_type);
        }
    }

    file.close();
}

void Level::save_to_file(const std::string& filepath) const {
    std::ofstream file(filepath);
    if(!file.is_open()) {
        throw std::runtime_error("Не удалось открыть файл для сохранения: " + filepath);
    }

    file << map.get_rows() << " " << map.get_cols() << "\n";

    for(int i = 0; i < map.get_rows(); ++i) {
        for(int j = 0; j < map.get_cols(); ++j) {
            CellType type = map[i][j].get_type();
            char cell_char;

            switch(type) {
                case CellType::EMPTY: cell_char = '.'; break;
                case CellType::WALL: cell_char = '#'; break;
                case CellType::GLASS: cell_char = 'G'; break;
                case CellType::PARTITION: cell_char = 'P'; break;
                case CellType::STORAGE_POINT: cell_char = 'S'; break;
                default: cell_char = '.'; break;
            }

            file << cell_char;
            if(j < map.get_cols() - 1) {
                file << " ";
            }
        }
        file << "\n";
    }

    file.close();
}

Cell& Level::get_cell(int x, int y) {
    if(!is_valid_position(x, y)) {
        throw std::out_of_range("Координаты клетки вне границ карты");
    }
    return map[y][x];
}

const Cell& Level::get_cell(int x, int y) const {
    if(!is_valid_position(x, y)) {
        throw std::out_of_range("Координаты клетки вне границ карты");
    }
    return map[y][x];
}

bool Level::is_valid_position(int x, int y) const {
    return x >= 0 && x < map.get_cols() && y >= 0 && y < map.get_rows();
}

bool Level::add_entity(ent::Entity* entity, int x, int y) {
    if(!entity) return false;
    if(!is_valid_position(x, y)) return false;

    Cell& cell = get_cell(x, y);

    if(!cell.is_passable() || cell.get_entity() != nullptr) {
        return false;
    }

    cell.set_entity(entity);
    entity->set_position(x, y);

    return true;
}

void Level::remove_entity(ent::Entity* entity) {
    if(!entity) return;

    int x = entity->get_x();
    int y = entity->get_y();

    if(is_valid_position(x, y)) {
        Cell& cell = get_cell(x, y);
        if(cell.get_entity() == entity) {
            cell.remove_entity();
        }
    }
}

void Level::move_entity(ent::Entity* entity, int new_x, int new_y) {
    if(!entity) return;
    if(!is_valid_position(new_x, new_y)) return;

    int old_x = entity->get_x();
    int old_y = entity->get_y();

    if(is_valid_position(old_x, old_y)) {
        Cell& old_cell = get_cell(old_x, old_y);
        if(old_cell.get_entity() == entity) {
            old_cell.remove_entity();
        }
    }

    Cell& new_cell = get_cell(new_x, new_y);
    if(new_cell.is_passable()) {
        new_cell.set_entity(entity);
        entity->set_position(new_x, new_y);
    }
}

void Level::add_operative(ent::Operative* op) {
    if(op) {
        operatives.push_back(op);
    }
}

void Level::add_monster(ent::Monster* monster) {
    if(monster) {
        monsters.push_back(monster);
    }
}

void Level::save() const {
    save_to_file("level_save.txt");
}

void Level::load() {
    load_from_file("level_save.txt");
}
