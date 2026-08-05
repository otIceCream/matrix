#pragma once
#include <initializer_list>
#include <iostream>
template <typename T>
class matrix {
    static_assert(
        std::is_same_v<T, int> ||
        std::is_same_v<T, double> ||
        std::is_same_v<T, float> ||
        std::is_same_v<T, size_t>
        );
private:
    size_t rows_count;
    size_t cols_count;
    T** data;
public:
    /*-----------------------Конструкторы---------------------------*/
    // — по умолчанию(пустая или нулевая) 3x3 по умолчанию
    matrix();
    // — с размерами(заполнить нулями или неинициализированно)
    matrix(size_t rows, size_t cols);
    //— из списка
    matrix(std::initializer_list<std::initializer_list<T>> list);
    // — копирующий
    matrix(const matrix& other);
    // — перемещающий
    matrix(matrix&& other) noexcept;
    /*-----------------------Деструктор---------------------------*/
    ~matrix();
    /*-----------------------ФУНКЦИИ---------------------------*/

    //— копирующее присваивание
    matrix& operator=(const matrix& other);
    //— перемещающее присваивание
    matrix& operator=(matrix&& other) noexcept;
    // — доступ только для чтения
    const T& operator()(size_t row, size_t col) const;
    // — доступ на чтение/запись
    T& operator()(size_t row, size_t col);
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
    //Доступ к элементам
    // — с проверкой границ (исключение)
    T& at(size_t row, size_t col);
    // const — аналогично, константная версия
    const T& at(size_t row, size_t col) const;
    //Заполнение и инициализация
    // — заполнить все элементы одним значением
    void fill(const T& value);
    // — обнулить (если числовой тип)
    void zero();
    // — сделать единичную матрицу (для квадратных)
    void identity();
    //Математические операции (если добавишь)
    // — сложение с присваиванием
    matrix& operator+=(const matrix& other);
    // — вычитание с присваиванием
    matrix& operator-=(const matrix& other);
    //T&
    // — умножение на скаляр
    matrix& operator*=(const T& scalar);
    // — сложение
    matrix operator+(const matrix& other) const;
    // — вычитание
    matrix operator-(const matrix& other) const;
    // — умножение матриц
    //я бы сам не скоро додумался бы до этого
    matrix operator*(const matrix& other) const;
    // — умножение на скаляр
    matrix operator*(const T& scalar) const;
    // — транспонирование
    matrix transpose() const;
   /* // — определитель (для квадратных) сложна нахуй, а я болею мозга не робить ээээ
    size_t determinant() const ;*/
    //std::ostream& operator<<(std::ostream& os, const matrix& m);
    bool operator==(const matrix& other) const;
    // — сравнение
    bool operator!=(const matrix& other) const;
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const matrix<T>& m);
#include "matrix.tpp"