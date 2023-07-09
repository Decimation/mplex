// mplex.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>

// #include <format>
// #include <format>

#include <Windows.h>
#include <psapi.h>
#include <array>
#include <tchar.h>
#include <boost/format.hpp>

using namespace std;

using boost::format;
using boost::io::group;

class my_struct
{
public:
	HANDLE h;
	HMODULE module;
	TCHAR name[MAX_PATH];
	DWORD pid;

	~my_struct()
	{
		CloseHandle(h);
		std::memset(this, 0, sizeof(this));
	}
};

my_struct* PrintProcessNameAndID(DWORD processID)
{
	my_struct* ms = new my_struct{
		.name = TEXT("<unknown>"),
		.pid = processID
	};

	// Get a handle to the process.

	ms->h = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,FALSE, processID);

	// Get the process name.

	if (NULL != ms->h) {
		HMODULE hMod;
		DWORD cbNeeded;

		if (EnumProcessModules(ms->h, &hMod, sizeof(hMod), &cbNeeded)) {
			GetModuleBaseName(ms->h, hMod, ms->name, sizeof(ms->name) / sizeof(TCHAR));
		}
	}

	// Print the process name and identifier.

	// _tprintf(TEXT("%s  (PID: %u)\n"), szProcessName, processID);

	// Release the handle to the process.

	return ms;
}

HWND w;
BOOL CALLBACK enumProc(HWND hwnd, LPARAM)
{
	TCHAR buf[1024]{};
	GetClassName(hwnd, buf, 100);
	GetWindowText(hwnd, buf, 100);
	if (std::wstring(buf).contains(L"MusicBee")) {
		DWORD pid = 0;
		GetWindowThreadProcessId(hwnd, &pid);
		printf("%p %ls %d", hwnd, buf, pid);

		w = hwnd;
		return false;
	}
	
	return TRUE;
}

int main()
{
	/*std::cout << "Hello World!\n";
	auto rg = std::array<DWORD, 256>();
	DWORD c;

	if (!EnumProcesses(rg.data(), sizeof(rg), &c)) {
		std::cout << "Error";
	}

	// Calculate how many process identifiers were returned.

	auto cProcesses = c / sizeof(DWORD);

	// Print the name and process identifier for each process.
	my_struct* mb;

	for (auto i = 0; i < cProcesses; i++) {
		if (rg[i] != 0) {
			auto ms = PrintProcessNameAndID(rg[i]);

			if (std::wstring(ms->name).contains(L"MusicBee")) {
				printf("%ls %lu", ms->name, ms->pid);
				mb = ms;
				continue;
			}
		}
	}

	auto w = GetWindow(nullptr, L"MusicBee");
	std::cout <<
		boost::format("%1x") % w;
		*/

	EnumWindows(&enumProc, 0);

	// std::cout<<SendMessage(w, WM_KEYDOWN, VK_MEDIA_PLAY_PAUSE, NULL);
	std::cout << SendMessage(w, WM_KEYDOWN, APPCOMMAND_MEDIA_PLAY_PAUSE, NULL);
	std::cout << SendMessage(w, WM_KEYUP, APPCOMMAND_MEDIA_PLAY_PAUSE, NULL);

	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
