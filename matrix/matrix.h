#pragma once
#include <iostream>
template <typename T>
class matrix {
    static_assert(
        std::is_same_v<T, int> ||
        std::is_same_v<T, float> ||
        std::is_same_v<T, double> ||
        std::is_same_v<T, size_t>
        );
private:
    size_t rows_count;
    size_t cols_count;
    std::unique_ptr<T[]> data;
public:
    /*-----------------------Конструкторы---------------------------*/
    // — по умолчанию(пустая или нулевая) 2x2 по умолчанию
    matrix();
    // — с размерами(заполнить нулями или неинициализированно)
    matrix(size_t rows, size_t cols);
    //— из списка
    matrix(std::initializer_list<std::initializer_list<T>> list);
    // — копирующий
    matrix(const matrix& other);
    // — перемещающий
    matrix(matrix&& other) noexcept;
    /*-----------------------ФУНКЦИИ---------------------------*/
    //— копирующее присваивание
    matrix& operator=(const matrix& other);
    //— перемещающее присваивание
    matrix& operator=(matrix&& other) noexcept;
    // — доступ только для чтения
    const T& operator()(size_t row, size_t col) const;
    // — доступ на чтение/запись
    T& operator()(size_t row, size_t col);
    // — с проверкой границ (исключение)
    T& at(size_t row, size_t col);
    // const — аналогично, константная версия
    const T& at(size_t row, size_t col) const;
    // — количество строк
    size_t rows() const;
    // — количество столбцов
    size_t cols() const;
    // — общее количество элементов
    size_t size() const;
    // — изменить размер(с потерей данных или с инициализацией)
    void resize(size_t new_rows, size_t new_cols);
    // — вставить пустую строку на позицию
    void insert_row(size_t pos);
    // — вставить пустой столбец
    void insert_col(size_t pos);
    // — удалить строку
    void remove_row(size_t pos);
    // — удалить столбец
    void remove_col(size_t pos);
    // — поменять строки местами строки
    void swap_rows(size_t r1, size_t r2);
    // — поменять столбцы местами
    void swap_cols(size_t c1, size_t c2);
    // — заполнить все элементы одним значением
    void fill(const T& value);
    // — обнулить
    void zero();
    // — сделать единичную матрицу (для квадратных)
    void identity();
    // — сложение с присваиванием
    matrix& operator+=(const matrix& other);
    // — вычитание с присваиванием
    matrix& operator-=(const matrix& other);
    // — умножение на скаляр
    matrix& operator*=(const T& scalar);
    // — умножение на матрицу
    matrix& operator*=(const matrix& other);
    // — сложение
    matrix operator+(const matrix& other) const;
    // — вычитание
    matrix operator-(const matrix& other) const;
    // — умножение матриц
    matrix operator*(const matrix& other) const;
    // — умножение на скаляр
    matrix operator*(const T& scalar) const;
    // — транспонирование
    matrix transpose() const;
    // — определитель (для квадратных)
    double determinant() const ;
    // — сравнение
    bool operator==(const matrix& other) const;
    // — сравнение
    bool operator!=(const matrix& other) const;
    template <typename U>
    friend std::ostream& operator<<(std::ostream& os, const matrix<U>& m);
};

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
#include "matrix.inl"