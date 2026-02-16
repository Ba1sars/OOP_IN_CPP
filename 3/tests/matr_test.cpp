#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include "../Map/matrix.hpp"
#include <numeric>

#include <vector>
#include <algorithm>
#include <sstream>

static_assert(std::random_access_iterator<Matrix<int>::iterator>);
static_assert(std::random_access_iterator<Matrix<int>::const_iterator>);

TEST_CASE("Matrix: Default constructor", "[constructor]") {
    Matrix<int> m;
    
    REQUIRE(m.get_rows() == 0);
    REQUIRE(m.get_cols() == 0);
    REQUIRE(m.size() == 0);
    REQUIRE(m.empty() == true);
    REQUIRE(m.data() == nullptr);
}

TEST_CASE("Matrix: Constructor with dimensions (valid)", "[constructor]") {
    Matrix<int> m(3, 4);
    
    REQUIRE(m.get_rows() == 3);
    REQUIRE(m.get_cols() == 4);
    REQUIRE(m.size() == 12);
    REQUIRE(m.empty() == false);
    REQUIRE(m.data() != nullptr);
    
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 4; ++j) {
            REQUIRE(m[i][j] == 0);
        }
    }
}

TEST_CASE("Matrix: Constructor with dimensions (invalid - negative rows)", "[constructor][exception]") {
    REQUIRE_THROWS_AS(Matrix<int>(-1, 5), std::invalid_argument);
}

TEST_CASE("Matrix: Constructor with dimensions (invalid - negative cols)", "[constructor][exception]") {
    REQUIRE_THROWS_AS(Matrix<int>(5, -1), std::invalid_argument);
}

TEST_CASE("Matrix: Constructor with dimensions (edge case - zero size)", "[constructor]") {
    Matrix<int> m1(0, 5);
    REQUIRE(m1.empty());
    REQUIRE(m1.size() == 0);
    
    Matrix<int> m2(5, 0);
    REQUIRE(m2.empty());
    REQUIRE(m2.size() == 0);
}

TEST_CASE("Matrix: Constructor with value", "[constructor]") {
    Matrix<int> m(2, 3, 42);
    
    REQUIRE(m.get_rows() == 2);
    REQUIRE(m.get_cols() == 3);
    
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 3; ++j) {
            REQUIRE(m[i][j] == 42);
        }
    }
}

TEST_CASE("Matrix: Constructor with value (invalid dimensions)", "[constructor][exception]") {
    REQUIRE_THROWS_AS(Matrix<int>(-1, 3, 10), std::invalid_argument);
}

TEST_CASE("Matrix: Copy constructor", "[constructor]") {
    Matrix<int> m1(2, 3, 5);
    m1[0][0] = 10;
    m1[1][2] = 20;
    
    Matrix<int> m2(m1);
    
    REQUIRE(m2.get_rows() == 2);
    REQUIRE(m2.get_cols() == 3);
    REQUIRE(m2[0][0] == 10);
    REQUIRE(m2[1][2] == 20);
    REQUIRE(m2[0][1] == 5);
    
    m2[0][0] = 100;
    REQUIRE(m1[0][0] == 10);
    REQUIRE(m2[0][0] == 100);
}

TEST_CASE("Matrix: Move constructor", "[constructor]") {
    Matrix<int> m1(2, 3, 7);
    int* original_data = m1.data();
    
    Matrix<int> m2(std::move(m1));
    
    REQUIRE(m2.get_rows() == 2);
    REQUIRE(m2.get_cols() == 3);
    REQUIRE(m2[0][0] == 7);
    REQUIRE(m2.data() == original_data);      
    REQUIRE(m1.get_rows() == 0);
    REQUIRE(m1.get_cols() == 0);
    REQUIRE(m1.data() == nullptr);
}

TEST_CASE("Matrix: Copy assignment", "[assignment]") {
    Matrix<int> m1(2, 3, 5);
    m1[0][0] = 10;
    
    Matrix<int> m2(1, 1, 0);
    m2 = m1;
    
    REQUIRE(m2.get_rows() == 2);
    REQUIRE(m2.get_cols() == 3);
    REQUIRE(m2[0][0] == 10);
    
    m2[0][0] = 100;
    REQUIRE(m1[0][0] == 10);
}

TEST_CASE("Matrix: Copy assignment (self-assignment)", "[assignment]") {
    Matrix<int> m(2, 2, 5);
    m[0][0] = 10;
    
    m = m;      
    REQUIRE(m.get_rows() == 2);
    REQUIRE(m.get_cols() == 2);
    REQUIRE(m[0][0] == 10);
}

TEST_CASE("Matrix: Move assignment", "[assignment]") {
    Matrix<int> m1(2, 3, 7);
    int* original_data = m1.data();
    
    Matrix<int> m2(1, 1, 0);
    m2 = std::move(m1);
    
    REQUIRE(m2.get_rows() == 2);
    REQUIRE(m2.get_cols() == 3);
    REQUIRE(m2[0][0] == 7);
    REQUIRE(m2.data() == original_data);
    
    REQUIRE(m1.empty());
}


TEST_CASE("Matrix: operator[][] - read (valid)", "[access]") {
    Matrix<int> m(3, 3, 0);
    m[1][2] = 42;
    
    REQUIRE(m[1][2] == 42);
}

TEST_CASE("Matrix: operator[][] - write (valid)", "[access]") {
    Matrix<int> m(3, 3, 0);
    
    m[0][0] = 1;
    m[1][1] = 2;
    m[2][2] = 3;
    
    REQUIRE(m[0][0] == 1);
    REQUIRE(m[1][1] == 2);
    REQUIRE(m[2][2] == 3);
}

TEST_CASE("Matrix: operator[][] - invalid row index", "[access][exception]") {
    Matrix<int> m(3, 3);
    
    REQUIRE_THROWS_AS(m[3][0], std::out_of_range);
    REQUIRE_THROWS_AS(m[-1][0], std::out_of_range);
}

TEST_CASE("Matrix: operator[][] - invalid column index", "[access][exception]") {
    Matrix<int> m(3, 3);
    
    REQUIRE_THROWS_AS(m[0][3], std::out_of_range);
    REQUIRE_THROWS_AS(m[0][-1], std::out_of_range);
}

TEST_CASE("Matrix: operator[][] - const version", "[access]") {
    const Matrix<int> m(2, 2, 10);
    
    REQUIRE(m[0][0] == 10);
    REQUIRE(m[1][1] == 10);
}


TEST_CASE("Matrix: get() - valid access", "[access]") {
    Matrix<int> m(3, 3, 0);
    m.set(1, 2, 99);
    
    REQUIRE(m.get(1, 2) == 99);
}

TEST_CASE("Matrix: get() - invalid row index", "[access][exception]") {
    Matrix<int> m(3, 3);
    
    REQUIRE_THROWS_AS(m.get(-1, 0), std::out_of_range);
    REQUIRE_THROWS_AS(m.get(3, 0), std::out_of_range);
}

TEST_CASE("Matrix: get() - invalid column index", "[access][exception]") {
    Matrix<int> m(3, 3);
    
    REQUIRE_THROWS_AS(m.get(0, -1), std::out_of_range);
    REQUIRE_THROWS_AS(m.get(0, 3), std::out_of_range);
}

TEST_CASE("Matrix: get() - both indices invalid", "[access][exception]") {
    Matrix<int> m(3, 3);
    
    REQUIRE_THROWS_AS(m.get(-1, -1), std::out_of_range);
    REQUIRE_THROWS_AS(m.get(10, 10), std::out_of_range);
}

TEST_CASE("Matrix: get() - const version", "[access]") {
    Matrix<int> m(2, 2, 5);
    const Matrix<int>& cm = m;
    
    REQUIRE(cm.get(0, 0) == 5);
    REQUIRE(cm.get(1, 1) == 5);
}

TEST_CASE("Matrix: set() - valid", "[access]") {
    Matrix<int> m(3, 3, 0);
    
    m.set(0, 0, 10);
    m.set(1, 2, 20);
    m.set(2, 1, 30);
    
    REQUIRE(m[0][0] == 10);
    REQUIRE(m[1][2] == 20);
    REQUIRE(m[2][1] == 30);
}

TEST_CASE("Matrix: set() - invalid row index", "[access][exception]") {
    Matrix<int> m(3, 3);
    
    REQUIRE_THROWS_AS(m.set(-1, 0, 5), std::out_of_range);
    REQUIRE_THROWS_AS(m.set(3, 0, 5), std::out_of_range);
}

TEST_CASE("Matrix: set() - invalid column index", "[access][exception]") {
    Matrix<int> m(3, 3);
    
    REQUIRE_THROWS_AS(m.set(0, -1, 5), std::out_of_range);
    REQUIRE_THROWS_AS(m.set(0, 3, 5), std::out_of_range);
}

TEST_CASE("Matrix: set() - both indices invalid", "[access][exception]") {
    Matrix<int> m(3, 3);
    
    REQUIRE_THROWS_AS(m.set(10, 10, 5), std::out_of_range);
}


TEST_CASE("Matrix: data() - access to raw pointer", "[access]") {
    Matrix<int> m(2, 3, 0);
    m[0][0] = 1;
    m[0][1] = 2;
    m[0][2] = 3;
    m[1][0] = 4;
    m[1][1] = 5;
    m[1][2] = 6;
    
    int* ptr = m.data();
    REQUIRE(ptr != nullptr);
    
    REQUIRE(ptr[0] == 1);
    REQUIRE(ptr[1] == 2);
    REQUIRE(ptr[2] == 3);
    REQUIRE(ptr[3] == 4);
    REQUIRE(ptr[4] == 5);
    REQUIRE(ptr[5] == 6);
}

TEST_CASE("Matrix: data() - modify through pointer", "[access]") {
    Matrix<int> m(2, 2, 0);
    
    int* ptr = m.data();
    ptr[0] = 10;
    ptr[3] = 40;
    
    REQUIRE(m[0][0] == 10);
    REQUIRE(m[1][1] == 40);
}

TEST_CASE("Matrix: data() - const version", "[access]") {
    Matrix<int> m(2, 2, 7);
    const Matrix<int>& cm = m;
    
    const int* ptr = cm.data();
    REQUIRE(ptr[0] == 7);
}

TEST_CASE("Matrix: data() - empty matrix", "[access]") {
    Matrix<int> m;
    REQUIRE(m.data() == nullptr);
}

TEST_CASE("Matrix: Iterators - begin/end", "[iterator]") {
    Matrix<int> m(2, 3, 0);
    m[0][0] = 1;
    m[0][1] = 2;
    m[0][2] = 3;
    m[1][0] = 4;
    m[1][1] = 5;
    m[1][2] = 6;
    
    std::vector<int> values;
    for (auto it = m.begin(); it != m.end(); ++it) {
        values.push_back(*it);
    }
    
    REQUIRE(values == std::vector<int>{1, 2, 3, 4, 5, 6});
}

TEST_CASE("Matrix: Iterators - range-based for loop", "[iterator]") {
    Matrix<int> m(2, 2, 5);
    
    int count = 0;
    for (int val : m) {
        REQUIRE(val == 5);
        ++count;
    }
    REQUIRE(count == 4);
}

TEST_CASE("Matrix: Iterators - modification", "[iterator]") {
    Matrix<int> m(2, 2, 0);
    
    int value = 1;
    for (auto it = m.begin(); it != m.end(); ++it) {
        *it = value++;
    }
    
    REQUIRE(m[0][0] == 1);
    REQUIRE(m[0][1] == 2);
    REQUIRE(m[1][0] == 3);
    REQUIRE(m[1][1] == 4);
}

TEST_CASE("Matrix: Iterators - const begin/end", "[iterator]") {
    const Matrix<int> m(2, 2, 10);
    
    std::vector<int> values;
    for (auto it = m.begin(); it != m.end(); ++it) {
        values.push_back(*it);
    }
    
    REQUIRE(values.size() == 4);
    REQUIRE(values == std::vector<int>{10, 10, 10, 10});
}

TEST_CASE("Matrix: Iterators - cbegin/cend", "[iterator]") {
    Matrix<int> m(1, 3, 7);
    
    std::vector<int> values;
    for (auto it = m.cbegin(); it != m.cend(); ++it) {
        values.push_back(*it);
    }
    
    REQUIRE(values == std::vector<int>{7, 7, 7});
}

TEST_CASE("Matrix: Iterators - empty matrix", "[iterator]") {
    Matrix<int> m;
    REQUIRE(m.begin() == m.end());
}

TEST_CASE("Matrix: Reverse iterators - rbegin/rend", "[iterator]") {
    Matrix<int> m(2, 3, 0);
    m[0][0] = 1;
    m[0][1] = 2;
    m[0][2] = 3;
    m[1][0] = 4;
    m[1][1] = 5;
    m[1][2] = 6;
    
    std::vector<int> values;
    for (auto it = m.rbegin(); it != m.rend(); ++it) {
        values.push_back(*it);
    }
    
    REQUIRE(values == std::vector<int>{6, 5, 4, 3, 2, 1});
}

TEST_CASE("Matrix: Reverse iterators - const", "[iterator]") {
    Matrix<int> m(1, 3);
    m[0][0] = 1;
    m[0][1] = 2;
    m[0][2] = 3;

    std::vector<int> values;
    for (auto it = m.rbegin(); it != m.rend(); ++it) {
        values.push_back(*it);
    }
    
    REQUIRE(values == std::vector<int>{3, 2, 1});
}

TEST_CASE("Matrix: Reverse iterators - crbegin/crend", "[iterator]") {
    Matrix<int> m(1, 2, 0);
    m[0][0] = 10;
    m[0][1] = 20;
    
    std::vector<int> values;
    for (auto it = m.crbegin(); it != m.crend(); ++it) {
        values.push_back(*it);
    }
    
    REQUIRE(values == std::vector<int>{20, 10});
}

TEST_CASE("Matrix: Iterator operations - increment", "[iterator]") {
    Matrix<int> m(1, 3, 0);
    m[0][0] = 1;
    m[0][1] = 2;
    m[0][2] = 3;
    
    auto it = m.begin();
    REQUIRE(*it == 1);
    
    ++it;
    REQUIRE(*it == 2);
    
    it++;
    REQUIRE(*it == 3);
}

TEST_CASE("Matrix: Iterator operations - decrement", "[iterator]") {
    Matrix<int> m(1, 3, 0);
    m[0][0] = 1;
    m[0][1] = 2;
    m[0][2] = 3;
    
    auto it = m.end();
    --it;
    REQUIRE(*it == 3);
    
    it--;
    REQUIRE(*it == 2);
}

TEST_CASE("Matrix: Iterator operations - arithmetic", "[iterator]") {
    Matrix<int> m(1, 5, 0);
    for (int i = 0; i < 5; ++i) {
        m[0][i] = i;
    }
    
    auto it = m.begin();
    
    REQUIRE(*(it + 2) == 2);
    REQUIRE(*(it + 4) == 4);
    
    it += 3;
    REQUIRE(*it == 3);
    
    it -= 1;
    REQUIRE(*it == 2);
    
    REQUIRE(*(it - 1) == 1);
}

TEST_CASE("Matrix: Iterator operations - difference", "[iterator]") {
    Matrix<int> m(2, 3);
    
    auto it1 = m.begin();
    auto it2 = m.end();
    
    REQUIRE(it2 - it1 == 6);
}

TEST_CASE("Matrix: Iterator operations - comparison", "[iterator]") {
    Matrix<int> m(1, 3);
    
    auto it1 = m.begin();
    auto it2 = m.begin() + 1;
    auto it3 = m.end();
    
    REQUIRE(it1 != it2);
    REQUIRE(it1 < it2);
    REQUIRE(it2 > it1);
    REQUIRE(it1 <= it2);
    REQUIRE(it2 >= it1);
    REQUIRE(it1 == m.begin());
}

TEST_CASE("Matrix: Iterator operations - subscript", "[iterator]") {
    Matrix<int> m(1, 5, 0);
    for (int i = 0; i < 5; ++i) {
        m[0][i] = i * 10;
    }
    
    auto it = m.begin();
    REQUIRE(it[0] == 0);
    REQUIRE(it[2] == 20);
    REQUIRE(it[4] == 40);
}

TEST_CASE("Matrix: size() method", "[size]") {
    Matrix<int> m1(3, 4);
    REQUIRE(m1.size() == 12);
    
    Matrix<int> m2(5, 5);
    REQUIRE(m2.size() == 25);
    
    Matrix<int> m3;
    REQUIRE(m3.size() == 0);
}

TEST_CASE("Matrix: empty() method", "[size]") {
    Matrix<int> m1;
    REQUIRE(m1.empty());
    
    Matrix<int> m2(3, 3);
    REQUIRE_FALSE(m2.empty());
    
    Matrix<int> m3(0, 5);
    REQUIRE(m3.empty());
    
    Matrix<int> m4(5, 0);
    REQUIRE(m4.empty());
}

TEST_CASE("Matrix: get_rows() and get_cols()", "[size]") {
    Matrix<int> m(7, 9);
    REQUIRE(m.get_rows() == 7);
    REQUIRE(m.get_cols() == 9);
}

TEST_CASE("Matrix: resize() - increase size", "[resize]") {
    Matrix<int> m(2, 2, 5);
    m[0][0] = 1;
    m[1][1] = 2;
    
    m.resize(3, 3);
    
    REQUIRE(m.get_rows() == 3);
    REQUIRE(m.get_cols() == 3);
    REQUIRE(m[0][0] == 1);  
    REQUIRE(m[1][1] == 2);
    REQUIRE(m[2][2] == 0);  
}

TEST_CASE("Matrix: resize() - decrease size", "[resize]") {
    Matrix<int> m(3, 3, 0);
    m[0][0] = 1;
    m[0][1] = 2;
    m[1][0] = 3;
    m[2][2] = 9;
    
    m.resize(2, 2);
    
    REQUIRE(m.get_rows() == 2);
    REQUIRE(m.get_cols() == 2);
    REQUIRE(m[0][0] == 1);
    REQUIRE(m[0][1] == 2);
    REQUIRE(m[1][0] == 3);
}

TEST_CASE("Matrix: resize() - same size (no-op)", "[resize]") {
    Matrix<int> m(3, 3, 7);
    m[1][1] = 10;
    
    m.resize(3, 3);
    
    REQUIRE(m.get_rows() == 3);
    REQUIRE(m.get_cols() == 3);
    REQUIRE(m[1][1] == 10);
}

TEST_CASE("Matrix: resize() - to zero size", "[resize]") {
    Matrix<int> m(3, 3, 5);
    
    m.resize(0, 0);
    
    REQUIRE(m.empty());
    REQUIRE(m.size() == 0);
    REQUIRE(m.data() == nullptr);
}

TEST_CASE("Matrix: resize() - invalid dimensions (negative rows)", "[resize][exception]") {
    Matrix<int> m(2, 2);
    REQUIRE_THROWS_AS(m.resize(-1, 2), std::invalid_argument);
}

TEST_CASE("Matrix: resize() - invalid dimensions (negative cols)", "[resize][exception]") {
    Matrix<int> m(2, 2);
    REQUIRE_THROWS_AS(m.resize(2, -1), std::invalid_argument);
}

TEST_CASE("Matrix: resize() with value - increase size", "[resize]") {
    Matrix<int> m(2, 2, 0);
    m[0][0] = 1;
    
    m.resize(3, 3, 99);
    
    REQUIRE(m[0][0] == 1);      
    REQUIRE(m[2][2] == 99);     
    REQUIRE(m[0][2] == 99);
    REQUIRE(m[2][0] == 99);
}

TEST_CASE("Matrix: resize() with value - invalid dimensions", "[resize][exception]") {
    Matrix<int> m(2, 2);
    REQUIRE_THROWS_AS(m.resize(-1, 3, 10), std::invalid_argument);
}


TEST_CASE("Matrix: clear() method", "[operations]") {
    Matrix<int> m(3, 3, 5);
    REQUIRE_FALSE(m.empty());
    
    m.clear();
    
    REQUIRE(m.empty());
    REQUIRE(m.size() == 0);
    REQUIRE(m.get_rows() == 0);
    REQUIRE(m.get_cols() == 0);
    REQUIRE(m.data() == nullptr);
}

TEST_CASE("Matrix: swap() method", "[operations]") {
    Matrix<int> m1(2, 2, 1);
    Matrix<int> m2(3, 3, 2);
    
    m1.swap(m2);
    
    REQUIRE(m1.get_rows() == 3);
    REQUIRE(m1.get_cols() == 3);
    REQUIRE(m1[0][0] == 2);
    
    REQUIRE(m2.get_rows() == 2);
    REQUIRE(m2.get_cols() == 2);
    REQUIRE(m2[0][0] == 1);
}

TEST_CASE("Matrix: swap() - ADL (Argument Dependent Lookup)", "[operations]") {
    Matrix<int> m1(2, 2, 10);
    Matrix<int> m2(1, 1, 20);
    
    using std::swap;
    swap(m1, m2);  
    
    REQUIRE(m1.get_rows() == 1);
    REQUIRE(m1[0][0] == 20);
    REQUIRE(m2.get_rows() == 2);
    REQUIRE(m2[0][0] == 10);
}

TEST_CASE("Matrix: fill() method", "[operations]") {
    Matrix<int> m(3, 3, 0);
    
    m.fill(42);
    
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            REQUIRE(m[i][j] == 42);
        }
    }
}

TEST_CASE("Matrix: is_valid_position() - valid positions", "[utility]") {
    Matrix<int> m(3, 4);
    
    REQUIRE(m.is_valid_position(0, 0));
    REQUIRE(m.is_valid_position(2, 3));
    REQUIRE(m.is_valid_position(1, 2));
}

TEST_CASE("Matrix: is_valid_position() - invalid positions", "[utility]") {
    Matrix<int> m(3, 4);
    
    REQUIRE_FALSE(m.is_valid_position(-1, 0));
    REQUIRE_FALSE(m.is_valid_position(0, -1));
    REQUIRE_FALSE(m.is_valid_position(3, 0));
    REQUIRE_FALSE(m.is_valid_position(0, 4));
    REQUIRE_FALSE(m.is_valid_position(10, 10));
}

TEST_CASE("Matrix: transpose() method", "[utility]") {
    Matrix<int> m(2, 3, 0);
    m[0][0] = 1;  m[0][1] = 2;  m[0][2] = 3;
    m[1][0] = 4;  m[1][1] = 5;  m[1][2] = 6;
    
    Matrix<int> t = m.transpose();
    
    REQUIRE(t.get_rows() == 3);
    REQUIRE(t.get_cols() == 2);
    REQUIRE(t[0][0] == 1);  REQUIRE(t[0][1] == 4);
    REQUIRE(t[1][0] == 2);  REQUIRE(t[1][1] == 5);
    REQUIRE(t[2][0] == 3);  REQUIRE(t[2][1] == 6);
    
    REQUIRE(m.get_rows() == 2);
    REQUIRE(m.get_cols() == 3);
}

TEST_CASE("Matrix: transpose() - square matrix", "[utility]") {
    Matrix<int> m(2, 2, 0);
    m[0][0] = 1;  m[0][1] = 2;
    m[1][0] = 3;  m[1][1] = 4;
    
    Matrix<int> t = m.transpose();
    
    REQUIRE(t[0][0] == 1);  REQUIRE(t[0][1] == 3);
    REQUIRE(t[1][0] == 2);  REQUIRE(t[1][1] == 4);
}

TEST_CASE("Matrix: operator== - equal matrices", "[comparison]") {
    Matrix<int> m1(2, 2, 5);
    Matrix<int> m2(2, 2, 5);
    
    REQUIRE(m1 == m2);
}

TEST_CASE("Matrix: operator== - different values", "[comparison]") {
    Matrix<int> m1(2, 2, 5);
    Matrix<int> m2(2, 2, 5);
    m2[0][0] = 10;
    
    REQUIRE_FALSE(m1 == m2);
}

TEST_CASE("Matrix: operator== - different dimensions", "[comparison]") {
    Matrix<int> m1(2, 2, 5);
    Matrix<int> m2(3, 3, 5);
    
    REQUIRE_FALSE(m1 == m2);
}

TEST_CASE("Matrix: operator!= - different matrices", "[comparison]") {
    Matrix<int> m1(2, 2, 1);
    Matrix<int> m2(2, 2, 2);
    
    REQUIRE(m1 != m2);
}

TEST_CASE("Matrix: operator!= - equal matrices", "[comparison]") {
    Matrix<int> m1(2, 2, 5);
    Matrix<int> m2(2, 2, 5);
    
    REQUIRE_FALSE(m1 != m2);
}

TEST_CASE("Matrix: std::fill", "[stl]") {
    Matrix<int> m(3, 3, 0);
    
    std::fill(m.begin(), m.end(), 7);
    
    for (const auto& val : m) {
        REQUIRE(val == 7);
    }
}

TEST_CASE("Matrix: std::count", "[stl]") {
    Matrix<int> m(3, 3, 5);
    m[0][0] = 10;
    m[1][1] = 10;
    
    int count = std::count(m.begin(), m.end(), 10);
    REQUIRE(count == 2);
}

TEST_CASE("Matrix: std::find", "[stl]") {
    Matrix<int> m(2, 3, 0);
    m[1][2] = 42;
    
    auto it = std::find(m.begin(), m.end(), 42);
    REQUIRE(it != m.end());
    REQUIRE(*it == 42);
}

TEST_CASE("Matrix: std::transform", "[stl]") {
    Matrix<int> m(2, 2, 1);
    
    std::transform(m.begin(), m.end(), m.begin(), [](int x) { return x * 2; });
    
    for (const auto& val : m) {
        REQUIRE(val == 2);
    }
}

TEST_CASE("Matrix: std::accumulate", "[stl]") {
    Matrix<int> m(2, 2, 0);
    m[0][0] = 1;
    m[0][1] = 2;
    m[1][0] = 3;
    m[1][1] = 4;
    
    int sum = std::accumulate(m.begin(), m.end(), 0);
    REQUIRE(sum == 10);
}

TEST_CASE("Matrix: std::all_of", "[stl]") {
    Matrix<int> m(2, 2, 5);
    
    bool all_five = std::all_of(m.begin(), m.end(), [](int x) { return x == 5; });
    REQUIRE(all_five);
}

TEST_CASE("Matrix: std::sort", "[stl]") {
    Matrix<int> m(1, 5, 0);
    m[0][0] = 5;
    m[0][1] = 2;
    m[0][2] = 8;
    m[0][3] = 1;
    m[0][4] = 9;
    
    std::sort(m.begin(), m.end());
    
    REQUIRE(m[0][0] == 1);
    REQUIRE(m[0][1] == 2);
    REQUIRE(m[0][2] == 5);
    REQUIRE(m[0][3] == 8);
    REQUIRE(m[0][4] == 9);
}

TEMPLATE_TEST_CASE("Matrix: Different types", "[template]", int, float, double, char) {
    Matrix<TestType> m(2, 2);
    
    REQUIRE(m.get_rows() == 2);
    REQUIRE(m.get_cols() == 2);
    REQUIRE(m.size() == 4);
}

TEST_CASE("Matrix: String type", "[types]") {
    Matrix<std::string> m(2, 2, "hello");
    
    REQUIRE(m[0][0] == "hello");
    REQUIRE(m[1][1] == "hello");
    
    m[0][1] = "world";
    REQUIRE(m[0][1] == "world");
}

TEST_CASE("Matrix: Custom struct", "[types]") {
    struct Point {
        int x, y;
        Point() : x(0), y(0) {}
        Point(int x_, int y_) : x(x_), y(y_) {}
        bool operator==(const Point& other) const {
            return x == other.x && y == other.y;
        }
    };
    
    Matrix<Point> m(2, 2);
    m[0][0] = Point(1, 2);
    m[1][1] = Point(3, 4);
    
    REQUIRE(m[0][0] == Point(1, 2));
    REQUIRE(m[1][1] == Point(3, 4));
}

TEST_CASE("Matrix: Large matrix", "[stress]") {
    Matrix<int> m(100, 100, 0);
    
    REQUIRE(m.size() == 10000);
    
    int counter = 0;
    for (int i = 0; i < 100; ++i) {
        for (int j = 0; j < 100; ++j) {
            m[i][j] = counter++;
        }
    }
    
    REQUIRE(m[99][99] == 9999);
}

TEST_CASE("Matrix: Single element", "[edge]") {
    Matrix<int> m(1, 1, 42);
    
    REQUIRE(m[0][0] == 42);
    REQUIRE(m.size() == 1);
}

TEST_CASE("Matrix: Single row", "[edge]") {
    Matrix<int> m(1, 10, 5);
    
    REQUIRE(m.get_rows() == 1);
    REQUIRE(m.get_cols() == 10);
    
    for (int j = 0; j < 10; ++j) {
        REQUIRE(m[0][j] == 5);
    }
}

TEST_CASE("Matrix: Single column", "[edge]") {
    Matrix<int> m(10, 1, 7);
    
    REQUIRE(m.get_rows() == 10);
    REQUIRE(m.get_cols() == 1);
    
    for (int i = 0; i < 10; ++i) {
        REQUIRE(m[i][0] == 7);
    }
}

TEST_CASE("Matrix: Multiple resizes", "[stress]") {
    Matrix<int> m(2, 2, 1);
    
    m.resize(5, 5);
    REQUIRE(m.size() == 25);
    
    m.resize(3, 3);
    REQUIRE(m.size() == 9);
    
    m.resize(10, 10);
    REQUIRE(m.size() == 100);
    
    m.resize(1, 1);
    REQUIRE(m.size() == 1);
}

TEST_CASE("Matrix: Game map simulation", "[game]") {
    enum class CellType { EMPTY, WALL, GLASS };
    
    Matrix<CellType> game_map(10, 10, CellType::EMPTY);
    
    for (int i = 0; i < 10; ++i) {
        game_map[0][i] = CellType::WALL;
        game_map[9][i] = CellType::WALL;
        game_map[i][0] = CellType::WALL;
        game_map[i][9] = CellType::WALL;
    }
    
    REQUIRE(game_map[0][0] == CellType::WALL);
    REQUIRE(game_map[5][5] == CellType::EMPTY);
    
    game_map.resize(15, 15, CellType::EMPTY);
    REQUIRE(game_map.get_rows() == 15);
    
    REQUIRE(game_map[0][0] == CellType::WALL);
}

TEST_CASE("Matrix: Iteration over game map", "[game]") {
    Matrix<int> visibility_map(5, 5, 0);
    
    visibility_map[2][2] = 1;
    visibility_map[2][1] = 1;
    visibility_map[2][3] = 1;
    visibility_map[1][2] = 1;
    visibility_map[3][2] = 1;
    
    int visible_count = std::count(visibility_map.begin(), visibility_map.end(), 1);
    REQUIRE(visible_count == 5);
}


