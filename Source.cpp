#include <string>
#include <set>
#include <map>
#include <any>
#include <variant>
#include <optional>
#include <iostream>
#include "inc/static_vector.hpp"

struct Test
{
	Test()
	{
		std::cout << "Object constructed!\n";
	}

	~Test()
	{
		std::cout << "Object destructed!\n";
	}
};

int main()
{
	static_vector<Test, 10> vec;

	auto it = vec.cbegin();

}