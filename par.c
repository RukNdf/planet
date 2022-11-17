#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"

//border size
#define MAXSTEPS 100
#define GRAVITATIONALMOD 2

//planet values
typedef struct{
	double size;
	double x;
	double y;
	double vx;
	double vy;
}values;


//planet, can eiter be acessed by varname or as array
typedef union{
	double value[5];
	values v;
}Planet;


//print planets values
void printPlanets(int numPlanets, Planet *planets){
	for(int i = 0; i < numPlanets; i++){
		printf("Planet mass %.2f x %.2f y %.2f vx %.2f vy %.2f\n", planets[i].v.size, planets[i].v.x, planets[i].v.y,planets[i].v.vx,planets[i].v.vy);		
	}
}


//---
int main(int argc, char **argv){
	//require parameters 
	if(argc < 2){
		printf("planet [file_in]\n");
		return 0;
	}

	double gConst = 6.674*GRAVITATIONALMOD;

	
	//--------------------------------
	//init planets from file
	//--------------------------------
	int numPlanets;
	FILE* file = fopen(argv[1], "r"); 
	fscanf(file, "%d", &numPlanets);
	//cur and next planet locations
	Planet *planets = malloc(sizeof(Planet)*numPlanets);
	Planet *nSPlanets = malloc(sizeof(Planet)*numPlanets); 
	//init size and pos
	for(int i = 0; i < numPlanets; i++){
		fscanf(file, "%lf %lf %lf", &planets[i].v.size, &planets[i].v.x, &planets[i].v.y);		
		planets[i].v.vx = 0; planets[i].v.vy = 0;
		nSPlanets[i].v.size = planets[i].v.size;
		nSPlanets[i].v.vx = 0; nSPlanets[i].v.vy = 0;
	}
	fclose(file);

	
	//--------------------------------
	//MPI
	//--------------------------------
	int my_rank, proc_n, message;
	MPI_Status status;
	MPI_Init(&argc, &argv);	
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &proc_n);
	

	//--------------------------------
	//init planets nums
	//--------------------------------
	//num of planets of each node
	int nums[proc_n];
	//calculate num values 
	double sum = 0;
	double div = numPlanets/(double)proc_n;
	for(int i = 0; i < proc_n-1; i++){
		double next = sum+div;
		nums[i] = next-((int)sum);
		sum = next;
	}
	nums[proc_n-1] = numPlanets-(int)sum;
	//number of previous planets
	int prev = 0; for(int i = 0; i < my_rank; i++) prev += nums[i];
	//number of planets that belong to node plus previous planets
	int limit = prev+nums[my_rank];


	//--------------------------------
	//print initial state
	//--------------------------------
	if(my_rank == 0){
		//initial state
		printPlanets(numPlanets, planets);
	}
	

	//--------------------------------
	//main loop
	//--------------------------------
	for(int s = 0; s < MAXSTEPS; s++){
		if(my_rank == 0) printf("\nSTEP %d\n", s);

		//--------------------------------
		//process
		//--------------------------------
		//for each planet that belongs to this node
		for(int i = prev; i < limit; i++){
			double resultingx = planets[i].v.vx;
			double resultingy = planets[i].v.vy;
			//for each pair of planets
			for(int j = 0; j < numPlanets; j++){
				if(i==j) continue;
				//calculate distance
				double dx = abs(planets[i].v.x - planets[j].v.x);
				double dy = abs(planets[i].v.y - planets[j].v.y);				
				double dist = sqrt((dx*dx)+(dy*dy));
				//calculate attraction force
				double f = gConst*planets[i].v.size*planets[j].v.size/dist;
				//calculate resulting speed according to mass
				f /= planets[i].v.size;
				//separate diagonal speed into x speed and y speed
				double vx = (dx/(dx+dy))*f;
				double vy = (dy/(dx+dy))*f;
				//fix direction
				if(planets[i].v.x > planets[j].v.x) vx *= -1;
				if(planets[i].v.y > planets[j].v.y) vy *= -1;
				//sum
				resultingx += vx;
				resultingy += vy;
			}
			//update values on auxiliar array
			nSPlanets[i].v.vx = resultingx;
			nSPlanets[i].v.vy = resultingy;
			nSPlanets[i].v.x = planets[i].v.x + nSPlanets[i].v.vx;
			nSPlanets[i].v.y = planets[i].v.y + nSPlanets[i].v.vy;	
		}

		//--------------------------------
		//comm
		//--------------------------------
		int index = 0;
		MPI_Bcast(nSPlanets, nums[0]*5, MPI_DOUBLE, 0, MPI_COMM_WORLD);
		index += nums[0];
		for(int i = 1; i < proc_n; i++){
			MPI_Bcast(&nSPlanets[index], nums[i]*5, MPI_DOUBLE, i, MPI_COMM_WORLD);
			index += nums[i];
		}

		//switch next into cur
		Planet *aux = nSPlanets;
		nSPlanets = planets;
		planets = aux;

		//print result
		if(my_rank == 0) printPlanets(numPlanets, planets);
	}

	MPI_Finalize();	
	return 0;
}