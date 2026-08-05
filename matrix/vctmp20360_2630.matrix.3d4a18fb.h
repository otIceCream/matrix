#pragma once
#include <initializer_list>
#include <iostream>
template <typename T>
class matrix
{
private:
    static_assert(
        std::is_same_v<T, int> ||
        std::is_same_v<T, double> ||
        std::is_same_v<T, float> ||
        std::is_same_v<T, size_t>
    );
    size_t rows_count;
    size_t cols_count;
    T** data;
public:
    /*-----------------------Конструкторы---------------------------*/

    // — по умолчанию(пустая или нулевая) 3x3 по умолчанию
    matrix() : rows_count(3), cols_count(3) {
        data = new T * [rows_count];//заменить на умные указатели
        for (size_t i{ 0 }; i < rows_count; ++i) {
            data[i] = new T[cols_count]();
        }
    }

    // — с размерами(заполнить нулями или неинициализированно)
    matrix(size_t rows, size_t cols) : rows_count(rows), cols_count(cols) { //конструктор для цифр, пока что
        data = new T * [rows_count];//заменить на умные указатели
        for (size_t i{ 0 }; i < rows_count; ++i) {
            data[i] = new T[cols_count]();
        }
    }

    // плохо знаю функции и аргументы но с работой разобрался кое как
    // РАЗОБРАЛСЯ ПОЛНОСТЬЮ
    //— из списка
    matrix(std::initializer_list<std::initializer_list<T>> list) : rows_count(list.size())
        , cols_count(list.size() > 0 ? list.begin()->size() : 0), data(nullptr) {

        data = new T * [rows_count];
        for (size_t i = 0; i < rows_count; ++i) {
            data[i] = new T[cols_count]();
        }
        // Заполнение
        size_t i = 0;
        for (auto& row_list : list) {
            size_t j = 0;
            for (auto val : row_list) {
                data[i][j] = val;
                ++j;
            }
            ++i;
        }
    }

    // — копирующий
    matrix(const matrix& other) :matrix(other.rows_count, other.cols_count) {
        for (size_t i{ 0 }; i < rows_count; ++i) {
            for (size_t j{ 0 }; j < cols_count; ++j) {
                data[i][j] = other.data[i][j];
            }
        }
    }

    // — перемещающий
    matrix(matrix&& other) noexcept
        : rows_count(other.rows_count), cols_count(other.cols_count), data(other.data) {
        other.rows_count = 0;
        other.cols_count = 0;
        other.data = nullptr;
    }

    /*-----------------------Деструктор---------------------------*/

    ~matrix() {
        for (size_t i{ 0 }; i < rows_count; ++i) {
            delete[] data[i];
            data[i] = nullptr;
        }
        delete[] data;
        data = nullptr;
        rows_count = 0;
        cols_count = 0;
    }

    /*-----------------------ФУНКЦИИ---------------------------*/

    //— копирующее присваивание
    matrix& operator=(const matrix& other) {
        if (this == &other) {
            return *this;
        }

        for (size_t i{ 0 }; i < rows_count; ++i) {
            delete[] data[i];
        }
        delete[] data;

        rows_count = other.rows_count;
        cols_count = other.cols_count;
        data = new T * [rows_count];//заменить на умные указатели
        for (size_t i{ 0 }; i < rows_count; ++i) {
            data[i] = new T[cols_count];
        }

        for (size_t i{ 0 }; i < rows_count; ++i) {
            for (size_t j{ 0 }; j < cols_count; ++j) {
                data[i][j] = other.data[i][j];
            }
        }
        return *this;
    }

    //— перемещающее присваивание
    matrix& operator=(matrix&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        for (size_t i{ 0 }; i < rows_count; ++i) {
            delete[] data[i];
            data[i] = nullptr;
        }
        delete[] data;
        data = nullptr;

        rows_count = other.rows_count;
        cols_count = other.cols_count;
        data = other.data;

        other.rows_count = 0;
        other.cols_count = 0;
        other.data = nullptr;

        return *this;
    }

    // — доступ только для чтения
    const T& operator()(size_t row, size_t col) const {
        return data[row][col];
    }

    // — доступ на чтение/запись
    //T&
    T& operator()(size_t row, size_t col) {
        return data[row][col];
    }

    // — количество строк
    size_t rows() const {
        return rows_count;
    }

    // — количество столбцов
    size_t cols() const {
        return cols_count;
    }

    // — общее количество элементов
    size_t size() const {
        return rows_count * cols_count;
    }

    // — изменить размер(с потерей данных или с инициализацией)
    void resize(size_t new_rows, size_t new_cols) {

        if (new_rows == rows_count && new_cols == cols_count) return;

        T** temp_data = new T * [new_rows];

        for (size_t i{ 0 }; i < new_rows; ++i) {
            temp_data[i] = new T[new_cols]();
        }

        size_t copy_rows = (rows_count < new_rows) ? rows_count : new_rows;
        size_t copy_cols = (cols_count < new_cols) ? cols_count : new_cols;

        for (size_t i{ 0 }; i < copy_rows; ++i) {
            for (size_t j{ 0 }; j < copy_cols; ++j) {
                temp_data[i][j] = data[i][j];
            }
        }

        if (data != nullptr) {
            for (size_t i = 0; i < rows_count; ++i) {
                delete[] data[i];
            }
            delete[] data;
        }

        rows_count = new_rows;
        cols_count = new_cols;
        data = temp_data;
    }

    // — вставить пустую строку на позицию
    void insert_row(size_t pos) {
        if (pos > rows_count) {
            return;
        }
        T old_rows = rows_count;
        rows_count += 1;
        T** temp_data = new T * [rows_count];
        for (size_t i{ 0 }; i < rows_count; ++i) {
            temp_data[i] = new T[cols_count];
            if (i == pos) {
                for (size_t j{ 0 }; j < cols_count; ++j)
                    temp_data[i][j] = 0;      // пустая строка
            }
        }

        T data_index = 0;
        for (size_t i{ 0 }; i < rows_count; ++i) {
            if (i == pos) continue;
            for (size_t j{ 0 }; j < cols_count; ++j) {
                temp_data[i][j] = data[data_index][j];
            }
            ++data_index;
        }

        for (size_t i{ 0 }; i < old_rows; ++i) {
            delete[] data[i];
        }
        delete[] data;

        data = new T * [rows_count];//заменить на умные указатели
        for (size_t i{ 0 }; i < rows_count; ++i) {
            data[i] = new T[cols_count];
        }

        for (size_t i{ 0 }; i < rows_count; ++i) {
            for (size_t j{ 0 }; j < cols_count; ++j) {
                data[i][j] = temp_data[i][j];
            }
        }

        for (size_t i{ 0 }; i < rows_count; ++i) {
            delete[] temp_data[i];
        }
        delete[] temp_data;
    }

    // — вставить пустой столбец
    void insert_col(size_t pos) {
        if (pos > cols_count) {
            return;
        }
        T new_cols = cols_count + 1;

        T** temp_data = new T * [rows_count];

        for (size_t i{ 0 }; i < rows_count; ++i) {
            temp_data[i] = new T[new_cols];
            for (size_t j{ 0 }; j < new_cols; ++j) {
                if (j == pos) temp_data[i][j] = 0;
            }
        }

        size_t data_col = 0;
        for (size_t i{ 0 }; i < rows_count; ++i) {
            for (size_t j{ 0 }; j < new_cols; ++j) {
                if (j == pos) continue;
                temp_data[i][j] = data[i][data_col];
                ++data_col;
            }
            data_col = 0;
        }

        for (size_t i{ 0 }; i < rows_count; ++i) {
            delete[] data[i];
        }
        delete[] data;

        cols_count = new_cols;

        data = new T * [rows_count];//заменить на умные указатели

        for (size_t i{ 0 }; i < rows_count; ++i) {
            data[i] = new T[cols_count];
        }

        for (size_t i{ 0 }; i < rows_count; ++i) {
            for (size_t j{ 0 }; j < cols_count; ++j) {
                data[i][j] = temp_data[i][j];
            }
        }

        for (size_t i{ 0 }; i < rows_count; ++i) {
            delete[] temp_data[i];
        }
        delete[] temp_data;
    }

    // — удалить строку
    void remove_row(size_t pos) {
        if (pos > rows_count) {
            return;
        }

        size_t old_rows = rows_count;
        rows_count -= 1;

        T** temp_data = new T * [rows_count];
        for (size_t i{ 0 }; i < rows_count; ++i) {
            temp_data[i] = new T[cols_count];
        }

        size_t data_index = 0;
        for (size_t i{ 0 }; i < old_rows; ++i) {
            if (i == pos) continue;
            for (size_t j{ 0 }; j < cols_count; ++j) {
                temp_data[data_index][j] = data[i][j];
            }
            ++data_index;
        }

        for (size_t i{ 0 }; i < old_rows; ++i) {
            delete[] data[i];
        }
        delete[] data;

        data = new T * [rows_count];//заменить на умные указатели
        for (size_t i{ 0 }; i < rows_count; ++i) {
            data[i] = new T[cols_count];
        }

        for (size_t i{ 0 }; i < rows_count; ++i) {
            for (size_t j{ 0 }; j < cols_count; ++j) {
                data[i][j] = temp_data[i][j];
            }
        }

        for (size_t i{ 0 }; i < rows_count; ++i) {
            delete[] temp_data[i];
        }
        delete[] temp_data;
    }

    // — удалить столбец
    void remove_col(size_t pos) {
        if (pos > cols_count) {
            return;
        }

        if (cols_count == 0 || rows_count == 0) {
            return;
        }

        size_t new_cols = cols_count - 1;

        T** temp_data = new T * [rows_count];

        for (size_t i{ 0 }; i < rows_count; ++i) {
            temp_data[i] = new T[new_cols];
        }

        size_t data_col = 0;
        for (size_t i{ 0 }; i < rows_count; ++i) {
            for (size_t j{ 0 }; j < cols_count; ++j) {
                if (j == pos) continue;
                temp_data[i][data_col] = data[i][j];
                ++data_col;
            }
            data_col = 0;
        }

        for (size_t i{ 0 }; i < rows_count; ++i) {
            delete[] data[i];
        }
        delete[] data;

        cols_count = new_cols;

        data = new T * [rows_count];//заменить на умные указатели

        for (size_t i{ 0 }; i < rows_count; ++i) {
            data[i] = new T[cols_count];
        }

        for (size_t i{ 0 }; i < rows_count; ++i) {
            for (size_t j{ 0 }; j < cols_count; ++j) {
                data[i][j] = temp_data[i][j];
            }
        }

        for (size_t i{ 0 }; i < rows_count; ++i) {
            delete[] temp_data[i];
        }
        delete[] temp_data;
    }

    // — поменять строки местами строки
    void swap_rows(size_t r1, size_t r2) {
        T* temp_row = new T[rows_count];
        for (size_t i{ 0 }; i < cols_count; ++i) {
            temp_row[i] = data[r1][i];
            data[r1][i] = data[r2][i];
            data[r2][i] = temp_row[i];
        }
        delete[] temp_row;
    }

    // — поменять столбцы местами
    void swap_cols(size_t c1, size_t c2) {
        T* temp_col = new T[cols_count];
        size_t index = 0;
        for (size_t i{ 0 }; i < rows_count; ++i) {
            temp_col[index] = data[i][c1];
            data[i][c1] = data[i][c2];
            data[i][c2] = temp_col[index];
            ++index;
        }
        delete[] temp_col;
    }

    //Доступ к элементам
    //T&
    // — с проверкой границ (исключение)
    T& at(size_t row, size_t col) {
        if (row >= rows_count || col >= cols_count) {
            throw std::out_of_range("Индекс вне диапазона!");
        }
        return data[row][col];
    }
    //const T&
    // const — аналогично, константная версия
    const T& at(size_t row, size_t col) const {
        if (row >= rows_count || col >= cols_count) {
            throw std::out_of_range("Индекс вне диапазона!");
        }
        return data[row][col];
    }

    //Заполнение и инициализация
        //T&
    // — заполнить все элементы одним значением
    void fill(const T& value) {
        for (size_t i{ 0 }; i < rows_count; ++i) {
            for (size_t j{ 0 }; j < cols_count; ++j) {
                data[i][j] = value;
            }
        }
    }
    // — обнулить (если числовой тип)
    void zero() {
        fill(0);
    }
    // — сделать единичную матрицу (для квадратных)
    void identity() {
        for (size_t i{ 0 }; i < rows_count; ++i) {
            for (size_t j{ 0 }; j < cols_count; ++j) {
                if (i == j) {
                    data[i][j] = 1;
                    continue;
                }
                data[i][j] = 0;
            }
        }
    }

    //Математические операции (если добавишь)
    // — сложение с присваиванием
    matrix& operator+=(const matrix& other) {
        for (size_t i{ 0 }; i < rows_count; ++i) {
            for (size_t j{ 0 }; j < cols_count; ++j) {
                data[i][j] += other.data[i][j];
            }
        }
        return *this;
    }
    // — вычитание с присваиванием
    matrix& operator-=(const matrix& other) {
        for (size_t i{ 0 }; i < rows_count; ++i) {
            for (size_t j{ 0 }; j < cols_count; ++j) {
                data[i][j] -= other.data[i][j];
            }
        }
        return *this;
    }
    //T&
    // — умножение на скаляр
    matrix& operator*=(const T& scalar) {
        for (size_t i{ 0 }; i < rows_count; ++i) {
            for (size_t j{ 0 }; j < cols_count; ++j) {
                data[i][j] *= scalar;
            }
        }
        return *this;
    }
    // — сложение
    matrix operator+(const matrix& other) const {
        matrix result(rows_count, cols_count);

        // Заполняем результат суммой
        for (size_t i = 0; i < rows_count; ++i) {
            for (size_t j = 0; j < cols_count; ++j) {
                result.data[i][j] = data[i][j] + other.data[i][j];
            }
        }

        return result;
    }
    // — вычитание
    matrix operator-(const matrix& other) const {
        matrix result(rows_count, cols_count);

        // Заполняем результат суммой
        for (size_t i = 0; i < rows_count; ++i) {
            for (size_t j = 0; j < cols_count; ++j) {
                result.data[i][j] = data[i][j] - other.data[i][j];
            }
        }

        return result;
    }

    // — умножение матриц
    //я бы сам не скоро додумался бы до этого
    matrix operator*(const matrix& other) const {
    if (cols_count != other.rows_count) {
        throw std::invalid_argument("Нельзя умножить: число столбцов первой != числу строк второй");
    }
    
    matrix result(rows_count, other.cols_count);
    
    // Тройной цикл для умножения
    for (size_t i = 0; i < rows_count; ++i) {           
        for (size_t j = 0; j < other.cols_count; ++j) { 
            result.data[i][j] = 0;  
            
            for (size_t k = 0; k < cols_count; ++k) {   // k - общий индекс
                result.data[i][j] += data[i][k] * other.data[k][j];
            }
        }
    }
    
    return result;
    }
    //T&
    // — умножение на скаляр
    matrix operator*(const T& scalar) const {

        matrix result(rows_count,cols_count);

        for (size_t i = 0; i < rows_count; ++i) {
            for (size_t j = 0; j < cols_count; ++j) {
                    result.data[i][j] = data[i][j] * scalar;
            }
        }

        return result;
    }
    // — транспонирование
    matrix transpose() const {
        matrix result(cols_count, rows_count);

        for (size_t i = 0; i < rows_count; ++i) {
            for (size_t j = 0; j < cols_count; ++j) {
                result.data[j][i] = data[i][j];
            }
        }

        return result;
    }
    //T
   /* // — определитель (для квадратных) сложна нахуй, а я болею мозга не робить ээээ
    size_t determinant() const {
        return data[0][0];
    }*/

    friend std::ostream& operator<<(std::ostream& os, const matrix& m) {
        for (size_t i = 0; i < m.rows_count; ++i) {
            for (size_t j = 0; j < m.cols_count; ++j) {
                os << m.data[i][j] << " ";
            }
                os << "\n";
        }
        return os;
    }

    /*
    friend std::istream& operator>>(std::istream& is, matrix& m) {
    // Сначала читаем размеры
    std::cout << "Enter rows count: ";
    is >> m.rows_count;
    std::cout << "Enter cols count: ";
    is >> m.cols_count;
    
    // Изменяем размер матрицы
    m.data.resize(m.rows_count, std::vector<size_t>(m.cols_count));
    
    // Читаем элементы
    std::cout << "Enter matrix elements:\n";
    for (size_t i = 0; i < m.rows_count; ++i) {
        for (size_t j = 0; j < m.cols_count; ++j) {
            is >> m.data[i][j];
        }
    }
    
    return is;
}
    */

    bool operator==(const matrix& other) const {
        if (rows_count != other.rows_count || cols_count != other.cols_count) {
            return false;
        }
            for (size_t i = 0; i < rows_count; ++i) {
                for (size_t j = 0; j < cols_count; ++j) {
                    if (data[i][j] != other.data[i][j]) {
                        return false;
                    }
                }
            }
            return true;
    }

    // — сравнение
    bool operator!=(const matrix& other) const {
            if (rows_count != other.rows_count || cols_count != other.cols_count) {
                return true;
            }
            for (size_t i = 0; i < rows_count; ++i) {
                for (size_t j = 0; j < cols_count; ++j) {
                    if (data[i][j] != other.data[i][j]) {
                        return true;
                    }
                }
            }
            return false;
    }
};