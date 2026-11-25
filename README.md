# ОТЧЁТ

**по лабораторной работе № 5 по курсу «Алгоритмизация и программирование»**

**Тема работы:** Использование объектов своих классов в упорядоченных и неупорядоченных контейнерах библиотеки STL.

**Вариант:** № 11

**Выполнил:** студент группы ИУ8-14/6, Калявкин Родион Алексеевич  
**Проверил:** преподаватель Барыкин Дмитрий Васильевич

Москва, МГТУ — 2026

---

## 1. ЦЕЛЬ РАБОТЫ

Изучение использования объектов собственного класса в упорядоченных и неупорядоченных контейнерах стандартной библиотеки шаблонов STL. Получение навыков добавления объектов в контейнеры `set` и `unordered_set`, задания порядка сравнения объектов и реализации функции хеширования.

## 2. УСЛОВИЕ ЗАДАЧИ

Для класса `Student`, разработанного в лабораторной работе № 4, необходимо обеспечить возможность добавления объектов в контейнер `set` с сортировкой по среднему баллу, как в задании на ЛР4, и в контейнер `unordered_set`.

Исходные данные, как и в ЛР4, необходимо считывать из файла. Для контроля требуется вывести на печать объекты обоих контейнеров.

## 3. ОПИСАНИЕ РЕАЛИЗАЦИИ

Класс `Student` содержит следующие поля:

- `fio` — ФИО студента;
- `group` — учебная группа;
- `recordBook` — номер зачетной книжки;
- `grades` — массив из четырех оценок.

Метод `average()` вычисляет средний балл студента.

Для контейнера `set` используется перегруженная операция `operator<`. Сначала объекты сравниваются по среднему баллу, а при совпадении среднего балла — по остальным полям: ФИО, группе, номеру зачетной книжки и оценкам. Это позволяет сохранять разных студентов с одинаковым средним баллом.

Для контейнера `unordered_set` реализована операция `operator==`, которая сравнивает все поля двух объектов.

Также реализована структура `StudentHash`, вычисляющая хеш по тем же полям объекта. Благодаря этому соблюдается необходимое условие: равные объекты должны иметь одинаковое хеш-значение.

Операция `operator>>` считывает данные студента из файла `input.txt`, а перегруженная операция `operator<<` используется для вывода информации об объектах.

В `main.cpp` данные сначала считываются во вспомогательный контейнер `vector<Student>`, после чего каждый объект добавляется в `set<Student>` и `unordered_set<Student, StudentHash>`.

Контейнер `set` автоматически располагает элементы в упорядоченном виде согласно операции сравнения. Контейнер `unordered_set` использует хеширование, поэтому порядок обхода его элементов не определен.

## 4. ТЕКСТ ПРОГРАММЫ С КОММЕНТАРИЯМИ

### 4.1. Файл `student.h`

```cpp
#ifndef STUDENT_H
#define STUDENT_H

#include <array>
#include <iostream>
#include <string>

using namespace std;

// Класс для хранения информации о студенте
class Student {
private:
    string fio;                 // Фамилия, имя и отчество
    string group;               // Учебная группа
    int recordBook;             // Номер зачетной книжки
    array<int, 4> grades;       // Оценки за четыре экзамена

public:
    Student();
    Student(const string& f, const string& g, int r, const array<int, 4>& a);

    // Все поля самостоятельно управляют своими ресурсами,
    // поэтому достаточно стандартных операций копирования и перемещения.
    Student(const Student& s) = default;
    Student(Student&& s) noexcept = default;
    Student& operator=(const Student& s) = default;
    Student& operator=(Student&& s) noexcept = default;

    double average() const;

    // Для set порядок определяется средним баллом.
    bool operator<(const Student& s) const;

    // Для unordered_set сравниваются все поля объекта.
    bool operator==(const Student& s) const;

    friend struct StudentHash;
    friend ostream& operator<<(ostream& os, const Student& s);
    friend istream& operator>>(istream& is, Student& s);
};

struct StudentHash {
    size_t operator()(const Student& s) const;
};

#endif
```

### 4.2. Файл `student.cpp`

```cpp
#include "student.h"

#include <functional>
#include <sstream>
#include <tuple>
#include <utility>

using namespace std;

// Создаем студента с пустыми данными
Student::Student() : fio(""), group(""), recordBook(0), grades{} {}

// Создаем студента с заданными данными
Student::Student(const string& f, const string& g, int r, const array<int, 4>& a)
    : fio(f), group(g), recordBook(r), grades(a) {}

// Вычисляем средний балл по четырем оценкам
double Student::average() const {
    int sum = 0;

    for (int grade : grades) {
        sum += grade;
    }

    return static_cast<double>(sum) / grades.size();
}

// Сравниваем студентов по среднему баллу
bool Student::operator<(const Student& s) const {
    // Сначала сравниваем средний балл, затем остальные поля.
    // Это сохраняет разных студентов с одинаковым средним баллом в set.
    if (average() != s.average()) {
        return average() < s.average();
    }

    return tie(fio, group, recordBook, grades) <
           tie(s.fio, s.group, s.recordBook, s.grades);
}

// Сравниваем все поля студентов
bool Student::operator==(const Student& s) const {
    return fio == s.fio &&
           group == s.group &&
           recordBook == s.recordBook &&
           grades == s.grades;
}

// Выводим данные студента в поток
ostream& operator<<(ostream& os, const Student& s) {
    os << "ФИО: " << s.fio << endl;
    os << "Группа: " << s.group << endl;
    os << "Номер зачетной книжки: " << s.recordBook << endl;
    os << "Оценки: ";

    for (int grade : s.grades) {
        os << grade << " ";
    }

    os << endl;
    os << "Средний балл: " << s.average() << endl;

    return os;
}

// Считываем одну запись из строки файла.
// Формат: ФИО;группа;номер_зачетной_книжки;оценка1 оценка2 оценка3 оценка4
istream& operator>>(istream& is, Student& s) {
    string line;

    if (!getline(is >> ws, line)) {
        return is;
    }

    stringstream ss(line);
    string f, g, record, gradeLine;

    if (!getline(ss, f, ';') || !getline(ss, g, ';') ||
        !getline(ss, record, ';') || !getline(ss, gradeLine)) {
        is.setstate(ios::failbit);
        return is;
    }

    int r;
    stringstream recordStream(record);

    if (!(recordStream >> r)) {
        is.setstate(ios::failbit);
        return is;
    }

    array<int, 4> a{};
    stringstream gradeStream(gradeLine);

    for (int i = 0; i < 4; i++) {
        if (!(gradeStream >> a[i]) || a[i] < 2 || a[i] > 5) {
            is.setstate(ios::failbit);
            return is;
        }
    }

    // Сначала заполняем временный объект, затем присваиваем его результату.
    Student tmp(f, g, r, a);
    s = move(tmp);

    return is;
}

// Вычисляем хеш по всем полям студента
size_t StudentHash::operator()(const Student& s) const {
    size_t h = hash<string>()(s.fio);
    h ^= hash<string>()(s.group) + 0x9e3779b9 + (h << 6) + (h >> 2);
    h ^= hash<int>()(s.recordBook) + 0x9e3779b9 + (h << 6) + (h >> 2);

    for (int grade : s.grades) {
        h ^= hash<int>()(grade) + 0x9e3779b9 + (h << 6) + (h >> 2);
    }

    return h;
}
```

### 4.3. Файл `main.cpp`

```cpp
#include <fstream>
#include <iomanip>
#include <iostream>
#include <set>
#include <unordered_set>
#include <vector>

#include "student.h"

using namespace std;

int main() {
    // Открываем файлы для чтения и записи
    ifstream in("input.txt");
    ofstream out("output.txt");

    if (!in || !out) {
        cout << "Ошибка открытия файлов!" << endl;
        return 1;
    }

    vector<Student> students;
    Student s;

    // Считываем исходные данные в промежуточный vector
    while (in >> s) {
        students.push_back(s);
    }

    if (!in.eof()) {
        cout << "Ошибка чтения данных из файла!" << endl;
        out << "Ошибка чтения данных из файла!" << endl;
        return 1;
    }

    if (students.empty()) {
        cout << "Входной файл не содержит данных." << endl;
        out << "Входной файл не содержит данных." << endl;
        return 1;
    }

    // Создаем упорядоченный и неупорядоченный контейнеры
    set<Student> sortedStudents;
    unordered_set<Student, StudentHash> uniqueStudents;

    // Добавляем объекты в оба контейнера
    for (const Student& st : students) {
        sortedStudents.insert(st);
        uniqueStudents.insert(st);
    }

    out << fixed << setprecision(2);

    // Выводим элементы контейнера set
    out << "КОНТЕЙНЕР set (сортировка по среднему баллу):" << endl << endl;

    for (const Student& st : sortedStudents) {
        out << st << endl;
    }

    // Выводим элементы контейнера unordered_set
    out << "КОНТЕЙНЕР unordered_set:" << endl << endl;

    for (const Student& st : uniqueStudents) {
        out << st << endl;
    }

    // Выводим количество элементов
    out << "Количество исходных записей: " << students.size() << endl;
    out << "Количество элементов set: " << sortedStudents.size() << endl;
    out << "Количество элементов unordered_set: " << uniqueStudents.size() << endl;

    cout << "Работа завершена. Результаты записаны в output.txt." << endl;

    return 0;
}
```

## 5. КОНТРОЛЬНЫЙ ПРИМЕР И РЕЗУЛЬТАТЫ РАБОТЫ

Для проверки используется тот же набор из шести студентов, что и в лабораторной работе № 4.

Содержимое файла `input.txt`:

```text
Иванов Иван Иванович;ИУ8-14/6;240011;3 3 4 3
Петров Петр Сергеевич;ИУ8-14/6;240012;3 4 3 4
Сидорова Анна Павловна;ИУ8-14/6;240013;4 4 4 4
Кузнецов Дмитрий Олегович;ИУ8-14/6;240014;4 4 5 4
Смирнова Мария Андреевна;ИУ8-14/6;240015;5 4 5 5
Орлов Максим Игоревич;ИУ8-14/6;240016;5 5 5 5
```

Средние баллы студентов:

| Студент | Средний балл |
|---|---:|
| Иванов Иван Иванович | 3,25 |
| Петров Петр Сергеевич | 3,50 |
| Сидорова Анна Павловна | 4,00 |
| Кузнецов Дмитрий Олегович | 4,25 |
| Смирнова Мария Андреевна | 4,75 |
| Орлов Максим Игоревич | 5,00 |

После запуска программы в `output.txt` последовательно выводятся элементы контейнера `set`, элементы контейнера `unordered_set`, а также количество исходных записей и количество элементов в каждом контейнере.

В контейнере `set` студенты располагаются по возрастанию среднего балла.

В контейнере `unordered_set` находятся те же объекты, однако порядок их вывода может отличаться. Это связано с тем, что данный контейнер использует хеширование и не поддерживает сортировку элементов.

Для контрольного набора программа выводит следующие количества:

```text
Количество исходных записей: 6
Количество элементов set: 6
Количество элементов unordered_set: 6
```

В данном примере все студенты различаются по своим данным, поэтому оба контейнера содержат по шесть элементов.

## 6. ВЫВОД

В ходе лабораторной работы изучено использование объектов собственного класса в упорядоченных и неупорядоченных контейнерах стандартной библиотеки STL.

Для класса `Student` реализована возможность добавления объектов в контейнеры `set` и `unordered_set`. Для `set` задан порядок сравнения по среднему баллу с дополнительным сравнением остальных полей при совпадении среднего балла. Для `unordered_set` реализованы операция сравнения объектов по всем полям и функция хеширования.

Исходные данные считываются из файла `input.txt`, а содержимое контейнеров и количество элементов выводятся в файл `output.txt`.

Поставленные задачи выполнены, программа проверена на контрольных данных.

## 7. СБОРКА И ЗАПУСК ПРОГРАММЫ

Проект собирается с использованием CMake и стандарта C++11.

Для запуска в CLion необходимо открыть папку проекта, дождаться загрузки CMake и запустить конфигурацию `Lab5_Variant11`.

Рабочая директория должна быть установлена в корень проекта (`$PROJECT_DIR$`), чтобы программа могла найти файл `input.txt`.

Сборка из терминала:

```bash
cmake -S . -B build
cmake --build build
```

После запуска результаты записываются в файл `output.txt`.

## 8. СОСТАВ ПРОЕКТА

| Файл | Назначение |
|---|---|
| `main.cpp` | Основная функция программы |
| `student.h` | Объявление класса `Student` |
| `student.cpp` | Реализация методов и перегруженных операций класса |
| `input.txt` | Исходные данные |
| `output.txt` | Результаты работы программы (создается при запуске) |
| `CMakeLists.txt` | Настройки сборки проекта |
