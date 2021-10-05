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

	static_vector<std::string, 10> vec_2 =
	{ "Ceva", "mesaj", "care", "trebuie", "sa", "fie", "sters" };

	std::cout << vec_2.size() << '\n';

	vec_2.assign(vec.begin(), vec.end());

	for (const auto& word : vec_2)
	{
		std::cout << word << ' ';
	}

	std::cout << '\n';
	std::cout << vec_2.size();
}
