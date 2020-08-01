#pragma once
#include <string>

class Dice
{
public:
	int Roll(int nDice)
	{
		int total = 0;
		for (int n = 0; n < nDice; n++)
		{
			total += d6(rng);
		}
		return total;
	}
private:
	std::uniform_int_distribution<int> d6 = std::uniform_int_distribution<int>(1, 6);
	std::mt19937 rng = std::mt19937(std::random_device{}());
};

Dice dice;

int Roll(int amount = 1)
{
	return dice.Roll(amount);
}

struct Stats
{
	Stats(int health, int speed, int power)
		:
		power(power),
		health(health),
		maxHealth(health),
		speed(speed)
	{}
	int health;
	int maxHealth;
	int power;
	int speed;
};

class Weapon
{
	std::string name;
	int rank;

public:
	Weapon(const std::string name, int rank)
		:
		name(name),
		rank(rank)
	{}
	virtual int GetDamage(Stats& user) = 0;
	virtual const std::string& GetName() const
	{
		return name;
	}
	virtual int GetRank() const
	{
		return rank;
	}
};

struct Knife : public Weapon 
{
	Knife(const std::string name = "Knife")
		:
		Weapon(name, 2)
	{}
	int GetDamage(Stats& user) override
	{
		return user.speed * 3 + Roll(3);
	}
};

struct Bat : public Weapon
{
	Bat(const std::string name = "Bat")
		:
		Weapon(name, 1)
	{}
	int GetDamage(Stats& user) override
	{
		return user.power * 2 + Roll();
	}
};

struct Hands : public Weapon
{
	Hands(std::string name = "Fists")
		:
		Weapon(name, 0)
	{}
	int GetDamage(Stats& user) override
	{
		return user.power + Roll(2);
	}
};