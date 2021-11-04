
laserTanks: main.o map.o player.o enemy.o tank.o
	gcc main.o map.o player.o enemy.o tank.o -o laserTanks
	
main.o: main.c log.h
	gcc -c main.c

map.o: map.c map.h log.h
	gcc -c map.c

player.o: player.c player.h
	gcc -c player.c

enemy.o: enemy.c enemy.h
	gcc -c enemy.c

tank.o: tank.c tank.h
	gcc -c tank.c

clean:
	rm *.o laserTanks
