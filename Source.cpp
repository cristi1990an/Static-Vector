#include <string>
#include <set>
#include <map>
#include <any>
#include <variant>
#include <optional>
#include <iostream>
#include "inc/static_vector.hpp"

int main()
{
	static_vector<std::string, 10> vec =
	{ "Cristi", "nu", "stie", "sa", "programeze" };

	for (const auto& word : vec)
	{
		std::cout << word << ' ';
	}
	std::cout << '\n';

	vec.erase(vec.begin() + 1);

	for (const auto& word : vec)
	{
		std::cout << word << ' ';
	}
	std::cout << '\n';

	vec.erase(vec.begin() + 3);

	for (const auto& word : vec)
	{
		std::cout << word << ' ';
	}
	std::cout << '\n';
}
