#include <fstream>
#include <iomanip>
#include <iostream>
#include <set>
#include <unordered_set>
#include <vector>
#include <windows.h>
#include "student.h"

using namespace std;

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
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

    for (const Student& st : students) {
        sortedStudents.insert(st);
        uniqueStudents.insert(st);
    }

    out << fixed << setprecision(2);

    out << "КОНТЕЙНЕР set (сортировка по среднему баллу):" << endl << endl;
    for (const Student& st : sortedStudents) {
        out << st << endl;
    }

    out << "КОНТЕЙНЕР unordered_set:" << endl << endl;
    for (const Student& st : uniqueStudents) {
        out << st << endl;
    }

    out << "Количество исходных записей: " << students.size() << endl;
    out << "Количество элементов set: " << sortedStudents.size() << endl;
    out << "Количество элементов unordered_set: " << uniqueStudents.size() << endl;

    cout << "Работа завершена. Результаты записаны в output.txt." << endl;
    return 0;
}
