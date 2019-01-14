#include "allocator.hpp"

#include <Windows.h>
#include <iostream>
#include <fcntl.h>
#include <io.h>

void CreateConsole() {
	if (::AllocConsole())
	{
		int hCrt = ::_open_osfhandle((intptr_t) ::GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
		FILE *hf = ::_fdopen(hCrt, "w");
		*stdout = *hf;
		::setvbuf(stdout, NULL, _IONBF, 0);

		hCrt = ::_open_osfhandle((intptr_t) ::GetStdHandle(STD_ERROR_HANDLE), _O_TEXT);
		hf = ::_fdopen(hCrt, "w");
		*stderr = *hf;
		::setvbuf(stderr, NULL, _IONBF, 0);
	}
}

// program entry
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	CreateConsole();

	bf::Pool = new bf::MemoryPool;

	bf::vector<int> test;
	test.push_back(1);
	test.push_back(2);
	test.push_back(3);

	for (auto i : test) {
		std::cout << "test" << std::endl;
	}

	delete bf::Pool;

	system("PAUSE");
	// Exit program
	exit(EXIT_SUCCESS);
}