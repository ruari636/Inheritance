#include <iostream>
#include <string>
#include <conio.h>
#include <random>

class Dice
{
public:
	int Roll( int nDice )
	{
		int total = 0;
		for( int n = 0; n < nDice; n++ )
		{
			total += d6( rng );
		}
		return total;
	}
private:
	std::uniform_int_distribution<int> d6 = std::uniform_int_distribution<int>( 1,6 );
	std::mt19937 rng = std::mt19937( std::random_device{}() );
};

class MemeFighter : protected Dice
{
protected:
	MemeFighter(std::string name, int hp, int speed, int power)
		:
		name(name),
		maxHp(hp),
		hp(hp),
		speed(speed),
		power(power)
	{
	}
	std::string name;
	int maxHp;
	int hp;
	int speed;
	int power;
	bool alive = true;
public:
	std::string GetName() { return name; }
	bool IsAlive() { return alive; }
	int GetInitiative() { return speed + Roll(2); }
	int GetPower() { return power; }
	void Punch(MemeFighter& enemy) 
	{
		int damageDealt = power + Roll(2);
		enemy.changeHp(-damageDealt);
		std::cout << name << " punched " << enemy.GetName() << " for " << damageDealt
			<< " hitpoints." << '\n';
		if (!enemy.IsAlive())
		{
			std::cout << "Shame that " << enemy.GetName() << " is already dead noob.\n";
		}
	}
	void changeHp(int deltaHp) { hp += deltaHp; }
	void Tick() 
	{
		alive = hp > 0;
		if (alive)
		{
			int hpGained = std::min(Roll(1), maxHp - hp);
			if (hpGained != 0)
			{
				hp += hpGained;
				std::cout << name << " recovered " << hpGained << " health.\n";
			}
		}
	}
};

class MemeFrog : public MemeFighter
{
public:
	MemeFrog(std::string name)
		:
		MemeFighter(name, 69, 7, 14)
	{
	}
	void SpecialMove(MemeFighter& enemy)
	{
		if (Roll(1) < 3)
		{
			int damage = Roll(2) + MemeFighter::GetPower();
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
		std::cout << MemeFighter::GetName() << "'s attack was unsuccessful";
	}
	void Tick()
	{
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
	MemeStoner(std::string name)
		:
		MemeFighter(name, 80, 4, 10)
	{
	}
	void SpecialMove()
	{
		if (Roll(1) < 4)
		{
			speed += 3;
			power = (int)((float)power * 69.0f / 42.0f);
			hp += 10;
			maxHp += 10;
			Super = true;
			name = "Super " + name;			
			std::cout << name << " has super powered up. Fighting capabilities increased.\n";
		}
	}
};

void Engage( MemeFighter& f1,MemeFighter& f2 )
{
	// pointers for sorting purposes
	auto* p1 = &f1;
	auto* p2 = &f2;
	// determine attack order
	if( p1->GetInitiative() < p2->GetInitiative() )
	{
		std::swap( p1,p2 );
	}
	// execute attacks
	p1->Punch( *p2 );
	p2->Punch( *p1 );
}

int main()
{
	MemeFrog f1( "Dat Boi" );
	MemeStoner f2( "Good Guy Greg" );

	while( f1.IsAlive() && f2.IsAlive() )
	{
		// trade blows
		Engage( f1,f2 );
		// special moves
		f2.SpecialMove();
		f1.SpecialMove( f2 );
		// end of turn maintainence
		f1.Tick();
		f2.Tick();

		std::cout << "Press any key to continue...";
		while( !_kbhit() );
		_getch();
		std::cout << std::endl << std::endl;
	}

	if( f1.IsAlive() )
	{
		std::cout << f1.GetName() << " is victorious!";
	}
	else
	{
		std::cout << f2.GetName() << " is victorious!";
	}
	while( !_kbhit() );
	return 0;
}