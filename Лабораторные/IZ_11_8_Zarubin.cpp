#include <iostream>
#include <limits>
#include "Head.h"

using namespace std;

void Circle() {
    cout << " –”√" << endl;
    double radius;

    do {
        cout << "¬ведите радиус круга: ";
        cin >> radius;

        if (cin.fail() || radius <= 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "ќшибка! ¬ведите положительное число." << endl;
        }
    } while (cin.fail() || radius <= 0);

    double S = radius * radius * 3.14;
    cout << "ѕлощадь фигуры = " << S << endl;
}

void Paralelogram() {
    cout << "ѕј–јЋЋ≈Ћќ√–јћћ" << endl;
    double storona, visota;

    do {
        cout << "¬ведите сторону параллелограмма: ";
        cin >> storona;

        if (cin.fail() || storona <= 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "ќшибка! ¬ведите положительное число." << endl;
        }
    } while (cin.fail() || storona <= 0);

    do {
        cout << "¬ведите высоту параллелограмма: ";
        cin >> visota;

        if (cin.fail() || visota <= 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "ќшибка! ¬ведите положительное число." << endl;
        }
    } while (cin.fail() || visota <= 0);

    double S = storona * visota;
    cout << "ѕлощадь фигуры = " << S << endl;
}

void Primoyugol() {
    cout << "ѕ–яћќ”√ќЋ№Ќ» " << endl;
    double dlina, shirina;

    do {
        cout << "¬ведите длину пр€моугольника: ";
        cin >> dlina;

        if (cin.fail() || dlina <= 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "ќшибка! ¬ведите положительное число." << endl;
        }
    } while (cin.fail() || dlina <= 0);

    do {
        cout << "¬ведите ширину пр€моугольника: ";
        cin >> shirina;

        if (cin.fail() || shirina <= 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "ќшибка! ¬ведите положительное число." << endl;
        }
    } while (cin.fail() || shirina <= 0);

    double S = dlina * shirina;
    cout << "ѕлощадь фигуры = " << S << endl;
}

void Romb() {
    cout << "–ќћЅ" << endl;
    double diagonal1, diagonal2;

    do {
        cout << "¬ведите первую диагональ ромба: ";
        cin >> diagonal1;

        if (cin.fail() || diagonal1 <= 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "ќшибка! ¬ведите положительное число." << endl;
        }
    } while (cin.fail() || diagonal1 <= 0);

    do {
        cout << "¬ведите вторую диагональ ромба: ";
        cin >> diagonal2;

        if (cin.fail() || diagonal2 <= 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "ќшибка! ¬ведите положительное число." << endl;
        }
    } while (cin.fail() || diagonal2 <= 0);

    double S = (diagonal1 * diagonal2) / 2;
    cout << "ѕлощадь фигуры = " << S << endl;
}

void Sector() {
    cout << "—≈ “ќ–  –”√ј" << endl;
    double radius, angle;

    do {
        cout << "¬ведите радиус сектора: ";
        cin >> radius;

        if (cin.fail() || radius <= 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "ќшибка! ¬ведите положительное число." << endl;
        }
    } while (cin.fail() || radius <= 0);

    do {
        cout << "¬ведите угол сектора (в градусах): ";
        cin >> angle;

        if (cin.fail() || angle <= 0 || angle > 360) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "ќшибка! ¬ведите угол от 1 до 360 градусов." << endl;
        }
    } while (cin.fail() || angle <= 0 || angle > 360);

    double S = 3.14 * radius * radius * angle / 360;
    cout << "ѕлощадь фигуры = " << S << endl;
}

void Trapecia() {
    cout << "“–јѕ≈÷»я" << endl;
    double osnovanie1, osnovanie2, visota;

    do {
        cout << "¬ведите первое основание трапеции: ";
        cin >> osnovanie1;

        if (cin.fail() || osnovanie1 <= 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "ќшибка! ¬ведите положительное число." << endl;
        }
    } while (cin.fail() || osnovanie1 <= 0);

    do {
        cout << "¬ведите второе основание трапеции: ";
        cin >> osnovanie2;

        if (cin.fail() || osnovanie2 <= 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "ќшибка! ¬ведите положительное число." << endl;
        }
    } while (cin.fail() || osnovanie2 <= 0);

    do {
        cout << "¬ведите высоту трапеции: ";
        cin >> visota;

        if (cin.fail() || visota <= 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "ќшибка! ¬ведите положительное число." << endl;
        }
    } while (cin.fail() || visota <= 0);

    double S = ((osnovanie1 + osnovanie2) / 2) * visota;
    cout << "ѕлощадь фигуры = " << S << endl;
}



using namespace std;

void Treugolnic() {
    cout << "“–≈”√ќЋ№Ќ» " << endl;
    double first, second, third;

    do {
        cout << "¬ведите 1-ю сторону треугольника: ";
        cin >> first;

        if (cin.fail() || first <= 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "ќшибка! ¬ведите положительное число." << endl;
        }
    } while (cin.fail() || first <= 0);

    do {
        cout << "¬ведите 2-ю сторону треугольника: ";
        cin >> second;

        if (cin.fail() || second <= 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "ќшибка! ¬ведите положительное число." << endl;
        }
    } while (cin.fail() || second <= 0);

    do {
        cout << "¬ведите 3-ю сторону треугольника: ";
        cin >> third;

        if (cin.fail() || third <= 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "ќшибка! ¬ведите положительное число." << endl;
        }
    } while (cin.fail() || third <= 0);

    if (first + second <= third || first + third <= second || second + third <= first) {
        cout << "ќшибка! “реугольник с такими сторонами не существует." << endl;
    }
    else {
        double poluperimetr = (first + second + third) / 2;
        double S = sqrt(poluperimetr * (poluperimetr - first) *
            (poluperimetr - second) * (poluperimetr - third));
        cout << "ѕлощадь фигуры = " << S << endl;
    }
}