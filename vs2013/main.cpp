#include "allocator.hpp"

#include <iostream>
#include <thread>
#include <future>
#include <numeric>
#include <random>

#include <Windows.h>

struct node : bf::atomic::node {
	explicit node(const int v) : value(v) {}
	int value;
};

void test_stack() {
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
int main() {
	std::cout << "Hello World!" << std::endl;

	bf::pool = new bf::memory_pool;

	test_stack();
	test_thread_safe();

	delete bf::pool;

	system("PAUSE");
	// Exit program
	exit(EXIT_SUCCESS);
}