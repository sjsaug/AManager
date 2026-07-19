#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NAME_LEN 64
#define LINE_LEN 256

typedef struct node{
    char raw_line[LINE_LEN];
    char program_name[NAME_LEN];
    int enable_hour, enable_min;
    int disable_hour, disable_min;
    struct node *next;
}NODE;

void insertAtEnd(NODE **head, char raw_line[LINE_LEN], 
    char program_name[NAME_LEN], int enable_hour, 
    int enable_min, int disable_hour, int disable_min){

    NODE* newNode = (NODE*)malloc(sizeof(NODE));
    if (newNode == NULL){return;}

    strcpy(newNode->raw_line, raw_line);
    strcpy(newNode->program_name, program_name);
    newNode->disable_hour=disable_hour;
    newNode->disable_min=disable_min;
    newNode->enable_hour=enable_hour;
    newNode->enable_min=enable_min;
    newNode->next=NULL;

    if (*head == NULL){*head=newNode; return;}

    NODE *current = *head;
    while (current->next != NULL){
        current=current->next;
    }
    current->next=newNode;
}

void printLL(NODE *head){
    NODE *current = head;
    while (current != NULL){
        printf("%s : Blocked from %02d:%02d until %02d:%02d\n", 
       current->program_name, current->disable_hour, current->disable_min, current->enable_hour, current->enable_min);
       current=current->next;
    }
}

void freeLL(NODE *head){
    NODE *current = head;
    while (current != NULL){
        NODE *next = current->next;
        free(current);
        current = next;
    }
}

int main(){
    FILE *programs = fopen("programs.txt", "r");
    if (programs == NULL){
        printf("Error opening programs.txt");
        return EXIT_FAILURE;
    }
    char line[256], name[NAME_LEN], enable_str[16], disable_str[16];
    int en_h, en_m, dis_h, dis_m;
    NODE* head = NULL;
    while (fgets(line, sizeof(line), programs) != NULL){
        sscanf(line, "%[^=]=%[^;];%s", name, enable_str, disable_str);
        sscanf(enable_str,  "%dh%dm", &en_h, &en_m);
        sscanf(disable_str, "%dh%dm", &dis_h, &dis_m);
        insertAtEnd(&head, line, name, en_h, en_m, dis_h, dis_m);
    }
    fclose(programs);
    printLL(head);
    
    freeLL(head);
}