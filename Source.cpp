#include "MyForm.h"
#include <iostream>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

SOCKET Connection;

void ClientHendler() {
	char msg[256];
	while (true) {
		recv(Connection, msg, sizeof(msg), NULL);
		std::cout << msg << std::endl;
	}
}

int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "Russian");
	//�������� �������� ��� ��������� ����������
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		std::cout << "Error" << std::endl;
		exit(1);
	}
	int i = 5;
	while (i != 0) {
		std::cout << "�� ����� ������ ������������? (1 / 2 / (0 ��� ������)): ";
		std::cin >> i;
		if (i == 0) {
			std::cout << "�����" <<std::endl;
			break;
		}
		else {
			if (i == 1) {
				//���������� ���������� �� ������ ������
				SOCKADDR_IN addr;
				int sizeofaddr = sizeof(addr);
				addr.sin_addr.s_addr = inet_addr("127.0.0.1");
				addr.sin_port = htons(1111);
				addr.sin_family = AF_INET;

				//�������� ������ ��� ���������� � ��������
				Connection = socket(AF_INET, SOCK_STREAM, NULL);

				//������������� ���������� � ��������
				if (connect(Connection, (SOCKADDR*)& addr, sizeof(sockaddr)) != 0) {
					std::cout << "Error: failed connect to server" << std::endl;
					return 1;
				}
				std::cout << "Connected!" << std::endl;

				CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHendler, NULL, NULL, NULL);

				char msg1[256];
				while (true) {
					std::cin.getline(msg1, sizeof(msg1));
					send(Connection, msg1, sizeof(msg1), NULL);
					Sleep(10);
				}
			}
			else {
				if (i == 2) {
					//���������� ���������� �� ������ ������
					SOCKADDR_IN addr;
					int sizeofaddr = sizeof(addr);
					addr.sin_addr.s_addr = inet_addr("127.0.0.1");
					addr.sin_port = htons(1112);
					addr.sin_family = AF_INET;

					//�������� ������ ��� ���������� � ��������
					Connection = socket(AF_INET, SOCK_STREAM, NULL);

					//������������� ���������� � ��������
					if (connect(Connection, (SOCKADDR*)& addr, sizeof(sockaddr)) != 0) {
						std::cout << "Error: failed connect to server" << std::endl;
						return 1;
					}
					std::cout << "Connected!" << std::endl;

					CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHendler, NULL, NULL, NULL);

					char msg1[256];
					while (true) {
						std::cin.getline(msg1, sizeof(msg1));
						send(Connection, msg1, sizeof(msg1), NULL);
						Sleep(10);
					}

				}
				else {
					std::cout << "�� ������� �������������� ������.";
				}
			}
		}
	}
	system("pause");
	return 0;
}