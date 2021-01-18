#include <iostream>
#include <string>
#include <windows.h>

using namespace std;


int* ipCheck(string IP) {

	cout << "----------IP----------" << endl;
	string buff;
	int* ipOct = new int[4];
	int j = 0;
	for (int i = 0, dot = 0; dot < 4 ; i++) {
		if (IP[i] != '.') {
			buff += IP[i];
		}
		else {
			ipOct[j] = stoi(buff);
			j++;
			buff = "";
			dot++;
		}
	}

	for (int i = 0; i < 4; i++) {
		cout << "Октет " << i + 1 << ", значение " << ipOct[i] << endl;
	}

	for (int i = 0; i < 4; i++) {
		if (ipOct[i] > 255 || ipOct[i] < 0) {
			cout << "Ошибка! Октет " << i + 1 << " не входит в диапазон(0;255)! Значение октета " << ipOct[i] << "!" << endl;
			exit(0);
		}
	}

	return ipOct;
}


int* maskCheck(string MASK) {

	cout << "----------МАСКА----------" << endl;
	string buff;
	int* maskOct = new int[4];
	int j = 0;
	for (int i = 0, dot = 0; dot < 4; i++) {
		if (MASK[i] != '.') {
			buff += MASK[i];
		}
		else {
			maskOct[j] = stoi(buff);
			j++;
			buff = "";
			dot++;
		}
	}

	for (int i = 0; i < 4; i++) {
		cout << "Октет " << i + 1 << ", значение " << maskOct[i] << endl;
	}

	for (int i = 0; i < 4; i++) {
		if (maskOct[i] > 255 || maskOct[i] < 0) {
			cout << "Ошибка! Октет " << i + 1 << " не входит в диапазон(0;255)! Значение октета " << maskOct[i] << "!" << endl;
			exit(0);
		}
	}

	//ПОСЛЕДОВАТЕЛЬНОСТЬ БИТ
	int a, i, bitBuff[99];
	int numOf = 0;
	for (int bitP = 0; bitP < 4; bitP++) {
		string BinNum = "";
		//cout << "Введите число: " << endl;
		//cin >> a;
		a = maskOct[bitP];
		i = 0;
		while (a != 0)
		{
			if (a % 2 == 0)
			{
				bitBuff[i] = 0;
				a = a / 2;
			}
			else
			{
				bitBuff[i] = 1;
				a = a / 2;
			}
			i++;
		}
		i = i - 1;
		for (int j = 0; i >= 0; i--, j++) {
			BinNum += to_string(bitBuff[i]);
		}
		if (BinNum == "") {
			cout << "00000000" << '.';
		}
		else
		cout << BinNum << '.';

		if (BinNum == "" || BinNum == "10000000" || BinNum == "11000000" || BinNum == "11100000" || BinNum == "11110000" || BinNum == "11111000" || BinNum == "11111100" || BinNum == "11111110" || BinNum == "11111111") {
			numOf += 1;
		}
	}
	if (numOf == 4) {
		cout << "\nПоследовательность соблюдается!" << endl;
	}
	else
		cout << "\nПоследовательность не соблюдается!" << endl;
	return maskOct;
}


int* NetworkID(int* IP, int* MASK) {
	int* subnet = new int[4];
	cout << "Network ID: \n\t";
	for (int i = 0; i < 4; i++) {
		subnet[i] = IP[i] & MASK[i];
		cout << subnet[i] << ".";
	}
	return subnet;
}


void HostID(int* IP, int* MASK) {
	int* host = new int[4];
	cout << "Host ID: \n\t";
	for (int i = 0; i < 4; i++) {
		host[i] = IP[i] & ~MASK[i];
		cout << host[i] << ".";
	}
}

void Broadcast(int* subnet, int* MASK) {
	int* broadcast = new int[4];
	cout << "Broadcast: \n\t";
	for (int i = 0; i < 4; i++) {

		broadcast[i] = subnet[i] | ~MASK[i];
		cout << broadcast[i] << ".";
	}
}

/*void toBin() {
	int a, i, buff[99];
	string BinNum = "";
	cout << "Введите число: " << endl;
	cin >> a;
	i = 0;
	while (a != 0)
	{
		if (a % 2 == 0)
		{
			buff[i] = 0;
			a = a / 2;
		}
		else
		{
			buff[i] = 1;
			a = a / 2;
		}
		i++;
	}
	i = i - 1;
	for (int j = 0; i >= 0; i--, j++) {
		BinNum += to_string(buff[i]);
		cout << buff[i];
	}
	cout << BinNum;

	if (BinNum == "" || BinNum == "10000000" || BinNum == "11000000" || BinNum == "11100000" || BinNum == "11110000" || BinNum == "11111000" || BinNum == "11111100" || BinNum == "11111110" || BinNum == "11111111") {
		cout << "Последовательность соблюдается!" << endl;
	}
	else {
		cout << "Последовательность не соблюдается!" << endl;
	}
}*/

void main() {

	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	
	string ip;
	string mask;

	cout << "Введите IP: " << endl;
	ip = "192.168.1.100.";
	cout << ip <<endl;

	cout << "Введите маску: " << endl;
	mask = "255.255.255.0.";
	cout << mask << endl;

	int* ipArr = new int[4];
	ipArr = ipCheck(ip);

	int* maskArr = new int[4];
	maskArr = maskCheck(mask);

	/*for (int i = 0; i < 4; i++) {
		cout << ipArr[i] << endl;
		cout << maskArr[i] << endl;
	}*/

	int* subnetArr = new int[4];
	subnetArr = NetworkID(ipArr, maskArr);
	cout << endl;
	HostID(ipArr, maskArr);
	cout << endl;
	//Broadcast(subnetArr, maskArr);
	//cout << endl;
	system("pause");

	//toBin();
}