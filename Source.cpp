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
	static_vector<std::string, 10> vec = { "Cristi", "e", "foarte", "tare" };

	vec.insert(vec.cbegin() + 1, "nu");

	for (const auto& word : vec)
	{
		std::cout << word << ' ';
	}
}