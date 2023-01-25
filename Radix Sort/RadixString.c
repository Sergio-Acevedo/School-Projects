#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>

//   Program receives no user input, as it randomly creates its own input for ease of testing (See inputGenerator function). Input is created at f.txt, and output is saved at g.txt.
//   Uses Posix functions/headers, and so will not run on Windows systems. Program written on a Linux computer.

FILE *fp;
char *map;
struct list *array[27]; // Array of linked lists. 27 buckets, one per unique letter, 1 bucket for blanks, with blanks going before letter buckets since "AA" goes before "AAB".
int character; //Character counter for entire document for when copying g.txt back to struct *array character-by-character.

struct list{
	char word[22];
	struct list* next;
	struct list* prev;
};

void insert(int letterIndex, char *word){ 						   // Inserting given string into a bucket determined by the index integer given. 
	struct list* node = (struct list*)malloc(sizeof(struct list)); // Since a string is always put at the end of the linked list, a (half circular) doubly linked list was used. 
	strncpy(node->word, word, 21);								   // This allows direct access to the tail from the head of the list, without needlessly traversing the entire list to reach the tail every time.
	node->next = NULL;											   // Since the tail will never need access to the head, the tail->next is simply set to NULL, so circular one way, but not the other, hence, half circular.
	if(array[letterIndex] == NULL){
		array[letterIndex] = node;
		array[letterIndex]->prev = array[letterIndex];
		return;
	}
	struct list* node1 = array[letterIndex]->prev;
	node1->next = node;
	node->prev = node1;
	array[letterIndex]->prev = node;
}		

void printNum(struct list* head){ // Print every string in array of linked list, blank bucket goes first, then A-Z buckets, in that order.
	struct list* node = head;
	while(node != NULL){
		printf("%s\n", node->word);
		node = node->next;
	}
}

void inputGenerator(){ // Randomly generate a file with random string quantity numbering between 1-1000 inclusively.
	srand(time(0));    // Each individual string is assigned a random length between 1-21, with the remaining space filled with space characters.
	int size = (rand() % 1000)+1;
	fprintf(fp, "%d\n", size);
	for(int i = 0; i < size; i++){
		int stringLength = (rand() % 21 ) + 1;
		char word[21];
		int j;
		for(j = 0; j < stringLength; j++) 
			word[j] = 'A' + (random() % 26);
		while(j < 21) word[j++] = ' ';
		fprintf(fp, "%s\n", word);
	}
}

void dump(){ 						// Writing all values from the struct *array into the g.txt file.
	character = 0;					// Since a large file is constantly being (over)written to, mmap is used for higher speed.
	for(int i = 0; i < 27; i++){
		struct list *node = array[i];
		while(node != NULL){
			for(int j = 0; j < 21; j++)
				map[character++] = node->word[j];
			map[character++] = '\n';
			node = node->next;
		}
	}	
}

void destroy(){		//Freeing every node in struct *array either to start a new round of sorting, or end the program with no memory leaks.
	for(int i = 0; i < 27; i++){
		if(array[i] == NULL) continue; // Empty bucket, try to free next one. Effectively guaranteed to happen for first few rounds since
		struct list *node = array[i];  // most strings aren't the full 21 characters long, most will be sorted to the blank bucket, leaving most letter buckets empty.
		struct list *node1 = array[i]->next;
		while(node1 != NULL){
			free(node);
			node = node1;
			node1 = node->next;
		}
		free(node);
		array[i] = NULL;
	}
}

int main(){
	fp = fopen("f.txt", "w+");
	inputGenerator();
	rewind(fp);

	char *line = NULL;
	size_t len = 0;
	int size;
	int countLines = 0;
	
	getline(&line, &len, fp); //Linux/Unix/Posix

	size = atoi(line);
	int filesize = size * sizeof(char)*22;
	int fd = open("g.txt", O_RDWR | O_CREAT | O_TRUNC, (mode_t)0600);
	lseek(fd, filesize-1, SEEK_SET);
	write(fd, "", 1);
	map = mmap(0, filesize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	for(int i = 0; i < 27; i++)
		array[i] = NULL;

	for(int j = 0; j < size; j++){ // First round of sorting is based on input f.txt.
		int x = getline(&line, &len, fp);		
		if(x == -1) continue;
		if(line[20] == ' '){
			insert(0, line);
			countLines++;
			continue;
		}
		insert(line[20]-64, line);
		countLines++;
	}
	dump();
	destroy();
	
	for(int i = 19; i >= 0; i--){ // Subsequent rounds of sorting read/write from/to g.txt 
		int index = 0;		      // with every round of sorting to leave the original input f.txt file unaltered.
		for(int j = 0; j < countLines; j++){
			char word[22];
			for(int k = 0; k < 22 && index < character; k++)
				word[k] = map[index++];
			if(word[i] == ' ')
				insert(0, word);
			else
				insert(word[i]-64, word);
		}		
		// for(int j = 0; j < 27; j++){ 	// prints the resulting buckets in order per sort round
		// 	printf("list %d: \n", j);
		// 	printNum(array[j]);
		// 	printf("\n\n");
		// }
		dump();
		destroy();
	}
	fclose(fp);
	munmap(map, filesize);
	return 0;
}