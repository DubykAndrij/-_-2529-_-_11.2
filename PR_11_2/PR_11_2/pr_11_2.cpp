#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <windows.h>

using namespace std;

enum Speciality { COMPUTER_SCIENCE, INFORMATICS, MATH_ECON, PHYS_INF, HANDICRAFT };

string specialityStr[] = {
    "Комп’ютерні науки",
    "Інформатика",
    "Математика та економіка",
    "Фізика та інформатика",
    "Трудове навчання"
};

struct Student {
    char prizv[51];
    int kurs;
    Speciality speciality;
    int physics;
    int math;
    union {
        int programming;
        int numerical_methods;
        int pedagogy;
    };
};

void Create(char* fname);
void Print(char* fname);
void PrintExcellent(char* fname);
double GetPercentageHighAverage(char* fname);

int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    char fname[100];
    cout << "Введіть ім'я файлу: "; cin >> fname;

    int menuItem;
    do {
        cout << "\n--- МЕНЮ ---\n";
        cout << "1. Створити (заповнити) файл\n";
        cout << "2. Вивести файл у вигляді таблиці\n";
        cout << "3. Вивести прізвища відмінників\n";
        cout << "4. Обчислити % студентів із сер. балом > 4.5\n";
        cout << "0. Вихід\n";
        cout << "Вибір: "; cin >> menuItem;

        switch (menuItem) {
        case 1:
            Create(fname);
            break;
        case 2:
            Print(fname);
            break;
        case 3:
            cout << "\nСтуденти-відмінники:\n";
            PrintExcellent(fname);
            break;
        case 4:
            cout << "\nВідсоток студентів із сер. балом > 4.5: "
                << fixed << setprecision(2) << GetPercentageHighAverage(fname) << "%" << endl;
            break;
        }
    } while (menuItem != 0);

    return 0;
}

void Create(char* fname) {
    ofstream f(fname, ios::binary | ios::app);
    char ch;
    int spec;
    Student s;

    do {
        cout << "Прізвище: "; cin >> s.prizv;
        cout << "Курс: "; cin >> s.kurs;
        cout << "Спеціальність (0 - КН, 1 - ІНФ, 2 - МЕ, 3 - ФІ, 4 - ТН): ";
        cin >> spec;
        s.speciality = (Speciality)spec;

        cout << "Оцінка з фізики: "; cin >> s.physics;
        cout << "Оцінка з математики: "; cin >> s.math;

        switch (s.speciality) {
        case COMPUTER_SCIENCE:
            cout << "Оцінка з програмування: "; cin >> s.programming;
            break;
        case INFORMATICS:
            cout << "Оцінка з чисельних методів: "; cin >> s.numerical_methods;
            break;
        default:
            cout << "Оцінка з педагогіки: "; cin >> s.pedagogy;
            break;
        }

        f.write((char*)&s, sizeof(Student));

        cout << "Продовжити введення? (y/n): "; cin >> ch;
    } while (ch == 'y' || ch == 'Y');
}

void Print(char* fname) {
    ifstream f(fname, ios::binary);
    Student s;
    int i = 1;

    cout << "\n==================================================================================================================\n";
    cout << "| № | Прізвище    | Курс | Спеціальність          | Фіз | Мат | Прог | Числ | Пед |\n";
    cout << "------------------------------------------------------------------------------------------------------------------\n";

    while (f.read((char*)&s, sizeof(Student))) {
        cout << "| " << setw(1) << i++ << " | "
            << setw(11) << left << s.prizv << " | "
            << setw(4) << right << s.kurs << " | "
            << setw(22) << left << specialityStr[s.speciality] << " | "
            << setw(3) << s.physics << " | "
            << setw(3) << s.math << " | ";

        if (s.speciality == COMPUTER_SCIENCE)
            cout << setw(4) << s.programming << " |      |      |";
        else if (s.speciality == INFORMATICS)
            cout << "     | " << setw(4) << s.numerical_methods << " |      |";
        else
            cout << "     |      | " << setw(4) << s.pedagogy << " |";

        cout << endl;
    }
    cout << "==================================================================================================================\n";
}

void PrintExcellent(char* fname) {
    ifstream f(fname, ios::binary);
    Student s;
    bool found = false;

    while (f.read((char*)&s, sizeof(Student))) {
        int thirdGrade;
        if (s.speciality == COMPUTER_SCIENCE) thirdGrade = s.programming;
        else if (s.speciality == INFORMATICS) thirdGrade = s.numerical_methods;
        else thirdGrade = s.pedagogy;

        if (s.physics == 5 && s.math == 5 && thirdGrade == 5) {
            cout << s.prizv << endl;
            found = true;
        }
    }
    if (!found) cout << "Відмінників не знайдено." << endl;
}

double GetPercentageHighAverage(char* fname) {
    ifstream f(fname, ios::binary);
    Student s;
    int total = 0;
    int countHigh = 0;

    while (f.read((char*)&s, sizeof(Student))) {
        total++;
        int thirdGrade;
        if (s.speciality == COMPUTER_SCIENCE) thirdGrade = s.programming;
        else if (s.speciality == INFORMATICS) thirdGrade = s.numerical_methods;
        else thirdGrade = s.pedagogy;

        double avg = (s.physics + s.math + thirdGrade) / 3.0;
        if (avg > 4.5) {
            countHigh++;
        }
    }

    if (total == 0) return 0.0;
    return (double)countHigh / total * 100.0;
}