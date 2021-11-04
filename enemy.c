#include "enemy.h"

int enemy_aggro(Enemy* this, Tank* tank){
	switch(this->tank.rotation){
		case 'l':
			if(tank->position.x == this->tank.position.x)
			if(tank->position.y <= this->tank.position.y)
			return 1;
			break;
		case 'r':
			if(tank->position.x == this->tank.position.x)
			if(tank->position.y >= this->tank.position.y)
			return 1;
			break;
		case 'u':
			if(tank->position.y == this->tank.position.y)
			if(tank->position.x <= this->tank.position.x)
			return 1;
			break;
		case 'd':
			if(tank->position.y == this->tank.position.y)
			if(tank->position.x >= this->tank.position.x)
			return 1;
			break;
	}
	return 0;
}
