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

	Test(Test&&) noexcept
	{
		std::cout << "Move constructor called!\n";
	}

	Test(const Test&)
	{
		std::cout << "Copy constructor called!\n";
	}

	~Test()
	{
		std::cout << "Object destructed!\n";
	}
};

int main()
{
	static_vector<Test, 1> v1(1);

	static_vector<Test, 1> v2 = std::move(v1);
}
