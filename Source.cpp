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
	Test() = default;

	Test(Test&&) 
	{
		std::cout << "Move constructor called!\n";
	}

	Test(const Test&) noexcept
	{
		std::cout << "Copy constructor called!\n";
	}

	Test& operator=(Test&&) noexcept
	{
		std::cout << "Move assignment called!\n";
		return *this;
	}

	Test& operator=(const Test&) noexcept
	{
		std::cout << "Copy assignment called!\n";
		return *this;
	}

	~Test() = default;
};

int main()
{
	static_vector<Test, 10> v1(5);
	static_vector<Test, 10> v2(7);

	v1 = std::move(v2);
}
