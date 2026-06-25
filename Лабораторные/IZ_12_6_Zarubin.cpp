#include <iostream>
#include <climits>

using namespace std;
unsigned int create_mask() {
	return 0x0000FF00;
}

void print_bin(unsigned int x) {
	int bits = sizeof(unsigned int) * 8;
	for (int i = bits - 1; i >= 0; i--) {
		cout << ((x >> i) & 1);
		if (i % 4 == 0 && i != 0) cout << " ";
	}
}

int main() {
	setlocale(LC_ALL, "RU");
	unsigned int F;
	
	cout << "Введите число информацию о студенте:";
	cin >> F;

	if (cin.fail()) {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');  // очищаем буфер ввода
		cout << "ОШИБКА: Необходимо ввести целое число!\n";
	}
	else {

		
		// Код переводим в число
		unsigned int MASK_KOD = create_mask(); 
		unsigned int kod = (F & MASK_KOD) >> 8;
		print_bin(kod);
		bool poll = (F >> 7) & 1; // 7 бит
		bool obshaga = (F >> 6) & 1; // 6 бит и. т. д
		bool zachislenie_po_naboru = (F >> 5) & 1;
		bool zachislenie_po_konkursu = (F >> 4) & 1;
		bool zachislenie_po_spesial = (F >> 3) & 1;
		bool dopball = (F >> 2) & 1;
		bool original_docks = (F >> 1) & 1;
		bool without_EGE = (F >> 0) & 0;

		cout << "Код аббитуреента: " << kod << endl;
		cout << "Пол: " << (poll ? "Мужской" : "Женский") << endl;
		cout << "Нужно ли Общежите: " << (obshaga ? "Да" : "Нет") << endl;
		cout << "Зачислен по целевому набору: " << (zachislenie_po_naboru ? "Да" : "Нет") << endl;
		cout << "Зачислен по общему конкурсу: " << (zachislenie_po_konkursu ? "Бюджеи" : "Догово") << endl;
		cout << "Зачислен по особой квоте: " << (zachislenie_po_spesial ? "Да" : "Нет") << endl;
		cout << "Наличие доп баллов: " << (dopball ? "Есть" : "Нет") << endl;
		cout << "Наличие оригинала документов: " << (original_docks ? "Есть" : "Нет") << endl;
		cout << "Поступление без ЕГЭ: " << (without_EGE ? "Да" : "Нет") << endl;

		if (zachislenie_po_konkursu == 0) {
			cout << "Студенту на договорной основе нужна ли общага: " << (obshaga ? "Нужна" : "Не нужна") << endl;
		}

		if (zachislenie_po_spesial == 1) {
			cout << "Имеет ли доп баллы аббитуриент, зачисленный по особой квоте: " << (dopball ? "Есть" : "Нет") << endl;
		}
	}
	return 0;

}

