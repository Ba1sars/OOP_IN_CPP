#include "game_engine.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>

GameEngine::GameEngine(Level* game_level)
    : level(game_level)
    , state(GameState::RUNNING)
    , current_turn(0)
    , operatives_turn(true) {
    if(!level) {
        throw std::invalid_argument("Уровень не может быть nullptr");
    }
}

GameEngine::~GameEngine() {
}

void GameEngine::reset_team_time_points(bool operatives) {
    std::lock_guard<std::mutex> lock(game_mutex);

    if(operatives) {
        for(auto* op : level->get_operatives()) {
            if(op && op->is_alive()) {
                op->reset_time_points();
            }
        }
    } else {
        for(auto* monster : level->get_monsters()) {
            if(monster && monster->is_alive()) {
                monster->reset_time_points();
            }
        }
    }
}

void GameEngine::process_operatives_turn() {
    std::lock_guard<std::mutex> lock(game_mutex);

    auto& operatives = level->get_operatives();

    for(auto* op : operatives) {
        if(!op || !op->is_alive()) continue;

        op->update();

        auto& monsters = level->get_monsters();
        for(auto* monster : monsters) {
            if(monster && monster->is_alive() && op->can_see(monster)) {
                int dx = std::abs(monster->get_x() - op->get_x());
                int dy = std::abs(monster->get_y() - op->get_y());
                int distance = std::max(dx, dy);

                if(distance <= op->get_attack_range() && op->get_current_time_points() > 0) {
                    op->attack(monster);
                    break;
                }
            }
        }
    }
}

void GameEngine::process_monsters_turn() {
    std::lock_guard<std::mutex> lock(game_mutex);

    auto& monsters = level->get_monsters();

    for(auto* monster : monsters) {
        if(!monster || !monster->is_alive()) continue;

        monster->update();

        auto& operatives = level->get_operatives();
        ent::Operative* closest_op = nullptr;
        int min_distance = 9999;

        for(auto* op : operatives) {
            if(op && op->is_alive() && monster->can_see(op)) {
                int dx = op->get_x() - monster->get_x();
                int dy = op->get_y() - monster->get_y();
                int distance = std::abs(dx) + std::abs(dy);

                if(distance < min_distance) {
                    min_distance = distance;
                    closest_op = op;
                }
            }
        }

        if(closest_op) {
            int dx = closest_op->get_x() - monster->get_x();
            int dy = closest_op->get_y() - monster->get_y();

            if(std::abs(dx) <= 1 && std::abs(dy) <= 1) {
                ent::IAttacker* attacker = dynamic_cast<ent::IAttacker*>(monster);
                if(attacker) {
                    attacker->attack(closest_op);
                }
            } else {
                int new_x = monster->get_x();
                int new_y = monster->get_y();

                if(std::abs(dx) > std::abs(dy)) {
                    new_x += (dx > 0) ? 1 : -1;
                } else {
                    new_y += (dy > 0) ? 1 : -1;
                }

                if(level->is_valid_position(new_x, new_y)) {
                    Cell& target_cell = level->get_cell(new_x, new_y);
                    if(target_cell.is_passable()) {
                        level->move_entity(monster, new_x, new_y);
                        monster->move(new_x, new_y);
                    }
                }
            }
        }
    }
}

void GameEngine::check_win_conditions() {
    std::lock_guard<std::mutex> lock(game_mutex);

    int alive_operatives = 0;
    for(auto* op : level->get_operatives()) {
        if(op && op->is_alive()) {
            alive_operatives++;
        }
    }

    int alive_monsters = 0;
    for(auto* monster : level->get_monsters()) {
        if(monster && monster->is_alive()) {
            alive_monsters++;
        }
    }

    if(alive_operatives == 0) {
        state = GameState::MONSTERS_WIN;
    } else if(alive_monsters == 0) {
        state = GameState::OPERATIVES_WIN;
    }
}

void GameEngine::tick() {
    if(state != GameState::RUNNING) {
        return;
    }

    reset_team_time_points(operatives_turn);

    std::thread turn_thread;

    if(operatives_turn) {
        turn_thread = std::thread(&GameEngine::process_operatives_turn, this);
    } else {
        turn_thread = std::thread(&GameEngine::process_monsters_turn, this);
    }

    turn_thread.join();

    bool all_out_of_time = true;

    if(operatives_turn) {
        for(auto* op : level->get_operatives()) {
            if(op && op->is_alive() && op->get_current_time_points() > 0) {
                all_out_of_time = false;
                break;
            }
        }
    } else {
        for(auto* monster : level->get_monsters()) {
            if(monster && monster->is_alive() && monster->get_current_time_points() > 0) {
                all_out_of_time = false;
                break;
            }
        }
    }

    if(all_out_of_time) {
        operatives_turn = !operatives_turn;
        current_turn++;
    }

    check_win_conditions();
}

void GameEngine::pause() {
    std::lock_guard<std::mutex> lock(game_mutex);
    if(state == GameState::RUNNING) {
        state = GameState::PAUSED;
    }
}

void GameEngine::resume() {
    std::lock_guard<std::mutex> lock(game_mutex);
    if(state == GameState::PAUSED) {
        state = GameState::RUNNING;
    }
}

void GameEngine::save_game(const std::string& filepath) {
    std::lock_guard<std::mutex> lock(game_mutex);

    std::ofstream file(filepath, std::ios::binary);
    if(!file.is_open()) {
        throw std::runtime_error("Не удалось открыть файл для сохранения: " + filepath);
    }

    file.write(reinterpret_cast<const char*>(&current_turn), sizeof(current_turn));
    file.write(reinterpret_cast<const char*>(&operatives_turn), sizeof(operatives_turn));

    level->save_to_file(filepath + ".map");

    file.close();
}

void GameEngine::load_game(const std::string& filepath) {
    std::lock_guard<std::mutex> lock(game_mutex);

    std::ifstream file(filepath, std::ios::binary);
    if(!file.is_open()) {
        throw std::runtime_error("Не удалось открыть файл для загрузки: " + filepath);
    }

    file.read(reinterpret_cast<char*>(&current_turn), sizeof(current_turn));
    file.read(reinterpret_cast<char*>(&operatives_turn), sizeof(operatives_turn));

    level->load_from_file(filepath + ".map");

    state = GameState::RUNNING;

    file.close();
}
