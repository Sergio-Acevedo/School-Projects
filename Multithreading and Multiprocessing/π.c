#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <pthread.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdint.h>
#include <sys/types.h>

int main(int argc, char *argv[]){
    int processes = atoi(argv[1]);
	int N = atoi(argv[2]);
	int* map;
	int id;
	float *destinationFile;
	int processKey;
	int fileSize = 8 *sizeof(double);
	double PI25DT = 3.141592653589793238462643;

	int dFile = open ("texto", O_RDWR | O_CREAT | O_TRUNC, (mode_t)0777);
	int shareFile = open("algo.txt", O_RDWR | O_CREAT | O_TRUNC, (mode_t)0777);

	ftruncate(shareFile, fileSize);
	ftruncate(dFile, fileSize);

	map = mmap(NULL, fileSize, PROT_READ | PROT_WRITE, MAP_SHARED, shareFile, 0);
	destinationFile = mmap(NULL, fileSize, PROT_READ | PROT_WRITE, MAP_SHARED, dFile, 0);

	for(int i = 0; i < 8; i++){
		map[i] = 10;
	}
	for(int i = 1; i < processes; i++){
		id = fork();
		if(id == 0) break;
	}
	for(int i = 0; i < 8; i++){
		if(map[i] != i){
			map[i] = i;
			processKey = i;
			break;
		}
	}
	close(shareFile);
    int i, j; // rank = processkey
    double x;
	double h = 1/(double)N;

	destinationFile[processKey] = 0;
	for(i = N/processes*processKey+1; i <=N/processes*(processKey+1); i++){
		x = h*((double)i - .5);
		destinationFile[processKey] += 4.0/(1.0+x*x);
	}
	float mypi = 0;
	for(int i = 0; i < processes; i++){
		mypi += 1.0/(double)N*destinationFile[i];
	}
	if(processKey == processes-1) {
		destinationFile[0] = mypi;
	}
	
	while(wait(NULL) != -1 || errno != ECHILD);
	if (id != 0)
		printf("pi is approximately %.16f, Error is %.16f\n", destinationFile[0], fabs(destinationFile[0] - PI25DT));
		
	munmap(destinationFile, fileSize);
	close(dFile);
	return 0;
}