#include <stdio.h>
#include <math.h>

//border size
#define SIZE 20
#define MAXSTEPS 2

//planet values
typedef struct{
	int size;
	int x;
	int y;
	int vx;
	int vy;
}values;


//planet, can eiter be acessed by varname or as array
typedef union{
	int value[5];
	values v;
}Planet;


void printPlanets(int numPlanets, Planet *planets){
	for(int i = 0; i < numPlanets; i++){
		printf("Planet %d %d %d %d %d\n", planets[i].v.size, planets[i].v.x, planets[i].v.y,planets[i].v.vx,planets[i].v.vy);		
	}
}


int main(int argc, char **argv){
	//require parameters 
	if(argc < 2){
		printf("planet [file_in]\n");
		return 0;
	}

	double gConst = 6.674*0.00000001;
	
	//init planets 
	int numPlanets;
	FILE* file = fopen(argv[1], "r"); 
	fscanf(file, "%d", &numPlanets);
	Planet planets[numPlanets];
	Planet nSPlanets[numPlanets]; //next planet locations
	for(int i = 0; i < numPlanets; i++){
		fscanf(file, "%d %d %d", &planets[i].v.size, &planets[i].v.x, &planets[i].v.y);		
		planets[i].v.vx = 0; planets[i].v.vy = 0;
		nSPlanets[i].v.size = planets[i].v.size;
		nSPlanets[i].v.vx = 0; nSPlanets[i].v.vy = 0;
	}
	fclose(file);

	printPlanets(numPlanets, planets);
	for(int s = 0; s < MAXSTEPS; s++){
		printf("STEP %d\n", s);
		for(int i = 0; i < numPlanets; i++){
			int resultingx = planets[i].v.vx;
			int resultingy = planets[i].v.vy;
			for(int j = 0; j < numPlanets; j++){
				if(i==j) continue;
				double dx = (planets[i].v.x - planets[j].v.x);
				double dy = (planets[i].v.y - planets[j].v.y);				
				double dist = sqrt((dx*dx)+(dy*dy));
				double f = gConst*planets[i].v.size*planets[j].v.size/dist;
				f /= planets[i].v.size;
				resultingx += (dx/(dx+dy))*f;
				resultingy += (dx/(dx+dy))*f;
			}
			nSPlanets[i].v.vx = planets[i].v.vx + resultingx;
			nSPlanets[i].v.vy = planets[i].v.vy + resultingy;
			nSPlanets[i].v.x = planets[i].v.x + nSPlanets[i].v.vx;
			nSPlanets[i].v.y = planets[i].v.y + nSPlanets[i].v.vy;			
		}
		printPlanets(numPlanets, nSPlanets);
		break;
	/*
		Planet *aux = (Planet*)nSPlanets;
		*nSPlanets = &planets;
		planets = aux;

		printPlanets(numPlanets, planets);*/
	}

	

	Planet a;
	printf("ax %d int[1] %d\n", a.v.x, a.value[1]);
	a.v.x = 1;
	printf("ax %d int[1] %d\n", a.v.x, a.value[1]);
	return 0;
}