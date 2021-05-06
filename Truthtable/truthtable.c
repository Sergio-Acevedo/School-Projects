#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct inputNode{
    char value[20];
    struct inputNode* next;
    int boolean;
};
struct tempNode{
    char value[20];
    struct tempNode* next;
    int boolean;
};
struct circuitNode{
    char operator[16];
    char input1[16];
    char input2[16];
    char words1[32][32];
    char words2[32][32];
    char output[32];
    int n;
    int extrainput;
    int arr1[64];
    int arr2[64];
    struct circuitNode* nextgate;
};
int binarytodecimal(char stringnumber[]){
    int binary = atoi(stringnumber);
    int hold = binary;
    int left;
    int base = 1;
    int decimal = 0;
     while(hold > 0)
    {
        left = hold % 10;
        decimal = decimal + left * base;
        hold = hold / 10;
        base = base * 2;
    }
    return decimal;
}
int powoftwo(int base, int exponent){
    for(int i = exponent; i > 1; i --){
        base = base * 2;
    }
    return base;
}
long longpowoftwo(long base, long exponent){
    for(int i = exponent; i > 1; i --){
        base = base * 2;
    }
    return base;
}
void freeinputNode(struct inputNode* head){
    struct inputNode* node2;
    struct inputNode* node1 = head;
    while(node1 != NULL){
        node2 = node1->next;
        free(node1);
        node1 = node2;
    }
}
void freetempNode(struct tempNode* head){
    struct tempNode* node2;
    struct tempNode* node1 = head;
    while(node1 != NULL){
        node2 = node1->next;
        free(node1);
        node1 = node2;
    }
}
void freecircuitNode(struct circuitNode* head){
    struct circuitNode* node2;
    struct circuitNode* node1 = head;
    while(node1 != NULL){
        node2 = node1->nextgate;
        free(node1);
        node1 = node2;
    }
}

int main(int argc, char **argv){
    FILE* fp = fopen(argv[1], "r");
    char word[20];
    int clock[64] = {0};
    int inputnum, outputnum;
    long inputpower = 2;
    //long shit = 2;
    struct inputNode* headinput = NULL;
    fscanf(fp, "%s", word);
    fscanf(fp, "%s", word);
    inputnum = atoi(word);
    // for (int i = inputnum; i > 1; i--){
    //     inputpower = inputpower * 2;
    // }
    inputpower = longpowoftwo(inputpower,(long) inputnum);
   // printf("Number of inputs: %d\n", inputnum);

    struct inputNode* headnode = (struct inputNode*)malloc(sizeof(struct inputNode));
    fscanf(fp, "%s", headnode->value);
    headnode->next = NULL;
  //  headnode->booleans = NULL;
    headinput = headnode;

    struct inputNode* node = headinput;
    int i = 1;
    while(i < inputnum){
        struct inputNode* node1 = (struct inputNode*)malloc(sizeof(struct inputNode));
        fscanf(fp, "%s", node1->value);
        node1->next = NULL;
       // node1->booleans = NULL;
        node->next = node1;

        node = node->next;
        node1 = node1->next;
        i++;
    }
    node = headinput;
    // while(node != NULL){
    //     printf("%s", node->value);
    //     node = node->next;
    // }
    fscanf(fp, "%s", word);
    fscanf(fp, "%s", word);
    outputnum = atoi(word);
   // printf("\nOutputs: %d\n", outputnum);
    struct inputNode* headoutput = NULL;
    struct inputNode* headnode1 = (struct inputNode*)malloc(sizeof(struct inputNode));
    fscanf(fp, "%s", headnode1->value);
    headnode1->next = NULL;
  //  headnode1->booleans = NULL;
    headoutput = headnode1;

    node = headoutput;
    i = 1;
    while(i < outputnum){
        struct inputNode* node1 = (struct inputNode*)malloc(sizeof(struct inputNode));
        fscanf(fp, "%s", node1->value);
        node1->next = NULL;
      //  node1->booleans = NULL;
        node->next = node1;

        node = node->next;
        node1 = node1->next;
        i++;
    }
     node = headoutput;
    // while(node != NULL){
    //     printf("%s", node->value);
    //     node = node->next;
    // }

    // Build circuit data structure
    struct tempNode* headtemp = NULL;
    struct circuitNode* headcircuit = NULL;
    while(fscanf(fp, "%s", word) != EOF){
        if(strcmp(word, "NOT") == 0){
            if(headcircuit == NULL){
                struct circuitNode* nodec= (struct circuitNode*)malloc(sizeof(struct circuitNode));
                strcpy(nodec->operator, word);
                fscanf(fp, "%s", word);
                strcpy(nodec->input1, word);
                fscanf(fp, "%s", word);
                strcpy(nodec->output, word);
                nodec->nextgate = NULL;
                headcircuit = nodec;
                printf("\n\n\n%s, %s, %s", headcircuit->operator, headcircuit->input1, headcircuit->output);
                continue;
            }
            struct circuitNode* nodee = headcircuit;
            while(nodee->nextgate != NULL){
                nodee = nodee->nextgate;
            }
            struct circuitNode* nodec = (struct circuitNode*)malloc(sizeof(struct circuitNode));
            strcpy(nodec->operator, word);
            fscanf(fp, "%s", word);
            strcpy(nodec->input1, word);
            fscanf(fp, "%s", word);
            strcpy(nodec->output, word);
            nodec->nextgate = NULL;
            nodee->nextgate = nodec;
            continue;
        }
        if(strcmp(word, "AND") == 0){
            if(headcircuit == NULL){
                struct circuitNode* nodec = (struct circuitNode*)malloc(sizeof(struct circuitNode));
                nodec->extrainput = -1;
                strcpy(nodec->operator, word);
                fscanf(fp, "%s", word);
                if(strcmp(word, "0") == 0){
                    nodec->extrainput = 0;
                }
                else if(strcmp(word, "1") == 0){
                    nodec->extrainput = 1;
                }
                else{
                    strcpy(nodec->input1, word);
                }
                fscanf(fp, "%s", word);
                if(strcmp(word, "0") == 0){
                    nodec->extrainput = 0;
                }
                else if(strcmp(word, "1") == 0){
                    nodec->extrainput = 1;
                }
                else{
                    strcpy(nodec->input2, word);
                }
                fscanf(fp, "%s", word);
                strcpy(nodec->output, word);
                nodec->nextgate = NULL;
                headcircuit = nodec;
                continue;
            }
            struct circuitNode* nodee = headcircuit;
            while(nodee->nextgate != NULL){
                nodee = nodee->nextgate;
            }
            struct circuitNode* nodec = (struct circuitNode*)malloc(sizeof(struct circuitNode));
            nodec->extrainput = -1;
                strcpy(nodec->operator, word);
                fscanf(fp, "%s", word);
                if(strcmp(word, "0") == 0){
                    nodec->extrainput = 0;
                }
                else if(strcmp(word, "1") == 0){
                    nodec->extrainput = 1;
                }
                else{
                    strcpy(nodec->input1, word);
                }
                fscanf(fp, "%s", word);
                if(strcmp(word, "0") == 0){
                    nodec->extrainput = 0;
                }
                else if(strcmp(word, "1") == 0){
                    nodec->extrainput = 1;
                }
                else{
                    strcpy(nodec->input2, word);
                }
                fscanf(fp, "%s", word);
                strcpy(nodec->output, word);
                nodec->nextgate = NULL;
                nodee->nextgate = nodec;
                continue;
        }
        if(strcmp(word, "OR") == 0){
            if(headcircuit == NULL){
                struct circuitNode* nodec = (struct circuitNode*)malloc(sizeof(struct circuitNode));
                nodec->extrainput = -1;
                strcpy(nodec->operator, word);
                fscanf(fp, "%s", word);
                if(strcmp(word, "0") == 0){
                    nodec->extrainput = 0;
                }
                else if(strcmp(word, "1") == 0){
                    nodec->extrainput = 1;
                }
                else{
                    strcpy(nodec->input1, word);
                }
                fscanf(fp, "%s", word);
                if(strcmp(word, "0") == 0){
                    nodec->extrainput = 0;
                }
                else if(strcmp(word, "1") == 0){
                    nodec->extrainput = 1;
                }
                else{
                    strcpy(nodec->input2, word);
                }
                fscanf(fp, "%s", word);
                strcpy(nodec->output, word);
                nodec->nextgate = NULL;
                headcircuit = nodec;
                continue;
            }
            struct circuitNode* nodee = headcircuit;
            while(nodee->nextgate != NULL){
                nodee = nodee->nextgate;
            }
            struct circuitNode* nodec = (struct circuitNode*)malloc(sizeof(struct circuitNode));
            nodec->extrainput = -1;
                strcpy(nodec->operator, word);
                fscanf(fp, "%s", word);
                if(strcmp(word, "0") == 0){
                    nodec->extrainput = 0;
                }
                else if(strcmp(word, "1") == 0){
                    nodec->extrainput = 1;
                }
                else{
                    strcpy(nodec->input1, word);
                }
                fscanf(fp, "%s", word);
                if(strcmp(word, "0") == 0){
                    nodec->extrainput = 0;
                }
                else if(strcmp(word, "1") == 0){
                    nodec->extrainput = 1;
                }
                else{
                    strcpy(nodec->input2, word);
                }
                fscanf(fp, "%s", word);
                strcpy(nodec->output, word);
                nodec->nextgate = NULL;
                nodee->nextgate = nodec;
                continue;
        }
        if(strcmp(word, "NAND") == 0){
            if(headcircuit == NULL){
                struct circuitNode* nodec = (struct circuitNode*)malloc(sizeof(struct circuitNode));
                nodec->extrainput = -1;
                strcpy(nodec->operator, word);
                fscanf(fp, "%s", word);
                if(strcmp(word, "0") == 0){
                    nodec->extrainput = 0;
                }
                else if(strcmp(word, "1") == 0){
                    nodec->extrainput = 1;
                }
                else{
                    strcpy(nodec->input1, word);
                }
                fscanf(fp, "%s", word);
                if(strcmp(word, "0") == 0){
                    nodec->extrainput = 0;
                }
                else if(strcmp(word, "1") == 0){
                    nodec->extrainput = 1;
                }
                else{
                    strcpy(nodec->input2, word);
                }
                fscanf(fp, "%s", word);
                strcpy(nodec->output, word);
                nodec->nextgate = NULL;
                headcircuit = nodec;
                continue;
            }
            struct circuitNode* nodee = headcircuit;
            while(nodee->nextgate != NULL){
                nodee = nodee->nextgate;
            }
            struct circuitNode* nodec = (struct circuitNode*)malloc(sizeof(struct circuitNode));
            nodec->extrainput = -1;
                strcpy(nodec->operator, word);
                fscanf(fp, "%s", word);
                if(strcmp(word, "0") == 0){
                    nodec->extrainput = 0;
                }
                else if(strcmp(word, "1") == 0){
                    nodec->extrainput = 1;
                }
                else{
                    strcpy(nodec->input1, word);
                }
                fscanf(fp, "%s", word);
                if(strcmp(word, "0") == 0){
                    nodec->extrainput = 0;
                }
                else if(strcmp(word, "1") == 0){
                    nodec->extrainput = 1;
                }
                else{
                    strcpy(nodec->input2, word);
                }
                fscanf(fp, "%s", word);
                strcpy(nodec->output, word);
                nodec->nextgate = NULL;
                nodee->nextgate = nodec;
                continue;
        }
        if(strcmp(word, "NOR") == 0){
            if(headcircuit == NULL){
                struct circuitNode* nodec = (struct circuitNode*)malloc(sizeof(struct circuitNode));
                nodec->extrainput = -1;
                strcpy(nodec->operator, word);
                fscanf(fp, "%s", word);
                if(strcmp(word, "0") == 0){
                    nodec->extrainput = 0;
                }
                else if(strcmp(word, "1") == 0){
                    nodec->extrainput = 1;
                }
                else{
                    strcpy(nodec->input1, word);
                }
                fscanf(fp, "%s", word);
                if(strcmp(word, "0") == 0){
                    nodec->extrainput = 0;
                }
                else if(strcmp(word, "1") == 0){
                    nodec->extrainput = 1;
                }
                else{
                    strcpy(nodec->input2, word);
                }
                fscanf(fp, "%s", word);
                strcpy(nodec->output, word);
                nodec->nextgate = NULL;
                headcircuit = nodec;
                continue;
            }
            struct circuitNode* nodee = headcircuit;
            while(nodee->nextgate != NULL){
                nodee = nodee->nextgate;
            }
            struct circuitNode* nodec = (struct circuitNode*)malloc(sizeof(struct circuitNode));
            nodec->extrainput = -1;
                strcpy(nodec->operator, word);
                fscanf(fp, "%s", word);
                if(strcmp(word, "0") == 0){
                    nodec->extrainput = 0;
                }
                else if(strcmp(word, "1") == 0){
                    nodec->extrainput = 1;
                }
                else{
                    strcpy(nodec->input1, word);
                }
                fscanf(fp, "%s", word);
                if(strcmp(word, "0") == 0){
                    nodec->extrainput = 0;
                }
                else if(strcmp(word, "1") == 0){
                    nodec->extrainput = 1;
                }
                else{
                    strcpy(nodec->input2, word);
                }
                fscanf(fp, "%s", word);
                strcpy(nodec->output, word);
                nodec->nextgate = NULL;
                nodee->nextgate = nodec;
                continue;
        }
        if(strcmp(word, "XOR") == 0){
            if(headcircuit == NULL){
                struct circuitNode* nodec = (struct circuitNode*)malloc(sizeof(struct circuitNode));
                nodec->extrainput = -1;
                strcpy(nodec->operator, word);
                fscanf(fp, "%s", word);
                if(strcmp(word, "0") == 0){
                    nodec->extrainput = 0;
                }
                else if(strcmp(word, "1") == 0){
                    nodec->extrainput = 1;
                }
                else{
                    strcpy(nodec->input1, word);
                }
                fscanf(fp, "%s", word);
                if(strcmp(word, "0") == 0){
                    nodec->extrainput = 0;
                }
                else if(strcmp(word, "1") == 0){
                    nodec->extrainput = 1;
                }
                else{
                    strcpy(nodec->input2, word);
                }
                fscanf(fp, "%s", word);
                strcpy(nodec->output, word);
                nodec->nextgate = NULL;
                headcircuit = nodec;
                continue;
            }
            struct circuitNode* nodee = headcircuit;
            while(nodee->nextgate != NULL){
                nodee = nodee->nextgate;
            }
            struct circuitNode* nodec = (struct circuitNode*)malloc(sizeof(struct circuitNode));
            nodec->extrainput = -1;
                strcpy(nodec->operator, word);
                fscanf(fp, "%s", word);
                if(strcmp(word, "0") == 0){
                    nodec->extrainput = 0;
                }
                else if(strcmp(word, "1") == 0){
                    nodec->extrainput = 1;
                }
                else{
                    strcpy(nodec->input1, word);
                }
                fscanf(fp, "%s", word);
                if(strcmp(word, "0") == 0){
                    nodec->extrainput = 0;
                }
                else if(strcmp(word, "1") == 0){
                    nodec->extrainput = 1;
                }
                else{
                    strcpy(nodec->input2, word);
                }
                fscanf(fp, "%s", word);
                strcpy(nodec->output, word);
                nodec->nextgate = NULL;
                nodee->nextgate = nodec;
                continue;
        }
        if(strcmp(word, "DECODER") == 0){
            if(headcircuit == NULL){
                int pow;
                struct circuitNode* nodec = (struct circuitNode*)malloc(sizeof(struct circuitNode));
                strcpy(nodec->operator, word);
                fscanf(fp, "%s", word);
                nodec->n = atoi(word);
                for(int i = 0;i < nodec->n; i++){
                    fscanf(fp, "%s", word);
                    strcpy(nodec->words1[i], word);
                }
                pow = powoftwo(2, nodec->n);
                for(int i = 0; i < pow; i++){
                    fscanf(fp, "%s", word);
                    strcpy(nodec->words2[i], word);
                }
                nodec->extrainput = -1;
                nodec->nextgate = NULL;
                headcircuit = nodec;
                continue;
            }
            struct circuitNode* nodee = headcircuit;
            while(nodee->nextgate != NULL){
                nodee = nodee->nextgate;
            }
            int pow;
                struct circuitNode* nodec = (struct circuitNode*)malloc(sizeof(struct circuitNode));
                strcpy(nodec->operator, word);
                fscanf(fp, "%s", word);
                nodec->n = atoi(word);
                for(int i = 0;i < nodec->n; i++){
                    fscanf(fp, "%s", word);
                    strcpy(nodec->words1[i], word);
                }
                pow = powoftwo(2, nodec->n);
                for(int i = 0; i < pow; i++){
                    fscanf(fp, "%s", word);
                    strcpy(nodec->words2[i], word);
                }
                nodec->extrainput = -1;
                nodec->nextgate = NULL;
                nodee->nextgate = nodec;
                continue;
        }
        if(strcmp(word, "MULTIPLEXER") == 0){
            if(headcircuit == NULL){
                // printf("MULTI FOUND\n");
                int pow;
              //  printf("Line number: %d\n", __LINE__);
                struct circuitNode* nodec = (struct circuitNode*)malloc(sizeof(struct circuitNode));
               // printf("\nLine number: %d\n", __LINE__);
                strcpy(nodec->operator, word);
              //  printf("\nLine number: %d\n", __LINE__);
                fscanf(fp, "%s", word);
                nodec->n = atoi(word);
            //    printf("\nLine number: %d\n", nodec->n);
                pow = powoftwo(2, nodec->n);
                // printf("Pow is %d", pow);
                for(int i = 0; i < pow; i++){
                    fscanf(fp, "%s", word);
                    strcpy(nodec->words1[i], word);
                }
                for(int i = 0; i < nodec->n; i++){
                    fscanf(fp, "%s", word);
                    strcpy(nodec->words2[i], word);
                }
                fscanf(fp, "%s", word);
                strcpy(nodec->output, word);
                nodec->extrainput = -1;
                nodec->nextgate = NULL;
                headcircuit = nodec;
                continue;
            }
            struct circuitNode* nodee = headcircuit;
            while(nodee->nextgate != NULL){
                nodee = nodee->nextgate;
            }
            int pow;
                struct circuitNode* nodec = (struct circuitNode*)malloc(sizeof(struct circuitNode));
                strcpy(nodec->operator, word);
                fscanf(fp, "%s", word);
                nodec->n = atoi(word);
                pow = powoftwo(2, nodec->n);
                for(int i = 0; i < pow; i++){
                    fscanf(fp, "%s", word);
                    strcpy(nodec->words1[i], word);
                }
                for(int i = 0; i < nodec->n; i++){
                    fscanf(fp, "%s", word);
                    strcpy(nodec->words2[i], word);
                }
                fscanf(fp, "%s", word);
                strcpy(nodec->output, word);
                nodec->extrainput = -1;
                nodec->nextgate = NULL;
                nodee->nextgate = nodec;
                continue;
        }
    }
    //printing back whole circuit print back print circuit
    // struct circuitNode* nodee = headcircuit;
    // while(nodee != NULL){
    //     printf("\n%s, ", nodee->operator);
    //     if(strcmp(nodee->operator, "AND") == 0 || strcmp(nodee->operator, "OR") == 0 || strcmp(nodee->operator, "NAND") == 0 || strcmp(nodee->operator, "NOR") == 0 || strcmp(nodee->operator, "XOR") == 0){
    //         printf("%s, %s, %s\n", nodee->input1, nodee->input2, nodee->output);
    //     }
    //     else if(strcmp(nodee->operator, "NOT") == 0){
    //         printf("%s, %s\n", nodee->input1, nodee->output);
    //     }
    //     else if(strcmp(nodee->operator, "DECODER") == 0){
    //         printf("%d ..........", nodee->n);
    //         printf("%s", nodee->words1[0]);
    //         for(int i = 1; i < nodee->n; i++){
    //             printf(", %s", nodee->words1[i]);
    //         }
    //         printf(".......... ");
    //         int j = powoftwo(2, nodee->n);
    //         printf("%s", nodee->words2[0]);
    //         for(int i = 1; i < j; i++){
    //             printf(", %s", nodee->words2[i]);
    //         }
    //     }
    //     else if(strcmp(nodee->operator, "MULTIPLEXER") == 0){
    //         printf("%d ......", nodee->n);
    //         int j = powoftwo(2, nodee->n);
    //         printf("%s", nodee->words1[0]);
    //         for(int i = 1; i < j; i++){
    //             printf(", %s", nodee->words1[i]);
    //         }
    //         printf("..........");
    //         printf("%s", nodee->words2[0]);
    //         for(int i = 1; i < nodee->n; i++){
    //             printf(", %s", nodee->words2[i]);
    //         }
    //         printf("........ %s", nodee->output);
    //     }
    //     nodee = nodee->nextgate;
    // }   

    //clock cycling through circuit
        //printing binary inputs
    printf("\n");
    for(int i = 0 ; i < inputpower; i++){
       // printf("%d", clock[inputnum]);
        for(int i = 0; i < inputnum; i++){
            printf(" %d", clock[i]);
        }
        printf(" |");



        //output logic
        struct inputNode* node = headinput;
        int k = 0;
        while(node != NULL){
            node->boolean = clock[k];
            k++;
         //   printf("hola\n");
            node = node->next;
        }
        node = headinput;
        struct circuitNode* gate = headcircuit;
        while(gate != NULL){
        //    printf("\nLine number: %d\n", __LINE__);
            //finding where input var is
            if(strcmp(gate->operator, "NOT") == 0){
                node = headinput;
               // printf("inputs: %s %s %s", headinput->value, headinput->next->value, headinput->next->next->value);
                struct tempNode* temp = headtemp;
                int bool = 0;
                int temporinput = 0;
               // printf("Node values: ");
                while(node != NULL){
                    if(strcmp(gate->input1, node->value) == 0){
                        bool = 1;
                        temporinput = 1;
                    //    printf("\nLine number: %d\n", __LINE__);
                        break;
                    }
                //    printf("\njust checked: %s\n", node->value);
                    node = node->next;
                }
                if(bool == 0){
                    while(temp != NULL){
                        if(strcmp(gate->input1, temp->value) == 0){
                           // printf(".........temp input found: %s, %s...........", gate->input1, temp->value);
                            break;
                        }
                        temp = temp->next;
                    }
                }
                if(temp == NULL){
                    // printf("C'EST NULL");
                    // printf("%s", gate->input1);
                    // printf("%s", headtemp->value);
                }
                //if bool == 0, int vars are original inputs, else they're temp vars, this node holds this value.

                //checking to see if gate output var already exists in either output linkedlist, or temp var linked list, if not, creating new node in temp linked list
                struct inputNode* node1 = headoutput;
                struct tempNode* temp1 = headtemp;
                int booloutput = 0;
                int temporout = 0;
                // printf("gate output: %s", gate->output);
                while(node1 != NULL){
                    if(strcmp(node1->value, gate->output) == 0){
                        booloutput = 1;
                        temporout = 1;
                        break;
                    }
                    node1 = node1->next;
                }
                if(node1 == NULL){
                   //     printf("Its null");
                    }
                if(booloutput == 0){
                    if(headtemp == NULL){
                        struct tempNode* tempnode = (struct tempNode*)malloc(sizeof(struct tempNode));
                        tempnode->next = NULL;
                        strcpy(tempnode->value, gate->output);
                        headtemp = tempnode;
                        temp1 = headtemp;
                        booloutput = 1;
                        
                     //   printf("first temp name is: %s", headtemp->value);
                    }
                    else{
                        
                        while(temp1->next != NULL){
                            if(strcmp(temp1->value, gate->output) == 0){
                                booloutput = 1;
                                break;
                            }
                            temp1 = temp1->next;
                        }
                        if(strcmp(temp1->value, gate->output) == 0){
                                booloutput = 1;
                            }
                        else{
                            struct tempNode* tempnode = (struct tempNode*)malloc(sizeof(struct tempNode));
                            strcpy(tempnode->value, gate->output);
                            tempnode->next = NULL;
                            temp1->next = tempnode;
                            temp1 = temp1->next;
                        }
                    }              
                }
                // or gate
            //    printf("\nLine number: %d\n", __LINE__);
                if(temporinput == 1){
                //    printf("\nLine number: %d\n", __LINE__);
                    if(temporout == 1){
                    //    printf("\nLine number: %d\n", __LINE__);
                        if(node->boolean == 1){
                            node1->boolean = 0;
                        //    printf("node to node1: %s is %d and %s is %d.........", node->value,node->boolean, node1->value, node1->boolean);
                        }
                        else{
                            node1->boolean = 1;
                          //  printf("node to node1: %s is %d and %s is %d.........", node->value,node->boolean, node1->value, node1->boolean);
                        }
                    }
                    else{
                    //    printf("\nLine number: %d\n", __LINE__);
                        if(node->boolean == 1){
                        //    printf("\nLine number: %d\n", __LINE__);
                            temp1->boolean = 0;
                           // printf("node to temp1: %s is %d and %s is %d.........", node->value,node->boolean, temp1->value, temp1->boolean);
                        }
                        else{
                            temp1->boolean = 1;
                          //  printf("node to temp1:%s is %d and %s is %d.........", node->value,node->boolean, temp1->value, temp1->boolean);
                        }
                    //    printf("\nLine number: %d\n", __LINE__);
                    }
                }
                else{
                    if(temporout == 1){
                    //    printf("\nLine number: %d\n", __LINE__);
                        if(temp->boolean == 1){
                        //    printf("\nLine number: %d\n", __LINE__);
                            node1->boolean = 0;
                           // printf("temp to node1: %s is %d and %s is %d.........", temp->value,temp->boolean, node1->value, node1->boolean);
                            
                        //    printf("\nLine number: %d\n", __LINE__);
                        }
                        else{
                        //    printf("\nLine number: %d\n", __LINE__);
                            node1->boolean = 1;
                           // printf("temp to node1: %s is %d and %s is %d.........", temp->value,temp->boolean, node1->value, node1->boolean);
                        }
                    }
                    else{
                    //    printf("\nLine number: %d\n", __LINE__);
                        if(temp->boolean == 1){
                            // printf("\nLine number: %d\n", __LINE__);
                            temp1->boolean = 0;
                           // printf("temp to temp1: %s is %d and %s is %d.........",  temp->value,temp->boolean, temp1->value, temp1->boolean);
                        }
                        else{
                            temp1->boolean = 1;
                          //  printf("temp to temp1: %s is %d and %s is %d.........", temp->value,temp->boolean, temp1->value, temp1->boolean);
                        }
                    }
                }

            }
            else if(strcmp(gate->operator, "AND") == 0){
                node = headinput;
                struct inputNode* node2 = headinput;
               // printf("inputs: %s %s %s", headinput->value, headinput->next->value, headinput->next->next->value);
                struct tempNode* temp = headtemp;
                struct tempNode* temp2 = headtemp;
                int bool = 0;
                int temporinput = 0;
                int temporinput2 = 0;
               // printf("Node values: ");
                while(node != NULL){
                    if(strcmp(gate->input1, node->value) == 0){
                        bool = 1;
                        temporinput = 1;
                    //    printf("\nLine number: %d\n", __LINE__);
                        break;
                    }
                //    printf("\njust checked: %s\n", node->value);
                    node = node->next;
                }
                if(bool == 0){
                    while(temp != NULL){
                        if(strcmp(gate->input1, temp->value) == 0){
                           // printf(".........temp input found: %s, %s...........", gate->input1, temp->value);
                            break;
                        }
                        temp = temp->next;
                    }
                }
                //same stuff for second input now:
                int bool2 = 0;
                while(node2 != NULL){
                    if(strcmp(gate->input2, node2->value) == 0){
                        bool2 = 1;
                        temporinput2 = 1;
                    //    printf("\nLine number: %d\n", __LINE__);
                        break;
                    }
                //    printf("\njust checked: %s\n", node->value);
                    node2 = node2->next;
                }
                if(bool2 == 0){
                    while(temp2 != NULL){
                        if(strcmp(gate->input2, temp2->value) == 0){
                           // printf(".........temp input found: %s, %s...........", gate->input1, temp->value);
                            break;
                        }
                        temp2 = temp2->next;
                    }
                }
            
                //if bool == 0, int vars are original inputs, else they're temp vars, this node holds this value.

                //checking to see if gate output var already exists in either output linkedlist, or temp var linked list, if not, creating new node in temp linked list
                struct inputNode* node1 = headoutput;
                struct tempNode* temp1 = headtemp;
                int booloutput = 0;
                int temporout = 0;
                // printf("gate output: %s", gate->output);
                while(node1 != NULL){
                    if(strcmp(node1->value, gate->output) == 0){
                        booloutput = 1;
                        temporout = 1;
                        break;
                    }
                    node1 = node1->next;
                }
                if(node1 == NULL){
                   //     printf("Its null");
                    }
                if(booloutput == 0){
                    if(headtemp == NULL){
                        struct tempNode* tempnode = (struct tempNode*)malloc(sizeof(struct tempNode));
                        tempnode->next = NULL;
                        strcpy(tempnode->value, gate->output);
                        headtemp = tempnode;
                        temp1 = headtemp;
                        booloutput = 1;
                        
                     //   printf("first temp name is: %s", headtemp->value);
                    }
                    else{
                        
                        while(temp1->next != NULL){
                            if(strcmp(temp1->value, gate->output) == 0){
                                booloutput = 1;
                                break;
                            }
                            temp1 = temp1->next;
                        }
                        if(strcmp(temp1->value, gate->output) == 0){
                                booloutput = 1;
                            }
                        else{
                            struct tempNode* tempnode = (struct tempNode*)malloc(sizeof(struct tempNode));
                            strcpy(tempnode->value, gate->output);
                            tempnode->next = NULL;
                            temp1->next = tempnode;
                            temp1 = temp1->next;
                        }
                    }              
                }
                // or gate
                     //                                               NODE NODE NODE
                if(temporinput == 1){
                    if(temporinput2 == 1){
                //    printf("\nLine number: %d\n", __LINE__);
                        if(temporout == 1){
                    //    printf("\nLine number: %d\n", __LINE__);
                            if(node == NULL || node2 == NULL){
                                node1->boolean = 0;
                            }
                            else if(node->boolean == 1 && node2->boolean){
                                node1->boolean = 1;
                        //    printf("node to node1: %s is %d and %s is %d.........", node->value,node->boolean, node1->value, node1->boolean);
                            }
                            else{
                                node1->boolean = 0;
                          //  printf("node to node1: %s is %d and %s is %d.........", node->value,node->boolean, node1->value, node1->boolean);
                            }
                        }
                        else{                               // NODE NODE TEMP
                    //    printf("\nLine number: %d\n", __LINE__);
                            if(node == NULL || node2 == NULL){
                                temp1->boolean = 0;
                            }
                            else if(node->boolean == 1 && node2->boolean == 1){
                        //    printf("\nLine number: %d\n", __LINE__);
                                temp1->boolean = 1;
                           // printf("node to temp1: %s is %d and %s is %d.........", node->value,node->boolean, temp1->value, temp1->boolean);
                        }
                            else{
                                temp1->boolean = 0;
                          //  printf("node to temp1:%s is %d and %s is %d.........", node->value,node->boolean, temp1->value, temp1->boolean);
                            }
                    //    printf("\nLine number: %d\n", __LINE__);
                        }
                    }
                    else{                               //NODE TEMP NODE
                        if(temporout == 1){
                    //    printf("\nLine number: %d\n", __LINE__);
                            if(node == NULL || temp2 == NULL){
                                node1->boolean = 0;
                            }
                            else if(node->boolean == 1 && temp2->boolean == 1){
                                node1->boolean = 1;
                        //    printf("node to node1: %s is %d and %s is %d.........", node->value,node->boolean, node1->value, node1->boolean);
                            }
                            else{
                                node1->boolean = 0;
                          //  printf("node to node1: %s is %d and %s is %d.........", node->value,node->boolean, node1->value, node1->boolean);
                            }
                        }
                        else{
                    //    printf("\nLine number: %d\n", __LINE__);
                            if(node == NULL || temp2 == NULL){
                                temp1->boolean = 0;
                            }
                            else if(node->boolean == 1 && temp2->boolean == 1){
                        //    printf("\nLine number: %d\n", __LINE__);
                                temp1->boolean = 1;
                           // printf("node to temp1: %s is %d and %s is %d.........", node->value,node->boolean, temp1->value, temp1->boolean);
                        }
                            else{
                                temp1->boolean = 0;
                          //  printf("node to temp1:%s is %d and %s is %d.........", node->value,node->boolean, temp1->value, temp1->boolean);
                            }
                    //    printf("\nLine number: %d\n", __LINE__);
                        }
                    }
                }
                else{//                     TEMP NODE NODE
                    if(temporinput2 == 1){
                        if(temporout == 1){
                    //    printf("\nLine number: %d\n", __LINE__);
                            if(temp == NULL || node2 == NULL){
                                node1->boolean = 0;
                            }
                            if(temp->boolean == 1 && node2->boolean == 1){
                        //    printf("\nLine number: %d\n", __LINE__);
                                node1->boolean = 1;                            
                        //    printf("\nLine number: %d\n", __LINE__);
                            }
                            else{
                        //    printf("\nLine number: %d\n", __LINE__);
                                node1->boolean = 0;
                            }
                        }
                        else{           //TEMP NODE TEMP
                    //    printf("\nLine number: %d\n", __LINE__);
                            if(temp == NULL || node2 == NULL){
                                temp1->boolean = 0;
                            }
                            if(temp->boolean == 1 && node2->boolean == 1){
                            // printf("\nLine number: %d\n", __LINE__);
                                temp1->boolean = 1;
                           // printf("temp to temp1: %s is %d and %s is %d.........",  temp->value,temp->boolean, temp1->value, temp1->boolean);
                            }
                            else{
                                temp1->boolean = 0;
                          //  printf("temp to temp1: %s is %d and %s is %d.........", temp->value,temp->boolean, temp1->value, temp1->boolean);
                            }
                        }
                    }
                    else{
                        if(temporout == 1){
                    //    printf("\nLine number: %d\n", __LINE__);
                            if(temp == NULL || temp2 == NULL){
                                node1->boolean = 0;
                            }
                            if(temp->boolean == 1 && temp2->boolean == 1){
                        //    printf("\nLine number: %d\n", __LINE__);
                                node1->boolean = 1;                            
                        //    printf("\nLine number: %d\n", __LINE__);
                            }
                            else{
                        //    printf("\nLine number: %d\n", __LINE__);
                                node1->boolean = 0;
                            }
                        }
                        else{           //TEMP NODE TEMP
                    //    printf("\nLine number: %d\n", __LINE__);
                            if(temp == NULL || temp2 == NULL){
                                temp1->boolean = 0;
                            }
                            if(temp->boolean == 1 && temp2->boolean == 1){
                            // printf("\nLine number: %d\n", __LINE__);
                                temp1->boolean = 1;
                           // printf("temp to temp1: %s is %d and %s is %d.........",  temp->value,temp->boolean, temp1->value, temp1->boolean);
                            }
                            else{
                                temp1->boolean = 0;
                          //  printf("temp to temp1: %s is %d and %s is %d.........", temp->value,temp->boolean, temp1->value, temp1->boolean);
                            }
                        }
                    }
                }


            }
            else if(strcmp(gate->operator, "NAND") == 0){
                node = headinput;
                struct inputNode* node2 = headinput;
               // printf("inputs: %s %s %s", headinput->value, headinput->next->value, headinput->next->next->value);
                struct tempNode* temp = headtemp;
                struct tempNode* temp2 = headtemp;
                int bool = 0;
                int temporinput = 0;
                int temporinput2 = 0;
               // printf("Node values: ");
                while(node != NULL){
                    if(strcmp(gate->input1, node->value) == 0){
                        bool = 1;
                        temporinput = 1;
                    //    printf("\nLine number: %d\n", __LINE__);
                        break;
                    }
                //    printf("\njust checked: %s\n", node->value);
                    node = node->next;
                }
                if(bool == 0){
                    while(temp != NULL){
                        if(strcmp(gate->input1, temp->value) == 0){
                           // printf(".........temp input found: %s, %s...........", gate->input1, temp->value);
                            break;
                        }
                        temp = temp->next;
                    }
                }
                //same stuff for second input now:
                int bool2 = 0;
                while(node2 != NULL){
                    if(strcmp(gate->input2, node2->value) == 0){
                        bool2 = 1;
                        temporinput2 = 1;
                    //    printf("\nLine number: %d\n", __LINE__);
                        break;
                    }
                //    printf("\njust checked: %s\n", node->value);
                    node2 = node2->next;
                }
                if(bool2 == 0){
                    while(temp2 != NULL){
                        if(strcmp(gate->input2, temp2->value) == 0){
                           // printf(".........temp input found: %s, %s...........", gate->input1, temp->value);
                            break;
                        }
                        temp2 = temp2->next;
                    }
                }
            
                //if bool == 0, int vars are original inputs, else they're temp vars, this node holds this value.

                //checking to see if gate output var already exists in either output linkedlist, or temp var linked list, if not, creating new node in temp linked list
                struct inputNode* node1 = headoutput;
                struct tempNode* temp1 = headtemp;
                int booloutput = 0;
                int temporout = 0;
                // printf("gate output: %s", gate->output);
                while(node1 != NULL){
                    if(strcmp(node1->value, gate->output) == 0){
                        booloutput = 1;
                        temporout = 1;
                        break;
                    }
                    node1 = node1->next;
                }
                if(node1 == NULL){
                   //     printf("Its null");
                    }
                if(booloutput == 0){
                    if(headtemp == NULL){
                        struct tempNode* tempnode = (struct tempNode*)malloc(sizeof(struct tempNode));
                        tempnode->next = NULL;
                        strcpy(tempnode->value, gate->output);
                        headtemp = tempnode;
                        temp1 = headtemp;
                        booloutput = 1;
                        
                     //   printf("first temp name is: %s", headtemp->value);
                    }
                    else{
                        
                        while(temp1->next != NULL){
                            if(strcmp(temp1->value, gate->output) == 0){
                                booloutput = 1;
                                break;
                            }
                            temp1 = temp1->next;
                        }
                        if(strcmp(temp1->value, gate->output) == 0){
                                booloutput = 1;
                            }
                        else{
                            struct tempNode* tempnode = (struct tempNode*)malloc(sizeof(struct tempNode));
                            strcpy(tempnode->value, gate->output);
                            tempnode->next = NULL;
                            temp1->next = tempnode;
                            temp1 = temp1->next;
                        }
                    }              
                }
                // or gate
                     //                                               NODE NODE NODE
                if(temporinput == 1){
                    if(temporinput2 == 1){
                //    printf("\nLine number: %d\n", __LINE__);
                        if(temporout == 1){
                    //    printf("\nLine number: %d\n", __LINE__);
                            if(node == NULL || node2 == NULL){
                                node1->boolean = 1;
                            }
                            else if(node->boolean == 1 && node2->boolean){
                                node1->boolean = 0;
                        //    printf("node to node1: %s is %d and %s is %d.........", node->value,node->boolean, node1->value, node1->boolean);
                            }
                            else{
                                node1->boolean = 1;
                          //  printf("node to node1: %s is %d and %s is %d.........", node->value,node->boolean, node1->value, node1->boolean);
                            }
                        }
                        else{                               // NODE NODE TEMP
                    //    printf("\nLine number: %d\n", __LINE__);
                            if(node == NULL || node2 == NULL){
                                temp1->boolean = 1;
                            }
                            else if(node->boolean == 1 && node2->boolean == 1){
                        //    printf("\nLine number: %d\n", __LINE__);
                                temp1->boolean = 0;
                           // printf("node to temp1: %s is %d and %s is %d.........", node->value,node->boolean, temp1->value, temp1->boolean);
                        }
                            else{
                                temp1->boolean = 1;
                          //  printf("node to temp1:%s is %d and %s is %d.........", node->value,node->boolean, temp1->value, temp1->boolean);
                            }
                    //    printf("\nLine number: %d\n", __LINE__);
                        }
                    }
                    else{                               //NODE TEMP NODE
                        if(temporout == 1){
                    //    printf("\nLine number: %d\n", __LINE__);
                            if(node == NULL || temp2 == NULL){
                                node1->boolean = 1;
                            }
                            else if(node->boolean == 1 && temp2->boolean == 1){
                                node1->boolean = 0;
                        //    printf("node to node1: %s is %d and %s is %d.........", node->value,node->boolean, node1->value, node1->boolean);
                            }
                            else{
                                node1->boolean = 1;
                          //  printf("node to node1: %s is %d and %s is %d.........", node->value,node->boolean, node1->value, node1->boolean);
                            }
                        }
                        else{
                    //    printf("\nLine number: %d\n", __LINE__);
                            if(node == NULL || temp2 == NULL){
                                temp1->boolean = 1;
                            }
                            else if(node->boolean == 1 && temp2->boolean == 1){
                        //    printf("\nLine number: %d\n", __LINE__);
                                temp1->boolean = 0;
                           // printf("node to temp1: %s is %d and %s is %d.........", node->value,node->boolean, temp1->value, temp1->boolean);
                        }
                            else{
                                temp1->boolean = 1;
                          //  printf("node to temp1:%s is %d and %s is %d.........", node->value,node->boolean, temp1->value, temp1->boolean);
                            }
                    //    printf("\nLine number: %d\n", __LINE__);
                        }
                    }
                }
                else{//                     TEMP NODE NODE
                    if(temporinput2 == 1){
                        if(temporout == 1){
                    //    printf("\nLine number: %d\n", __LINE__);
                            if(temp == NULL || node2 == NULL){
                                node1->boolean = 1;
                            }
                            if(temp->boolean == 1 && node2->boolean == 1){
                        //    printf("\nLine number: %d\n", __LINE__);
                                node1->boolean = 0;                            
                        //    printf("\nLine number: %d\n", __LINE__);
                            }
                            else{
                        //    printf("\nLine number: %d\n", __LINE__);
                                node1->boolean = 1;
                            }
                        }
                        else{           //TEMP NODE TEMP
                    //    printf("\nLine number: %d\n", __LINE__);
                            if(temp == NULL || node2 == NULL){
                                temp1->boolean = 1;
                            }
                            if(temp->boolean == 1 && node2->boolean == 1){
                            // printf("\nLine number: %d\n", __LINE__);
                                temp1->boolean = 0;
                           // printf("temp to temp1: %s is %d and %s is %d.........",  temp->value,temp->boolean, temp1->value, temp1->boolean);
                            }
                            else{
                                temp1->boolean = 1;
                          //  printf("temp to temp1: %s is %d and %s is %d.........", temp->value,temp->boolean, temp1->value, temp1->boolean);
                            }
                        }
                    }
                    else{
                        if(temporout == 1){
                    //    printf("\nLine number: %d\n", __LINE__);
                            if(temp == NULL || temp2 == NULL){
                                node1->boolean = 1;
                            }
                            if(temp->boolean == 1 && temp2->boolean == 1){
                        //    printf("\nLine number: %d\n", __LINE__);
                                node1->boolean = 0;                            
                        //    printf("\nLine number: %d\n", __LINE__);
                            }
                            else{
                        //    printf("\nLine number: %d\n", __LINE__);
                                node1->boolean = 1;
                            }
                        }
                        else{           //TEMP NODE TEMP
                    //    printf("\nLine number: %d\n", __LINE__);
                            if(temp == NULL || temp2 == NULL){
                                temp1->boolean = 1;
                            }
                            if(temp->boolean == 1 && temp2->boolean == 1){
                            // printf("\nLine number: %d\n", __LINE__);
                                temp1->boolean = 0;
                           // printf("temp to temp1: %s is %d and %s is %d.........",  temp->value,temp->boolean, temp1->value, temp1->boolean);
                            }
                            else{
                                temp1->boolean = 1;
                          //  printf("temp to temp1: %s is %d and %s is %d.........", temp->value,temp->boolean, temp1->value, temp1->boolean);
                            }
                        }
                    }
                }

            }
            else if(strcmp(gate->operator, "OR") == 0){
                node = headinput;
                struct inputNode* node2 = headinput;
               // printf("inputs: %s %s %s", headinput->value, headinput->next->value, headinput->next->next->value);
                struct tempNode* temp = headtemp;
                struct tempNode* temp2 = headtemp;
                int bool = 0;
                int temporinput = 0;
                int temporinput2 = 0;
               // printf("Node values: ");
                while(node != NULL){
                    if(strcmp(gate->input1, node->value) == 0){
                        bool = 1;
                        temporinput = 1;
                    //    printf("\nLine number: %d\n", __LINE__);
                        break;
                    }
                //    printf("\njust checked: %s\n", node->value);
                    node = node->next;
                }
                if(bool == 0){
                    while(temp != NULL){
                        if(strcmp(gate->input1, temp->value) == 0){
                           // printf(".........temp input found: %s, %s...........", gate->input1, temp->value);
                            break;
                        }
                        temp = temp->next;
                    }
                }
                //same stuff for second input now:
                int bool2 = 0;
                while(node2 != NULL){
                    if(strcmp(gate->input2, node2->value) == 0){
                        bool2 = 1;
                        temporinput2 = 1;
                    //    printf("\nLine number: %d\n", __LINE__);
                        break;
                    }
                //    printf("\njust checked: %s\n", node->value);
                    node2 = node2->next;
                }
                if(bool2 == 0){
                    while(temp2 != NULL){
                        if(strcmp(gate->input2, temp2->value) == 0){
                           // printf(".........temp input found: %s, %s...........", gate->input1, temp->value);
                            break;
                        }
                        temp2 = temp2->next;
                    }
                }
            
                //if bool == 0, int vars are original inputs, else they're temp vars, this node holds this value.

                //checking to see if gate output var already exists in either output linkedlist, or temp var linked list, if not, creating new node in temp linked list
                struct inputNode* node1 = headoutput;
                struct tempNode* temp1 = headtemp;
                int booloutput = 0;
                int temporout = 0;
                // printf("gate output: %s", gate->output);
                while(node1 != NULL){
                    if(strcmp(node1->value, gate->output) == 0){
                        booloutput = 1;
                        temporout = 1;
                        break;
                    }
                    node1 = node1->next;
                }
                if(node1 == NULL){
                   //     printf("Its null");
                    }
                if(booloutput == 0){
                    if(headtemp == NULL){
                        struct tempNode* tempnode = (struct tempNode*)malloc(sizeof(struct tempNode));
                        tempnode->next = NULL;
                        strcpy(tempnode->value, gate->output);
                        headtemp = tempnode;
                        temp1 = headtemp;
                        booloutput = 1;
                        
                     //   printf("first temp name is: %s", headtemp->value);
                    }
                    else{
                        
                        while(temp1->next != NULL){
                            if(strcmp(temp1->value, gate->output) == 0){
                                booloutput = 1;
                                break;
                            }
                            temp1 = temp1->next;
                        }
                        if(strcmp(temp1->value, gate->output) == 0){
                                booloutput = 1;
                            }
                        else{
                            struct tempNode* tempnode = (struct tempNode*)malloc(sizeof(struct tempNode));
                            strcpy(tempnode->value, gate->output);
                            tempnode->next = NULL;
                            temp1->next = tempnode;
                            temp1 = temp1->next;
                        }
                    }              
                }
                // or gate
                     //                                               NODE NODE NODE
                if(temporinput == 1){
                    if(temporinput2 == 1){
                //    printf("\nLine number: %d\n", __LINE__);
                        if(temporout == 1){
                    //    printf("\nLine number: %d\n", __LINE__);
                            if(node == NULL){
                                if(node2->boolean == 1){
                                    node1->boolean = 1;
                                }
                                else{
                                    node1->boolean = 0;
                                }
                            }
                            else if(node2 == NULL){
                                if(node->boolean == 1){
                                    node1->boolean = 1;
                                }
                                else{
                                    node1->boolean = 0;
                                }
                            }
                            else if(node->boolean == 1 || node2->boolean == 1){
                                node1->boolean = 1;
                        //    printf("node to node1: %s is %d and %s is %d.........", node->value,node->boolean, node1->value, node1->boolean);
                            }
                            else{
                                node1->boolean = 0;
                          //  printf("node to node1: %s is %d and %s is %d.........", node->value,node->boolean, node1->value, node1->boolean);
                            }
                        }
                        else{                               // NODE NODE TEMP
                    //    printf("\nLine number: %d\n", __LINE__);
                            if(node == NULL){
                                if(node2->boolean == 1){
                                    temp1->boolean = 1;
                                }
                                else{
                                    temp1->boolean = 0;
                                }
                            }
                            else if( node2 == NULL){
                                if(node->boolean == 1){
                                    temp1->boolean = 1;
                                }
                                else{
                                    temp1->boolean = 0;
                                }
                            }
                            else if(node->boolean == 1 || node2->boolean == 1){
                        //    printf("\nLine number: %d\n", __LINE__);
                                temp1->boolean = 1;
                           // printf("node to temp1: %s is %d and %s is %d.........", node->value,node->boolean, temp1->value, temp1->boolean);
                        }
                            else{
                                temp1->boolean = 0;
                          //  printf("node to temp1:%s is %d and %s is %d.........", node->value,node->boolean, temp1->value, temp1->boolean);
                            }
                    //    printf("\nLine number: %d\n", __LINE__);
                        }
                    }
                    else{                               //NODE TEMP NODE
                        if(temporout == 1){
                    //    printf("\nLine number: %d\n", __LINE__);
                            if(node == NULL){
                                if(temp2->boolean == 1){
                                    node1->boolean = 1;
                                }
                                else{
                                    node1->boolean = 0;
                                }
                            }
                            else if(temp2 == NULL){
                                if(node->boolean == 1){
                                    node1->boolean = 1;
                                }
                                else{
                                    node1->boolean = 0;
                                }
                            }
                            else if(node->boolean == 1 || temp2->boolean == 1){
                                node1->boolean = 1;
                        //    printf("node to node1: %s is %d and %s is %d.........", node->value,node->boolean, node1->value, node1->boolean);
                            }
                            else{
                                node1->boolean = 0;
                          //  printf("node to node1: %s is %d and %s is %d.........", node->value,node->boolean, node1->value, node1->boolean);
                            }
                        }
                        else{
                    //    printf("\nLine number: %d\n", __LINE__);
                            if(node == NULL){
                                if(temp2->boolean == 1){
                                    temp1->boolean = 1;
                                }
                                else{
                                    temp1->boolean = 0;
                                }
                            }
                            else if(temp2 == NULL){
                                if(node->boolean == 1){
                                    temp1->boolean = 1;
                                }
                                else{
                                    temp1->boolean = 0;
                                }
                            }
                            if(node->boolean == 1 || temp2->boolean == 1){
                        //    printf("\nLine number: %d\n", __LINE__);
                                temp1->boolean = 1;
                           // printf("node to temp1: %s is %d and %s is %d.........", node->value,node->boolean, temp1->value, temp1->boolean);
                        }
                            else{
                                temp1->boolean = 0;
                          //  printf("node to temp1:%s is %d and %s is %d.........", node->value,node->boolean, temp1->value, temp1->boolean);
                            }
                    //    printf("\nLine number: %d\n", __LINE__);
                        }
                    }
                }
                else{//                     TEMP NODE NODE
                    if(temporinput2 == 1){
                        if(temporout == 1){
                    //    printf("\nLine number: %d\n", __LINE__);
                            if(temp == NULL){
                                if(node2->boolean == 1){
                                    node1->boolean = 1;
                                }
                                else{
                                    node1->boolean = 0;
                                }
                            }
                            else if(node2 == NULL){
                                if(temp->boolean == 1){
                                    node1->boolean = 1;
                                }
                                else{
                                    node1->boolean = 0;
                                }
                            }
                            else if(temp->boolean == 1 || node2->boolean == 1){
                        //    printf("\nLine number: %d\n", __LINE__);
                                node1->boolean = 1;                            
                        //    printf("\nLine number: %d\n", __LINE__);
                            }
                            else{
                        //    printf("\nLine number: %d\n", __LINE__);
                                node1->boolean = 0;
                            }
                        }
                        else{           //TEMP NODE TEMP
                    //    printf("\nLine number: %d\n", __LINE__);
                            if(temp == NULL){
                                if(node2->boolean == 1){
                                    temp1->boolean = 1;
                                }
                                else{
                                    temp1->boolean = 0;
                                }
                            }
                            else if(node2 == NULL){
                                if(temp->boolean == 1){
                                    temp1->boolean = 1;
                                }
                                else{
                                    temp1->boolean = 0;
                                }
                            }
                            else if(temp->boolean == 1 || node2->boolean == 1){
                            // printf("\nLine number: %d\n", __LINE__);
                                temp1->boolean = 1;
                           // printf("temp to temp1: %s is %d and %s is %d.........",  temp->value,temp->boolean, temp1->value, temp1->boolean);
                            }
                            else{
                                temp1->boolean = 0;
                          //  printf("temp to temp1: %s is %d and %s is %d.........", temp->value,temp->boolean, temp1->value, temp1->boolean);
                            }
                        }
                    }
                    else{
                        if(temporout == 1){
                    //    printf("\nLine number: %d\n", __LINE__);
                            if(temp == NULL){
                                if(temp2->boolean == 1){
                                    node1->boolean = 1;
                                }
                                else{
                                    node1->boolean = 0;
                                }
                            }
                            else if(temp2 == NULL){
                                if(temp->boolean == 1){
                                    node1->boolean = 1;
                                }
                                else{
                                    node1->boolean = 0;
                                }
                            }
                             else if(temp->boolean == 1 || temp2->boolean == 1){
                        //    printf("\nLine number: %d\n", __LINE__);
                                node1->boolean = 1;                            
                        //    printf("\nLine number: %d\n", __LINE__);
                            }
                            else{
                        //    printf("\nLine number: %d\n", __LINE__);
                                node1->boolean = 0;
                            }
                        }
                        else{           //TEMP NODE TEMP
                    //    printf("\nLine number: %d\n", __LINE__);
                            if(temp == NULL){
                                if(temp2->boolean == 1){
                                    temp1->boolean = 1;
                                }
                                else{
                                    temp1->boolean = 0;
                                }
                            }
                            else if(temp2 == NULL){
                                if(temp->boolean == 1){
                                    temp1->boolean = 1;
                                }
                                else{
                                    temp1->boolean = 0;
                                }
                            }
                            else if(temp->boolean == 1 || temp2->boolean == 1){
                            // printf("\nLine number: %d\n", __LINE__);
                                temp1->boolean = 1;
                           // printf("temp to temp1: %s is %d and %s is %d.........",  temp->value,temp->boolean, temp1->value, temp1->boolean);
                            }
                            else{
                                temp1->boolean = 0;
                          //  printf("temp to temp1: %s is %d and %s is %d.........", temp->value,temp->boolean, temp1->value, temp1->boolean);
                            }
                        }
                    }
                }

            }
            else if(strcmp(gate->operator, "NOR") == 0){
                node = headinput;
                struct inputNode* node2 = headinput;
               // printf("inputs: %s %s %s", headinput->value, headinput->next->value, headinput->next->next->value);
                struct tempNode* temp = headtemp;
                struct tempNode* temp2 = headtemp;
                int bool = 0;
                int temporinput = 0;
                int temporinput2 = 0;
               // printf("Node values: ");
                while(node != NULL){
                    if(strcmp(gate->input1, node->value) == 0){
                        bool = 1;
                        temporinput = 1;
                    //    printf("\nLine number: %d\n", __LINE__);
                        break;
                    }
                //    printf("\njust checked: %s\n", node->value);
                    node = node->next;
                }
                if(bool == 0){
                    while(temp != NULL){
                        if(strcmp(gate->input1, temp->value) == 0){
                           // printf(".........temp input found: %s, %s...........", gate->input1, temp->value);
                            break;
                        }
                        temp = temp->next;
                    }
                }
                //same stuff for second input now:
                int bool2 = 0;
                while(node2 != NULL){
                    if(strcmp(gate->input2, node2->value) == 0){
                        bool2 = 1;
                        temporinput2 = 1;
                    //    printf("\nLine number: %d\n", __LINE__);
                        break;
                    }
                //    printf("\njust checked: %s\n", node->value);
                    node2 = node2->next;
                }
                if(bool2 == 0){
                    while(temp2 != NULL){
                        if(strcmp(gate->input2, temp2->value) == 0){
                           // printf(".........temp input found: %s, %s...........", gate->input1, temp->value);
                            break;
                        }
                        temp2 = temp2->next;
                    }
                }
            
                //if bool == 0, int vars are original inputs, else they're temp vars, this node holds this value.

                //checking to see if gate output var already exists in either output linkedlist, or temp var linked list, if not, creating new node in temp linked list
                struct inputNode* node1 = headoutput;
                struct tempNode* temp1 = headtemp;
                int booloutput = 0;
                int temporout = 0;
                // printf("gate output: %s", gate->output);
                while(node1 != NULL){
                    if(strcmp(node1->value, gate->output) == 0){
                        booloutput = 1;
                        temporout = 1;
                        break;
                    }
                    node1 = node1->next;
                }
                if(node1 == NULL){
                   //     printf("Its null");
                    }
                if(booloutput == 0){
                    if(headtemp == NULL){
                        struct tempNode* tempnode = (struct tempNode*)malloc(sizeof(struct tempNode));
                        tempnode->next = NULL;
                        strcpy(tempnode->value, gate->output);
                        headtemp = tempnode;
                        temp1 = headtemp;
                        booloutput = 1;
                        
                     //   printf("first temp name is: %s", headtemp->value);
                    }
                    else{
                        
                        while(temp1->next != NULL){
                            if(strcmp(temp1->value, gate->output) == 0){
                                booloutput = 1;
                                break;
                            }
                            temp1 = temp1->next;
                        }
                        if(strcmp(temp1->value, gate->output) == 0){
                                booloutput = 1;
                            }
                        else{
                            struct tempNode* tempnode = (struct tempNode*)malloc(sizeof(struct tempNode));
                            strcpy(tempnode->value, gate->output);
                            tempnode->next = NULL;
                            temp1->next = tempnode;
                            temp1 = temp1->next;
                        }
                    }              
                }
                // or gate
                     //                                               NODE NODE NODE
                if(temporinput == 1){
                    if(temporinput2 == 1){
                //    printf("\nLine number: %d\n", __LINE__);
                        if(temporout == 1){
                    //    printf("\nLine number: %d\n", __LINE__);
                            if(node == NULL){
                                if(node2->boolean == 1){
                                    node1->boolean = 0;
                                }
                                else{
                                    node1->boolean = 1;
                                }
                            }
                            else if(node2 == NULL){
                                if(node1->boolean == 1){
                                    node1->boolean = 0;
                                }
                                else{
                                    node1->boolean = 1;
                                }
                            }
                            else if(node->boolean == 1 || node2->boolean == 1){
                                node1->boolean = 0;
                        //    printf("node to node1: %s is %d and %s is %d.........", node->value,node->boolean, node1->value, node1->boolean);
                            }
                            else{
                                node1->boolean = 1;
                          //  printf("node to node1: %s is %d and %s is %d.........", node->value,node->boolean, node1->value, node1->boolean);
                            }
                        }
                        else{                               // NODE NODE TEMP
                    //    printf("\nLine number: %d\n", __LINE__);
                            if(node == NULL){
                                if(node2->boolean == 1){
                                    temp1->boolean = 0;
                                }
                                else{
                                    temp1->boolean = 1;
                                }
                            }
                            else if(node2 == NULL){
                                if(node->boolean == 1){
                                    temp1->boolean = 0;
                                }
                                else{
                                    temp1->boolean = 1;
                                }
                            }
                            else if(node->boolean == 1 || node2->boolean == 1){
                        //    printf("\nLine number: %d\n", __LINE__);
                                temp1->boolean = 0;
                           // printf("node to temp1: %s is %d and %s is %d.........", node->value,node->boolean, temp1->value, temp1->boolean);
                        }
                            else{
                                temp1->boolean = 1;
                          //  printf("node to temp1:%s is %d and %s is %d.........", node->value,node->boolean, temp1->value, temp1->boolean);
                            }
                    //    printf("\nLine number: %d\n", __LINE__);
                        }
                    }
                    else{                               //NODE TEMP NODE
                        if(temporout == 1){
                    //    printf("\nLine number: %d\n", __LINE__);
                            if(node == NULL){
                                if(temp2->boolean == 1){
                                    node1->boolean = 0;
                                }
                                else{
                                    node1->boolean = 1;
                                }
                            }
                            else if(temp2 == NULL){
                                if(node->boolean == 1){
                                    node1->boolean = 0;
                                }
                                else{
                                    node1->boolean = 1;
                                }
                            }
                            else if(node->boolean == 1 || temp2->boolean == 1){
                                node1->boolean = 0;
                        //    printf("node to node1: %s is %d and %s is %d.........", node->value,node->boolean, node1->value, node1->boolean);
                            }
                            else{
                                node1->boolean = 1;
                          //  printf("node to node1: %s is %d and %s is %d.........", node->value,node->boolean, node1->value, node1->boolean);
                            }
                        }
                        else{
                    //    printf("\nLine number: %d\n", __LINE__);
                            if(node == NULL){
                                if(temp2->boolean == 1){
                                    temp1->boolean = 0;
                                }
                                else{
                                    temp1->boolean = 1;
                                }
                            }
                            else if(temp2 == NULL){
                                if(node->boolean == 1){
                                    temp1->boolean = 0;
                                }
                                else{
                                    temp1->boolean = 1;
                                }
                            }
                            else if(node->boolean == 1 || temp2->boolean == 1){
                        //    printf("\nLine number: %d\n", __LINE__);
                                temp1->boolean = 0;
                           // printf("node to temp1: %s is %d and %s is %d.........", node->value,node->boolean, temp1->value, temp1->boolean);
                        }
                            else{
                                temp1->boolean = 1;
                          //  printf("node to temp1:%s is %d and %s is %d.........", node->value,node->boolean, temp1->value, temp1->boolean);
                            }
                    //    printf("\nLine number: %d\n", __LINE__);
                        }
                    }
                }
                else{//                     TEMP NODE NODE
                    if(temporinput2 == 1){
                        if(temporout == 1){
                    //    printf("\nLine number: %d\n", __LINE__);
                            if(temp == NULL){
                                if(node2->boolean == 1){
                                    node1->boolean = 0;
                                }
                                else{
                                    node1->boolean = 1;
                                }
                            }
                            else if(node2 == NULL){
                                if(temp->boolean == 1){
                                    node1->boolean = 0;
                                }
                                else{
                                    node1->boolean = 1;
                                }
                            }
                            else if(temp->boolean == 1 || node2->boolean == 1){
                        //    printf("\nLine number: %d\n", __LINE__);
                                node1->boolean = 0;                            
                        //    printf("\nLine number: %d\n", __LINE__);
                            }
                            else{
                        //    printf("\nLine number: %d\n", __LINE__);
                                node1->boolean = 1;
                            }
                        }
                        else{           //TEMP NODE TEMP
                    //    printf("\nLine number: %d\n", __LINE__);
                            if(temp == NULL){
                                if(node2->boolean == 1){
                                    temp1->boolean = 0;
                                }
                                else{
                                    temp1->boolean = 1;
                                }
                            }
                            else if(node2 == NULL){
                                if(temp->boolean == 1){
                                    temp1->boolean = 0;
                                }
                                else{
                                    temp1->boolean = 1;
                                }
                            }
                            else if(temp->boolean == 1 || node2->boolean == 1){
                            // printf("\nLine number: %d\n", __LINE__);
                                temp1->boolean = 0;
                           // printf("temp to temp1: %s is %d and %s is %d.........",  temp->value,temp->boolean, temp1->value, temp1->boolean);
                            }
                            else{
                                temp1->boolean = 1;
                          //  printf("temp to temp1: %s is %d and %s is %d.........", temp->value,temp->boolean, temp1->value, temp1->boolean);
                            }
                        }
                    }
                    else{
                        if(temporout == 1){
                    //    printf("\nLine number: %d\n", __LINE__);
                            if(temp == NULL){
                                if(temp2->boolean == 1){
                                    node1->boolean = 0;
                                }
                                else{
                                    node1->boolean = 1;
                                }
                            }
                            else if(temp2 == NULL){
                                if(temp->boolean == 1){
                                    node1->boolean = 0;
                                }
                                else{
                                    node1->boolean = 1;
                                }
                            }
                            else if(temp->boolean == 1 || temp2->boolean == 1){
                        //    printf("\nLine number: %d\n", __LINE__);
                                node1->boolean = 0;                            
                        //    printf("\nLine number: %d\n", __LINE__);
                            }
                            else{
                        //    printf("\nLine number: %d\n", __LINE__);
                                node1->boolean = 1;
                            }
                        }
                        else{           //TEMP NODE TEMP
                    //    printf("\nLine number: %d\n", __LINE__);
                            if(temp == NULL){
                                if(temp2->boolean == 1){
                                    temp1->boolean = 0;
                                }
                                else{
                                    temp1->boolean = 1;
                                }
                            }
                            else if(temp2 == NULL){
                                if(temp1->boolean == 1){
                                    temp1->boolean = 0;
                                }
                                else{
                                    temp1->boolean = 1;
                                }
                            }
                            else if(temp->boolean == 1 || temp2->boolean == 1){
                            // printf("\nLine number: %d\n", __LINE__);
                                temp1->boolean = 0;
                           // printf("temp to temp1: %s is %d and %s is %d.........",  temp->value,temp->boolean, temp1->value, temp1->boolean);
                            }
                            else{
                                temp1->boolean = 1;
                          //  printf("temp to temp1: %s is %d and %s is %d.........", temp->value,temp->boolean, temp1->value, temp1->boolean);
                            }
                        }
                    }
                }

            }
            else if(strcmp(gate->operator, "XOR") == 0){
                node = headinput;
                struct inputNode* node2 = headinput;
               // printf("inputs: %s %s %s", headinput->value, headinput->next->value, headinput->next->next->value);
                struct tempNode* temp = headtemp;
                struct tempNode* temp2 = headtemp;
                int bool = 0;
                int temporinput = 0;
                int temporinput2 = 0;
               // printf("Node values: ");
                while(node != NULL){
                    if(strcmp(gate->input1, node->value) == 0){
                        bool = 1;
                        temporinput = 1;
                    //    printf("\nLine number: %d\n", __LINE__);
                        break;
                    }
                //    printf("\njust checked: %s\n", node->value);
                    node = node->next;
                }
                if(bool == 0){
                    while(temp != NULL){
                        if(strcmp(gate->input1, temp->value) == 0){
                           // printf(".........temp input found: %s, %s...........", gate->input1, temp->value);
                            break;
                        }
                        temp = temp->next;
                    }
                }
                //same stuff for second input now:
                int bool2 = 0;
                while(node2 != NULL){
                    if(strcmp(gate->input2, node2->value) == 0){
                        bool2 = 1;
                        temporinput2 = 1;
                    //    printf("\nLine number: %d\n", __LINE__);
                        break;
                    }
                //    printf("\njust checked: %s\n", node->value);
                    node2 = node2->next;
                }
                if(bool2 == 0){
                    while(temp2 != NULL){
                        if(strcmp(gate->input2, temp2->value) == 0){
                           // printf(".........temp input found: %s, %s...........", gate->input1, temp->value);
                            break;
                        }
                        temp2 = temp2->next;
                    }
                }
            
                //if bool == 0, int vars are original inputs, else they're temp vars, this node holds this value.

                //checking to see if gate output var already exists in either output linkedlist, or temp var linked list, if not, creating new node in temp linked list
                struct inputNode* node1 = headoutput;
                struct tempNode* temp1 = headtemp;
                int booloutput = 0;
                int temporout = 0;
                // printf("gate output: %s", gate->output);
                while(node1 != NULL){
                    if(strcmp(node1->value, gate->output) == 0){
                        booloutput = 1;
                        temporout = 1;
                        break;
                    }
                    node1 = node1->next;
                }
                if(node1 == NULL){
                   //     printf("Its null");
                    }
                if(booloutput == 0){
                    if(headtemp == NULL){
                        struct tempNode* tempnode = (struct tempNode*)malloc(sizeof(struct tempNode));
                        tempnode->next = NULL;
                        strcpy(tempnode->value, gate->output);
                        headtemp = tempnode;
                        temp1 = headtemp;
                        booloutput = 1;
                        
                     //   printf("first temp name is: %s", headtemp->value);
                    }
                    else{
                        
                        while(temp1->next != NULL){
                            if(strcmp(temp1->value, gate->output) == 0){
                                booloutput = 1;
                                break;
                            }
                            temp1 = temp1->next;
                        }
                        if(strcmp(temp1->value, gate->output) == 0){
                                booloutput = 1;
                            }
                        else{
                            struct tempNode* tempnode = (struct tempNode*)malloc(sizeof(struct tempNode));
                            strcpy(tempnode->value, gate->output);
                            tempnode->next = NULL;
                            temp1->next = tempnode;
                            temp1 = temp1->next;
                        }
                    }              
                }
                // xor gate
                     //                                               NODE NODE NODE
                if(temporinput == 1){
                    if(temporinput2 == 1){
                //    printf("\nLine number: %d\n", __LINE__);
                        if(temporout == 1){
                    //    printf("\nLine number: %d\n", __LINE__);
                            if(node->boolean == 1 || node2->boolean == 1 ){
                                if(node->boolean != node2->boolean){
                                node1->boolean = 1;
                                }
                                else{node1->boolean = 0;}
                        //    printf("node to node1: %s is %d and %s is %d.........", node->value,node->boolean, node1->value, node1->boolean);
                            }
                            else{
                                node1->boolean = 0;
                          //  printf("node to node1: %s is %d and %s is %d.........", node->value,node->boolean, node1->value, node1->boolean);
                            }
                        }
                        else{                               // NODE NODE TEMP
                    //    printf("\nLine number: %d\n", __LINE__);
                            if(node->boolean == 1 || node2->boolean == 1){
                        //    printf("\nLine number: %d\n", __LINE__);
                        if(node->boolean != node2->boolean){
                                temp1->boolean = 1;
                        }
                        else{
                            temp1->boolean = 0;
                        }
                           // printf("node to temp1: %s is %d and %s is %d.........", node->value,node->boolean, temp1->value, temp1->boolean);
                        }
                            else{
                                temp1->boolean = 0;
                          //  printf("node to temp1:%s is %d and %s is %d.........", node->value,node->boolean, temp1->value, temp1->boolean);
                            }
                    //    printf("\nLine number: %d\n", __LINE__);
                        }
                    }
                    else{                               //NODE TEMP NODE
                        if(temporout == 1){
                    //    printf("\nLine number: %d\n", __LINE__);
                            if(node->boolean == 1 || temp2->boolean == 1){
                                if(node->boolean != temp2->boolean){
                                node1->boolean = 1;
                                }
                                else{node1->boolean = 0;}
                        //    printf("node to node1: %s is %d and %s is %d.........", node->value,node->boolean, node1->value, node1->boolean);
                            }
                            else{
                                node1->boolean = 0;
                          //  printf("node to node1: %s is %d and %s is %d.........", node->value,node->boolean, node1->value, node1->boolean);
                            }
                        }
                        else{
                    //    printf("\nLine number: %d\n", __LINE__);
                            if(node->boolean == 1 || temp2->boolean == 1){
                        //    printf("\nLine number: %d\n", __LINE__);
                        if(node->boolean != temp2->boolean){
                                temp1->boolean = 1;
                        }
                        else{temp1->boolean = 0;}
                           // printf("node to temp1: %s is %d and %s is %d.........", node->value,node->boolean, temp1->value, temp1->boolean);
                            }
                            else{
                                temp1->boolean = 0;
                          //  printf("node to temp1:%s is %d and %s is %d.........", node->value,node->boolean, temp1->value, temp1->boolean);
                            }
                    //    printf("\nLine number: %d\n", __LINE__);
                        }
                    }
                }
                else{//                     TEMP NODE NODE
                    if(temporinput2 == 1){
                        if(temporout == 1){
                    //    printf("\nLine number: %d\n", __LINE__);
                            if(temp->boolean == 1 || node2->boolean == 1){
                                if(temp->boolean != node2->boolean){
                        //    printf("\nLine number: %d\n", __LINE__);
                                node1->boolean = 1;     
                                }
                                else{node1->boolean = 0;}                       
                        //    printf("\nLine number: %d\n", __LINE__);
                            }
                            else{
                        //    printf("\nLine number: %d\n", __LINE__);
                                node1->boolean = 0;
                            }
                        }
                        else{           //TEMP NODE TEMP
                    //    printf("\nLine number: %d\n", __LINE__);
                            if(temp->boolean == 1 || node2->boolean == 1 ){
                            // printf("\nLine number: %d\n", __LINE__);
                            if(temp->boolean != node2->boolean){
                                temp1->boolean = 1;
                            }
                            else{temp1->boolean = 0;}
                           // printf("temp to temp1: %s is %d and %s is %d.........",  temp->value,temp->boolean, temp1->value, temp1->boolean);
                            }
                            else{
                                temp1->boolean = 0;
                          //  printf("temp to temp1: %s is %d and %s is %d.........", temp->value,temp->boolean, temp1->value, temp1->boolean);
                            }
                        }
                    }
                    else{
                        if(temporout == 1){
                    //    printf("\nLine number: %d\n", __LINE__);
                            if(temp->boolean == 1 || temp2->boolean == 1){
                                if(temp->boolean != temp2->boolean){
                        //    printf("\nLine number: %d\n", __LINE__);
                                node1->boolean = 1;    }
                                else{node1->boolean = 0;}                        
                        //    printf("\nLine number: %d\n", __LINE__);
                            }
                            else{
                        //    printf("\nLine number: %d\n", __LINE__);
                                node1->boolean = 0;
                            }
                        }
                        else{           //TEMP NODE TEMP
                    //    printf("\nLine number: %d\n", __LINE__);
                            if(temp->boolean == 1 || temp2->boolean == 1){
                                if(temp->boolean != temp2->boolean){
                            // printf("\nLine number: %d\n", __LINE__);
                                temp1->boolean = 1;
                                }
                                else{temp1->boolean = 0;}
                           // printf("temp to temp1: %s is %d and %s is %d.........",  temp->value,temp->boolean, temp1->value, temp1->boolean);
                            }
                            else{
                                temp1->boolean = 0;
                          //  printf("temp to temp1: %s is %d and %s is %d.........", temp->value,temp->boolean, temp1->value, temp1->boolean);
                            }
                        }
                    }
                }

            }
            else if(strcmp(gate->operator, "MULTIPLEXER") == 0){
                node = headinput;
                // struct inputNode* node2 = headinput;
                // printf("\nline number: %d\n", __LINE__);
               // printf("inputs: %s %s %s", headinput->value, headinput->next->value, headinput->next->next->value);
                // struct tempNode* temp = headtemp;
                // struct tempNode* temp2 = headtemp;
                char holdbinaryinputs[32][32];
                for(int i = 0; i < 32; i++){
                    strcpy(holdbinaryinputs[i], "");
                }
                int binaryinputscounter = 0;
                // for(int i = 0;i < gate->n; i++){
                //     strcpy(holdbinaryinputs[i], gate->words2[i]);
                // }
                // int bool = 0;
                int newbool;
                // int temporinput = 0;
              //  int temporinput2 = 0;
               // printf("Node values: ");
                // printf("\nline number: %d\n", __LINE__);
                for(int i = 0; i < gate->n; i++){
                    // printf("\nline number: %d\n", __LINE__);
                    newbool = 0 ;
                    struct inputNode* newnode = headinput;
                    while(newnode != NULL){
                        // printf("\nline number: %d\n", __LINE__);
                        // printf("Values of n and i: %d %d", gate->n, i);
                        // printf("node and gate names: %s %s", newnode->value, gate->words2[i]);
                        if(strcmp(newnode->value, gate->words2[i]) == 0){
                            // printf("\nline number: %d\n", __LINE__);
                            newbool = 1;
                            //itoa(newnode->boolean, holdbinaryinputs[binaryinputscounter], 10);
                            sprintf(holdbinaryinputs[binaryinputscounter], "%d", newnode->boolean);
                            binaryinputscounter++;
                            // printf("\nline number: %d\n", __LINE__);
                            break;
                        }
                        // printf("\nline number: %d\n", __LINE__);
                        newnode = newnode->next;
                    }
                    // printf("\nLine number: %d\n", __LINE__);
                    if(newbool == 0){
                        // printf("\nline number: %d\n", __LINE__);
                        struct tempNode* newtemp = headtemp;
                        while(newtemp != NULL){
                            if(strcmp(newtemp->value, gate->words2[i]) == 0){
                                // printf("\nline number: %d\n", __LINE__);
                                //itoa(newtemp->boolean, holdbinaryinputs[binaryinputscounter], 10);
                                sprintf(holdbinaryinputs[binaryinputscounter], "%d", newtemp->boolean);
                                binaryinputscounter++;
                                // printf("\nline number: %d\n", __LINE__);
                                break;
                            }
                            // printf("\nline number: %d\n", __LINE__);
                            newtemp = newtemp->next;
                        }
                        // printf("\nline number: %d\n", __LINE__);
                    }
                }
                // printf("\nline number: %d\n", __LINE__);

                char allinone[32];
                strcpy(allinone, "");
                // printf("\nLine number: %d\n", __LINE__);
                for(int i = 0; i < gate->n; i++){
                    if(strcmp(holdbinaryinputs[i], "1") == 0){
                        strcat(allinone, "1");
                    }
                    else{
                        strcat(allinone, "0");
                    }
                } 
                // printf("binary number: %s\n", allinone);              
                int index = binarytodecimal(allinone);                  
                // printf("\nLine number: %d\n", __LINE__);
                        //holdbinaryinputs holds all of the multiplexerinputs, we will convert from binary to decimal to find the appropriate output.
                    /////////////////////////////////////////////////////////////////////////////////////////////////////
                // while(node != NULL){
                //     if(strcmp(gate->input1, node->value) == 0){
                //         bool = 1;
                //         temporinput = 1;
                //     //    printf("\nLine number: %d\n", __LINE__);
                //         break;
                //     }
                // //    printf("\njust checked: %s\n", node->value);
                //     node = node->next;
                // }
                // if(bool == 0){
                //     while(temp != NULL){
                //         if(strcmp(gate->input1, temp->value) == 0){
                //            // printf(".........temp input found: %s, %s...........", gate->input1, temp->value);
                //             break;
                //         }
                //         temp = temp->next;
                //     }
                // }
                // //same stuff for second input now:
                // int bool2 = 0;
                // while(node2 != NULL){
                //     if(strcmp(gate->input2, node2->value) == 0){
                //         bool2 = 1;
                //         temporinput2 = 1;
                //     //    printf("\nLine number: %d\n", __LINE__);
                //         break;
                //     }
                // //    printf("\njust checked: %s\n", node->value);
                //     node2 = node2->next;
                // }
                // if(bool2 == 0){
                //     while(temp2 != NULL){
                //         if(strcmp(gate->input2, temp2->value) == 0){
                //            // printf(".........temp input found: %s, %s...........", gate->input1, temp->value);
                //             break;
                //         }
                //         temp2 = temp2->next;
                //     }
                // }
              ///////////////////////////////////////////////////////////////////////////////////////////////////////

                        //saving value of desired index to eventually output.
                int result;
                // printf("\nLine number: %d\n", __LINE__);
                // printf("index value: %d", index);
                if(strcmp(gate->words1[index], "1") == 0){
                    result = 1;
                }
                else if(strcmp(gate->words1[index], "0") == 0){
                    result = 0;
                }
                else{
                    int indexbool = 0;
                    struct inputNode* indexnode = headinput;
                    while(indexnode != NULL){
                        if(strcmp(indexnode->value, gate->words1[index]) == 0){
                            result = indexnode->boolean;
                            indexbool = 1;
                            break;
                        }
                        indexnode = indexnode->next;
                    }
                    if(indexbool == 0){
                        struct tempNode* indextemp = headtemp;
                        while(indextemp != NULL){
                            if(strcmp(indextemp->value, gate->words1[index]) == 0){
                                result = indextemp->boolean;
                                break;
                            }
                            indextemp = indextemp->next;
                        }
                    }
                }
                // printf("\nLine number: %d\n", __LINE__);

                //checking to see if gate output var already exists in either output linkedlist, or temp var linked list, if not, creating new node in temp linked list

                struct inputNode* node1 = headoutput;
                struct tempNode* temp1 = headtemp;
                int booloutput = 0;
                int temporout = 0;
                // printf("gate output: %s", gate->output);
                while(node1 != NULL){
                    if(strcmp(node1->value, gate->output) == 0){
                        booloutput = 1;
                        temporout = 1;
                        break;
                    }
                    node1 = node1->next;
                }
                // printf("\nLine number: %d\n", __LINE__);

                if(booloutput == 0){
                    if(headtemp == NULL){
                        struct tempNode* tempnode = (struct tempNode*)malloc(sizeof(struct tempNode));
                        tempnode->next = NULL;
                        strcpy(tempnode->value, gate->output);
                        headtemp = tempnode;
                        temp1 = headtemp;
                        booloutput = 1;
                        
                     //   printf("first temp name is: %s", headtemp->value);
                    }
                    else{
                        
                        while(temp1->next != NULL){
                            if(strcmp(temp1->value, gate->output) == 0){
                                booloutput = 1;
                                break;
                            }
                            temp1 = temp1->next;
                        }
                        if(strcmp(temp1->value, gate->output) == 0){
                                booloutput = 1;
                            }
                        else{
                            struct tempNode* tempnode = (struct tempNode*)malloc(sizeof(struct tempNode));
                            strcpy(tempnode->value, gate->output);
                            tempnode->next = NULL;
                            temp1->next = tempnode;
                            temp1 = temp1->next;
                        }
                    }              
                }
                    // printf("\nLine number: %d\n", __LINE__);
                if(temporout == 1){
                    node1->boolean = result;
                }
                else{
                    temp1->boolean = result;
                }
                // xor gate
                     //                                               NODE NODE NODE
                // if(temporinput == 1){
                //     if(temporinput2 == 1){
                // //    printf("\nLine number: %d\n", __LINE__);
                //         if(temporout == 1){
                //     //    printf("\nLine number: %d\n", __LINE__);
                //             if(node->boolean == 1 || node2->boolean == 1 ){
                //                 if(node->boolean != node2->boolean){
                //                 node1->boolean = 1;
                //                 }
                //                 else{node1->boolean = 0;}
                //         //    printf("node to node1: %s is %d and %s is %d.........", node->value,node->boolean, node1->value, node1->boolean);
                //             }
                //             else{
                //                 node1->boolean = 0;
                //           //  printf("node to node1: %s is %d and %s is %d.........", node->value,node->boolean, node1->value, node1->boolean);
                //             }
                //         }
                //         else{                               // NODE NODE TEMP
                //     //    printf("\nLine number: %d\n", __LINE__);
                //             if(node->boolean == 1 || node2->boolean == 1){
                //         //    printf("\nLine number: %d\n", __LINE__);
                //         if(node->boolean != node2->boolean){
                //                 temp1->boolean = 1;
                //         }
                //         else{
                //             temp1->boolean = 0;
                //         }
                //            // printf("node to temp1: %s is %d and %s is %d.........", node->value,node->boolean, temp1->value, temp1->boolean);
                //         }
                //             else{
                //                 temp1->boolean = 0;
                //           //  printf("node to temp1:%s is %d and %s is %d.........", node->value,node->boolean, temp1->value, temp1->boolean);
                //             }
                //     //    printf("\nLine number: %d\n", __LINE__);
                //         }
                //     }
                //     else{                               //NODE TEMP NODE
                //         if(temporout == 1){
                //     //    printf("\nLine number: %d\n", __LINE__);
                //             if(node->boolean == 1 || temp2->boolean == 1){
                //                 if(node->boolean != temp2->boolean){
                //                 node1->boolean = 1;
                //                 }
                //                 else{node1->boolean = 0;}
                //         //    printf("node to node1: %s is %d and %s is %d.........", node->value,node->boolean, node1->value, node1->boolean);
                //             }
                //             else{
                //                 node1->boolean = 0;
                //           //  printf("node to node1: %s is %d and %s is %d.........", node->value,node->boolean, node1->value, node1->boolean);
                //             }
                //         }
                //         else{
                //     //    printf("\nLine number: %d\n", __LINE__);
                //             if(node->boolean == 1 || temp2->boolean == 1){
                //         //    printf("\nLine number: %d\n", __LINE__);
                //         if(node->boolean != temp2->boolean){
                //                 temp1->boolean = 1;
                //         }
                //         else{temp1->boolean = 0;}
                //            // printf("node to temp1: %s is %d and %s is %d.........", node->value,node->boolean, temp1->value, temp1->boolean);
                //             }
                //             else{
                //                 temp1->boolean = 0;
                //           //  printf("node to temp1:%s is %d and %s is %d.........", node->value,node->boolean, temp1->value, temp1->boolean);
                //             }
                //     //    printf("\nLine number: %d\n", __LINE__);
                //         }
                //     }
                // }
                // else{//                     TEMP NODE NODE
                //     if(temporinput2 == 1){
                //         if(temporout == 1){
                //     //    printf("\nLine number: %d\n", __LINE__);
                //             if(temp->boolean == 1 || node2->boolean == 1){
                //                 if(temp->boolean != node2->boolean){
                //         //    printf("\nLine number: %d\n", __LINE__);
                //                 node1->boolean = 1;     
                //                 }
                //                 else{node1->boolean = 0;}                       
                //         //    printf("\nLine number: %d\n", __LINE__);
                //             }
                //             else{
                //         //    printf("\nLine number: %d\n", __LINE__);
                //                 node1->boolean = 0;
                //             }
                //         }
                //         else{           //TEMP NODE TEMP
                //     //    printf("\nLine number: %d\n", __LINE__);
                //             if(temp->boolean == 1 || node2->boolean == 1 ){
                //             // printf("\nLine number: %d\n", __LINE__);
                //             if(temp->boolean != node2->boolean){
                //                 temp1->boolean = 1;
                //             }
                //             else{temp1->boolean = 0;}
                //            // printf("temp to temp1: %s is %d and %s is %d.........",  temp->value,temp->boolean, temp1->value, temp1->boolean);
                //             }
                //             else{
                //                 temp1->boolean = 0;
                //           //  printf("temp to temp1: %s is %d and %s is %d.........", temp->value,temp->boolean, temp1->value, temp1->boolean);
                //             }
                //         }
                //     }
                //     else{
                //         if(temporout == 1){
                //     //    printf("\nLine number: %d\n", __LINE__);
                //             if(temp->boolean == 1 || temp2->boolean == 1){
                //                 if(temp->boolean != temp2->boolean){
                //         //    printf("\nLine number: %d\n", __LINE__);
                //                 node1->boolean = 1;    }
                //                 else{node1->boolean = 0;}                        
                //         //    printf("\nLine number: %d\n", __LINE__);
                //             }
                //             else{
                //         //    printf("\nLine number: %d\n", __LINE__);
                //                 node1->boolean = 0;
                //             }
                //         }
                //         else{           //TEMP NODE TEMP
                //     //    printf("\nLine number: %d\n", __LINE__);
                //             if(temp->boolean == 1 || temp2->boolean == 1){
                //                 if(temp->boolean != temp2->boolean){
                //             // printf("\nLine number: %d\n", __LINE__);
                //                 temp1->boolean = 1;
                //                 }
                //                 else{temp1->boolean = 0;}
                //            // printf("temp to temp1: %s is %d and %s is %d.........",  temp->value,temp->boolean, temp1->value, temp1->boolean);
                //             }
                //             else{
                //                 temp1->boolean = 0;
                //           //  printf("temp to temp1: %s is %d and %s is %d.........", temp->value,temp->boolean, temp1->value, temp1->boolean);
                //             }
                //         }
                //     }
                // }

            }
            else if(strcmp(gate->operator, "DECODER") == 0){
                node = headinput;
                // struct inputNode* node2 = headinput;
                // printf("\nline number: %d\n", __LINE__);
                // struct tempNode* temp = headtemp;
                // struct tempNode* temp2 = headtemp;
                char holdbinaryinputs[32][32];
                for(int i = 0; i < 32; i++){
                    strcpy(holdbinaryinputs[i], "");
                }
                int binaryinputscounter = 0;

                // int bool = 0;
                int newbool;
                // int temporinput = 0;
                // int temporinput2 = 0;
                for(int i = 0; i < gate->n; i++){
                    newbool = 0 ;
                    struct inputNode* newnode = headinput;
                    while(newnode != NULL){
                        if(strcmp(newnode->value, gate->words1[i]) == 0){
                            newbool = 1;
                            //itoa(newnode->boolean, holdbinaryinputs[binaryinputscounter], 10);
                            sprintf(holdbinaryinputs[binaryinputscounter], "%d", newnode->boolean);

                          //  itoa(newnode->boolean, holdbinaryinputs[binaryinputscounter], 10);
                            binaryinputscounter++;
                            break;
                        }
                        newnode = newnode->next;
                    }
                    // printf("\nLine number: %d\n", __LINE__);
                    if(newbool == 0){
                        // printf("\nline number: %d\n", __LINE__);
                        struct tempNode* newtemp = headtemp;
                        while(newtemp != NULL){
                            if(strcmp(newtemp->value, gate->words1[i]) == 0){
                                // printf("\nline number: %d\n", __LINE__);
                                sprintf(holdbinaryinputs[binaryinputscounter], "%d", newtemp->boolean);

                              //  itoa(newtemp->boolean, holdbinaryinputs[binaryinputscounter], 10);
                                binaryinputscounter++;
                                // printf("\nline number: %d\n", __LINE__);
                                break;
                            }
                            // printf("\nline number: %d\n", __LINE__);
                            newtemp = newtemp->next;
                        }
                        // printf("\nline number: %d\n", __LINE__);
                    }
                }
                // printf("\nline number: %d\n", __LINE__);

                char allinone[32];
                strcpy(allinone, "");
                // printf("\nLine number: %d\n", __LINE__);
                for(int i = 0; i < gate->n; i++){
                    if(strcmp(holdbinaryinputs[i], "1") == 0){
                        strcat(allinone, "1");
                    }
                    else{
                        strcat(allinone, "0");
                    }
                } 
                // printf("binary number: %s\n", allinone);              
                int index = binarytodecimal(allinone);                  
                // printf("\nLine number: %d\n", __LINE__);
                        //holdbinaryinputs holds all of the multiplexerinputs, we will convert from binary to decimal to find the appropriate output.
                    /////////////////////////////////////////////////////////////////////////////////////////////////////
                // while(node != NULL){
                //     if(strcmp(gate->input1, node->value) == 0){
                //         bool = 1;
                //         temporinput = 1;
                //     //    printf("\nLine number: %d\n", __LINE__);
                //         break;
                //     }
                // //    printf("\njust checked: %s\n", node->value);
                //     node = node->next;
                // }
                // if(bool == 0){
                //     while(temp != NULL){
                //         if(strcmp(gate->input1, temp->value) == 0){
                //            // printf(".........temp input found: %s, %s...........", gate->input1, temp->value);
                //             break;
                //         }
                //         temp = temp->next;
                //     }
                // }
                // //same stuff for second input now:
                // int bool2 = 0;
                // while(node2 != NULL){
                //     if(strcmp(gate->input2, node2->value) == 0){
                //         bool2 = 1;
                //         temporinput2 = 1;
                //     //    printf("\nLine number: %d\n", __LINE__);
                //         break;
                //     }
                // //    printf("\njust checked: %s\n", node->value);
                //     node2 = node2->next;
                // }
                // if(bool2 == 0){
                //     while(temp2 != NULL){
                //         if(strcmp(gate->input2, temp2->value) == 0){
                //            // printf(".........temp input found: %s, %s...........", gate->input1, temp->value);
                //             break;
                //         }
                //         temp2 = temp2->next;
                //     }
                // }
              ///////////////////////////////////////////////////////////////////////////////////////////////////////

                        //saving value of desired index to eventually output.
                // int result;
                // printf("\nLine number: %d\n", __LINE__);
                // printf("index value: %d", index);
                // if(strcmp(gate->words1[index], "1") == 0){
                //     result = 1;
                // }
                // else if(strcmp(gate->words1[index], "0") == 0){
                //     result = 0;
                // }
                // else{
                //     int indexbool = 0;
                //     struct inputNode* indexnode = headinput;
                //     while(indexnode != NULL){
                //         if(strcmp(indexnode->value, gate->words1[index]) == 0){
                //             result = indexnode->boolean;
                //             indexbool = 1;
                //             break;
                //         }
                //         indexnode = indexnode->next;
                //     }
                //     if(indexbool == 0){
                //         struct tempNode* indextemp = headtemp;
                //         while(indextemp != NULL){
                //             if(strcmp(indextemp->value, gate->words1[index]) == 0){
                //                 result = indextemp->boolean;
                //                 break;
                //             }
                //             indextemp = indextemp->next;
                //         }
                //     }
                // }
                // printf("\nLine number: %d\n", __LINE__);

                //checking to see if gate output var already exists in either output linkedlist, or temp var linked list, if not, creating new node in temp linked list
                int x = powoftwo(2, gate->n);
                for(int i = 0; i < x; i++){
                    if(strcmp(gate->words2[i], "_") == 0){
                        continue;
                    }
                    struct inputNode*node1 = headoutput;
                    struct tempNode* temp1 = headtemp;
                    int booloutput = 0;
                    // int temporout;
                    while(node1 != NULL){
                        if(strcmp(node1->value, gate->words2[i]) == 0){
                            if(index == i){
                                node1->boolean = 1;
                            }
                            else{
                                node1->boolean = 0;
                            }
                            booloutput = 1;
                        //    temporout = 1;
                            break;
                        }
                    node1 = node1->next;
                    }
                    if(booloutput == 0){
                        if(headtemp == NULL){
                            struct tempNode* tempnode = (struct tempNode*)malloc(sizeof(struct tempNode));
                            tempnode->next = NULL;
                            strcpy(tempnode->value, gate->words2[i]);
                            if(index == i){
                                tempnode->boolean = 1;
                            }
                            else{
                                tempnode->boolean = 0;
                            }
                            headtemp = tempnode;
                            temp1 = headtemp;
                            booloutput = 1;
                        
                     //   printf("first temp name is: %s", headtemp->value);
                    }
                    else{   
                        while(temp1->next != NULL){
                            if(strcmp(temp1->value, gate->words2[i]) == 0){
                                if(index == i){
                                    temp1->boolean = 1;
                                }
                                else{
                                    temp1->boolean = 0;
                                }
                                booloutput = 1;
                                break;
                            }
                            temp1 = temp1->next;
                        }
                        if(strcmp(temp1->value, gate->words2[i]) == 0){
                                if(index == i){
                                    temp1->boolean = 1;
                                }
                                else{
                                    temp1->boolean = 0;
                                }
                                booloutput = 1;
                            }
                        else{
                            struct tempNode* tempnode = (struct tempNode*)malloc(sizeof(struct tempNode));
                            strcpy(tempnode->value, gate->words2[i]);
                            tempnode->next = NULL;
                            if(index == i){
                                tempnode->boolean = 1;
                            }
                            else{
                                tempnode->boolean = 0;
                            }
                            temp1->next = tempnode;
                            temp1 = temp1->next;
                        }
                    }              
                }
                }

                // struct inputNode* node1 = headoutput;
                // struct tempNode* temp1 = headtemp;
                // int booloutput = 0;
                // int temporout = 0;
                // // printf("gate output: %s", gate->output);
                // while(node1 != NULL){
                //     if(strcmp(node1->value, gate->output) == 0){
                //         booloutput = 1;
                //         temporout = 1;
                //         break;
                //     }
                //     node1 = node1->next;
                // }
                // // printf("\nLine number: %d\n", __LINE__);

                // if(booloutput == 0){
                //     if(headtemp == NULL){
                //         struct tempNode* tempnode = (struct tempNode*)malloc(sizeof(struct tempNode));
                //         tempnode->next = NULL;
                //         strcpy(tempnode->value, gate->output);
                //         headtemp = tempnode;
                //         temp1 = headtemp;
                //         booloutput = 1;
                        
                //      //   printf("first temp name is: %s", headtemp->value);
                //     }
                //     else{
                        
                //         while(temp1->next != NULL){
                //             if(strcmp(temp1->value, gate->output) == 0){
                //                 booloutput = 1;
                //                 break;
                //             }
                //             temp1 = temp1->next;
                //         }
                //         if(strcmp(temp1->value, gate->output) == 0){
                //                 booloutput = 1;
                //             }
                //         else{
                //             struct tempNode* tempnode = (struct tempNode*)malloc(sizeof(struct tempNode));
                //             strcpy(tempnode->value, gate->output);
                //             tempnode->next = NULL;
                //             temp1->next = tempnode;
                //             temp1 = temp1->next;
                //         }
                //     }              
                // }
                    // printf("\nLine number: %d\n", __LINE__);
                // if(temporout == 1){
                //     node1->boolean = result;
                // }
                // else{
                //     temp1->boolean = result;
                // }
                // xor gate
                     //                                               NODE NODE NODE
                // if(temporinput == 1){
                //     if(temporinput2 == 1){
                // //    printf("\nLine number: %d\n", __LINE__);
                //         if(temporout == 1){
                //     //    printf("\nLine number: %d\n", __LINE__);
                //             if(node->boolean == 1 || node2->boolean == 1 ){
                //                 if(node->boolean != node2->boolean){
                //                 node1->boolean = 1;
                //                 }
                //                 else{node1->boolean = 0;}
                //         //    printf("node to node1: %s is %d and %s is %d.........", node->value,node->boolean, node1->value, node1->boolean);
                //             }
                //             else{
                //                 node1->boolean = 0;
                //           //  printf("node to node1: %s is %d and %s is %d.........", node->value,node->boolean, node1->value, node1->boolean);
                //             }
                //         }
                //         else{                               // NODE NODE TEMP
                //     //    printf("\nLine number: %d\n", __LINE__);
                //             if(node->boolean == 1 || node2->boolean == 1){
                //         //    printf("\nLine number: %d\n", __LINE__);
                //         if(node->boolean != node2->boolean){
                //                 temp1->boolean = 1;
                //         }
                //         else{
                //             temp1->boolean = 0;
                //         }
                //            // printf("node to temp1: %s is %d and %s is %d.........", node->value,node->boolean, temp1->value, temp1->boolean);
                //         }
                //             else{
                //                 temp1->boolean = 0;
                //           //  printf("node to temp1:%s is %d and %s is %d.........", node->value,node->boolean, temp1->value, temp1->boolean);
                //             }
                //     //    printf("\nLine number: %d\n", __LINE__);
                //         }
                //     }
                //     else{                               //NODE TEMP NODE
                //         if(temporout == 1){
                //     //    printf("\nLine number: %d\n", __LINE__);
                //             if(node->boolean == 1 || temp2->boolean == 1){
                //                 if(node->boolean != temp2->boolean){
                //                 node1->boolean = 1;
                //                 }
                //                 else{node1->boolean = 0;}
                //         //    printf("node to node1: %s is %d and %s is %d.........", node->value,node->boolean, node1->value, node1->boolean);
                //             }
                //             else{
                //                 node1->boolean = 0;
                //           //  printf("node to node1: %s is %d and %s is %d.........", node->value,node->boolean, node1->value, node1->boolean);
                //             }
                //         }
                //         else{
                //     //    printf("\nLine number: %d\n", __LINE__);
                //             if(node->boolean == 1 || temp2->boolean == 1){
                //         //    printf("\nLine number: %d\n", __LINE__);
                //         if(node->boolean != temp2->boolean){
                //                 temp1->boolean = 1;
                //         }
                //         else{temp1->boolean = 0;}
                //            // printf("node to temp1: %s is %d and %s is %d.........", node->value,node->boolean, temp1->value, temp1->boolean);
                //             }
                //             else{
                //                 temp1->boolean = 0;
                //           //  printf("node to temp1:%s is %d and %s is %d.........", node->value,node->boolean, temp1->value, temp1->boolean);
                //             }
                //     //    printf("\nLine number: %d\n", __LINE__);
                //         }
                //     }
                // }
                // else{//                     TEMP NODE NODE
                //     if(temporinput2 == 1){
                //         if(temporout == 1){
                //     //    printf("\nLine number: %d\n", __LINE__);
                //             if(temp->boolean == 1 || node2->boolean == 1){
                //                 if(temp->boolean != node2->boolean){
                //         //    printf("\nLine number: %d\n", __LINE__);
                //                 node1->boolean = 1;     
                //                 }
                //                 else{node1->boolean = 0;}                       
                //         //    printf("\nLine number: %d\n", __LINE__);
                //             }
                //             else{
                //         //    printf("\nLine number: %d\n", __LINE__);
                //                 node1->boolean = 0;
                //             }
                //         }
                //         else{           //TEMP NODE TEMP
                //     //    printf("\nLine number: %d\n", __LINE__);
                //             if(temp->boolean == 1 || node2->boolean == 1 ){
                //             // printf("\nLine number: %d\n", __LINE__);
                //             if(temp->boolean != node2->boolean){
                //                 temp1->boolean = 1;
                //             }
                //             else{temp1->boolean = 0;}
                //            // printf("temp to temp1: %s is %d and %s is %d.........",  temp->value,temp->boolean, temp1->value, temp1->boolean);
                //             }
                //             else{
                //                 temp1->boolean = 0;
                //           //  printf("temp to temp1: %s is %d and %s is %d.........", temp->value,temp->boolean, temp1->value, temp1->boolean);
                //             }
                //         }
                //     }
                //     else{
                //         if(temporout == 1){
                //     //    printf("\nLine number: %d\n", __LINE__);
                //             if(temp->boolean == 1 || temp2->boolean == 1){
                //                 if(temp->boolean != temp2->boolean){
                //         //    printf("\nLine number: %d\n", __LINE__);
                //                 node1->boolean = 1;    }
                //                 else{node1->boolean = 0;}                        
                //         //    printf("\nLine number: %d\n", __LINE__);
                //             }
                //             else{
                //         //    printf("\nLine number: %d\n", __LINE__);
                //                 node1->boolean = 0;
                //             }
                //         }
                //         else{           //TEMP NODE TEMP
                //     //    printf("\nLine number: %d\n", __LINE__);
                //             if(temp->boolean == 1 || temp2->boolean == 1){
                //                 if(temp->boolean != temp2->boolean){
                //             // printf("\nLine number: %d\n", __LINE__);
                //                 temp1->boolean = 1;
                //                 }
                //                 else{temp1->boolean = 0;}
                //            // printf("temp to temp1: %s is %d and %s is %d.........",  temp->value,temp->boolean, temp1->value, temp1->boolean);
                //             }
                //             else{
                //                 temp1->boolean = 0;
                //           //  printf("temp to temp1: %s is %d and %s is %d.........", temp->value,temp->boolean, temp1->value, temp1->boolean);
                //             }
                //         }
                //     }
                // }

            }
          //  printf("\nLine number: %d\n", __LINE__);
            gate = gate->nextgate;
        }

           // printf("\nLine number: %d\n", __LINE__);
        node = headoutput;
        while(node != NULL){
            printf(" %d", node->boolean);
            node = node->next;
        }
    
    

       //incrementing binary inputs
        int j = inputnum - 2;
        if(clock[inputnum - 1] == 0){
            clock[inputnum - 1] = 1;
        }
        else{
            clock[inputnum - 1] = 0;
            int j = inputnum - 2;
                while(clock[j] == 1){
                    clock[j] = 0;
                    j--;
                }
            
            if(j >= 0){
                clock[j] = 1;
            }
        }
        printf("\n");
    }
    freecircuitNode(headcircuit);
    freeinputNode(headinput);
    freeinputNode(headoutput);
    freetempNode(headtemp);    
    return(EXIT_SUCCESS);
}