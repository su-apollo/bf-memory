#include "allocator.hpp"

#include <iostream>
#include <thread>
#include <future>
#include <numeric>
#include <random>

#if BF_WINDOWS_FAMILY

#include <Windows.h>
#include <fcntl.h>
#include <io.h>

void CreateConsole() {
	if (::AllocConsole())
	{
		auto hCrt = ::_open_osfhandle((intptr_t) ::GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
		auto *hf = ::_fdopen(hCrt, "w");
		auto so = stdout, se = stderr;
		*so = *hf;
		::setvbuf(stdout, nullptr, _IONBF, 0);

		hCrt = ::_open_osfhandle((intptr_t) ::GetStdHandle(STD_ERROR_HANDLE), _O_TEXT);
		hf = ::_fdopen(hCrt, "w");
		*se = *hf;
		::setvbuf(stderr, nullptr, _IONBF, 0);
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

	for ([[maybe_unused]] auto _ : test) {
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

void test_stack() {
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
}

void accumulate(bf::vector<int>::iterator first, bf::vector<int>::iterator last, std::promise<int> accumulate_promise) {

	std::cout << "promise ref " << &accumulate_promise << std::endl;

	int sum = std::accumulate(first, last, 0);
	accumulate_promise.set_value(sum);
}

void test_promise() {
	bf::vector<int> numbers = { 1, 2, 3, 4, 5, 6};
	std::promise<int> accumulate_promise;
	std::future<int> accumulate_future = accumulate_promise.get_future();

	std::cout << "promise ref " << &accumulate_promise << std::endl;

	std::thread work(accumulate, numbers.begin(), numbers.end(), std::move(accumulate_promise));

    accumulate_future.wait();
    std::cout << "acccumulate result " << accumulate_future.get() << std::endl;
	work.join();
}

struct test {
	test(int i) : value(i) {};
	~test() = default;

	int value;
};

void test_thread_safe() {
	std::packaged_task<int()> task([]{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(1, 100);

		for (auto i = 0 ; i < 10 ; ++i) {
			std::cout << "call task" << std::endl;
			auto p = bf::make_unique<test>(dis(gen));
			std::cout << "value " << p->value << std::endl;
		}
		return 7;
	});
    std::future<int> f1 = task.get_future();
    std::thread t(std::move(task));

    std::future<int> f2 = std::async(std::launch::async, []{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(1, 100);

		for (auto i = 0 ; i < 10 ; ++i) {
			std::cout << "call async" << std::endl;
			auto p = bf::make_unique<test>(dis(gen));
			std::cout << "value " << p->value << std::endl;
		}
		return 8;
	});

    std::promise<int> p;
    std::future<int> f3 = p.get_future();
    std::thread( [&p]{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(1, 100);

		for (auto i = 0 ; i < 10 ; ++i) {
			std::cout << "call thread" << std::endl;
			auto p = bf::make_unique<test>(dis(gen));
			std::cout << "value " << p->value << std::endl;
		}
		p.set_value_at_thread_exit(9);
	}).detach();

    std::cout << "Waiting..." << std::flush;
    f1.wait();
    f2.wait();
	f3.wait();

    std::cout << "Done!\nResults are: "
              << f1.get() << ' ' << f2.get() << ' ' << f3.get() << '\n';
    t.join();
}

int main() {
	std::cout << "Hello World!" << std::endl;
	bf::pool = new bf::memory_pool;

	test_stack();
	test_promise();
	test_thread_safe();

	delete bf::pool;
	return 0;
}

#endif