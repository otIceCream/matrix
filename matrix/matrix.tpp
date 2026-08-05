//#include "matrix.h"

template <typename T>
matrix<T>::matrix() : rows_count(3), cols_count(3) {
    data = new T * [rows_count];//заменить на умные указатели
    for (size_t i{ 0 }; i < rows_count; ++i) {
        data[i] = new T[cols_count]();
    }
}

template <typename T>
matrix<T>::matrix(size_t rows, size_t cols) : rows_count(rows), cols_count(cols) { //конструктор для цифр, пока что
    data = new T * [rows_count];//заменить на умные указатели
    for (size_t i{ 0 }; i < rows_count; ++i) {
        data[i] = new T[cols_count]();
    }
}

template <typename T>
matrix<T>::matrix(std::initializer_list<std::initializer_list<T>> list) : rows_count(list.size())
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
template <typename T>
matrix<T>::matrix(const matrix<T>& other) :matrix(other.rows_count, other.cols_count) {
    for (size_t i{ 0 }; i < rows_count; ++i) {
        for (size_t j{ 0 }; j < cols_count; ++j) {
            data[i][j] = other.data[i][j];
        }
    }
}

// — перемещающий
template <typename T>
matrix<T>::matrix(matrix<T>&& other) noexcept
    : rows_count(other.rows_count), cols_count(other.cols_count), data(other.data) {
    other.rows_count = 0;
    other.cols_count = 0;
    other.data = nullptr;
}

/*-----------------------Деструктор---------------------------*/

template <typename T>
matrix<T>::~matrix() {
    for (size_t i{ 0 }; i < rows_count; ++i) {
        delete[] data[i];
        data[i] = nullptr;
    }
    delete[] data;
    data = nullptr;
    rows_count = 0;
    cols_count = 0;
}

//— копирующее присваивание
template <typename T>
matrix<T>& matrix<T>::operator=(const matrix<T>& other) {
    if (this == &other) {
        return *this;
    }
    if (data != nullptr) {
        for (size_t i{ 0 }; i < rows_count; ++i) {
            delete[] data[i];
        }
        delete[] data;
        data = nullptr;
    }
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
template <typename T>
matrix<T>& matrix<T>::operator=(matrix<T>&& other) noexcept {
    if (this == &other) {
        return *this;
    }
    if (data != nullptr) {
        for (size_t i{ 0 }; i < rows_count; ++i) {
            delete[] data[i];
        }
        delete[] data;
        data = nullptr;
    }
    rows_count = other.rows_count;
    cols_count = other.cols_count;
    data = other.data;

    other.rows_count = 0;
    other.cols_count = 0;
    other.data = nullptr;

    return *this;
}

// — доступ только для чтения
template <typename T>
const T& matrix<T>::operator()(size_t row, size_t col) const {
    return data[row][col];
}

// — доступ на чтение/запись
template <typename T>
T& matrix<T>::operator()(size_t row, size_t col) {
    return data[row][col];
}

// — количество строк
template <typename T>
size_t matrix<T>::rows() const {
    return rows_count;
}

// — количество столбцов
template <typename T>
size_t matrix<T>::cols() const {
    return cols_count;
}

// — общее количество элементов
template <typename T>
size_t matrix<T>::size() const {
    return rows_count * cols_count;
}

// — изменить размер(с потерей данных или с инициализацией)
template <typename T>
void matrix<T>::resize(size_t new_rows, size_t new_cols) {
    if (new_rows == 0 || new_cols == 0) {
        throw std::invalid_argument("Размеры матрицы должны быть больше 0!");
    }

    if (new_rows == rows_count && new_cols == cols_count) return;

    matrix temp_data(new_rows, new_cols);

    size_t copy_rows = (rows_count < new_rows) ? rows_count : new_rows;
    size_t copy_cols = (cols_count < new_cols) ? cols_count : new_cols;

    for (size_t i{ 0 }; i < copy_rows; ++i) {
        for (size_t j{ 0 }; j < copy_cols; ++j) {
            temp_data.data[i][j] = data[i][j];
        }
    }

    *this = std::move(temp_data);
}

// — вставить пустую строку на позицию
template <typename T>
void matrix<T>::insert_row(size_t pos) {
    if (pos > rows_count) {
        throw std::out_of_range("Индекс вне диапазона!");
    }

    matrix temp_data(rows_count+1, cols_count);

    size_t data_index = 0;
    for (size_t i = 0; i < temp_data.rows_count; ++i) {
        if (i == pos) continue;
        for (size_t j = 0 ; j < cols_count; ++j) {
            temp_data.data[i][j] = data[data_index][j];
        }
        ++data_index;
    }
    *this = std::move(temp_data);
}

// — вставить пустой столбец
template <typename T> 
void matrix<T>::insert_col(size_t pos) {
    if (pos > cols_count) {
        throw std::out_of_range("Индекс вне диапазона!");
    }

    matrix temp_data(rows_count, cols_count+1);

    for (size_t i = 0; i < rows_count; ++i) {
        size_t data_index = 0;
        for (size_t j = 0; j < temp_data.cols_count; ++j) {
            if (j == pos) continue;
            temp_data.data[i][j] = data[i][data_index];
            ++data_index;
        }
    }
    *this = std::move(temp_data);
}

// — удалить строку
template <typename T>
void matrix<T>::remove_row(size_t pos) {
    if (pos >= rows_count) {
        throw std::out_of_range("Индекс вне диапазона!");
    }

    matrix temp_data(rows_count - 1, cols_count);

    size_t data_index = 0;
    for (size_t i = 0; i < rows_count; ++i) {
        if (i == pos) continue;
        for (size_t j = 0; j < cols_count; ++j) {
            temp_data.data[data_index][j] = data[i][j];
        }
        ++data_index;
    }
    *this = std::move(temp_data);
}

// — удалить столбец
template <typename T>
void matrix<T>::remove_col(size_t pos) {
    if (pos >= cols_count) {
        throw std::out_of_range("Индекс вне диапазона!");
    }

    matrix temp_data(rows_count, cols_count-1);

    for (size_t i = 0; i < rows_count; ++i) {
        size_t data_index = 0;
        for (size_t j = 0; j < cols_count; ++j) {
            if (j == pos) continue;
            temp_data.data[i][data_index] = data[i][j];
            ++data_index;
        }
    }

    *this = std::move(temp_data);
}

// — поменять строки местами строки
template <typename T>
void matrix<T>::swap_rows(size_t r1, size_t r2) {
    T* temp_row = new T[rows_count];
    for (size_t i{ 0 }; i < cols_count; ++i) {
        temp_row[i] = data[r1][i];
        data[r1][i] = data[r2][i];
        data[r2][i] = temp_row[i];
    }
    delete[] temp_row;
}

// — поменять столбцы местами
template <typename T>
void matrix<T>::swap_cols(size_t c1, size_t c2) {
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
// — с проверкой границ (исключение)
template <typename T>
T& matrix<T>::at(size_t row, size_t col) {
    if (row >= rows_count || col >= cols_count) {
        throw std::out_of_range("Индекс вне диапазона!");
    }
    return data[row][col];
}
// const — аналогично, константная версия
template <typename T>
const T& matrix<T>::at(size_t row, size_t col) const {
    if (row >= rows_count || col >= cols_count) {
        throw std::out_of_range("Индекс вне диапазона!");
    }
    return data[row][col];
}

//Заполнение и инициализация
// — заполнить все элементы одним значением
template <typename T>
void matrix<T>::fill(const T& value) {
    for (size_t i{ 0 }; i < rows_count; ++i) {
        for (size_t j{ 0 }; j < cols_count; ++j) {
            data[i][j] = value;
        }
    }
}
// — обнулить (если числовой тип)
template <typename T>
void matrix<T>::zero() {
    fill(0);
}
// — сделать единичную матрицу (для квадратных)
template <typename T>
void matrix<T>::identity() {
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
template <typename T>
matrix<T>& matrix<T>::operator+=(const matrix<T>& other) {
    for (size_t i{ 0 }; i < rows_count; ++i) {
        for (size_t j{ 0 }; j < cols_count; ++j) {
            data[i][j] += other.data[i][j];
        }
    }
    return *this;
}
// — вычитание с присваиванием
template <typename T>
matrix<T>& matrix<T>::operator-=(const matrix<T>& other) {
    for (size_t i{ 0 }; i < rows_count; ++i) {
        for (size_t j{ 0 }; j < cols_count; ++j) {
            data[i][j] -= other.data[i][j];
        }
    }
    return *this;
}

// — умножение на скаляр
template <typename T>
matrix<T>& matrix<T>::operator*=(const T& scalar) {
    for (size_t i{ 0 }; i < rows_count; ++i) {
        for (size_t j{ 0 }; j < cols_count; ++j) {
            data[i][j] *= scalar;
        }
    }
    return *this;
}
// — сложение
template <typename T>
matrix<T> matrix<T>::operator+(const matrix<T>& other) const {
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
template <typename T>
matrix<T> matrix<T>::operator-(const matrix<T>& other) const {
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
template <typename T>
matrix<T> matrix<T>::operator*(const matrix<T>& other) const {
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

// — умножение на скаляр
template <typename T>
matrix<T> matrix<T>::operator*(const T& scalar) const {

    matrix result(rows_count, cols_count);

    for (size_t i = 0; i < rows_count; ++i) {
        for (size_t j = 0; j < cols_count; ++j) {
            result.data[i][j] = data[i][j] * scalar;
        }
    }

    return result;
}
// — транспонирование
template <typename T>
matrix<T> matrix<T>::transpose() const {
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
* template <typename T>
 size_t matrix<T>::determinant() const {
     return data[0][0];
 }*/

template <typename T>
std::ostream& operator<<(std::ostream& os, const matrix<T>& m) {
    for (size_t i = 0; i < m.rows(); ++i) {
        for (size_t j = 0; j < m.cols(); ++j) {
            os << m(i, j) << " ";
        }
        os << "\n";
    }
    return os;
}

template <typename T>
bool matrix<T>::operator==(const matrix<T>& other) const {
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
template <typename T>
bool matrix<T>::operator!=(const matrix<T>& other) const {
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