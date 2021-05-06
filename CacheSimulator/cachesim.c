#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


int hit;
int miss;
int read;
int write;
uint64_t address;
unsigned long long int cachesize;
unsigned long long int blocksize;
unsigned long long int numoflines;
unsigned long long int indexsize;
unsigned long long int queuesize;
unsigned long long int asn;
int blockoffset;
char assocn[7];

struct cacheline{
    //int hasdata;
    //unsigned long long int age;
    unsigned long long int tag;
    unsigned long long int setindex;
    unsigned long long int indexlistsize;
    struct cacheline* next;
    struct cacheline* setnext;
    struct cacheline* setprev;
};
//unsigned long long int age = 0;

struct cacheline* enqueue(struct cacheline* head, unsigned long long int index, unsigned long long int tag){
    // printf("ENQUEUE:");
    struct cacheline* replace = (struct cacheline*)malloc(sizeof(struct cacheline));
    replace->next = NULL;
    replace->setindex = index;
    replace->tag = tag;
    if(head == NULL){
        head = replace;
        queuesize++;
        return head;
    }
    struct cacheline* node = head;
    while(node->next != NULL){
        node = node->next;
    }
    node->next = replace;
    queuesize++;
    node = node->next;
    return node;
}
struct cacheline* dequeue(struct cacheline* head){
    if(head->next != NULL){
        struct cacheline* node = head;
        if(node->setnext != NULL){
            struct cacheline* setnode = node->setnext;
            while (setnode->setnext != NULL){
                struct cacheline* hold = setnode;
                setnode = setnode->setnext;
                free(hold);
            }
            free(setnode);
        }
        head = head->next;
        free(node);
        queuesize--;
       // printf("next != null");
        return head;
    }
    else if(head != NULL){
        struct cacheline* node = head;
        if(node->setnext != NULL){
            struct cacheline* setnode = node->setnext;
            while (setnode->setnext != NULL){
                struct cacheline* hold = setnode;
                setnode = setnode->setnext;
                free(hold);
            }
            free(setnode);
        }
        free(head);
        //printf("next IS null");
        head = NULL;
        queuesize--;
        return head;
    }
    else return NULL;
    // printf("failed dequeue?");
}/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////suka bliet
struct cacheline* findnode(struct cacheline* head, unsigned long long int index, unsigned long long int tag, char operator, int prefetch, unsigned long long int address, unsigned long long int tag1, unsigned long long int set1){
    struct cacheline* node = head;
    if(operator == 'W'){
        write++;
    }
    if(node->setindex == index){
        if(node->tag == tag){
            hit++;
            return node;
        }
        else if(strcmp(assocn, "assoc:") == 0){
            struct cacheline* asnode = node;
            while(asnode->setnext != NULL){
                if(asnode->tag == tag){
                    hit++;
                    return asnode;
                }
                asnode = asnode->setnext;
            }
            if(asnode->tag == tag){   
                hit++;
                return asnode;
            }
            else if(node->indexlistsize < asn){
                struct cacheline* setnode = (struct cacheline*)malloc(sizeof(struct cacheline));
                setnode->tag = tag;
                setnode->setindex = index;
                // setnode->setprev = asnode;
                setnode->setnext = NULL;
                asnode->setnext = setnode;
                asnode = asnode->next;
                node->indexlistsize++;
                miss++;
                read++;
                if(prefetch == 1){
                    struct cacheline* node1 = head;
                    while(node1->next != NULL){///////////////
                        if(node1->setindex == set1){
                            struct cacheline* setnode1 = node1;
                            while(setnode1->setnext != NULL){
                                if(setnode1->tag == tag1){
                                    return node;
                                }
                                setnode1 = setnode1->setnext;
                            }
                            if(setnode1->tag == tag1){
                                return node;
                            }
                            if(node1->indexlistsize < asn){
                                struct cacheline* prefetchnode = (struct cacheline*)malloc(sizeof(struct cacheline));
                                prefetchnode->setnext = NULL;
                                prefetchnode->setindex = set1;
                                prefetchnode->tag = tag1;
                                setnode1->setnext = prefetchnode;
                                read++;
                                node1->indexlistsize++;
                                return setnode1;
                            }
                            else{
                                struct cacheline* hold = node1;
                                while (hold->setnext != NULL){
                                    hold->tag = hold->setnext->tag;
                                    hold = hold->setnext;
                                }
                                hold->tag = tag1;
                                read++;
                                return hold;
                            }
                        }
                        node1 = node1->next;
                    }////////////////////////////////////////
                    //checking last node1 where ->next == NULL
                    if(node1->setindex == set1){
                        struct cacheline* setnode1 = node1;
                        while(setnode1->setnext != NULL){
                            if(setnode1->tag == tag1){
                                return node;
                            }
                            setnode1 = setnode1->setnext;
                        }
                        if(setnode1->tag == tag1){
                            return node;
                        }
                        if(node1->indexlistsize < asn){
                            struct cacheline* prefetchnode = (struct cacheline*)malloc(sizeof(struct cacheline));
                            prefetchnode->setnext = NULL;
                            prefetchnode->setindex = set1;
                            prefetchnode->tag = tag1;
                            setnode1->setnext = prefetchnode;
                            read++;
                            node1->indexlistsize++;
                            return setnode1;
                        }
                        else{
                            struct cacheline* hold = node1;
                            while (hold->setnext != NULL){
                                hold->tag = hold->setnext->tag;
                                hold = hold->setnext;
                            }
                            hold->tag = tag1;
                            read++;
                            return hold;
                        }
                    }
                    else{
                        struct cacheline* prefetchnode = (struct cacheline*)malloc(sizeof(struct cacheline));
                        prefetchnode->indexlistsize = 1;
                        prefetchnode->next = NULL;
                        prefetchnode->setnext = NULL;
                        prefetchnode->setindex = set1;
                        prefetchnode->tag = tag1;
                        node1->next = prefetchnode;
                        read++;
                    }
                }
                return asnode;
            }
            else{
                //fifo set node is head, asnode is tail.
                struct cacheline* hold = node; //now this is some next-level inefficiency, and pod racing.
                while(hold->setnext != NULL){
                    hold->tag = hold->setnext->tag;
                    hold = hold->setnext;
                }
                hold->tag = tag;
                miss++;
                read++;
                if(prefetch == 1){
                    struct cacheline* node1 = head;
                    while(node1->next != NULL){///////////////
                        if(node1->setindex == set1){
                            struct cacheline* setnode1 = node1;
                            while(setnode1->setnext != NULL){
                                if(setnode1->tag == tag1){
                                    return node;
                                }
                                setnode1 = setnode1->setnext;
                            }
                            if(setnode1->tag == tag1){
                                return node;
                            }
                            if(node1->indexlistsize < asn){
                                struct cacheline* prefetchnode = (struct cacheline*)malloc(sizeof(struct cacheline));
                                prefetchnode->setnext = NULL;
                                prefetchnode->setindex = set1;
                                prefetchnode->tag = tag1;
                                setnode1->setnext = prefetchnode;
                                read++;
                                node1->indexlistsize++;
                                return setnode1;
                            }
                            else{
                                struct cacheline* hold = node1;
                                while (hold->setnext != NULL){
                                    hold->tag = hold->setnext->tag;
                                    hold = hold->setnext;
                                }
                                hold->tag = tag1;
                                read++;
                                return hold;
                            }

                        }
                        node1 = node1->next;
                    }////////////////////////////////////////
                    //checking last node1 where ->next == NULL
                    if(node1->setindex == set1){
                        struct cacheline* setnode1 = node1;
                        while(setnode1->setnext != NULL){
                            if(setnode1->tag == tag1){
                                return node;
                            }
                            setnode1 = setnode1->setnext;
                        }
                        if(setnode1->tag == tag1){
                            return node;
                        }
                        if(node1->indexlistsize < asn){
                            struct cacheline* prefetchnode = (struct cacheline*)malloc(sizeof(struct cacheline));
                            prefetchnode->setnext = NULL;
                            prefetchnode->setindex = set1;
                            prefetchnode->tag = tag1;
                            setnode1->setnext = prefetchnode;
                            read++;
                            node1->indexlistsize++;
                            return setnode1;
                        }
                        else{
                            struct cacheline* hold = node1;
                            while (hold->setnext != NULL){
                                hold->tag = hold->setnext->tag;
                                hold = hold->setnext;
                            }
                            hold->tag = tag1;
                            read++;
                            return hold;
                        }
                    }
                    else{
                        struct cacheline* prefetchnode = (struct cacheline*)malloc(sizeof(struct cacheline));
                        prefetchnode->indexlistsize = 1;
                        prefetchnode->next = NULL;
                        prefetchnode->setnext = NULL;
                        prefetchnode->setindex = set1;
                        prefetchnode->tag = tag1;
                        node1->next = prefetchnode;
                        read++;
                    }
                }
                return hold;
            }
        }
        miss++;
        read++;
        node->tag = tag;
        if(prefetch == 1){
            // printf("\nLine number: %d\n", __LINE__);
            struct cacheline* node1 = head;
            while(node1->next != NULL){
                if(node1->setindex == set1){
                    if(node1->tag == tag1){
                        // read++;
                        return node;
                    }                    
                    node1->tag = tag1;
                    read++;
                    return node;
                }
                node1 = node1->next;
            }
            if(node1->setindex == set1){
                    if(node1->tag == tag1){
                        // read++;
                        return node;
                    }
                    node1->tag = tag1;
                    read++;
                    return node;
            }
            else{
                struct cacheline* prefetchnode = (struct cacheline*)malloc(sizeof(struct cacheline));
                prefetchnode->indexlistsize = 1;
                prefetchnode->next = NULL;
                prefetchnode->setnext = NULL;
                prefetchnode->setindex = set1;
                prefetchnode->tag = tag1;
                node1->next = prefetchnode;
                queuesize++;
                read++;
            }
        }
        return node;
    }
    if(node != NULL){
        while(node->next != NULL){
            if(node->setindex == index){
                if(node->tag == tag){
                    hit++;
                    return node;
                }
                else if(strcmp(assocn, "assoc:") == 0){
                    struct cacheline* asnode = node;
                    while(asnode->setnext != NULL){
                        if(asnode->tag == tag){
                            hit++;
                            return asnode;
                        }
                        asnode = asnode->setnext;
                    }
                    if(asnode->tag == tag){                  
                        hit++;
                        return asnode;
                    }
                    else if(node->indexlistsize < asn){
                        struct cacheline* setnode = (struct cacheline*)malloc(sizeof(struct cacheline));
                        setnode->tag = tag;
                        setnode->setindex = index;
                        // setnode->setprev = asnode;
                        setnode->setnext = NULL;
                        asnode->setnext = setnode;
                        asnode = asnode->setnext;
                        node->indexlistsize++;
                        miss++;
                        read++;
                        if(prefetch == 1){
                            struct cacheline* node1 = head;
                            while(node1->next != NULL){///////////////
                                if(node1->setindex == set1){
                                    struct cacheline* setnode1 = node1;
                                    while(setnode1->setnext != NULL){
                                        if(setnode1->tag == tag1){
                                            return node;
                                        }
                                        setnode1 = setnode1->setnext;
                                    }
                                    if(setnode1->tag == tag1){
                                        return node;
                                    }
                                    if(node1->indexlistsize < asn){
                                        struct cacheline* prefetchnode = (struct cacheline*)malloc(sizeof(struct cacheline));
                                        prefetchnode->setnext = NULL;
                                        prefetchnode->setindex = set1;
                                        prefetchnode->tag = tag1;
                                        setnode1->setnext = prefetchnode;
                                        read++;
                                        node1->indexlistsize++;
                                        return setnode1;
                                    }
                                    else{
                                        struct cacheline* hold = node1;
                                        while (hold->setnext != NULL){
                                            hold->tag = hold->setnext->tag;
                                            hold = hold->setnext;
                                        }
                                        hold->tag = tag1;
                                        read++;
                                        return hold;
                                    }

                                }
                                node1 = node1->next;
                            }////////////////////////////////////////
                            //checking last node1 where ->next == NULL
                            if(node1->setindex == set1){
                                struct cacheline* setnode1 = node1;
                                while(setnode1->setnext != NULL){
                                    if(setnode1->tag == tag1){
                                        return node;
                                    }
                                    setnode1 = setnode1->setnext;
                                }
                                if(setnode1->tag == tag1){
                                    return node;
                                }
                                if(node1->indexlistsize < asn){
                                    struct cacheline* prefetchnode = (struct cacheline*)malloc(sizeof(struct cacheline));
                                    prefetchnode->setnext = NULL;
                                    prefetchnode->setindex = set1;
                                    prefetchnode->tag = tag1;
                                    setnode1->setnext = prefetchnode;
                                    read++;
                                    node1->indexlistsize++;
                                    return setnode1;
                                }
                                else{
                                    struct cacheline* hold = node1;
                                    while (hold->setnext != NULL){
                                        hold->tag = hold->setnext->tag;
                                        hold = hold->setnext;
                                    }
                                    hold->tag = tag1;
                                    read++;
                                    return hold;
                                }
                            }
                            else{
                                struct cacheline* prefetchnode = (struct cacheline*)malloc(sizeof(struct cacheline));
                                prefetchnode->indexlistsize = 1;
                                prefetchnode->next = NULL;
                                prefetchnode->setnext = NULL;
                                prefetchnode->setindex = set1;
                                prefetchnode->tag = tag1;
                                node1->next = prefetchnode;
                                read++;
                            }
                        }
                        return asnode;
                    }
                    else{
                        // printf("%d\n", node->indexlistsize);
                        //fifo set node is head, asnode is tail.
                        struct cacheline* hold = node; //now this is some next-level inefficiency, and pod racing.
                        while(hold->setnext != NULL){
                            hold->tag = hold->setnext->tag;
                            hold = hold->setnext;
                        }
                        hold->tag = tag;
                        miss++;
                        read++;
                        if(prefetch == 1){
                    struct cacheline* node1 = head;
                    while(node1->next != NULL){///////////////
                        if(node1->setindex == set1){
                            struct cacheline* setnode1 = node1;
                            while(setnode1->setnext != NULL){
                                if(setnode1->tag == tag1){
                                    return node;
                                }
                                setnode1 = setnode1->setnext;
                            }
                            if(setnode1->tag == tag1){
                                return node;
                            }
                            if(node1->indexlistsize < asn){
                                struct cacheline* prefetchnode = (struct cacheline*)malloc(sizeof(struct cacheline));
                                prefetchnode->setnext = NULL;
                                prefetchnode->setindex = set1;
                                prefetchnode->tag = tag1;
                                setnode1->setnext = prefetchnode;
                                read++;
                                node1->indexlistsize++;
                                return setnode1;
                            }
                            else{
                                struct cacheline* hold = node1;
                                while (hold->setnext != NULL){
                                    hold->tag = hold->setnext->tag;
                                    hold = hold->setnext;
                                }
                                hold->tag = tag1;
                                read++;
                                return hold;
                            }

                        }
                        node1 = node1->next;
                    }////////////////////////////////////////
                    //checking last node1 where ->next == NULL
                    if(node1->setindex == set1){
                        struct cacheline* setnode1 = node1;
                        while(setnode1->setnext != NULL){
                            if(setnode1->tag == tag1){
                                return node;
                            }
                            setnode1 = setnode1->setnext;
                        }
                        if(setnode1->tag == tag1){
                            return node;
                        }
                        if(node1->indexlistsize < asn){
                            struct cacheline* prefetchnode = (struct cacheline*)malloc(sizeof(struct cacheline));
                            prefetchnode->setnext = NULL;
                            prefetchnode->setindex = set1;
                            prefetchnode->tag = tag1;
                            setnode1->setnext = prefetchnode;
                            read++;
                            node1->indexlistsize++;
                            return setnode1;
                        }
                        else{
                            struct cacheline* hold = node1;
                            while (hold->setnext != NULL){
                                hold->tag = hold->setnext->tag;
                                hold = hold->setnext;
                            }
                            hold->tag = tag1;
                            read++;
                            return hold;
                        }
                    }
                    else{
                        struct cacheline* prefetchnode = (struct cacheline*)malloc(sizeof(struct cacheline));
                        prefetchnode->indexlistsize = 1;
                        prefetchnode->next = NULL;
                        prefetchnode->setnext = NULL;
                        prefetchnode->setindex = set1;
                        prefetchnode->tag = tag1;
                        node1->next = prefetchnode;
                        read++;
                        queuesize++;
                    }
                }
                        return hold;
                    }
                }
                node->tag = tag;
                miss++;
                read++;
                if(prefetch == 1){
                    struct cacheline* node1 = head;
                    while(node1->next != NULL){
                        if(node1->setindex == set1){
                            if(node1->tag == tag1){
                                return node;
                            }
                            node1->tag = tag1;
                            read++;
                            return node;
                        }
                        node1 = node1->next;
                    }
                    if(node1->setindex == set1){
                            if(node1->tag == tag1){
                                return node;
                            }
                            node1->tag = tag1;
                            read++;
                            return node;
                    }
                    else{
                        struct cacheline* prefetchnode = (struct cacheline*)malloc(sizeof(struct cacheline));
                        prefetchnode->indexlistsize = 1;
                        prefetchnode->next = NULL;
                        prefetchnode->setnext = NULL;
                        prefetchnode->setindex = set1;
                        prefetchnode->tag = tag1;
                        node1->next = prefetchnode;
                        read++;
                        queuesize++;
                    }
                }
                return node;
            }
            node = node->next;
        }
        if(node->setindex == index){
            if(node->tag == tag){
                hit++;
                return node;
            }
            else if(strcmp(assocn, "assoc:") == 0){
            struct cacheline* asnode = node;
            while(asnode->setnext != NULL){
                if(asnode->tag == tag){
                    hit++;
                    return asnode;
                }
                asnode = asnode->setnext;
            }
            if(asnode->tag == tag){
                hit++;
                return asnode;
            }
            else if(node->indexlistsize < asn){
                struct cacheline* setnode = (struct cacheline*)malloc(sizeof(struct cacheline));
                setnode->tag = tag;
                setnode->setindex = index;
                // setnode->setprev = asnode;
                setnode->setnext = NULL;
                asnode->setnext = setnode;
                asnode = asnode->setnext;
                node->indexlistsize++;
                miss++;
                read++;
                if(prefetch == 1){
                    struct cacheline* node1 = head;
                    while(node1->next != NULL){///////////////
                        if(node1->setindex == set1){
                            struct cacheline* setnode1 = node1;
                            while(setnode1->setnext != NULL){
                                if(setnode1->tag == tag1){
                                    return node;
                                }
                                setnode1 = setnode1->setnext;
                            }
                            if(setnode1->tag == tag1){
                                return node;
                            }
                            if(node1->indexlistsize < asn){
                                struct cacheline* prefetchnode = (struct cacheline*)malloc(sizeof(struct cacheline));
                                prefetchnode->setnext = NULL;
                                prefetchnode->setindex = set1;
                                prefetchnode->tag = tag1;
                                setnode1->setnext = prefetchnode;
                                read++;
                                node1->indexlistsize++;
                                return setnode1;
                            }
                            else{
                                struct cacheline* hold = node1;
                                while (hold->setnext != NULL){
                                    hold->tag = hold->setnext->tag;
                                    hold = hold->setnext;
                                }
                                hold->tag = tag1;
                                read++;
                                return hold;
                            }

                        }
                        node1 = node1->next;
                    }////////////////////////////////////////
                    //checking last node1 where ->next == NULL
                    if(node1->setindex == set1){
                        struct cacheline* setnode1 = node1;
                        while(setnode1->setnext != NULL){
                            if(setnode1->tag == tag1){
                                return node;
                            }
                            setnode1 = setnode1->setnext;
                        }
                        if(setnode1->tag == tag1){
                            return node;
                        }
                        if(node1->indexlistsize < asn){
                            struct cacheline* prefetchnode = (struct cacheline*)malloc(sizeof(struct cacheline));
                            prefetchnode->setnext = NULL;
                            prefetchnode->setindex = set1;
                            prefetchnode->tag = tag1;
                            setnode1->setnext = prefetchnode;
                            read++;
                            node1->indexlistsize++;
                            return setnode1;
                        }
                        else{
                            struct cacheline* hold = node1;
                            while (hold->setnext != NULL){
                                hold->tag = hold->setnext->tag;
                                hold = hold->setnext;
                            }
                            hold->tag = tag1;
                            read++;
                            return hold;
                        }
                    }
                    else{
                        struct cacheline* prefetchnode = (struct cacheline*)malloc(sizeof(struct cacheline));
                        prefetchnode->indexlistsize = 1;
                        prefetchnode->next = NULL;
                        prefetchnode->setnext = NULL;
                        prefetchnode->setindex = set1;
                        prefetchnode->tag = tag1;
                        node1->next = prefetchnode;
                        read++;
                        queuesize++;
                    }
                }
                return asnode;
            }
            else{
                //fifo set node is head, asnode is tail.
                struct cacheline* hold = node; //now this is some next-level inefficiency, and pod racing.
                while(hold->setnext != NULL){
                    hold->tag = hold->setnext->tag;
                    hold = hold->setnext;
                }
                hold->tag = tag;
                miss++;
                read++;
                if(prefetch == 1){
                    struct cacheline* node1 = head;
                    while(node1->next != NULL){///////////////
                        if(node1->setindex == set1){
                            struct cacheline* setnode1 = node1;
                            while(setnode1->setnext != NULL){
                                if(setnode1->tag == tag1){
                                    return node;
                                }
                                setnode1 = setnode1->setnext;
                            }
                            if(setnode1->tag == tag1){
                                return node;
                            }
                            if(node1->indexlistsize < asn){
                                struct cacheline* prefetchnode = (struct cacheline*)malloc(sizeof(struct cacheline));
                                prefetchnode->setnext = NULL;
                                prefetchnode->setindex = set1;
                                prefetchnode->tag = tag1;
                                setnode1->setnext = prefetchnode;
                                read++;
                                node1->indexlistsize++;
                                return setnode1;
                            }
                            else{
                                struct cacheline* hold = node1;
                                while (hold->setnext != NULL){
                                    hold->tag = hold->setnext->tag;
                                    hold = hold->setnext;
                                }
                                hold->tag = tag1;
                                read++;
                                return hold;
                            }

                        }
                        node1 = node1->next;
                    }////////////////////////////////////////
                    //checking last node1 where ->next == NULL
                    if(node1->setindex == set1){
                        struct cacheline* setnode1 = node1;
                        while(setnode1->setnext != NULL){
                            if(setnode1->tag == tag1){
                                return node;
                            }
                            setnode1 = setnode1->setnext;
                        }
                        if(setnode1->tag == tag1){
                            return node;
                        }
                        if(node1->indexlistsize < asn){
                            struct cacheline* prefetchnode = (struct cacheline*)malloc(sizeof(struct cacheline));
                            prefetchnode->setnext = NULL;
                            prefetchnode->setindex = set1;
                            prefetchnode->tag = tag1;
                            setnode1->setnext = prefetchnode;
                            read++;
                            node1->indexlistsize++;
                            return setnode1;
                        }
                        else{
                            struct cacheline* hold = node1;
                            while (hold->setnext != NULL){
                                hold->tag = hold->setnext->tag;
                                hold = hold->setnext;
                            }
                            hold->tag = tag1;
                            read++;
                            return hold;
                        }
                    }
                    else{
                        struct cacheline* prefetchnode = (struct cacheline*)malloc(sizeof(struct cacheline));
                        prefetchnode->indexlistsize = 1;
                        prefetchnode->next = NULL;
                        prefetchnode->setnext = NULL;
                        prefetchnode->setindex = set1;
                        prefetchnode->tag = tag1;
                        node1->next = prefetchnode;
                        read++;
                    }
                }
                return hold;
            }
        }
            node->tag = tag;
            miss++;
            read++;
            if(prefetch == 1){
                // printf("\nLine number: %d\n", __LINE__);
                struct cacheline* node1 = head;
                    while(node1->next != NULL){
                        if(node1->setindex == set1){
                            if(node1->tag == tag1){
                                // read++;
                                return node;
                            }
                            node1->tag = tag1;
                            read++;
                            return node;
                        }
                        node1 = node1->next;
                    }
                    if(node1->setindex == set1){
                            if(node1->tag == tag1){
                                // read++;
                                return node;
                            }
                            node1->tag = tag1;
                            read++;
                            return node;
                    }
                    else{
                        struct cacheline* prefetchnode = (struct cacheline*)malloc(sizeof(struct cacheline));
                        prefetchnode->indexlistsize = 1;
                        prefetchnode->next = NULL;
                        prefetchnode->setnext = NULL;
                        prefetchnode->setindex = set1;
                        prefetchnode->tag = tag1;
                        node1->next = prefetchnode;
                        read++;
                    }
            }
            return node;
        }
    }
    if(queuesize == numoflines){ //fifo
        // printf("\nFIFO \n");
        miss++;
        read++;
        // printf("\naddress, tag, and index: %lx, %lx, %lx\n",address, tag, index);
        // printf("Line number: %d\n", __LINE__);
        head = dequeue(head);
        node = enqueue(head, index, tag);
        return node;
    }
    struct cacheline* now = (struct cacheline*)malloc(sizeof(struct cacheline));
    now->indexlistsize = 1;
    now->next = NULL;
    now->setnext = NULL;
    now->tag = tag;
    now->setindex = index;
    node->next = now;
    node = node->next;
    // printf("\naddress, tag, and index: %lx, %lx, %lx\n",address, tag, index);
    // printf("Line number: %d\n", __LINE__);
    queuesize++;
    miss++;
    read++;
    if(strcmp(assocn, "assoc:") == 0){
        if(prefetch == 1){
            // printf(" EndPrefetch");
                            struct cacheline* node1 = head;
                            while(node1->next != NULL){///////////////
                                if(node1->setindex == set1){
                                    struct cacheline* setnode1 = node1;
                                    while(setnode1->setnext != NULL){
                                        if(setnode1->tag == tag1){
                                            return node;
                                        }
                                        setnode1 = setnode1->setnext;
                                    }
                                    if(setnode1->tag == tag1){
                                        return node;
                                    }
                                    if(node1->indexlistsize < asn){
                                        struct cacheline* prefetchnode = (struct cacheline*)malloc(sizeof(struct cacheline));
                                        prefetchnode->setnext = NULL;
                                        prefetchnode->setindex = set1;
                                        prefetchnode->tag = tag1;
                                        setnode1->setnext = prefetchnode;
                                        read++;
                                        node1->indexlistsize++;
                                        return setnode1;
                                    }
                                    else{
                                        struct cacheline* hold = node1;
                                        while (hold->setnext != NULL){
                                            hold->tag = hold->setnext->tag;
                                            hold = hold->setnext;
                                        }
                                        hold->tag = tag1;
                                        read++;
                                        return hold;
                                    }

                                }
                                node1 = node1->next;
                            }////////////////////////////////////////
                            //checking last node1 where ->next == NULL
                            if(node1->setindex == set1){
                                struct cacheline* setnode1 = node1;
                                while(setnode1->setnext != NULL){
                                    if(setnode1->tag == tag1){
                                        return node;
                                    }
                                    setnode1 = setnode1->setnext;
                                }
                                if(setnode1->tag == tag1){
                                    return node;
                                }
                                if(node1->indexlistsize < asn){
                                    struct cacheline* prefetchnode = (struct cacheline*)malloc(sizeof(struct cacheline));
                                    prefetchnode->setnext = NULL;
                                    prefetchnode->setindex = set1;
                                    prefetchnode->tag = tag1;
                                    setnode1->setnext = prefetchnode;
                                    read++;
                                    node1->indexlistsize++;
                                    return setnode1;
                                }
                                else{
                                    struct cacheline* hold = node1;
                                    while (hold->setnext != NULL){
                                        hold->tag = hold->setnext->tag;
                                        hold = hold->setnext;
                                    }
                                    hold->tag = tag1;
                                    read++;
                                    return hold;
                                }
                            }
                            else{
                                struct cacheline* prefetchnode = (struct cacheline*)malloc(sizeof(struct cacheline));
                                prefetchnode->indexlistsize = 1;
                                prefetchnode->next = NULL;
                                prefetchnode->setnext = NULL;
                                prefetchnode->setindex = set1;
                                prefetchnode->tag = tag1;
                                node1->next = prefetchnode;
                                read++;
                            }
                        }
    }
    else if(prefetch == 1){
                    // printf("\nLine number: %d\n", __LINE__);
        struct cacheline* node1 = head;
        while(node1->next != NULL){
            if(node1->setindex == set1){
                if(node1->tag == tag1){
                                // read++;
                    return node;
                }
                node1->tag = tag1;
                read++;
                return node;
            }
            node1 = node1->next;
         }
        if(node1->setindex == set1){
                if(node1->tag == tag1){
                                // read++;
                    return node;
                }
                node1->tag = tag1;
                read++;
                return node;
        }
        else{
            struct cacheline* prefetchnode = (struct cacheline*)malloc(sizeof(struct cacheline));
            prefetchnode->indexlistsize = 1;
            prefetchnode->next = NULL;
            prefetchnode->setindex = set1;
            prefetchnode->setnext = NULL;
            prefetchnode->tag = tag1;
            node1->next = prefetchnode;
            read++;
            queuesize++;
        }
    }
    return node;
}
struct cacheline* fullassoc(struct cacheline* head, unsigned long long int tag, char operator, int prefetch, unsigned long long int tag1, unsigned long long int fully){
    if(operator == 'W'){
        write++;
    }
    struct cacheline* node = head;
    while(node->next != NULL){
        if(node->tag == tag){
            hit++;
            return head;
        }
        node = node->next;
    }
    if(node->tag == tag){
        hit++;
        return head;
    }
    struct cacheline* newnode = (struct cacheline*)malloc(sizeof(struct cacheline));
    newnode->tag = tag;
    newnode->next = NULL;
    node->next = newnode;
    miss++;
    read++;
    if(queuesize < fully){
        queuesize++;
          if(prefetch == 1){
            struct cacheline* nodanuova = head;
            while(nodanuova->next != NULL){
                if(nodanuova->tag == tag1){
                    return head;
                }
                nodanuova = nodanuova->next;
            }
            if(nodanuova->tag == tag1){
                    return head;
            }
            read++;
            struct cacheline* mewnode = (struct cacheline*)malloc(sizeof(struct cacheline));
            mewnode->next = NULL;
            mewnode->tag = tag1;
            nodanuova->next = mewnode;
            if(queuesize < fully){
                queuesize++;
                return head;
            }
            struct cacheline* hola = head->next;
            free(head);
            head = hola; 
          }
          return head;
    }
    struct cacheline* hola = head->next;
    free(head);
    head = hola;
    if(prefetch == 1){
            struct cacheline* nodanuova = head;
            while(nodanuova->next != NULL){
                if(nodanuova->tag == tag1){
                    return head;
                }
                nodanuova = nodanuova->next;
            }
            if(nodanuova->tag == tag1){
                    return head;
            }
            read++;
            struct cacheline* mewnode = (struct cacheline*)malloc(sizeof(struct cacheline));
            mewnode->next = NULL;
            mewnode->tag = tag1;
            nodanuova->next = mewnode;
            struct cacheline* hola = head->next;
            free(head);
            head = hola;
            return head;
          }
    return head;
}
//findnode
int checkpoweroftwo(unsigned long long int n){
    if(n == 0 || n == 1)
        return 1;
    while(n > 0){
        if(n%2 == 0){
            n /= 2;
            if(n == 1)
                return 1;
        }
        else
            return 0;
    }
    return 0;
}
int logbasetwo(unsigned long long int n){
    int r = 0;
    while(n != 1){
        n /= 2;
        r++;
    }
    return r;
}
//////////////////////////////////change prefetch assoc:n to from tag to tag1 set set1
int main(int argc, char **argv){
    hit = 0;
    miss = 0;
    read = 0;
    write = 0;
    cachesize = atoi(argv[1]);
    blocksize = atoi(argv[4]);
    unsigned long long int fully;
    
    // printf("%d is numofline", numoflines);
    queuesize = 0;
    int prefething = 0;
    strncpy(assocn, argv[2], sizeof(assocn)-1);
    // printf("\n%s\n", assocn);
    if(strcmp(assocn, "assoc:") == 0){
        // printf("\nis assoc:n\n");
        char x[16];
        int j = 0;
        for(int i = 6; i < strlen(argv[2]); i++){
            x[j] = argv[2][i];
            j++;
        }
        asn = atoi(x);
        if(checkpoweroftwo(asn) == 0)
            return(EXIT_FAILURE);
        // printf("\nrecorded number: %lu\n", asn);
    }
    else{
        strcpy(assocn, argv[2]);
    }
    if(strcmp(assocn, "assoc:") == 0){
        numoflines = cachesize / (blocksize * asn);
    }
    else{
         numoflines = cachesize / blocksize;
    }
    indexsize = logbasetwo(numoflines);  
    if(checkpoweroftwo(blocksize) == 0)
        return(EXIT_FAILURE);
    if(checkpoweroftwo(cachesize) == 0)
        return(EXIT_FAILURE);
    blockoffset = logbasetwo(blocksize);
    int fields;
    char operator;
    FILE* fp = fopen(argv[5], "r");
    fields = fscanf(fp, "%*x: %c %llx", &operator, &address);
    unsigned long long int tag;
    unsigned long long int set;
    //C = cachesize S = number of sets, E = lines per set, B = block size          E = C/(SB)     C = SEB
    fully = cachesize/(blocksize*1);
    struct cacheline* head = NULL;
    if(strcmp(assocn, "assoc") == 0){
        tag = address >> blockoffset;
        struct cacheline* firsthead = (struct cacheline*)malloc(sizeof(struct cacheline));
        firsthead->next = NULL;
        firsthead->tag = tag;
        head = firsthead;
    }
    else{
        set = (address >> blockoffset) & ((1 << indexsize) - 1);
        tag = address >> (blockoffset + indexsize);
        struct cacheline* firsthead = (struct cacheline*)malloc(sizeof(struct cacheline));
        firsthead->next = NULL;
        firsthead->setnext = NULL;
        firsthead->setindex = set;
        firsthead->tag = tag;
        firsthead->indexlistsize = 1;
        head = firsthead;
    }
    read++;
    miss++;
    if(operator == 'W')
        write++;
    queuesize++;
    int missfirst = 0;
    while(fields > 1){
        if(missfirst == 1){
            if(strcmp(assocn, "assoc") == 0){
                
                tag = address >> blockoffset;
                head = fullassoc(head, tag, operator, 0, 0, fully);
            }
            else{
            // if(strcmp(assocn, "direct") == 0){
                set = (address >> blockoffset) & ((1 << indexsize) - 1);
                tag = address >> (blockoffset + indexsize);
                findnode(head, set, tag, operator, prefething, 0, 0, 0);
            }
            // }
            // else if(strcmp(assocn, "assoc:") == 0){
            //     set = (address >> blockoffset) & ((1 << indexsize) - 1);
            //     tag = address >> (blockoffset + indexsize);

            // }
        }
        else 
            missfirst = 1;
        // printf("\nLine number: %d\n", __LINE__);
        fields = fscanf(fp, "%*x: %c %llx", &operator, &address);
    //   printf("\n%d", queuesize);
        // printf"\nHEAD VALUES: %lx %x", head->setindex, head->tag);   
    }
    printf("Prefetch 0\nMemory reads: %d\nMemory writes: %d\nCache hits: %d\nCache misses: %d", read, write, hit, miss);
    //
    read = 0;
    write = 0;
    hit = 0;
    miss = 0;
    queuesize = 0;
    fp = fopen(argv[5], "r");
    fields = fscanf(fp, "%*x: %c %llx", &operator, &address);
      if(strcmp(assocn, "assoc:") == 0){
        while (head != NULL){
            head = dequeue(head);
        }
    }
    else{
        while(head != NULL){
            struct cacheline* node = head;
            head = head->next;
            free(node);
        }
    }
    if(strcmp(assocn, "assoc") == 0){
        tag = address >> blockoffset;
        struct cacheline* secondhead = (struct cacheline*)malloc(sizeof(struct cacheline));
        secondhead->next = NULL;
        secondhead->tag = tag;
        head = secondhead;
    }
    else{
        set = (address >> blockoffset) & ((1 << indexsize) - 1);
        tag = address >> (blockoffset + indexsize);   
        struct cacheline* secondhead = (struct cacheline*) malloc (sizeof(struct cacheline));
        secondhead->next = NULL;
        secondhead->setindex = set;
        secondhead->tag = tag;
        secondhead->setnext = NULL;
        secondhead->indexlistsize = 1;
        head = secondhead;
    }
    read++;
    miss++;
    prefething = 1;
    if(operator == 'W')
        write++;
    queuesize++;
    missfirst = 0;
    // printf("\nhead tag and set: %lx %lx\n", tag, set);
    unsigned long long int prefetchaddress = address >> blockoffset;
    prefetchaddress++;
    unsigned long long int tag1 = prefetchaddress >> indexsize;
    unsigned long long int set1 = (prefetchaddress) & ((1 << indexsize) - 1); 
    // printf("head set and set1: %d, %d", head->setindex, set1); 
    // if(head->next != NULL){
    //     printf("mierdalklkjlkjlkjlkjlkj");
    // } 
    if(strcmp(assocn, "assoc") == 0){
        if(fully > 1){
            struct cacheline* prefetchnode = (struct cacheline*)malloc(sizeof(struct cacheline));
            prefetchnode->next = NULL;
            unsigned long long int prefetchaddress = address >> blockoffset;
            prefetchaddress++;
            prefetchnode->tag = prefetchaddress;
            head->next = prefetchnode;
            read++;
        }
    }
    else if(head->setindex == set1){
        if(head->tag == tag1){  
            // printf("\n== == \n") ;
        }
        else{
            struct cacheline* prefetchnode = (struct cacheline*)malloc(sizeof(struct cacheline));
            prefetchnode->next = NULL;
            prefetchnode->setnext = NULL;
            prefetchnode->setindex = set1;
            prefetchnode->tag = tag1;
            if(strcmp(assocn, "assoc:") == 0){
                if(asn > 1){
                    head->setnext = prefetchnode;
                    head->indexlistsize++;
                }
                else{
                    dequeue(head);
                    head = prefetchnode;
                }
            }                                               
            else
                head->next = prefetchnode;
            read++;
            queuesize++;
        }

    }
    else{
        struct cacheline* prefetchnode = (struct cacheline*)malloc(sizeof(struct cacheline));
        prefetchnode->next = NULL; 
        prefetchnode->setnext = NULL;
        prefetchnode->setindex = set1;
        prefetchnode->tag = tag1;
        prefetchnode->indexlistsize = 1;
        head->next = prefetchnode;
        // printf("hola");
        read++;
        // queuesize++;
    }
    queuesize++;
    // printf("set1 otra vez: %d", set1);
    // printf("\n\n head and next tag and sets: %lx %lx %lx %lx\n", head->tag, head->setnext, head->next->tag, head->next->setindex);
    // printf("adres and prefetch adres: %lx, %lx", address, prefetchaddress);
    // printf("\n\n");
    // printf("tag, set, tag1, set1 and address: %lx, %lx, %lx, %lx, %lld.\n", tag, set, tag1, set1, address);
    while(fields > 1){
        // printf("\n\n%lx", prefetchaddress);
        // printf("\n%d", read);
        if(missfirst == 1){
            if(strcmp(assocn, "assoc") == 0){
                unsigned long long int tag1 = address >> blockoffset;
                tag1++;
                tag = address >> blockoffset;
                head = fullassoc(head, tag, operator, 1, tag1, fully);
            }
            else{
                set = (address >> blockoffset) & ((1 << indexsize) - 1);
                tag = address >> (blockoffset + indexsize);                
                prefetchaddress = address >> blockoffset;
                prefetchaddress++;

                unsigned long long int tag1 = prefetchaddress >> indexsize;
                unsigned long long int set1 = (prefetchaddress) & ((1 << indexsize) - 1);
                // printf("tag, set, tag1, set1 and address: %lx, %lx, %lx, %lx, %lld.\n", tag, set, tag1, set1, address);
                findnode(head, set, tag, operator, prefething, prefetchaddress, tag1, set1);
            } 
           // else if(assocn)
        }
        else 
            missfirst = 1;
        // printf("\n\n\nReads: %d  Writes: %d  Hits: %d  Misses: %d  Address: %lx", read, write, hit, miss, prefetchaddress);
        fields = fscanf(fp, "%*x: %c %llx", &operator, &address);
            // printf"\nHEAD VALUES: %lx %x", head->setindex, head->tag);   
    }
printf("\nPrefetch 1\nMemory reads: %d\nMemory writes: %d\nCache hits: %d\nCache misses: %d", read, write, hit, miss);
//    printf("\nblock offset: %d", blockoffset);
    // printf("\n\n\nlines counter: %d", z);

    // struct cacheline* cabeza = NULL;
    // struct cacheline* hola = (struct cacheline*)malloc(sizeof(struct cacheline));
    // hola->next = NULL;
    // hola->setindex = 10;
    // hola->tag = 40;
    // cabeza = hola;

    // enqueue(cabeza, (unsigned long long int) 20, (unsigned long long int) 0 );
    // enqueue(cabeza, (unsigned long long int) 30, (unsigned long long int) 0 );
    // enqueue(cabeza, (unsigned long long int) 40, (unsigned long long int) 0 );
    // enqueue(cabeza, (unsigned long long int) 50, (unsigned long long int) 0 );
    // enqueue(cabeza, (unsigned long long int) 60, (unsigned long long int) 0 );
    // enqueue(cabeza, (unsigned long long int) 70, (unsigned long long int) 0 );
    // struct cacheline* nodo = cabeza;
    // while(nodo != NULL){
    //     printf("%ld\n", nodo->setindex);
    //     nodo = nodo->next;
    // }printf("\n\n\n");
    // cabeza = dequeue(cabeza);
    // // cabeza = dequeue(cabeza);
    // // cabeza = dequeue(cabeza);
    // nodo = cabeza;
    // printf("\n");
    // while(nodo != NULL){
    //     printf("hola %ld\n", nodo->setindex);
    //     nodo = nodo->next;
    // }
    if(strcmp(assocn, "assoc:") == 0){
        while (head != NULL){
            head = dequeue(head);
        }
    }
    else{
        while(head != NULL){
            struct cacheline* node = head;
            head = head->next;
            free(node);
        }
    }
    return(EXIT_SUCCESS);
}