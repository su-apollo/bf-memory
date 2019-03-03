#include "allocator.hpp"

#include <iostream>
#include <thread>
#include <future>
#include <numeric>
#include <random>

#include <Windows.h>
#include <fcntl.h>
#include <io.h>

void create_console() {
	if (::AllocConsole())
	{
		auto h_crt = ::_open_osfhandle(intptr_t(::GetStdHandle(STD_OUTPUT_HANDLE)), _O_TEXT);
		auto *hf = ::_fdopen(h_crt, "w");
		const auto so = stdout;
		const auto se = stderr;
		*so = *hf;
		::setvbuf(stdout, nullptr, _IONBF, 0);

		h_crt = ::_open_osfhandle(intptr_t(::GetStdHandle(STD_ERROR_HANDLE)), _O_TEXT);
		hf = ::_fdopen(h_crt, "w");
		*se = *hf;
		::setvbuf(stderr, nullptr, _IONBF, 0);
	}
}

struct node : bf::atomic::node {
	explicit node(const int v) : value(v) {}
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

	node* head;
	do {
		head = reinterpret_cast<node*>(bf::atomic::pop(s));
		if (head != nullptr)
			std::cout << "head : " << head->value << std::endl;
	} while (head != nullptr);
}

void accumulate(bf::vector<int>::iterator first, bf::vector<int>::iterator last, std::promise<int> accumulate_promise) {

	std::cout << "promise ref " << &accumulate_promise << std::endl;

	const auto sum = std::accumulate(first, last, 0);
	accumulate_promise.set_value(sum);
}

void test_promise() {
	auto numbers = bf::vector<int>{ 1, 2, 3, 4, 5, 6 };
	auto accumulate_promise = std::promise<int>();
	auto accumulate_future = accumulate_promise.get_future();

	std::cout << "promise ref " << &accumulate_promise << std::endl;

	auto work = std::thread(accumulate, numbers.begin(), numbers.end(), std::move(accumulate_promise));

	accumulate_future.wait();
	std::cout << "accumulate result " << accumulate_future.get() << std::endl;
	work.join();
}

struct test {
	explicit test(const int i) : value(i) {};
	int value;
};

void test_thread_safe() {
	auto task = std::packaged_task<int()>([]{
		std::random_device rd;
		auto gen = std::mt19937(rd());
		const auto dis = std::uniform_int_distribution<>(1, 100);

		for (auto i = 0; i < 10; ++i) {
			std::cout << "call task" << std::endl;
			const auto p = bf::make_unique<test>(dis(gen));
			std::cout << "value " << p->value << std::endl;
		}
		return 7;
	});
	auto f1 = task.get_future();
	auto t = std::thread(std::move(task));

	auto f2 = std::async(std::launch::async, []{
		std::random_device rd;
		auto gen = std::mt19937(rd());
		const auto dis = std::uniform_int_distribution<>(1, 100);

		for (auto i = 0; i < 10; ++i) {
			std::cout << "call async" << std::endl;
			const auto p = bf::make_unique<test>(dis(gen));
			std::cout << "value " << p->value << std::endl;
		}
		return 8;
	});

	auto prom = std::promise<int>();
	auto f3 = prom.get_future();
	std::thread([&prom]{
		std::random_device rd;
		auto gen = std::mt19937(rd());
		const auto dis = std::uniform_int_distribution<>(1, 100);

		for (auto i = 0; i < 10; ++i) {
			std::cout << "call thread" << std::endl;
			const auto p = bf::make_unique<test>(dis(gen));
			std::cout << "value " << p->value << std::endl;
		}
		prom.set_value_at_thread_exit(9);
	}).detach();

	std::cout << "Waiting..." << std::flush;
	f1.wait();
	f2.wait();
	f3.wait();

	std::cout << "Done!\nResults are: "
		<< f1.get() << ' ' << f2.get() << ' ' << f3.get() << '\n';
	t.join();
}

// program entry
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstanc, LPSTR CmdParam, int nCmdShow)
{
	create_console();

	std::cout << "Hello World!" << std::endl;
	bf::pool = new bf::memory_pool;

	test_stack();
	test_promise();
	test_thread_safe();

	delete bf::pool;

	system("PAUSE");
	// Exit program
	exit(EXIT_SUCCESS);
}