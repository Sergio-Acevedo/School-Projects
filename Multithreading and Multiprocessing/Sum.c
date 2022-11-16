#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <pthread.h>
#include <unistd.h>

float *memFile;
struct stat f;
int processes;

void* sum(void* start){
	int index = *(int*) start;
	float* sum = (float*)malloc(sizeof(float));
	*sum = 0;
	int size = f.st_size/sizeof *memFile;
	index = (size/processes)*index;
	// printf("\nthread beginning index is: %d", index);
	// printf("\nindex %% thread at beginning is and initial index are:%d / %d\n", index, index % threads);

	do{
		// printf("\n Added index/float value: %d / %f", index, memFile[index]);
		*sum += memFile[index++];
	} while(index < size && index % (size / processes) != 0);
	// printf("\nSum is: %f", *sum);
	free(start);
	// printf("\n%d", __LINE__);
	return (void*) sum;
}

int main(int argc, char *argv[]){
	float* results[8];
	processes = atoi(argv[1]);
	char *filename = argv[2];
	int file = open(filename, O_RDWR, S_IRUSR | S_IWUSR);

	if(fstat(file,&f) == -1){
		perror("no");
	}
	memFile = mmap(NULL, f.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, file, 0);
	pthread_t thread[8];

	for(int i = 0; i < processes; i++){
		int* arr = malloc(sizeof(int));
		*arr = i;
		// printf("\ninserting %d into new thread", i);
		if(pthread_create(&thread[i], NULL, sum, (void*) arr) != 0) {
			perror("Fail");
			return 1;
		}
	}
	for(int i = 0; i < processes; i++){
		if(pthread_join(thread[i], (void**) &results[i]) != 0) {
			perror("Fail");
			return 2;
		}
		// printf("\n%d", __LINE__);
	}
	// printf("\n%d", __LINE__);
	float x = 0;
	for(int i = 0; i < processes; i++){
		x += *results[i];
	}
	printf("%f\n", x);
	munmap(memFile, f.st_size);
	close(file);
	return 0;
}
