#include "tank.h"

void tank_damage(Tank* this){
	--this->m_health;
}

void tank_move(Tank* this, const char direction){
	if(this->rotation == direction){
		switch(direction){
			case 'u':
				--this->position.x;
				break;
			case 'd':
				++this->position.x;
				break;
			case 'l':
				--this->position.y;
				break;
			case 'r':
				++this->position.y;
				break;
		}
	}
	this->rotation = direction;
	return;
}

void tank_init(Tank* this, int health, int x, int y, char orientation){
	this->m_health = health;
	this->position.x = x;
	this->position.y = y;
	this->rotation = orientation;
}
