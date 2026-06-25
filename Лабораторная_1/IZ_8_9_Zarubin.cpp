#include <iostream>
#include <set>
#include <string>
#include <sstream>
#include <fstream>
#include <limits> // Для numeric_limits


using namespace std;
const int all = 100;
string alph = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

struct Name_elem {
	string el;
	double mass;
	string provod;
};
struct Change_elem {
	string elem;
	double mass;
};
set <string> provod = { "conductor", "semiconductor", "insulator" };


bool Proverka();
void Zapol_Name_elem(Name_elem go[], int al);
int Count_provod(Name_elem begin[], int al, string index_set);
void provodochki(Name_elem begin[], Change_elem go[], int al, string index_set);
void File_Output_1(Change_elem go[], int count);
void Sortirovka(Change_elem go[], int count);
void File_Output_2(Change_elem go[], int count);
bool CheckName(const string& name);
bool CheckWeight(const string& weightStr, double& weight);


int main() {
	setlocale(LC_ALL, "RU");
	bool proverka = Proverka();

	if (proverka == false) cout << "Ошибка в чтение файла";
	else {
		Name_elem All[all] = {};
		Zapol_Name_elem(All, all);
		int count_polu = Count_provod(All, all, "semiconductor");
		int count_p = Count_provod(All, all, "conductor");

		Change_elem prov[all] = {};
		Change_elem polu[all] = {};
		provodochki(All, prov, all, "conductor");
		provodochki(All, polu, all, "semiconductor");
		File_Output_1(polu, count_polu);
		Sortirovka(prov, count_p);
		File_Output_2(prov, count_p);
	}
	return 0;
}


bool Proverka() {
	ifstream file("INPUT.txt");
	if (!file.is_open()) {
		return false;
	}
	file.close();
	return true;
}

bool CheckName(const string& name) {
	if (name.empty()) return false;
	for (int i = 0; i < name.length(); i++) {
		char c = name[i];
		bool found = false;
		for (int j = 0; j < alph.length(); j++) {
			if (c == alph[j]) {
				found = true;
				break;
			}
		}
		if (!found) {
			return false;
		}
	}
	return true;
}

bool CheckWeight(const string& weightStr, double& weight) {
	istringstream iss(weightStr);
	iss >> weight;
	if (iss.fail()) {
		return false;
	}

	if (weight <= 0) return false;

	return true;
}

void Zapol_Name_elem(Name_elem go[], int al) {
	ifstream in_file("INPUT.txt");
	string line;
	int start = 0;
	while (getline(in_file, line) && start < al) {
		istringstream iz(line);
		Name_elem n;
		string massStr;
		if (iz >> n.el >> massStr >> n.provod) {
			double massValue;

			if (!CheckName(n.el)) {
				cout << "Ошибка: некорректное название вещества " << endl;
				continue;
			}
			if (!CheckWeight(massStr, massValue)) {
				cout << "Ошибка: Это не число или число <= 0!"<< endl;
				continue;
			}
			n.mass = massValue;

			if (provod.find(n.provod) != provod.end()) {
				go[start] = n;
				start++;
			}
			else {
				cout << "Ошибка: некорректная проводимость" << endl;
			}
		}
		else {
			cout << "Ошибка: некорректный формат строки" << endl;
		}
	}
	in_file.close();
}

int Count_provod(Name_elem begin[], int al, string index_set) {
	int count = 0;
	for (int i = 0; i < al; i++) {
		if (begin[i].provod == index_set) {
			count++;
		}
	}
	return count;
}

void provodochki(Name_elem begin[], Change_elem go[], int al, string index_set) {
	int j = 0;
	for (int i = 0; i < al; i++) {
		if (begin[i].provod == index_set) {
			go[j].elem = begin[i].el;
			go[j].mass = begin[i].mass;
			j++;
		}
	}
}

void File_Output_1(Change_elem go[], int count) {
	ofstream out_file("OUTPUT_1.txt");
	if (count == 0) out_file << "There were no semiconductors in the source file";
	else {
		for (int i = 0; i < count; i++) {
			out_file << go[i].elem << " " << go[i].mass << endl;
		}
	}
	out_file.close();
}

void File_Output_2(Change_elem go[], int count) {
	ofstream out_file("OUTPUT_2.txt");
	if (count == 0) out_file << "There were no conductors in the source file";
	else {
		for (int i = 0; i < count; i++) {
			out_file << go[i].elem << " " << go[i].mass << endl;
		}
	}
	out_file.close();
}

void Sortirovka(Change_elem go[], int count) {
	int max;
	double temp;
	string line;
	for (int i = 0; i < count - 1; i++) {
		max = i;
		for (int j = i + 1; j < count; j++) {
			if (go[j].mass >= go[max].mass)
				max = j;
		}
		if (max != i) {
			temp = go[i].mass;
			line = go[i].elem;

			go[i].mass = go[max].mass;
			go[i].elem = go[max].elem;

			go[max].mass = temp;
			go[max].elem = line;
		}
	}


}