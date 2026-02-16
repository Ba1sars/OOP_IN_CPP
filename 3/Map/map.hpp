#ifndef MAP_HPP
#define MAP_HPP

#include "../Items/items.hpp"
#include "../Entities/entities.hpp"
#include "matrix.hpp"
#include <vector>
#include <string>
#include <stdexcept>

/**
 * @brief Представляет физический тип ячейки карты.
 */
enum class CellType {
    EMPTY,           ///< Проходима и прозрачна (например, пол).
    WALL,            ///< Непроходима и блокирует видимость/выстрелы.
    GLASS,           ///< Проходима для зрения и выстрелов, но не для движения.
    PARTITION,       ///< Блокирует движение, но может позволять частичную видимость (например, перила).
    STORAGE_POINT    ///< Специальная ячейка, действующая как точка сдачи (например, для собирателей).
};

/**
 * @brief Конфигурация, используемая для инициализации макета карты.
 */
struct MapConfig {
    int width;                    ///< Ширина карты в клетках.
    int height;                   ///< Высота карты в клетках.
    std::string map_file_path;    ///< Опциональный путь к файлу, содержащему сериализованные данные карты.
};

/**
 * @brief Представляет одну клетку на игровой карте.
 * 
 * Ячейка содержит информацию о своем типе, любых предметах на ней и, возможно, сущности.
 */
class Cell {
private:
    CellType type;
    std::vector<itm::IItem*> items;
    ent::Entity* entity;

public:
    /**
     * @brief Конструктор по умолчанию; создает пустую ячейку EMPTY без предметов и сущностей.
     */
    Cell();

    /**
     * @brief Создает ячейку с указанным типом.
     * @param cell_type Исходный тип ячейки.
     */
    explicit Cell(CellType cell_type);

    /**
     * @brief Добавляет предмет на эту ячейку.
     * @param item Указатель на предмет для размещения на ячейке.
     */
    void add_item(itm::IItem* item);

    /**
     * @brief Удаляет конкретный предмет с этой ячейки.
     * @param item Указатель на предмет для удаления.
     */
    void remove_item(itm::IItem* item);

    /**
     * @brief Возвращает копию списка предметов на этой ячейке.
     * @return Вектор указателей на предметы, находящиеся в ячейке в данный момент.
     */
    std::vector<itm::IItem*> get_items() const { return items; }

    /**
     * @brief Удаляет и возвращает все предметы с этой ячейки.
     * Ячейка становится пустой после этой операции.
     * @return Вектор, содержащий все ранее хранившиеся предметы.
     */
    std::vector<itm::IItem*> take_all_items();

    /**
     * @brief Связывает сущность с этой ячейкой.
     * @param ent Указатель на сущность для размещения на ячейке (может быть nullptr).
     */
    void set_entity(ent::Entity* ent) { entity = ent; }

    /**
     * @brief Получает сущность, в данный момент занимающую эту ячейку.
     * @return Указатель на сущность, или nullptr если ее нет.
     */
    ent::Entity* get_entity() const { return entity; }

    /**
     * @brief Удаляет сущность с этой ячейки (устанавливает в nullptr).
     */
    void remove_entity() { entity = nullptr; }

    /**
     * @brief Проверяет, может ли сущность войти в эту ячейку.
     * @return true если тип ячейки позволяет движение (например, EMPTY).
     */
    bool is_passable() const;

    /**
     * @brief Проверяет, позволяет ли эта ячейка прямую видимость (зрение).
     * @return true если свет/зрение может проходить (например, EMPTY, GLASS).
     */
    bool is_transparent() const;

    /**
     * @brief Проверяет, могут ли снаряды или пули проходить через эту ячейку.
     * @return true если ячейка не блокирует дальние атаки.
     */
    bool is_shootable() const;

    /**
     * @brief Получает физический тип этой ячейки.
     * @return Текущее значение CellType.
     */
    CellType get_type() const { return type; }

    /**
     * @brief Изменяет физический тип этой ячейки.
     * @param new_type Новый CellType для назначения.
     */
    void set_type(CellType new_type) { type = new_type; }
};

/**
 * @brief Представляет полный игровой уровень с сеткой-картой и коллекциями сущностей.
 */
class Level {
private:
    Matrix<Cell> map;  ///< 2D сетка ячеек, представляющая игровой мир.
    std::vector<ent::Operative*> operatives;  ///< Список управляемых игроком юнитов.
    std::vector<ent::Monster*> monsters;      ///< Список управляемых ИИ врагов.

public:
    /**
     * @brief Создает уровень с заданным размером карты.
     * @param width Ширина карты в клетках.
     * @param height Высота карты в клетках.
     */
    Level(int width, int height);

    /**
     * @brief Деструктор; не удаляет сущности или предметы (контейнер без владения).
     */
    ~Level();

    /**
     * @brief Инициализирует карту, используя структуру конфигурации.
     * Может загружать из файла или генерировать процедурно, в зависимости от конфигурации.
     * @param config Конфигурация карты, включая размер и опциональный путь к файлу.
     */
    void build_map(const MapConfig& config);

    /**
     * @brief Загружает макет карты и размещение объектов из файла.
     * @param filepath Путь к файлу данных карты.
     */
    void load_from_file(const std::string& filepath);

    /**
     * @brief Сохраняет текущее состояние карты в файл.
     * @param filepath Путь к выходному файлу.
     */
    void save_to_file(const std::string& filepath) const;

    /**
     * @brief Возвращает ссылку на ячейку в (x, y) для модификации.
     * @param x X-координата (столбец).
     * @param y Y-координата (строка).
     * @return Ссылка на объект Cell.
     * @throws std::out_of_range если координаты недействительны.
     */
    Cell& get_cell(int x, int y);

    /**
     * @brief Возвращает константную ссылку на ячейку в (x, y).
     * @param x X-координата (столбец).
     * @param y Y-координата (строка).
     * @return Константная ссылка на объект Cell.
     * @throws std::out_of_range если координаты недействительны.
     */
    const Cell& get_cell(int x, int y) const;

    /**
     * @brief Проверяет, находятся ли заданные координаты в пределах границ карты.
     * @param x X-координата для проверки.
     * @param y Y-координата для проверки.
     * @return true если (x, y) является действительной позицией на карте.
     */
    bool is_valid_position(int x, int y) const;

    /**
     * @brief Размещает сущность на карте в указанной позиции.
     * Также устанавливает внутреннюю позицию сущности.
     * @param entity Указатель на сущность для размещения.
     * @param x Целевая X-координата.
     * @param y Целевая Y-координата.
     * @return true если размещение успешно (позиция действительна и ячейка проходима).
     */
    bool add_entity(ent::Entity* entity, int x, int y);

    /**
     * @brief Удаляет сущность с ее текущей ячейки (не уничтожает сущность).
     * @param entity Указатель на сущность для удаления.
     */
    void remove_entity(ent::Entity* entity);

    /**
     * @brief Перемещает сущность с ее текущей позиции на новую.
     * Обновляет как сетку карты, так и внутренние координаты сущности.
     * @param entity Указатель на сущность для перемещения.
     * @param new_x Новая X-координата.
     * @param new_y Новая Y-координата.
     */
    void move_entity(ent::Entity* entity, int new_x, int new_y);

    /**
     * @brief Добавляет оперативника в список оперативников уровня.
     * Не размещает его на карте — используйте add_entity() для этого.
     * @param op Указатель на оперативника.
     */
    void add_operative(ent::Operative* op);

    /**
     * @brief Добавляет монстра в список монстров уровня.
     * Не размещает его на карте — используйте add_entity() для этого.
     * @param monster Указатель на монстра.
     */
    void add_monster(ent::Monster* monster);

    /**
     * @brief Возвращает ссылку на список оперативников на этом уровне.
     * @return Ссылка на вектор указателей на Operative.
     */
    std::vector<ent::Operative*>& get_operatives() { return operatives; }

    /**
     * @brief Возвращает ссылку на список монстров на этом уровне.
     * @return Ссылка на вектор указателей на Monster.
     */
    std::vector<ent::Monster*>& get_monsters() { return monsters; }

    /**
     * @brief Получает ширину карты (количество столбцов).
     * @return Ширина карты в клетках.
     */
    int get_width() const { return map.get_cols(); }

    /**
     * @brief Получает высоту карты (количество строк).
     * @return Высота карты в клетках.
     */
    int get_height() const { return map.get_rows(); }

    /**
     * @brief Сохраняет все состояние уровня (карта + сущности) с использованием внутренней логики.
     */
    void save() const;

    /**
     * @brief Загружает все состояние уровня (карта + сущности) с использованием внутренней логики.
     */
    void load();
};

#endif // MAP_HPP
