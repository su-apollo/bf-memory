#pragma once
#include "memory-pool.h"

#include <list>
#include <vector>
#include <deque>
#include <set>
#include <unordered_map>
#include <map>
#include <queue>
#include <string>
#include <sstream>

namespace bf {
template <class T>
class allocator : public std::allocator<T> {
public:
	allocator() {}
	
	allocator(const allocator&) {}
	
	template <class U>
	allocator(const allocator<U>&) {}

	template <class U>
	allocator<T>& operator= (const allocator<U>&) { return (*this); }

	template <class U>
	struct rebind { typedef allocator<U> other; };

	T* allocate(size_t n) {
		return static_cast<T*>(Pool->Allocate((int)n*sizeof(T)));
	}

	void deallocate(T* ptr, size_t n) {
		Pool->Deallocate(ptr, -1599);
	}
};

template <class T>
using vector = std::vector<T, allocator<T>>;

template <class T>
using deque = std::deque<T, allocator<T>>;

template <class T>
using list = std::list<T, allocator<T>>;

template <class K, class T, class C = std::less<K>>
using map = std::map<K, T, C, allocator<std::pair<const K, T>>>;

template <class T, class C = std::less<T>>
using set = std::set<T, C, allocator<T>>;

template <class T, class H = std::hash<T>, class C = std::equal_to<T>>
using unordered_map = std::unordered_map<T, H, C, allocator<T>>;

template <class T, class C = std::less<std::vector<T>>>
using priority_queue = std::priority_queue<T, std::vector<T, allocator<T>>, C>;

using string = std::basic_string<char, std::char_traits<char>, allocator<char>>;
using wstring = std::basic_string<wchar_t, std::char_traits<wchar_t>, allocator<wchar_t>>;

using stringstream = std::basic_stringstream<char, std::char_traits<char>, allocator<char>>;
using wstringstream = std::basic_stringstream<wchar_t, std::char_traits<wchar_t>, allocator<wchar_t>>;

template <class T>
struct deleter {
	deleter() {}

	template <class U, class = typename std::enable_if<std::is_convertible<U*, T*>::value, void>::type>
	deleter(const deleter<U>&) {}

	void operator()(T* p) const { xdelete<T>(p); }
};

template <class T>
using shared_ptr = std::shared_ptr<T>;

template <class T>
using weak_ptr = std::weak_ptr<T>;

template <class T, class... Args>
shared_ptr<T> make_shared(Args&&... args) {
	return shared_ptr<T>(xnew<T>(std::forward<Args>(args)...), deleter<T>());
}

template <class T>
using unique_ptr = std::unique_ptr<T, deleter<T>>;

template <class T, class... Args>
unique_ptr<T> make_unique(Args&&... args) {
	return unique_ptr<T>(xnew<T>(std::forward<Args>(args)...));
}
}
