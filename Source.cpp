#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include <string>
#include <ctime>
#define SIZE 24
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "user32.lib")

SOCKET Connections[100];
int Counter = 0;
time_t now = time(0);
HANDLE mutex;//Глобальный хендл мютекса

std::string getPhysicalMemory() {
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);
	GlobalMemoryStatusEx(&statex);
	char* dt = ctime(&now);
	std::string time(dt, SIZE);
	std::string s = time + ": Процент используемой физической памяти: " + std::to_string(statex.dwMemoryLoad) + "% \n";
	return s;
}
std::string getVirtualMemory() {
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);
	GlobalMemoryStatusEx(&statex);
	char* dt = ctime(&now);
	std::string time(dt, SIZE);
	std::string s = time + ": Процент используемой виртуальной памяти: " + std::to_string(100.0 - (float)statex.ullAvailVirtual / (float)statex.ullTotalVirtual) + "% \n";
	return s;
}

void ClientHandler(int index) {
	char msg[256];
	while (true) {
		recv(Connections[index], msg, sizeof(msg), NULL);
		if (msg[0] == '1') {
			for (int i = 0; i < Counter; i++) {
				if (i == index) {
					std::string ans = getPhysicalMemory();
					strcpy(msg, ans.c_str());
					send(Connections[i], msg, sizeof(msg), NULL);
				}

			}
		}
		if (msg[0] == '2') {
			for (int i = 0; i < Counter; i++) {
				if (i == index) {
					std::string ans = getVirtualMemory();
					strcpy(msg, ans.c_str());
					send(Connections[i], msg, sizeof(msg), NULL);
				}

			}
		}
	}
}



int main(int argc, char* argv[]) {

	mutex = CreateMutex(NULL, true, TEXT("server2.exe"));
	DWORD result;
	result = WaitForSingleObject(mutex, 0);
	if (result != WAIT_OBJECT_0) {
		exit(1);
	}

	setlocale(LC_ALL, "Russian");
	//Создание струтуры для установки библиотеки
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		std::cout << "Error" << std::endl;
		exit(1);
	}

	//Заполнение информации об адресе сокета
	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1112);
	addr.sin_family = AF_INET;

	//Создание сокета
	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);

	//Привязка адреса сокету
	bind(sListen, (SOCKADDR*)& addr, sizeof(addr));

	//Начало прослушивания порта
	listen(sListen, SOMAXCONN);

	//Создание сокета для удерживания соединения с клиентом
	SOCKET newConnection;
	for (int i = 0; i < 100; i++) {
		newConnection = accept(sListen, (SOCKADDR*)& addr, &sizeofaddr);

		//Проверка на подключение
		if (newConnection == 0) {
			std::cout << "Error #2" << std::endl;
		}
		else {
			std::cout << "Client " << i <<  " connected to server 2!" << std::endl;
			char msg[256] = "Я могу: \n 1. Посчитать кол-во физической памяти. \n 2. Посчитать кол-во виртуальной памяти.";
			send(newConnection, msg, sizeof(msg), NULL);
			Connections[i] = newConnection;
			Counter++;
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, (LPVOID)(i), NULL, NULL);
		}
	}

	system("pause");
	return 0;
}