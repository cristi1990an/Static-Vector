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
		std::cout << "Destructor called!\n";
	}
};

int main()
{
	static_vector<Test, 10> v1(6);
	static_vector<Test, 10> v2(3);

	v1.swap(v2);
}
