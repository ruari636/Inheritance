#pragma once
#include <iostream>
#include <string>
#include <conio.h>
#include <random>
#include <algorithm>
#include "Weapon.h"

class MemeFighter
{
protected:
	MemeFighter(std::string name, int health, int speed, int power, Weapon* weapon = nullptr)
		:
		name(name),
		stats(health, speed, power),
		weapon(weapon)
	{}
	std::string name;
	Stats stats;
	bool alive = true;
	Weapon* weapon = nullptr;
public:
	virtual ~MemeFighter()
	{
		delete weapon;
	}
	std::string GetName() const { return name; }
	bool IsAlive() const { return alive; }
	int GetInitiative() const { return stats.speed + Roll(2); }
	int GetPower() const { return stats.power; }
	int GetHealth() const { return stats.health; }
	int WeaponRank() const { return weapon->GetRank(); }
	const Weapon* getWeapon() const { return weapon; }
	void UpdateAlive() { alive = alive && stats.health > 0; }
	std::string WeaponName() const { return weapon->GetName(); }
	void Attack(MemeFighter& enemy)
	{
		if (alive)
		{
			int damageDealt = weapon->GetDamage(stats);
			enemy.changeHp(-damageDealt);
			std::cout << name << " attacked " << enemy.GetName() << " with their " << weapon->GetName()
				<< " for " << damageDealt
				<< " hitpoints." << '\n';
			if (!enemy.IsAlive())
			{
				std::cout << "Shame that " << enemy.GetName() << " is already dead noob.\n";
			}
		}
	}
	virtual void SpecialMove(MemeFighter& enemy) {};
	void AssignWeapon(Weapon* weapon_in)
	{
		delete weapon;
		weapon = weapon_in;
	}
	Weapon* dropWeapon()
	{
		Weapon* temp = weapon;
		weapon = nullptr;
		return temp;
	}
	void changeHp(int deltaHp) { stats.health += deltaHp; }
	virtual void Tick()
	{
		alive = alive && stats.health > 0;
		if (alive)
		{
			int hpGained = std::min(Roll(1), stats.maxHealth - stats.health);
			if (hpGained != 0)
			{
				stats.health += hpGained;
				std::cout << name << " recovered " << hpGained << " health.\n";
			}
		}
	}
};

class MemeFrog : public MemeFighter
{
public:
	MemeFrog(std::string name, Weapon* weapon = new Hands("Moist Flipper"))
		:
		MemeFighter(name, 69, 7, 14, weapon)
	{
	}
	void SpecialMove(MemeFighter& enemy) override
	{
		if (Roll(1) < 3)
		{
			int damage = Roll(3) + 20;
			enemy.changeHp(-damage);
			if (enemy.IsAlive())
			{
				std::cout << MemeFighter::GetName() << " slapped " << enemy.GetName() << "'s health "
					<< damage << " damage.\n";
			}
			else
			{
				std::cout << enemy.GetName() << " got their ass handed to them by "
					<< MemeFighter::GetName() << "'s special attack.\n";
			}
			return;
		}
		std::cout << MemeFighter::GetName() << "'s attack was unsuccessful\n";
	}
	void Tick() override
	{
		alive = stats.health > 0;
		if (MemeFighter::IsAlive())
		{
			int damage = Roll(1);
			MemeFighter::changeHp(-damage);
			if (MemeFighter::IsAlive())
			{
				std::cout << MemeFighter::GetName() << " took " << damage << " damage.\n";
			}
			else
			{
				std::cout << MemeFighter::GetName() << " died from " << damage << " damage.\n";
			}
		}
	}
};

class MemeStoner : public MemeFighter
{
	bool Super = false;
public:
	MemeStoner(std::string name, Weapon* weapon = new Hands("Long Tongue"))
		:
		MemeFighter(name, 80, 4, 10, weapon)
	{
	}
	void SpecialMove(MemeFighter& x) override
	{
		if (Roll(1) < 4)
		{
			stats.speed += 3;
			stats.power = (stats.power * 69) / 42;
			stats.health += 10;
			stats.maxHealth += 10;
			Super = true;
			name = "Super " + name;
			std::cout << name << " has super powered up. Fighting capabilities increased.\n";
		}
	}
};

void takeWeapon(MemeFighter& taker, MemeFighter& giver)
{
	taker.AssignWeapon(giver.dropWeapon());
}

bool takeIfDead(MemeFighter& taker, MemeFighter& giver)
{
	if (taker.IsAlive() && !giver.IsAlive() && giver.getWeapon() != nullptr)
	{
		if (taker.WeaponRank() < giver.WeaponRank())
		{
 			takeWeapon(taker, giver);
			return true;
		}
	}
	return false;
}

void EngageBasic(MemeFighter& f1, MemeFighter& f2)
{
	// pointers for sorting purposes
	auto* p1 = &f1;
	auto* p2 = &f2;
	// determine attack order
	if (p1->GetInitiative() < p2->GetInitiative())
	{
		std::swap(p1, p2);
	}
	// execute attacks
	p1->Attack(*p2);
	p2->Attack(*p1);
	// do this for taking weapon option
	p1->UpdateAlive();
	p2->UpdateAlive();
}

void EngageSpecial(MemeFighter& f1, MemeFighter& f2)
{
	// pointers for sorting purposes
	auto* p1 = &f1;
	auto* p2 = &f2;
	// determine attack order
	if (p1->GetInitiative() < p2->GetInitiative())
	{
		std::swap(p1, p2);
	}
	p1->SpecialMove(*p2);
	p2->SpecialMove(*p1);
	// do this for taking weapon option
	p1->UpdateAlive();
	p2->UpdateAlive();
}

void Engage(MemeFighter& f1, MemeFighter& f2)
{
	EngageBasic(f1, f2);
	EngageSpecial(f1, f2);
	if (takeIfDead(f1, f2))
	{
		std::cout << "--------------------------------" << std::endl;
		std::cout << f1.GetName() << " took " << f2.GetName() << "'s " << f1.WeaponName() << std::endl;
	}
	else if (takeIfDead(f2, f1))
	{
		std::cout << f2.GetName() << " took " << f1.GetName() << "'s " << f2.WeaponName() << std::endl;
	}
	
}