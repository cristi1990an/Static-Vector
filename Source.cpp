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
	static_vector<std::string, 10> v_1 = { "Cristi", "stie", "sa", "programeze" };
	static_vector<std::string, 5> v_2 = { "Ceva", "stupid" };

	v_1.swap(v_2);

	for (const auto& word : v_1)
	{
		std::cout << word << ' ';
	}
	std::cout << '\n';

	for (const auto& word : v_2)
	{
		std::cout << word << ' ';
	}
	std::cout << '\n';
}
