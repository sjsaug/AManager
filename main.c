#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NAME_LEN 64
#define LINE_LEN 256

#define TABLE_SIZE 10

typedef struct node{
    char raw_line[LINE_LEN];
    char program_name[NAME_LEN];
    int enable_hour, enable_min;
    int disable_hour, disable_min;
    struct node *next;
}NODE;

typedef struct HashTable{
    NODE *buckets[TABLE_SIZE];
}HASHTABLE;

unsigned int hash(const char *key) {
    unsigned long int value = 5381;
    int i = 0;
    while (key[i] != '\0') {
        value = ((value << 5) + value) + key[i]; // value * 33 + key[i]
        i++;
    }
    return value % TABLE_SIZE;
}

HASHTABLE* create_table(){
    HASHTABLE *ht = malloc(sizeof(HASHTABLE));
    for (int i=0; i < TABLE_SIZE; i++){
        ht->buckets[i]=NULL;
    }
    return ht;
}

void update_table(HASHTABLE *ht, char program_name[NAME_LEN], char raw_line[LINE_LEN], int enable_hour, int enable_min, int disable_hour, int disable_min){
    unsigned int slot = hash(program_name);
    NODE *current = ht->buckets[slot];
    while (current != NULL){
        if (strcmp(current->program_name, program_name) == 0){
            strcpy(current->raw_line, raw_line);
            strcpy(current->program_name, program_name);
            current->disable_hour=disable_hour;
            current->disable_min=disable_min;
            current->enable_hour=enable_hour;
            current->enable_min=enable_min;
            return;
        }
        current=current->next;
    }

    NODE *new_node = malloc(sizeof(NODE));
    strcpy(new_node->program_name, program_name);
    strcpy(new_node->raw_line, raw_line);
    new_node->disable_hour=disable_hour;
    new_node->disable_min=disable_min;
    new_node->enable_hour=enable_hour;
    new_node->enable_min=enable_min;

    new_node->next = ht->buckets[slot];
    ht->buckets[slot]=new_node; //this last bit is essentially prepending a new node to the LL at the key's hash index
}

void free_table(HASHTABLE *ht){
    //traverse table
    for (int i=0;i<TABLE_SIZE;i++){
        NODE *current = ht->buckets[i];
        while(current!=NULL){
            NODE *temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(ht);
}

void enum_procs(){

}

int main(){
    FILE *programs = fopen("programs.txt", "r");
    if (programs == NULL){
        printf("Error opening programs.txt");
        return EXIT_FAILURE;
    }
    char line[256], name[NAME_LEN], enable_str[16], disable_str[16];
    int en_h, en_m, dis_h, dis_m;
    HASHTABLE *ht = create_table();
    while (fgets(line, sizeof(line), programs) != NULL){
        sscanf(line, "%[^=]=%[^;];%s", name, enable_str, disable_str);
        sscanf(enable_str,  "%dh%dm", &en_h, &en_m);
        sscanf(disable_str, "%dh%dm", &dis_h, &dis_m);
        update_table(ht, name, line, en_h, en_m, dis_h, dis_m);
    }
    fclose(programs);
}