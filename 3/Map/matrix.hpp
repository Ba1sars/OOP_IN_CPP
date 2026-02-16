/**
 * @file matrix.hpp
 * @brief Шаблонный контейнерный класс Matrix для двумерных массивов
 * 
 * Реализует полноценный контейнер с поддержкой:
 * - Container: базовые операции (begin, end, size, empty, swap)
 * - ReversibleContainer: обратные итераторы (rbegin, rend)
 * - ContiguousContainer: непрерывное хранение в памяти, data()
 * - RandomAccess: произвольный доступ за O(1) через operator[i][j]
 */

#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <stdexcept>
#include <algorithm>
#include <iterator>
#include <memory>
#include <initializer_list>

template<typename T>
class Matrix;

template<typename T>
class MatrixRow;

/**
 *
 * @brief Итератор с произвольным доступом для Matrix
 * 
 * Обходит матрицу построчно (row-major order): [0][0], [0][1], ..., [1][0], ...
 * Поддерживает все операции RandomAccessIterator из STL
 */

template<typename T>
class MatrixIterator {
public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

private:
    pointer ptr_; 
public:
    MatrixIterator() : ptr_(nullptr) {}
    explicit MatrixIterator(pointer p) : ptr_(p) {}
    
    MatrixIterator(const MatrixIterator&) = default;
    MatrixIterator& operator=(const MatrixIterator&) = default;
    
    reference operator*() const { return *ptr_; }
    pointer operator->() const { return ptr_; }
    reference operator[](difference_type n) const { return ptr_[n]; }
    
    MatrixIterator& operator++() {
        ++ptr_;
        return *this;
    }
    
    MatrixIterator operator++(int) {
        MatrixIterator temp = *this;
        ++ptr_;
        return temp;
    }
    
    MatrixIterator& operator--() {
        --ptr_;
        return *this;
    }
    
    MatrixIterator operator--(int) {
        MatrixIterator temp = *this;
        --ptr_;
        return temp;
    }
    
    MatrixIterator& operator+=(difference_type n) {
        ptr_ += n;
        return *this;
    }
    
    MatrixIterator& operator-=(difference_type n) {
        ptr_ -= n;
        return *this;
    }
    
    MatrixIterator operator+(difference_type n) const {
        return MatrixIterator(ptr_ + n);
    }
    
    MatrixIterator operator-(difference_type n) const {
        return MatrixIterator(ptr_ - n);
    }
    
    difference_type operator-(const MatrixIterator& other) const {
        return ptr_ - other.ptr_;
    }
    
    bool operator==(const MatrixIterator& other) const { return ptr_ == other.ptr_; }
    bool operator!=(const MatrixIterator& other) const { return ptr_ != other.ptr_; }
    bool operator<(const MatrixIterator& other) const { return ptr_ < other.ptr_; }
    bool operator>(const MatrixIterator& other) const { return ptr_ > other.ptr_; }
    bool operator<=(const MatrixIterator& other) const { return ptr_ <= other.ptr_; }
    bool operator>=(const MatrixIterator& other) const { return ptr_ >= other.ptr_; }
};

template<typename T>
MatrixIterator<T> operator+(typename MatrixIterator<T>::difference_type n,
                            const MatrixIterator<T>& it) {
    return it + n;
}

/**
 * @brief Прокси-класс для реализации синтаксиса matrix[i][j]
 * 
 * Возвращается оператором [] матрицы и предоставляет второй []
 * для доступа к конкретной ячейке строки
 */

template<typename T>
class MatrixRow {
private:
    T* row_ptr_;
    int cols_;
   
    template<typename U>
    friend class Matrix;
    
    MatrixRow(T* ptr, int cols) : row_ptr_(ptr), cols_(cols) {}

public:
    T& operator[](int j) {
        if (j < 0 || j >= cols_) {
            throw std::out_of_range("Matrix: column index out of range");
        }
        return row_ptr_[j];
    }
    
    const T& operator[](int j) const {
        if (j < 0 || j >= cols_) {
            throw std::out_of_range("Matrix: column index out of range");
        }
        return row_ptr_[j];
    }
};

/**
 * @brief Шаблонный контейнер для двумерных матриц
 * 
 * Особенности реализации:
 * - Хранение в виде одномерного массива (row-major order) для непрерывности памяти
 * - Поддержка синтаксиса matrix[i][j] через прокси-класс MatrixRow
 * - Совместимость с STL (итераторы, алгоритмы)
 * - RAII для автоматического управления памятью
 * - Move-семантика для эффективного перемещения
 * 
 * @tparam T Тип элементов матрицы
 */

template<typename T>
class Matrix {
public:
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    
    using iterator = MatrixIterator<T>;
    using const_iterator = MatrixIterator<const T>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

private:
    T* data_;     // Указатель на одномерный массив данных
    int rows_;    // Количество строк
    int cols_;    // Количество столбцов
    
    /**
     * @brief Выделяет память для матрицы
     */

    void allocate() {
        if (rows_ > 0 && cols_ > 0) {
            data_ = new T[rows_ * cols_];
        } else {
            data_ = nullptr;
        }
    }
    
    /**
     * @brief Освобождает память
     */
    void deallocate() {
        delete[] data_;
        data_ = nullptr;
    }
    
    /**
     * @brief Вычисляет линейный индекс из двумерных координат
     */
    inline int linear_index(int i, int j) const {
        return i * cols_ + j;
    }

public:
     /**
     * @brief Конструктор по умолчанию - создаёт пустую матрицу 0x0
     */
    Matrix() : data_(nullptr), rows_(0), cols_(0) {}
    
    /**
     * @brief Конструктор с размерами - создаёт матрицу rows x cols
     * 
     * Элементы инициализируются значением по умолчанию T()
     * 
     * @param rows Количество строк
     * @param cols Количество столбцов
     * @throws std::invalid_argument если размеры отрицательные
     */
    Matrix(int rows, int cols) : data_(nullptr), rows_(rows), cols_(cols) {
        if (rows < 0 || cols < 0) {
            throw std::invalid_argument("Matrix: dimensions must be non-negative");
        }
        allocate();
        std::fill_n(data_, rows_ * cols_, T());
    }
    
    /**
     * @brief Конструктор с размерами и начальным значением
     * 
     * Все элементы инициализируются заданным значением
     * 
     * @param rows Количество строк
     * @param cols Количество столбцов
     * @param value Значение для инициализации
     * @throws std::invalid_argument если размеры отрицательные
     */
    Matrix(int rows, int cols, const T& value) 
        : data_(nullptr), rows_(rows), cols_(cols) {
        if (rows < 0 || cols < 0) {
            throw std::invalid_argument("Matrix: dimensions must be non-negative");
        }
        allocate();
        std::fill_n(data_, rows_ * cols_, value);
    }
    
    /**
     * @brief Конструктор копирования
     * 
     * Создаёт глубокую копию матрицы
     */
    Matrix(const Matrix& other) 
        : data_(nullptr), rows_(other.rows_), cols_(other.cols_) {
        allocate();
        std::copy(other.data_, other.data_ + rows_ * cols_, data_);
    }
    
    /**
     * @brief Конструктор перемещения (move constructor)
     * 
     * Забирает ресурсы у временного объекта без копирования
     */
    Matrix(Matrix&& other) noexcept
        : data_(other.data_), rows_(other.rows_), cols_(other.cols_) {
        other.data_ = nullptr;
        other.rows_ = 0;
        other.cols_ = 0;
    }
    
    /**
     * @brief Деструктор
     * 
     * Автоматически освобождает выделенную память (RAII)
     */
    ~Matrix() {
        deallocate();
    }
    
     /**
     * @brief Оператор копирующего присваивания
     */
    Matrix& operator=(const Matrix& other) {
        if (this != &other) {
            deallocate();
            rows_ = other.rows_;
            cols_ = other.cols_;
            allocate();
            std::copy(other.data_, other.data_ + rows_ * cols_, data_);
        }
        return *this;
    }
    
    /**
     * @brief Оператор перемещающего присваивания
     */
    Matrix& operator=(Matrix&& other) noexcept {
        if (this != &other) {
            deallocate();
            data_ = other.data_;
            rows_ = other.rows_;
            cols_ = other.cols_;
            other.data_ = nullptr;
            other.rows_ = 0;
            other.cols_ = 0;
        }
        return *this;
    }
    
    iterator begin() { return iterator(data_); }
    iterator end() { return iterator(data_ + rows_ * cols_); }
    const_iterator begin() const { return const_iterator(data_); }
    const_iterator end() const { return const_iterator(data_ + rows_ * cols_); }
    const_iterator cbegin() const { return const_iterator(data_); }
    const_iterator cend() const { return const_iterator(data_ + rows_ * cols_); }
    
    reverse_iterator rbegin() { return reverse_iterator(end()); }
    reverse_iterator rend() { return reverse_iterator(begin()); }
    const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
    const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }
    const_reverse_iterator crbegin() const { return const_reverse_iterator(end()); }
    const_reverse_iterator crend() const { return const_reverse_iterator(begin()); }
    
    /**
     * @brief Доступ к строке через operator[][] (matrix[i][j])
     * 
     * Возвращает прокси-объект MatrixRow, который предоставляет второй []
     * 
     * @param i Индекс строки
     * @return MatrixRow для доступа к элементам строки
     * @throws std::out_of_range если индекс строки выходит за границы
     */
    MatrixRow<T> operator[](int i) {
        if (i < 0 || i >= rows_) {
            throw std::out_of_range("Matrix: row index out of range");
        }
        return MatrixRow<T>(data_ + i * cols_, cols_);
    }
    
    const MatrixRow<const T> operator[](int i) const {
        if (i < 0 || i >= rows_) {
            throw std::out_of_range("Matrix: row index out of range");
        }
        return MatrixRow<const T>(data_ + i * cols_, cols_);
    }
    
    /**
     * @brief Безопасный доступ к элементу через get(i, j)
     * 
     * Альтернатива operator[][], с явной проверкой границ
     * 
     * @throws std::out_of_range если индексы выходят за границы
     */
    T& get(int i, int j) {
        if (i < 0 || i >= rows_ || j < 0 || j >= cols_) {
            throw std::out_of_range("Matrix: indices out of range");
        }
        return data_[linear_index(i, j)];
    }
    
    const T& get(int i, int j) const {
        if (i < 0 || i >= rows_ || j < 0 || j >= cols_) {
            throw std::out_of_range("Matrix: indices out of range");
        }
        return data_[linear_index(i, j)];
    }
    
    /**
     * @brief Установка значения элемента
     */
    void set(int i, int j, const T& value) {
        if (i < 0 || i >= rows_ || j < 0 || j >= cols_) {
            throw std::out_of_range("Matrix: indices out of range");
        }
        data_[linear_index(i, j)] = value;
    }
    
    /**
     * @brief Прямой доступ к данным (ContiguousContainer)
     * 
     * Возвращает указатель на начало массива данных
     * Полезно для взаимодействия с C API или оптимизаций
     */
    T* data() { return data_; }
    const T* data() const { return data_; }
    
    /**
     * @brief Количество строк матрицы
     */
    int get_rows() const { return rows_; }
    
    /**
     * @brief Количество столбцов матрицы
     */
    int get_cols() const { return cols_; }
    
    /**
     * @brief Общее количество элементов (rows * cols)
     */
    size_type size() const { return static_cast<size_type>(rows_ * cols_); }
    
    /**
     * @brief Проверка на пустоту
     */
    bool empty() const { return rows_ == 0 || cols_ == 0; }
    
    /**
     * @brief Изменяет размер матрицы
     * 
     * Сохраняет существующие данные в пределах новых размеров.
     * Новые элементы инициализируются значением по умолчанию T().
     * 
     * @param new_rows Новое количество строк
     * @param new_cols Новое количество столбцов
     * @throws std::invalid_argument если размеры отрицательные
     */
    void resize(int new_rows, int new_cols) {
        if (new_rows < 0 || new_cols < 0) {
            throw std::invalid_argument("Matrix: dimensions must be non-negative");
        }
        
        if (new_rows == rows_ && new_cols == cols_) {
            return;
        }
        
        T* new_data = nullptr;
        if (new_rows > 0 && new_cols > 0) {
            new_data = new T[new_rows * new_cols];
            
            std::fill_n(new_data, new_rows * new_cols, T());
            
            int copy_rows = std::min(rows_, new_rows);
            int copy_cols = std::min(cols_, new_cols);
            
            for (int i = 0; i < copy_rows; ++i) {
                std::copy_n(data_ + i * cols_, copy_cols, new_data + i * new_cols);
            }
        }
        
        deallocate();
        data_ = new_data;
        rows_ = new_rows;
        cols_ = new_cols;
    }
    
    /**
     * @brief Изменяет размер с заполнением новых элементов заданным значением
     */
    void resize(int new_rows, int new_cols, const T& value) {
        int old_rows = rows_;
        int old_cols = cols_;
        resize(new_rows, new_cols);
        
        for (int i = 0; i < new_rows; ++i) {
            for (int j = 0; j < new_cols; ++j) {
                if (i >= old_rows || j >= old_cols) {
                    data_[linear_index(i, j)] = value;
                }
            }
        }
    }
    
    /**
     * @brief Очищает матрицу, делая её пустой (0x0)
     */
    void clear() {
        deallocate();
        rows_ = 0;
        cols_ = 0;
    }
    
    /**
     * @brief Обменивает содержимое с другой матрицей за O(1)
     */
    void swap(Matrix& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(rows_, other.rows_);
        std::swap(cols_, other.cols_);
    }
    
    /**
     * @brief Заполняет всю матрицу заданным значением
     */
    void fill(const T& value) {
        std::fill_n(data_, rows_ * cols_, value);
    }
    
    /**
     * @brief Проверяет валидность координат
     */
    bool is_valid_position(int i, int j) const {
        return i >= 0 && i < rows_ && j >= 0 && j < cols_;
    }
    
    /**
     * @brief Транспонирование матрицы
     * 
     * Возвращает новую матрицу - транспонированную копию
     */
    Matrix<T> transpose() const {
        Matrix<T> result(cols_, rows_);
        for (int i = 0; i < rows_; ++i) {
            for (int j = 0; j < cols_; ++j) {
                result.data_[j * rows_ + i] = data_[i * cols_ + j];
            }
        }
        return result;
    }
};


/**
 * @brief Внешняя функция swap для ADL (Argument Dependent Lookup)
 */
template<typename T>
void swap(Matrix<T>& lhs, Matrix<T>& rhs) noexcept {
    lhs.swap(rhs);
}

/**
 * @brief Оператор сравнения на равенство
 */
template<typename T>
bool operator==(const Matrix<T>& lhs, const Matrix<T>& rhs) {
    if (lhs.get_rows() != rhs.get_rows() || lhs.get_cols() != rhs.get_cols()) {
        return false;
    }
    
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

/**
 * @brief Оператор сравнения на неравенство
 */
template<typename T>
bool operator!=(const Matrix<T>& lhs, const Matrix<T>& rhs) {
    return !(lhs == rhs);
}

#endif // MATRIX_HPP
