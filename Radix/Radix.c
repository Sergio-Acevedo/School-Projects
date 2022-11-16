#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>

struct list{
	float num;
	struct list* next;
	struct list* prev;
};

union ufi{
	float f;
	int i;
}u;

void delete(struct list* head){
	if(head == NULL) return;
	if(head->next == NULL){
		free(head);
		return;
	}
	struct list* node = head;
	struct list* node1 = head->next;
	while(node1->next != NULL){
		free(node);
		node = node1;
		node1 = node1->next;
	}
	free(node);
	free(node1);
}

struct list* insert(struct list* head, float num){
	struct list* node = (struct list*)malloc(sizeof(struct list));
	node->num = num;
	node->next = NULL;
	if(head == NULL){
		head = node;
		head->prev = head;
		return head;
	}
	// struct list* node1 = head; 
	// struct list* node2 = head->next;
	// while(node2 != NULL){
	// 	node2 = node2->next;
	// 	node1 = node1->next;
	// }
	// node1->next = node;
	// node->prev = node1;
	// head->prev = node;
	// return head; Circular list implementation for faster insertions
	struct list* node1 = head->prev;
	node1->next = node;
	node->prev = node1;
	head->prev = node;
	return head;
}		

void printNum(struct list* head){
	if(head == NULL){
		printf("empty list.\n");
		return;
	}
	struct list* node = head;
	while(node->next != NULL){
		printf("%f, ", node->num);
		node = node->next;
	}
	printf("%f, ", node->num);
}

int main(int argc, char *argv[])
{
	char *filename = argv[1];
	int file = open(filename, O_RDWR, S_IRUSR | S_IWUSR);
	struct stat f;
	if(fstat(file,&f) == -1){
		perror("no");
	}

	float *memFile = mmap(NULL, f.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, file, 0);
	for(int x = 0; x < 32; x++){
		struct list* hhigh = NULL;
		struct list* hlow = NULL;
		for(int i = 0; i < f.st_size/sizeof *memFile; i++){
			union ufi u;
			u.f = memFile[i];
			unsigned int b = u.i & 0x1; 
			float sign = u.f;
			for(int j = 0; j < 31; j++){
				b = u.i & 0x1; 
				u.i = u.i >> 1;
			}
			u.f = memFile[i];
			int k = 0;
			while(k < x){
				k++;
				u.i = u.i >> 1;
			}
			b = u.i & 0x1;
			if(b == 1){
				hhigh = insert(hhigh, memFile[i]);
			}
			else if( b == 0){
				hlow = insert(hlow, memFile[i]);
			}
			u.i = u.i << 1;
		}

		struct list* lowNode = hlow;
		struct list* highNode = hhigh;
		if(x == 31){
			highNode = highNode->prev;
			while(highNode != hhigh){
				memFile[z] = highNode->num;
				z++;
				highNode = highNode->prev;
			}
			memFile[z] = highNode->num;
			z++;
			while(lowNode != NULL){
				memFile[z] = lowNode->num;
				z++;
				lowNode = lowNode->next;
			}
		}
		else{
			while(lowNode != NULL){
				memFile[z] = lowNode->num;
				z++;
				lowNode = lowNode->next;
			}

			while(highNode != NULL){
				memFile[z] = highNode->num;
				z++;
				highNode = highNode->next;
			}
		}
		
		for(int l = 0; l < f.st_size/sizeof *memFile; l++){
			u.f = memFile[l];
			unsigned int b = u.i & 0x1; 
			float sign = u.f;
			for(int m = 0; m < 31; m++){
				b = u.i & 0x1; 
				u.i = u.i >> 1;
			}
		}
		delete(hhigh);
		delete(hlow);
	}

	munmap(memFile, f.st_size);
	close(file);
	return 0;
}
