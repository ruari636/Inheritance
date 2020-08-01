#include <iostream>
#include <string>
#include <conio.h>
#include <random>
#include <algorithm>
#include "Fighter.h"
#include "Weapon.h"

std::vector<MemeFighter*> team1 =
{
	new MemeFrog("Dat Boi", new Bat),
	new MemeStoner("Good Guy Greg", new Knife),
	new MemeFrog("The WB Frog")
};

std::vector<MemeFighter*> team2 =
{
	new MemeStoner("Chong", new Knife),
	new MemeStoner("Scumbag Steve", new Bat),
	new MemeFrog("Pepe")
};

auto isAlive = [](const MemeFighter* f) {return f->IsAlive(); };

int main()
{
	while (std::any_of(team1.begin(), team1.end(), isAlive) &&
		std::any_of(team2.begin(), team2.end(), isAlive))
	{
		std::random_shuffle(team1.begin(), team1.end());
		std::partition(team1.begin(), team1.end(), isAlive);
		std::random_shuffle(team2.begin(), team2.end());
		std::partition(team2.begin(), team2.end(), isAlive);

		for (size_t i = 0; i < team1.size(); i++)
		{
			Engage(*team1[i], *team2[i]);
			std::cout << "--------------------------------" << std::endl;
		}
		for (size_t i = 0; i < team1.size(); i++)
		{
			team1[i]->Tick();
			team2[i]->Tick();
		}
		std::cout << "================================" << std::endl;

		std::cout << "Press any key to continue...";
		while (!_kbhit());
		_getch();
		std::cout << std::endl << std::endl;

		if (!std::any_of(team2.begin(), team2.end(), isAlive))
		{
			std::cout << std::endl << "Team 1 is victorious" << std::endl;
		}
		else if (!std::any_of(team1.begin(), team1.end(), isAlive))
		{
			std::cout << std::endl << "Team 2 is victorious" << std::endl;
		}
	}
	for (int i = 0; i < team1.size(); i++)
	{
		delete team1[i];
		delete team2[i];
	}
	std::cout << "Press any key to exit";
	while (!_kbhit());
	_getch();
	std::cout << std::endl << std::endl;
}