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
	Test() noexcept
	{
		//std::cout << "Default constructor called!\n";
	}

	Test(Test&&) noexcept
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

	~Test() noexcept
	{
		//std::cout << "Destructor called!\n";
	}
};

int main()
{
	static_vector<int, 10> v1 = { 1,2,2,4,5 };
	static_vector<int, 10> v2 = { 1,2,3,4,5 };

	std::cout << (v1 <= v2);
}
