#include "allocator.hpp"

#include <iostream>
#include <thread>
#include <future>
#include <numeric>
#include <random>

struct node : bf::atomic::node {
	explicit node(int v) : value(v) {}
	int value;
};

void test_stack() {
	std::cout << "malloc_info : " << alignof(bf::malloc_info) << std::endl;

	bf::atomic::stack s;
	auto t1 = node(1);
	auto t2 = node(3);
	auto t3 = node(2);
	auto t4 = node(4);

	bf::atomic::push(s, &t1);
	bf::atomic::push(s, &t2);
	bf::atomic::push(s, &t3);
	bf::atomic::push(s, &t4);

	node* head = nullptr;
	do {
		head = reinterpret_cast<node*>(bf::atomic::pop(s));
		if (head != nullptr)
			std::cout << "head : " << head->value << std::endl;
	} while (head != nullptr);
}

void accumulate(bf::vector<int>::iterator first, bf::vector<int>::iterator last, std::promise<int> accumulate_promise) {

	std::cout << "promise ref " << &accumulate_promise << std::endl;

	auto sum = std::accumulate(first, last, 0);
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
	explicit test(int v) : value(v) {};
	int value;
};

void test_thread_safe() {
	auto task = std::packaged_task<int()>([]{
		std::random_device rd;
		auto gen = std::mt19937(rd());
		auto dis = std::uniform_int_distribution<>(1, 100);

		for (auto i = 0 ; i < 10 ; ++i) {
			std::cout << "call task" << std::endl;
			auto p = bf::make_unique<test>(dis(gen));
			std::cout << "value " << p->value << std::endl;
		}
		return 7;
	});
    auto f1 = task.get_future();
    auto t = std::thread(std::move(task));

    auto f2 = std::async(std::launch::async, []{
		std::random_device rd;
		auto gen = std::mt19937(rd());
		auto dis = std::uniform_int_distribution<>(1, 100);

		for (auto i = 0 ; i < 10 ; ++i) {
			std::cout << "call async" << std::endl;
			auto p = bf::make_unique<test>(dis(gen));
			std::cout << "value " << p->value << std::endl;
		}
		return 8;
	});

    auto pro = std::promise<int>();
    auto f3 = pro.get_future();
    std::thread( [&pro]{
		std::random_device rd;
		auto gen = std::mt19937(rd());
		auto dis = std::uniform_int_distribution<>(1, 100);

		for (auto i = 0 ; i < 10 ; ++i) {
			std::cout << "call thread" << std::endl;
			auto p = bf::make_unique<test>(dis(gen));
			std::cout << "value " << p->value << std::endl;
		}
		pro.set_value_at_thread_exit(9);
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
