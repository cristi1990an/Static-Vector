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
	static_vector<std::string, 10> vec_1 = { "Cristi", "e", "tare" };
	static_vector<std::string, 10> vec_2 = { "Squid's", "game" };

	vec_1.swap(vec_2);

	for (const auto& word : vec_1)
	{
		std::cout << word << ' ';
	}
	std::cout << '\n';

	for (const auto& word : vec_2)
	{
		std::cout << word << ' ';
	}
	std::cout << '\n';
}
