#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct Node{
    int weight;
    int value;
    char name[15];
    int index;
    struct Node* next;
};
struct Word{
    char letter;
    int index;
    struct Word* next;
};
struct Word* wordhead = NULL;
// struct Table{
//     int indexI, indexJ;
//     int weight;
//     struct Node* next;
//     struct Node* prev;
// };
struct Node* head = NULL;
struct Node* head1 = NULL;
//struct Table* headtable = NULL;
int totalvalue = 0;
int totalweight = 0;
int rank = 0;
int maxweight;
struct Node* find(struct Node* node, int targetIndex);


struct Node* insert2(int weight, int value, char name[15]){
    if(head == NULL){
        struct Node* node = (struct Node*) malloc(sizeof(struct Node));
        node->index = rank++;
        node->weight = weight;
        node->value = value;
        strcpy(node->name, name);
        //memcpy(&node->name, &name, sizeof(name[15]));
        node->next = NULL;
        head = node;
        return head;
    }
    if(head->next == NULL){
        struct Node* node = (struct Node*) malloc(sizeof(struct Node));
        node->index = rank++;
        node->weight = weight;
        node->value = value;
        strcpy(node->name, name);
        node->next = NULL;
        //memcpy(&node->name, &name, sizeof(name[15]));
        head->next = node;
        return head;
    }
    struct Node* node1 = head;
    struct Node* node2 = head->next;
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->index = rank++;
    node->weight = weight;
    node->value = value;
    strcpy(node->name, name);
    node->next = NULL;
    
    while(node2 != NULL){
        node1 = node1->next;
        node2 = node2->next;
    }
    node1->next = node;
    return head;
}

struct Node* insert(int weight, int value, char name[15]){
    if(head == NULL){
        struct Node* node = (struct Node*) malloc(sizeof(struct Node));
        node->index = rank++;
        node->weight = weight;
        node->value = value;
        strcpy(node->name, name);
        //memcpy(&node->name, &name, sizeof(name[15]));
        node->next = NULL;
        head = node;
        return head;
    }
     if(value / weight > head->value / head->weight){
        struct Node* node = (struct Node*)malloc(sizeof(struct Node));
        node->index = rank++;
        node->weight = weight;
        node->value = value;
        strcpy(node->name, name);
       // memcpy(&node->name, &name, sizeof(name[15]));
        node->next = head;
        head = node;
        return head;
    }

    struct Node* node1 = head;
    struct Node* node2 = head->next;
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->index = rank++;
    node->weight = weight;
    node->value = value;
    strcpy(node->name, name);
    //memcpy(&node->name, &name, sizeof(name[15]));
    node->next = NULL;

    while(node2 != NULL){
        if(node2->value / node2->weight  > value / weight && node1->value / node1->weight < value / weight){
            node1->next = node;
            node->next = node2;
            return head;
        }
        if(node2->value / node2->weight < value / weight && node1->value / node1->weight == value/weight){
            node1->next = node;
            node->next = node2;
            return head;
        }
        node1 = node1->next;
        node2 = node2->next;
    }
    node1->next = node;
    return head;
}

//recursive method?
// struct Node* replace(struct Node* node, struct Node* x){
//     int weightsum = node->weight;
//     struct Node* node1 = node->next;
//     while(node1 != NULL){
//        // if(weightsum )
//     }

//     return head1;
// }

struct Node* insert1(struct Node* x){
    if(totalweight + x->weight > maxweight){
        return head1;
    }
    if(head1 == NULL){
        struct Node* node = (struct Node*) malloc(sizeof(struct Node));
        node->index = x->index;
        strcpy(node->name, x->name);
        node->value = x->value;
        node->weight = x->weight;
        node->next = NULL;
        head1 = node;
        totalweight = totalweight + node->weight;
        totalvalue = totalvalue + node->value;
        return head1;
    }
    if(x->index < head1->index){
        struct Node* node = (struct Node*)malloc(sizeof(struct Node));
        node->index = x->index;
        strcpy(node->name, x->name);
        node->value = x->value;
        node->weight = x->weight;
        node->next = head1;
        head1 = node;
        totalweight = totalweight + node->weight;
        totalvalue = totalvalue + node->value;
        return head1;
    }
    struct Node* node1 = head1;
    struct Node* node2 = head1->next;
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->index = x->index;
    strcpy(node->name, x->name);
    node->value = x->value;
    node->weight = x->weight;
    node->next = NULL;
    while(node2 != NULL){
        if(node2->index > x->index && node1->index < x->index){
            node1->next = node;
            node->next = node2;
            totalweight = totalweight + node->weight;
            totalvalue = totalvalue + node->value;
            return head1;
        }
        node1 = node1->next;
        node2 = node2->next;
    }
    node1->next = node;
    totalweight = totalweight + node->weight;
    totalvalue = totalvalue + node->value;
    return head1;
}

 void deleteList(struct Node* node){
     /*if(node->next == NULL){
         printf("\nLine number: %d\n", __LINE__);
         free(node);
         return;
     }
     struct Node* node2 = node->next;
     struct Node* node1 = node;
     while(node2 != NULL){
         printf("\nLine number: %d\n", __LINE__);
         
         free(node1);
         struct Node* node1 = node2;
         node2 = node2->next;
     }
     */
    struct Node* node2;
    struct Node* node1 = node;
    while(node1 != NULL){
        node2 = node1->next;
        free(node1);
        node1 = node2;
    }
 }

 int higher(int a, int b){ 
     if(a > b){
         return a;
     }
     return b;
}
// struct Table* intersect(struct ** table, int x, int y){

//     for(int i = 0; i <= x; i++){
//         for(int j = j; j <= y; j++){
//             if()
//         }
//     }

// }
 

int knapSack(int maxweight, struct Node* node, int numberofitems){
  //  printf("l;kjlLine number: %d", __LINE__);
    struct Node* tail = node;
  //  struct Table** table; 
    int table[100][100];
 
    // Build table K[][] in bottom up manner
    for (int i = 0; i <= numberofitems; i++){
        int binary = 1;
        node = head;

        for (int j = 0; j <= maxweight; j++){
            binary = 1;
            if (i == 0 || j == 0){

                table[i][j] = 0;
                binary = 0;
            }
            else if (tail->weight <= j){
                table[i][j] = higher(tail->value + table[i - 1][j - tail->weight], table[i - 1][j]);
            }
            else{
                table[i][j] = table[i - 1][j];
            }
        //printf("%s\n",tail->name );
          //  printf("%d, ", table[i][j]);
          //  printf("i, j: %d %d", i, j);
        }
        if(binary == 1){
           tail = tail->next;
            binary = 0;
        }
        if(tail == NULL){
            tail = head;
           // printf("REDO");
        }
       //  printf("\n");
    }

    int coordinate = table[numberofitems][maxweight];
 //   int answer[numberofitems];
        int boolean = 1;
        for(int i = numberofitems - 1; i >= 0; i--){
            
            boolean = 1;
         //   printf("Loop Value of iand coordinate: %d %d\n", i, coordinate);
            for(int j = maxweight; j >= 0; j--){
             //   printf("%d, %d, Value of coordinate and table: %d %d\n",i, j, coordinate, table[i][j]);
                if(coordinate == table[i][j]){
                   // answer[i] = 0;
                    while(coordinate == table[i][j] && i > 0){
                       // printf("Match between coordinate and table: %d, %d, i and j values: %d %d\n", coordinate, table[i][j], i, j);
                        i--;         
                    }

                    head1 = insert1(find(head, i));
                  //  printf("\nInserting node with name/index: %d, %s \n", i, find(head,i)->name);
                    coordinate = coordinate - find(head, i)->value;
                    

                    boolean = 0;
                    i++;
                    break;
                }
                
            }
            if(coordinate == 0){
                    break;
                }
            if(boolean == 1){
               // answer[i] = 1;
             //  printf("Looking for node with index: %d\n", i);
           //  printf("\nInserting node with name/index: %d, %s \n", i, find(head,i)->name);
                head1 = insert1(find(head, i));
              // printf("\ni = %d, we have found %s\n", i, find(head, i)->name);
                coordinate = coordinate - find(head, i)->value;
            }
        
        }            
    
 
    return table[numberofitems][maxweight];
}

struct Node* find(struct Node* node, int targetIndex){
    while(node != NULL){
        if(node->index == targetIndex){
         //   printf("%s is the solution.\n", node->name);
            return node;
        }
        node = node->next;
    }
    return NULL;
}

// char [15] readWord(FILE* fp){
//     char x = fgetc(fp);
//     char names[15];
//     int i = 0;
//     while(x != ' '){
//         names[i] = x;
//         x = fgetc(fp);
//     }

//     return names;
// }


int main(int argc, char **argv){
    FILE* fp = fopen(argv[2], "r");
    if(!fp) return(EXIT_FAILURE);
    //printf("\nLine number: %d\n", __LINE__);
    maxweight = atoi(argv[1]);
    int numberofitems;
    fscanf(fp, "%d", &numberofitems);

    if(numberofitems < 0){
        return(EXIT_FAILURE);
    }
    int i = 0;        
    int j = 0;

    while(i < numberofitems){
      //  printf("Value of i: %d\n", i);
        j = 0;
        char x = fgetc(fp);
        while(x != ' '){
            if(wordhead == NULL){
                struct Word* word = (struct Word*)malloc(sizeof(struct Word));
                word->letter = x;
                word->index = j;
                word->next = NULL;
                wordhead = word;
            }
            else{
                struct Word* word1 = wordhead;
                struct Word* word2 = wordhead->next;
                struct Word* word = (struct Word*)malloc(sizeof(struct Word));
                word->letter = x;
                word->index = j;
                word->next = NULL;
                while(word2 != NULL){
                    word1 = word1->next;
                    word2 = word2->next;
                }
                word1->next = word;
            }
            j++;
            x = fgetc(fp);
        }
        char name [15];
        struct Word* word = wordhead;
      //  printf("%d\n", j);
        for(int k = 0; k < j; k++){
           // printf("%c", word->letter);
            name[k] = word->letter;
            word = word->next;
          
        }
        name[j] = '\0';
      //  printf("\n");
        struct Word* word2;
        struct Word* word1 = wordhead;
        while(word1 != NULL){
        word2 = word1->next;
        free(word1);
        word1 = word2;
        wordhead = NULL;
    }
 //   printf("HOLA;");
       // printf("%s", name);

        int weight;
        int value;
        fscanf(fp, "%d", &weight);
        fscanf(fp, "%d", &value);
      //  printf("\n %s  , %d, %d",name, weight, value);
        head = insert2(weight, value, name);
        i++;
     //   printf("\nnext iteration, value of i: %d\n", i);
    }

    struct Node* node = head;

    // while(node != NULL){
    //     printf("%s, %d, %d, %d\n", node->name, node->weight, node->value, node->index);
    //     head1 = insert1(node);
    //     node = node->next;
    // }
 //   printf("\n\n\n");
    /*
    printf("\n\n\nNew Head:\n\n\n");

   
  
    */
  // printf("240:    %s\n",node->name );
 // printf("\n first: %d,third: %d\n",maxweight, numberofitems);
 // printf("%d, %s, %d, %d", node->index, node->name, node->value, node->weight);

    knapSack(maxweight, node, numberofitems);

    node = head1;
    //printf("\n\n\n");
    while(node != NULL){
        char x = node->name[1];
        int beta = 1;
            while(x != '\0'){
                beta++;
                printf("%c", x);
                x = node->name[beta];
            }
            printf("\n");
       // printf("CHAR 0 IS : %c", x);
       // printf("%s", node->name);
        node = node->next;
    }
    
      printf("%d / %d", totalvalue, totalweight);

    deleteList(head);
    deleteList(head1);

  //  printf("FIN.");

    return(EXIT_SUCCESS);
}