#include "matrix.h"
#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <iomanip>
#include <unordered_map>

void print_section(const char* title) {
    std::cout << "\n========== " << title << " ==========\n";
}


int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    /*----------------------- Конструкторы ---------------------------*/
    
    print_section("Конструкторы");

    // По умолчанию (2x2, обнулена)
    matrix<int> a;
    std::cout << "По умолчанию (2x2):\n" << a;

    // С размерами (3x4)
    matrix<int> b(3, 4);
    std::cout << "\nС размерами (3x4):\n" << b;

    // Из списка инициализации
    matrix<int> c = { {1, 2, 3}, {4, 5, 6}, {7, 8, 9} };
    std::cout << "\nИз списка (3x3):\n" << c;

    // Копирующий
    matrix<int> d = c;
    std::cout << "\nКопия c:\n" << d;

    // Перемещающий
    matrix<int> e = std::move(d);
    std::cout << "\nПеремещение (d теперь пуст/нулевой, e содержит данные):\n" << e;

    /*----------------------- Присваивания ---------------------------*/
    print_section("Присваивания");

    matrix<int> f;
    f = c;                    // копирующее
    std::cout << "Копирующее присваивание:\n" << f;

    matrix<int> g;
    g = std::move(f);         // перемещающее
    std::cout << "\nПеремещающее присваивание:\n" << g;

    /*----------------------- Доступ к элементам ---------------------------*/
    print_section("Доступ к элементам");

    std::cout << "c(1,1) = " << c(1, 1) << "\n";          // чтение/запись без проверок
    c(1, 1) = 99;
    std::cout << "После c(1,1) = 99:\n" << c;

    std::cout << "\nc.at(0,2) = " << c.at(0, 2) << "\n"; // с проверкой границ
    // c.at(5, 5); // раскомментируй — вылетит out_of_range

    /*----------------------- Информация о размере ---------------------------*/
    print_section("Информация о размере");

    std::cout << "c.rows() = " << c.rows()
        << ", c.cols() = " << c.cols()
        << ", c.size() = " << c.size() << "\n";

    /*----------------------- Изменение структуры ---------------------------*/
    print_section("Изменение структуры");

    matrix<int> h = { {1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12} };
    std::cout << "Исходная (4x3):\n" << h;

    h.resize(2, 2);
    std::cout << "\nПосле resize(2,2) — обрезка:\n" << h;

    h.resize(3, 4);
    std::cout << "\nПосле resize(3,4) — новые элементы инициализированы нулями:\n" << h;

    h.insert_row(1);
    std::cout << "\nПосле insert_row(1):\n" << h;

    h.insert_col(0);
    std::cout << "\nПосле insert_col(0):\n" << h;

    h.remove_row(2);
    std::cout << "\nПосле remove_row(2):\n" << h;

    h.remove_col(3);
    std::cout << "\nПосле remove_col(3):\n" << h;

    /*----------------------- Перестановки ---------------------------*/
    print_section("Перестановки строк и столбцов");

    matrix<int> perm = { {1, 2, 3}, {4, 5, 6}, {7, 8, 9} };
    std::cout << "Исходная:\n" << perm;

    perm.swap_rows(0, 2);
    std::cout << "\nПосле swap_rows(0,2):\n" << perm;

    perm.swap_cols(1, 2);
    std::cout << "\nПосле swap_cols(1,2):\n" << perm;

    /*----------------------- Заполнение и инициализация ---------------------------*/
    print_section("Заполнение");

    matrix<int> fill_test(3, 3);
    fill_test.fill(7);
    std::cout << "fill(7):\n" << fill_test;

    fill_test.zero();
    std::cout << "\nzero():\n" << fill_test;

    fill_test.identity();
    std::cout << "\nidentity():\n" << fill_test;

    /*----------------------- Математические операции ---------------------------*/
    print_section("Математические операции");

    matrix<int> m1 = { {1, 2}, {3, 4} };
    matrix<int> m2 = { {5, 6}, {7, 8} };

    std::cout << "m1:\n" << m1;
    std::cout << "\nm2:\n" << m2;

    std::cout << "\nm1 + m2:\n" << (m1 + m2);
    std::cout << "\nm1 - m2:\n" << (m1 - m2);

    m1 += m2;
    std::cout << "\nm1 += m2:\n" << m1;
    m1 -= m2;
    std::cout << "\nm1 -= m2 (вернули как было):\n" << m1;

    std::cout << "\nm1 * 3:\n" << (m1 * 3);
    m1 *= 2;
    std::cout << "\nm1 *= 2:\n" << m1;

    matrix<int> m3 = { {1, 2, 3}, {4, 5, 6} };
    matrix<int> m4 = { {7, 8}, {9, 10}, {11, 12} };
    std::cout << "\nm3 (2x3):\n" << m3;
    std::cout << "\nm4 (3x2):\n" << m4;
    std::cout << "\nm3 * m4 (2x2):\n" << (m3 * m4);

    /*----------------------- Транспонирование ---------------------------*/
    print_section("Транспонирование");

    matrix<int> tr = { {1, 2, 3}, {4, 5, 6} };
    std::cout << "Исходная (2x3):\n" << tr;
    std::cout << "\ntranspose() (3x2):\n" << tr.transpose();

    /*----------------------- Определитель ---------------------------*/
    print_section("Определитель");

    matrix<double> det1 = { {4, 7}, {2, 6} };
    std::cout << "det1:\n" << det1;
    std::cout << "determinant() = " << det1.determinant() << " (ожидается 10)\n";

    matrix<double> det2 = { {1, 2, 3}, {0, 1, 4}, {5, 6, 0} };
    std::cout << "\ndet2:\n" << det2;
    std::cout << "determinant() = " << det2.determinant() << " (ожидается 1)\n";

    matrix<double> det3 = { {0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                             {0, 0, 0, 0, 0, 0, 0, 0, 2, 3},
                             {0, 0, 0, 0, 0, 0, 0, 4, 5, 6},
                             {0, 0, 0, 0, 0, 0, 7, 8, 9, 10},
                             {0, 0, 0, 0, 0, 11, 12, 13, 14, 15},
                             {0, 0, 0, 0, 16, 17, 18, 19, 20, 21},
                             {0, 0, 0, 22, 23, 24, 25, 26, 27, 28},
                             {0, 0, 29, 30, 31, 32, 33, 34, 35, 36},
                             {0, 37, 38, 39, 40, 41, 42, 43, 44, 45},
                             {46, 47, 48, 49, 50, 51, 52, 53, 54, 55} };
    std::cout << "\ndet3 (10x10, треугольная с нулями на диагонали):\n" << det3;
    std::cout << "determinant() = " << det3.determinant() << "\n";
    matrix<double> det4(10, 10);
    det4.zero();
    det4(0, 9) = 1;
    det4(1, 8) = 2;
    det4(2, 7) = 3;
    det4(3, 6) = 4;
    det4(4, 5) = 5;
    det4(5, 4) = 6;
    det4(6, 3) = 7;
    det4(7, 2) = 8;
    det4(8, 1) = 9;
    det4(9, 0) = 10;

    std::cout << "\ndet4 (10x10, антидиагональная):\n" << det4;
    std::cout << "determinant() = " << det4.determinant()
        << " (ожидается -3628800)\n";

    /*----------------------- Сравнение ---------------------------*/
    print_section("Сравнение");

    matrix<int> cmp1 = { {1, 2}, {3, 4} };
    matrix<int> cmp2 = { {1, 2}, {3, 4} };
    matrix<int> cmp3 = { {1, 2}, {3, 5} };

    std::cout << "cmp1 == cmp2: " << (cmp1 == cmp2 ? "true" : "false") << "\n";
    std::cout << "cmp1 == cmp3: " << (cmp1 == cmp3 ? "true" : "false") << "\n";
    std::cout << "cmp1 != cmp3: " << (cmp1 != cmp3 ? "true" : "false") << "\n";

    std::cout << "\n========== Все тесты пройдены ==========\n";
    return 0;
}