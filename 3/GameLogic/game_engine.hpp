#ifndef GAME_ENGINE_HPP
#define GAME_ENGINE_HPP

#include "../Map/map.hpp"
#include "../Entities/entities.hpp"
#include <thread>
#include <mutex>
#include <vector>
#include <functional>
#include <string>

/**
 * @brief Представляет текущее состояние игры.
 */
enum class GameState {
    RUNNING,           ///< Игра в процессе.
    OPERATIVES_WIN,    ///< Оперативники выполнили условия победы.
    MONSTERS_WIN,      ///< Монстры выполнили условия победы.
    PAUSED             ///< Игра на паузе.
};

/**
 * @brief Основной координатор игровой логики, управляющий ходами, состоянием и условиями победы.
 */
class GameEngine {
private:
    Level* level;
    GameState state;
    int current_turn;
    bool operatives_turn;
    std::mutex game_mutex;

    /**
     * @brief Обрабатывает ход оперативников в отдельном потоке.
     * Выполняет действия для всех оперативников, пока у них не закончатся очки времени.
     */
    void process_operatives_turn();

    /**
     * @brief Обрабатывает ход монстров в отдельном потоке.
     * Выполняет решения ИИ и действия для всех монстров, пока у них не закончатся очки времени.
     */
    void process_monsters_turn();

    /**
     * @brief Проверяет условия победы/поражения после каждого хода.
     * Обновляет состояние игры, если какая-либо команда победила.
     */
    void check_win_conditions();

    /**
     * @brief Сбрасывает очки времени для всех сущностей заданной команды.
     * @param operatives Если true, сбрасывает очки оперативников; иначе сбрасывает очки монстров.
     */
    void reset_team_time_points(bool operatives);

public:
    /**
     * @brief Создает экземпляр GameEngine с заданным уровнем.
     * @param game_level Указатель на данные уровня (карта, сущности и т.д.).
     */
    GameEngine(Level* game_level);

    /**
     * @brief Деструктор по умолчанию.
     */
    ~GameEngine();

    /**
     * @brief Продвигает игру на один логический такт (ход).
     * Запускает параллельную обработку для оперативников и монстров, если применимо.
     * Обрабатывает смену ходов и проверку условий победы.
     */
    void tick();

    /**
     * @brief Получает текущее состояние игры.
     * @return Текущее значение перечисления GameState.
     */
    GameState get_state() const { return state; }

    /**
     * @brief Получает текущий номер хода.
     * @return Счетчик ходов (начиная с 0 или 1, в зависимости от реализации).
     */
    int get_turn() const { return current_turn; }

    /**
     * @brief Проверяет, является ли текущий ход ходом оперативников.
     * @return true если действуют оперативники; false если ход монстров.
     */
    bool is_operatives_turn() const { return operatives_turn; }

    /**
     * @brief Ставит игру на паузу (устанавливает состояние в PAUSED).
     */
    void pause();

    /**
     * @brief Возобновляет игру (возвращает состояние в RUNNING, если условия позволяют).
     */
    void resume();

    /**
     * @brief Сохраняет текущее состояние игры в файл.
     * @param filepath Путь к файлу сохранения.
     */
    void save_game(const std::string& filepath);

    /**
     * @brief Загружает сохраненное состояние игры из файл.
     * @param filepath Путь к файлу сохранения.
     */
    void load_game(const std::string& filepath);
};

#endif // GAME_ENGINE_HPP
