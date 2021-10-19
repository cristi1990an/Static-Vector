#include <string>
#include <set>
#include <map>
#include <list>
#include <any>
#include <variant>
#include <optional>
#include <iostream>
#include "inc/static_vector.hpp"
int main()
{

	std::list<std::string> vec =
	{ "Cristi", "nu", "stie", "sa", "programeze", "foarte", "bine", "asta", "este", "stiut"};

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
