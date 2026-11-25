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
