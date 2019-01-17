#include "allocator.hpp"

#include <iostream>

#if BF_WINDOWS_FAMILY

#include <Windows.h>
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

	bf::pool = new bf::memory_pool;

	bf::vector<int> test;
	test.push_back(1);
	test.push_back(2);
	test.push_back(3);

	for (auto i : test) {
		std::cout << "test" << std::endl;
	}

	delete bf::pool;

	system("PAUSE");
	// Exit program
	exit(EXIT_SUCCESS);
}

#else

struct node : bf::atomic::node {
	node(int v) : value(v) {}
	int value;
};

int main() {
	std::cout << "Hello World!" << std::endl;

	bf::pool = new bf::memory_pool;

	std::cout << "malloc_info : " << alignof(bf::malloc_info) << std::endl;

	bf::atomic::stack s;
	node t1(1);
	node t2(3);
	node t3(2);
	node t4(4);

	bf::atomic::push(s, &t1);
	bf::atomic::push(s, &t2);
	bf::atomic::push(s, &t3);
	bf::atomic::push(s, &t4);

	node* head = nullptr;
	do {
		head = static_cast<node*>(bf::atomic::pop(s));
		if (head != nullptr)
			std::cout << "head : " << head->value << std::endl;
	} while (head != nullptr);

	bf::vector<int> test;

	test.push_back(1);
	test.push_back(2);
	test.push_back(3);

	for (auto i : test) {
		std::cout << "test" << std::endl;
	}

	delete bf::pool;

	return 0;	
}

#endif