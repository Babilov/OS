#include <iostream>
#include <windows.h>
#include <locale>
#include <cstdio>
#include <tlhelp32.h>


using namespace std;

WCHAR buffer[MAX_PATH];

string getName(string FName) {
	string Name = "";
	for (int i = FName.length() - 1; i >= 0; i--) {
		if (FName[i] == L'\\') {
			break;
		}
		else {
			Name = FName[i] + Name;
		}
	}
	return Name;
}



void ShowProcesses()

{

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	cout << "\nSnapshot ID = " << hSnapshot;
	PROCESSENTRY32 e;
	e.dwSize = sizeof(PROCESSENTRY32);
	BOOL fRepeat = Process32First(hSnapshot, &e);
	int i = 0;
	while (fRepeat){
		i++;
		cout << "\n\nth32ProcessID = " << e.th32ProcessID;
		cout << "\nth32DefaultHeapID = " << e.th32DefaultHeapID;
		cout << "\nth32ModuleID = " << e.th32ModuleID;
		cout << "\nth32ParentProcessID = " << e.th32ParentProcessID;
		cout << "\ncntUsage = " << e.cntUsage;
		cout << "\ncntThreads = " << e.cntThreads;
		cout << "\npcPriClassBase = " << e.pcPriClassBase;
		cout << "\ndwFlags = " << e.dwFlags;
		cout << "\nszExeFile = " << e.szExeFile;
		fRepeat = Process32Next(hSnapshot, &e);
	}

	CloseHandle(hSnapshot);
	

}

void ShowModules()

{

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, 0);
	cout << "\nSnapshot ID = " << hSnapshot;
	MODULEENTRY32 e;
	e.dwSize = sizeof(MODULEENTRY32);
	BOOL fRepeat = Module32First(hSnapshot, &e);
	while (fRepeat)
	{
		cout << "\n\nth32ModuleID \t= \t" << e.th32ModuleID;
		cout << "\nth32ProcessID \t= \t" << e.th32ProcessID;
		cout << "\nGlblcntUsage \t= \t" << e.GlblcntUsage;
		cout << "\nProccntUsage \t= \t" << e.ProccntUsage;
		cout << "\nmodBaseAddr \t= \t" << e.modBaseAddr;
		cout << "\nmodBaseSize \t= \t" << e.modBaseSize;
		cout << "\nhModule \t= \t" << e.hModule;
		cout << "\nszModule \t= \t" << e.szModule;
		cout << "\nszExePath \t= \t" << e.szExePath;
		fRepeat = Module32Next(hSnapshot, &e);
	}
	CloseHandle(hSnapshot);

}

void ShowThreads() {
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	cout << "\nSnapshot ID = " << hSnapshot;
	THREADENTRY32 e;
	e.dwSize = sizeof(THREADENTRY32);
	BOOL fRepeat = Thread32First(hSnapshot, &e);
	DWORD dwCurentProcessId = GetCurrentProcessId();
	while (fRepeat)
	{
		if (e.th32OwnerProcessID == dwCurentProcessId)
		{
			cout << "\n\nth32ThreadID = " << e.th32ThreadID;
			cout << "\nth32OwnerProcessID = " << e.th32OwnerProcessID;
			cout << "\ncntUsage = " << e.cntUsage;
			cout << "\ntpBasePri = " << e.tpBasePri;
			cout << "\ntpDeltaPri = " << e.tpDeltaPri;
			cout << "\ndwFlags = " << e.dwFlags;
		}
		fRepeat = Thread32Next(hSnapshot, &e);
	}
	CloseHandle(hSnapshot);
}

int main(){
	// Вывод полного имени
	GetModuleFileName(NULL, buffer, sizeof(buffer) / sizeof(buffer[0])); 
	wstring ws(buffer);
	string FName(ws.begin(), ws.end());
	cout << "Filepath: " << FName << "\n";

	// Вывод короткого имени
	HMODULE m_HInstance;
	m_HInstance = GetModuleHandle(buffer);
	cout << "Name: " <<  getName(FName) << "\n";
	cout << "Handle: " << m_HInstance << "\n";

	// Вывод ID
	DWORD id = GetCurrentProcessId();
	cout << "ID: " << id << "\n";

	// Вывод псевдодискриптора
	HANDLE pseudodescriptor = GetCurrentProcess();
	cout << "Pseudodescriptor: " << pseudodescriptor << "\n";

	//Вывод дескриптора с помощью функции DuplicateHandle
	HANDLE duplicateHandle;
	DuplicateHandle(pseudodescriptor, pseudodescriptor, pseudodescriptor, &duplicateHandle, 0, false, DUPLICATE_SAME_ACCESS);
	cout << "Handle with DuplicateHandle function: " << duplicateHandle << "\n";

	//Вывод дескриптора с помощью функции OpenProcess
	HANDLE idOpen = OpenProcess(PROCESS_DUP_HANDLE, TRUE, id);
	cout << "Handle with OpenProcess function: " << idOpen << "\n";

	//Закрытие дескриптора, созданного функцией DuplicateHandle
	BOOL closedHandle = CloseHandle(duplicateHandle);
	cout << "The handle created by the function DuplicateHandle is closed: " << closedHandle << "\n";

	//Закрытие дескриптора, созданного функцией OpenProcess
	BOOL closedOpen = CloseHandle(idOpen);
	cout << "The handle created by the function OpenProcess is closed: " << closedHandle << "\n";

	int choose;
	while (true) {
		cout << "1. Show processes.\n2. Show threads\n3. Show modules\nElse will close programm";
		cout << "Choose: ";
		cin >> choose;

		//вывод процессов
		if (choose == 1) {
			ShowProcesses();
			cout << endl << "===================\n\n" << endl;
		}
		else {
			//Вывод потоков
			if (choose == 2) {
				ShowThreads();
				cout << endl << "===================\n\n" << endl;
			}
			else {
				//Вывод модулей
				if (choose == 3) {
					ShowModules();
					cout << endl << "===================\n\n" << endl;
				}
				else {
					cout << "Close.";
					break;
					exit;
				}
			}
		}
	}
}
