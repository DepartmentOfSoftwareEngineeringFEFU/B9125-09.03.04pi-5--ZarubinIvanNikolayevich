#include <iostream>
#include <ctime>
#include <thread>
#include <chrono>
#include <cstdlib>

using namespace std;
struct spisok {
	char hranit;
	spisok* vpered;
	spisok* nazad;
};

spisok* sozdati(int n);
void MENU(spisok*& mylist);
void delete_all_spisok(spisok*& glava);
void vivod(spisok* glava);
void Chistka_Screen();
void Input(spisok*& mylist);
void Delet(spisok*& mylist);
char char_random();
int Dlina_spiska(spisok* glava);
void delete_all_spaces(spisok*& mylist);



int main() {
	srand(time(0));
	setlocale(LC_ALL, "RU");
	int n = 0;
	spisok* mylist = nullptr;
	MENU(mylist);
	return 1;
}

char char_random() {
	const char simbols[] = "abcdefghijklmnopqrstuvwxyz   ";
	return simbols[rand() % 29];
}

spisok* sozdati(int n) {
	spisok* glava = nullptr;
	spisok* hvost = nullptr;


	for (int i = 0; i < n; i++) {
		spisok* new_elem = new spisok;
		new_elem->hranit = char_random();
		new_elem->vpered = nullptr;
		new_elem->nazad = nullptr;
		if (glava == nullptr) {
			glava = new_elem;
			hvost = new_elem;
		}
		else {
			hvost->vpered = new_elem;
			new_elem->nazad = hvost;
			hvost = new_elem;
		}
	}
	return glava;
}

int Dlina_spiska(spisok* glava) {
	int count_dlina = 0;
	spisok* proverka = glava;
	while (proverka != nullptr) {
		count_dlina++;
		proverka = proverka->vpered;
	}
	return count_dlina;
}


void Chistka_Screen() {
	cin.ignore();
	std::cout << endl << "Чтобы вернуться в меню нажмите на Еnter";
	cin.get();
	system("CLS");
}

void MENU(spisok*& mylist) {
	int g;
	cout << "Выберите число из меню(1-7) затем нажмите ENTER\n";
	cout << "----------------MENU----------------\n";
	cout << "1. Создать список \n2. Вывести список \n3. Удаление всего списка\n4. вставка одного элемента\n5. удаление одного элемента\n6. Удаление всех пробелов\n7. Выход\n";
	cin >> g;
	system("CLS");
	switch (g) {
	case 1:
		if (mylist != nullptr) {
			cout << "Ошибка: удалите предыдущий созданый список\n";
			Chistka_Screen();
			MENU(mylist);
		}
		else {
			int n = 0;
			cout << "Введите количество элемнтов списка: ";
			cin >> n;
			if (n <= 2) {
				cout << "Ошибка: количество элемнтов меньше 2" << endl;
				Chistka_Screen();
				MENU(mylist);
			}
			else {

				mylist = sozdati(n);
				cout << "Список из " << n << " элементов создан" << endl;
				Chistka_Screen();
				MENU(mylist);
			}
		}
	case 2:
		vivod(mylist);
		Chistka_Screen();
		MENU(mylist);
	case 3:
		delete_all_spisok(mylist);
		Chistka_Screen();
		MENU(mylist);
	case 4:
		Input(mylist);
		Chistka_Screen();
		MENU(mylist);
	case 5:
		Delet(mylist);
		Chistka_Screen();
		MENU(mylist);
	case 6:
		if (mylist == nullptr) {
			cout << "Сначала создайте список " << endl;
			Chistka_Screen();
			MENU(mylist);
		}
		else {
			cout << "Исходная последовательность" << endl;
			vivod(mylist);
			delete_all_spaces(mylist);
			cout << "Полученная последовательность" << endl;
			if (mylist == nullptr) {
				cout << "List is empty" << endl;
			}
			else {
				vivod(mylist);
			}
			Chistka_Screen();
			MENU(mylist);
		}
	case 7:
		exit(0);
		break;
	default:
		cout << "Ошибка, неверный символ";
		cout << endl << "Чтобы вернуться в меню нажмите на Еnter";
		cin.clear();
		cin.ignore(1000, '\n');
		cin.get();
		system("CLS");
		MENU(mylist);

	}
}



void vivod(spisok* glava) {
	if (glava == nullptr) {
		cout << "List is Empty";
	}
	else {
		spisok* hvost = glava;
		while (hvost->vpered != nullptr) {
			hvost = hvost->vpered;
		}
		int dlina = Dlina_spiska(glava);

		cout << "От головы к хвосту: " << endl;
		spisok* current = glava;
		if (dlina == 1) {
			cout << current->hranit << endl;
		}
		else {
			while (current != nullptr) {
				cout << current->hranit;
				if (current->vpered != nullptr) {
					cout << " => ";
				}
				current = current->vpered;
			}
			cout << endl;
		}

		cout << "От хвоста к голове: ";
		current = hvost;
		if (dlina == 1) {
			cout << current->hranit << endl;
		}
		else {
			while (current != nullptr) {
				cout << current->hranit;
				if (current->nazad != nullptr) {
					cout << " => ";
				}
				current = current->nazad;
			}
			cout << endl;
		}
	}
}


void delete_all_spisok(spisok*& glava) {
	if (glava == nullptr) {
		cout << "List is empty";
	}
	else {
		spisok* temp;
		while (glava != nullptr) {
			temp = glava;
			glava = glava->vpered;
			delete temp;
		}
		cout << "Список успешно удалён!!!";
	}
}


void Input(spisok*& mylist) {
	if (mylist == nullptr) {
		cout << "List is empty, create list first" << endl;
	}
	else {
		int count_spisok = Dlina_spiska(mylist);
		int vstavka;
		cout << "Введите на какую позицию хотите вставить элемент(1-начало, " << count_spisok << "-конец): ";
		cin >> vstavka;
		if (vstavka < 1) {
			cout << "Ошибка: число должно быть >=1" << endl;
			return;
		}
		if (vstavka > count_spisok + 1) {
			cout << "Ошибка: число должно быть небольше количества строк списка" << endl;
			return;
		}
		spisok* temp = new spisok;
		temp->hranit = char_random();
		temp->vpered = nullptr;
		temp->nazad = nullptr;
		if (vstavka == 1) {
			temp->vpered = mylist;
			if (mylist != nullptr) {
				mylist->nazad = temp;
			}
			mylist = temp;
			cout << "Значение успешно вставлено в начало списка" << endl;
			return;
		}

		spisok* p = mylist;
		for (int i = 1; i < vstavka - 1; i++) {
			p = p->vpered;
		}

		if (p->vpered == nullptr) {
			p->vpered = temp;
			temp->nazad = p;
			cout << "Элемент успешно вставлен в конец списка" << endl;
		}
		else {
			temp->vpered = p->vpered;
			temp->nazad = p;
			p->vpered->nazad = temp;
			p->vpered = temp;
			cout << "Элемент успешно вставлен на позицию " << vstavka << endl;
			return;
		}
	}
}



void Delet(spisok*& mylist) {
	if (mylist == nullptr) {
		cout << "List is empty";
		return;
	}
	int count_spisok = Dlina_spiska(mylist);
	int dela;
	cout << "Введите номер удаленного элемента(1-начало, " << count_spisok << "-конец): ";
	cin >> dela;
	if (dela < 1) {
		cout << "Ошибка: число должно быть >=1" << endl;
		return;
	}
	if (dela > count_spisok + 1) {
		cout << "Ошибка: число должно быть небольше количества строк списка" << endl;
		return;
	}
	spisok* toDelete = nullptr;
	if (dela == 1) {
		toDelete = mylist;
		mylist = mylist->vpered;
		if (mylist != nullptr) {
			mylist->nazad = nullptr;
		}
		cout << "Удален элемент с позиции 1" << endl;
		delete toDelete;
		return;
	}

	spisok* p = mylist;
	for (int i = 1; i < dela; i++) {
		p = p->vpered;
	}
	toDelete = p;

	// последний
	if (toDelete->vpered == nullptr) {
		toDelete->nazad->vpered = nullptr;
	}
	//  из середины
	else {
		toDelete->nazad->vpered = toDelete->vpered;
		toDelete->vpered->nazad = toDelete->nazad;
	}

	cout << "Удален элемент с позиции " << dela << endl;
	delete toDelete;
}


void Delet_Data(spisok*& mylist, int& count_spisok) {
	if (mylist == nullptr) {
		cout << "Список пуст, удаление не требуеться";
		return;
	}
	int data_pole;
	cout << "Введите число от 0 до 100:";
	cin >> data_pole;
	if (data_pole < 0) {
		cout << "ОШИБКА: число < 0";
		return;
	}
	if (data_pole > 100) {
		cout << "ОШИБКА: число > 100";
		return;
	}
	bool found = 0;
	// first element delete
	if (mylist->hranit == data_pole) {
		spisok* temp = mylist;
		mylist = mylist->vpered;
		cout << "Удален элемент с значением " << data_pole << ": [" << temp->hranit << " | " << temp << "]" << endl;
		delete temp;
		count_spisok--;
		found = true;
		return;
	}

	spisok* p = mylist;
	while (p->vpered != nullptr) {
		if (p->vpered->hranit == data_pole) {
			spisok* temp = p->vpered;
			p->vpered = temp->vpered;
			cout << "Элемент удален" << data_pole << ": [" << temp->hranit << " | " << temp << "]" << endl;
			delete temp;
			count_spisok--;
			found = true;
			return;
		}
		p = p->vpered;
	}
	if (!found) {
		cout << "Элемент " << data_pole << "не найден в списке" << endl;
	}
}

void delete_all_spaces(spisok*& mylist) {
	if (mylist == nullptr) {
		cout << "List is empty" << endl;
		return;
	}

	spisok* current = mylist;
	int removedCount = 0;

	while (current != nullptr) {
		spisok* next = current->vpered;

		if (current->hranit == ' ') {
			if (current == mylist) {
				// Удаление головы
				mylist = current->vpered;
				if (mylist != nullptr) {
					mylist->nazad = nullptr;
				}
			}
			else if (current->vpered == nullptr) {
				// Удаление хвоста
				current->nazad->vpered = nullptr;
			}
			else {
				// Удаление из середины
				current->nazad->vpered = current->vpered;
				current->vpered->nazad = current->nazad;
			}
			delete current;
			removedCount++;
		}
		current = next;
	}

	cout << "Удалено пробелов: " << removedCount << endl;
}