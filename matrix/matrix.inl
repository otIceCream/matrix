#include <initializer_list>
#include<memory>
#include<algorithm>
#include<utility>
#include<stdexcept>

template <typename T>
matrix<T>::matrix() : rows_count(2),
                      cols_count(2),
                      data(std::make_unique<T[]>(rows_count* cols_count)) {}


template <typename T>
matrix<T>::matrix(size_t rows, size_t cols) : rows_count(rows),
                                              cols_count(cols),
                                              data(std::make_unique<T[]>(rows_count* cols_count)) {}

template <typename T>
matrix<T>::matrix(std::initializer_list<std::initializer_list<T>> list) : rows_count(list.size()),
                                                                          cols_count(list.size() > 0 ? list.begin()->size() : 0),
                                                                          data(std::make_unique<T[]>(rows_count* cols_count))
{
    size_t count = 0;
    for (auto& row_list : list) {
        if (cols_count != row_list.size()) {
            throw std::invalid_argument("Матрица должна иметь одинаковый размер строк!");
        }
        std::copy(row_list.begin(),
                  row_list.end(),
                  data.get() + count);
        count += row_list.size();
    }
}

// — копирующий
template <typename T>
matrix<T>::matrix(const matrix<T>& other) : rows_count(other.rows_count),
                                            cols_count(other.cols_count),
                                            data(std::make_unique<T[]>(rows_count* cols_count))
{
    std::copy(other.data.get(),
              other.data.get() + other.rows_count * other.cols_count,
              data.get());
}

// — перемещающий
template <typename T>
matrix<T>::matrix(matrix<T>&& other) noexcept : data(std::move(other.data)),
                                                rows_count(std::exchange(other.rows_count, 0)),
                                                cols_count(std::exchange(other.cols_count, 0)) {}

//— копирующее присваивание
template <typename T>
matrix<T>& matrix<T>::operator=(const matrix<T>& other) {
    if (this == &other) {
        return *this;
    }

    matrix<T> temp_data(other);
    *this = std::move(temp_data);

    return *this;
}

//— перемещающее присваивание
template <typename T>
matrix<T>& matrix<T>::operator=(matrix<T>&& other) noexcept {
    if (this == &other) {
        return *this;
    }

    data = std::move(other.data);

    rows_count = std::exchange(other.rows_count, 0);
    cols_count = std::exchange(other.cols_count, 0);

    return *this;
}

// — доступ только для чтения
template <typename T>
const T& matrix<T>::operator()(size_t row, size_t col) const {
    return data[row * cols_count + col];
}

// — доступ на чтение/запись
template <typename T>
T& matrix<T>::operator()(size_t row, size_t col) {
    return data[row * cols_count + col];
}

// — с проверкой границ (исключение)
template <typename T>
T& matrix<T>::at(size_t row, size_t col) {
    if (row >= rows_count || col >= cols_count) {
        throw std::out_of_range("Индекс вне диапазона!");
    }

    return data[row * cols_count + col];
}
// const — аналогично, константная версия
template <typename T>
const T& matrix<T>::at(size_t row, size_t col) const {
    if (row >= rows_count || col >= cols_count) {
        throw std::out_of_range("Индекс вне диапазона!");
    }

    return data[row * cols_count + col];
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

    if (rows_count == new_rows && cols_count == new_cols) return;

    matrix<T> temp_data(new_rows, new_cols);

    size_t min_rows = std::min(rows_count, new_rows);
    size_t min_cols = std::min(cols_count, new_cols);

    for (size_t i = 0; i < min_rows; ++i) {
        std::copy(data.get() + i * cols_count,
                  data.get() + i * cols_count + min_cols,
                  temp_data.data.get() + i * new_cols);
    }

    *this = std::move(temp_data);
}

// — вставить пустую строку на позицию
template <typename T>
void matrix<T>::insert_row(size_t pos) {
    if (pos > rows_count) {
        throw std::out_of_range("Индекс вне диапазона!");
    }

    matrix<T> temp_data(rows_count + 1, cols_count);

    std::copy(data.get(),
              data.get() + pos * cols_count,
              temp_data.data.get());

    std::copy(data.get() + pos * cols_count,
              data.get() + rows_count * cols_count,
              temp_data.data.get() + (pos + 1) * cols_count);

    *this = std::move(temp_data);
}

// — вставить пустой столбец
template <typename T>
void matrix<T>::insert_col(size_t pos) {
    if (pos > cols_count) {
        throw std::out_of_range("Индекс вне диапазона!");
    }

    matrix<T> temp_data(rows_count, cols_count + 1);

    for (size_t i = 0; i < rows_count; ++i) {
        std::copy(data.get() + i * cols_count,
                  data.get() + i * cols_count + pos,
                  temp_data.data.get() + i * (cols_count + 1));

        std::copy(data.get() + i * cols_count + pos,
                  data.get() + i * cols_count + cols_count,
                  temp_data.data.get() + i * (cols_count + 1) + (pos + 1));
    }

    *this = std::move(temp_data);
}

// — удалить строку
template <typename T>
void matrix<T>::remove_row(size_t pos) {
    if (pos >= rows_count || cols_count == 0) {
        throw std::out_of_range("Индекс вне диапазона!");
    }

    matrix<T> temp_data(rows_count - 1, cols_count);

    std::copy(data.get(),
              data.get() + pos * cols_count,
              temp_data.data.get());

    std::copy(data.get() + (pos + 1) * cols_count,
              data.get() + rows_count * cols_count,
              temp_data.data.get() + pos * cols_count);

    *this = std::move(temp_data);
}

// — удалить столбец
template <typename T>
void matrix<T>::remove_col(size_t pos) {
    if (pos >= cols_count) {
        throw std::out_of_range("Индекс вне диапазона!");
    }

    matrix<T> temp_data(rows_count, cols_count - 1);

    for (size_t i = 0; i < rows_count; ++i) {
        std::copy(data.get() + i * cols_count,
                  data.get() + i * cols_count + pos,
                  temp_data.data.get() + i * (cols_count - 1));

        std::copy(data.get() + i * cols_count + pos + 1,
                  data.get() + i * cols_count + cols_count,
                  temp_data.data.get() + i * (cols_count - 1) + pos);
    }

    *this = std::move(temp_data);
}

// — поменять строки местами строки
template <typename T>
void matrix<T>::swap_rows(size_t r1, size_t r2) {
    if (r1 == r2) return;

    for (size_t i = 0; i < cols_count; ++i) {
        std::swap(at(r1, i), at(r2, i));
    }
}

// — поменять столбцы местами
template <typename T>
void matrix<T>::swap_cols(size_t c1, size_t c2) {
    if (c1 == c2) return;

    for (size_t i = 0; i < rows_count; ++i) {
        std::swap(at(i, c1), at(i, c2));
    }
}

// — заполнить все элементы одним значением
template <typename T>
void matrix<T>::fill(const T& value) {
    std::fill(data.get(), data.get() + rows_count * cols_count, value);
}

// — обнулить
template <typename T>
void matrix<T>::zero() {
    fill(static_cast<T>(0));
}

// — сделать единичную матрицу (для квадратных)
template <typename T>
void matrix<T>::identity() {
    if (rows_count != cols_count) {
        throw std::invalid_argument("Матрица должна быть квадратной");
    }

    zero();

    for (size_t i = 0; i < rows_count; ++i) {
        (*this)(i, i) = static_cast<T>(1);
    }
}

// — сложение с присваиванием
template <typename T>
matrix<T>& matrix<T>::operator+=(const matrix<T>& other) {
    if (rows_count != other.rows_count || cols_count != other.cols_count) {
        throw std::invalid_argument("Матрицы должны быть одинакового размера");
    }

    for (size_t i = 0; i < rows_count * cols_count; ++i) {
        data[i] += other.data[i];
    }

    return *this;
}
// — вычитание с присваиванием
template <typename T>
matrix<T>& matrix<T>::operator-=(const matrix<T>& other) {
    *this += other * static_cast<T>(-1);

    return *this;
}

// — умножение на скаляр
template <typename T>
matrix<T>& matrix<T>::operator*=(const T& scalar) {
    for (size_t i = 0; i < rows_count * cols_count; ++i) {
        data[i] *= scalar;
    }

    return *this;
}

// — умножение на матрицу
template <typename T>
matrix<T>& matrix<T>::operator*=(const matrix<T>& other) {
    if (cols_count != other.rows_count) {
        throw std::invalid_argument("Нельзя умножить: число столбцов первой не равно числу строк второй");
    }

    matrix<T> temp_data(rows_count, other.cols_count);

    // Тройной цикл для умножения
    for (size_t i = 0; i < rows_count; ++i) {
        for (size_t j = 0; j < other.cols_count; ++j) {
            for (size_t k = 0; k < cols_count; ++k) {
                temp_data(i, j) += (*this)(i, k) * other(k, j);
            }
        }
    }

    *this = std::move(temp_data);

    return *this;
}

// — сложение
template <typename T>
matrix<T> matrix<T>::operator+(const matrix<T>& other) const {
    matrix<T> result = *this;
    result += other;

    return result;
}

// — вычитание
template <typename T>
matrix<T> matrix<T>::operator-(const matrix<T>& other) const {
    matrix <T> result = *this;
    result -= other;

    return result;
}

// — умножение матриц
template <typename T>
matrix<T> matrix<T>::operator*(const matrix<T>& other) const {
    matrix<T> result = *this;
    result *= other;

    return result;
}

// — умножение на скаляр
template <typename T>
matrix<T> matrix<T>::operator*(const T& scalar) const {
    matrix<T> result = *this;
    result *= scalar;

    return result;
}

// — транспонирование
template <typename T>
matrix<T> matrix<T>::transpose() const {
    matrix<T> result(cols_count, rows_count);

    for (size_t i = 0; i < rows_count; ++i) {
        for (size_t j = 0; j < cols_count; ++j) {
            result(j, i) = (*this)(i, j);
        }
    }

    return result;
}

// — определитель (для квадратных) мб изменить подход? переосмыслить
template <typename T>
 double matrix<T>::determinant() const {
     if (rows_count != cols_count) {
         throw std::invalid_argument("Матрица должна быть квадратной");
     }

     //копирование данных
     matrix<double> temp_data(rows_count, cols_count);
     for (size_t i = 0; i < rows_count; ++i) {
         for (size_t j = 0; j < cols_count; ++j) {
             temp_data(i, j) = static_cast<double>((*this)(i, j));
         }
     }

     //приводим к треугольнику
     double factor = 1.0;
     for (size_t j = 0; j < cols_count-1; ++ j) {
         size_t count_zero = 1;
         while (temp_data(j, j) == 0) {
             temp_data.swap_rows(j, j + count_zero);
             factor *= -1;
             ++count_zero;
             if (j + count_zero == cols_count) {
                 break;
             }
         }
         if (temp_data(j, j) == 0) {
             continue;
         }
         for (size_t i = 0; i < rows_count-(j+1); ++i) {
             double temp_subtraction = temp_data(i + 1 +j, j) / temp_data(j, j);

             for (size_t k = 0; k < cols_count - j; k++) {
                 temp_data(i + 1 + j, k+j) -= temp_subtraction * temp_data(j, k + j);
             }
         }
     }

     // вычисление детерминанта без учета знака
     double temp_multiplication = 1;
     for (size_t j = 0; j < cols_count; ++j) {
         temp_multiplication *= temp_data(j, j);
     }

     return temp_multiplication*factor;
 }

 // — сравнение
template <typename T>
bool matrix<T>::operator==(const matrix<T>& other) const {
    if (rows_count != other.rows_count || cols_count != other.cols_count) {
        return false;
    }

    for (size_t i = 0; i < rows_count * cols_count; ++i) {
        if (data[i] != other.data[i]) {
            return false;
        }
    }

    return true;
}

// — сравнение
template <typename T>
bool matrix<T>::operator!=(const matrix<T>& other) const {
    return !(*this == other);
}
//end 41