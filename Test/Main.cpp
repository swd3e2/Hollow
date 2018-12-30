#include <iostream>
#include <thread>
#include <functional>
#include <vector>
#include <chrono>

class foo {
public:
	void operator=(const foo& o) { std::cout << "im called (foo)" << std::endl; }
};

class kek: public foo{
};

class Example
{
public:
	static void Foo()
	{
		std::cout << "i'm example foo" << std::endl;
	}
};

static std::thread* workers[10];
static std::vector<std::pair<size_t, void(*)()>> vector;

void foo();

void bar(void(*func)(void)) { func(); }

int main()
{
	kek k;
	kek f;
	k = f;

	/*vector.push_back(std::make_pair(0, Example::Foo));
	vector.push_back(std::make_pair(0, Example::Foo));
	vector.push_back(std::make_pair(0, Example::Foo));
	vector.push_back(std::make_pair(0, []() {
		std::cout << "im lambda" << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}));
	vector.push_back(std::make_pair(0, []() {
		std::cout << "im 2nd lambda" << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}));

	for (auto& it : vector) 
		it.second();

	std::cin.get();

	std::function<void()> kek;
	for (int i = 0; i < 10; i++)
		workers[i] = new std::thread(foo);

	for (int i = 0; i < 10; i++)
		workers[i]->join();
*/
	std::cin.get();
	return 0;
}

void foo () {
	for (int i = 0; i < 100; i++)
		std::cout << "i'm working" << std::endl;
}