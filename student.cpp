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
