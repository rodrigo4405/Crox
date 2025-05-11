#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int STRING_SIZE_LIMIT = 50;

typedef struct node{
    char* text;
    struct node* next;
    struct node* prev;
} node;

enum OPTION {Insert = 1, Find, Delete, Quit};

int toint(char c);
node* insert(node* HEAD);
void get_search_input(char* buffer);
node* find(node* HEAD);
void delete(node* HEAD);

void clear(node* HEAD);

int main(void) {
    node HEAD;
    HEAD.next = NULL;
    HEAD.prev = NULL;
    HEAD.text = "";

    node* pHead = &HEAD;

    printf("DOUBLE LINKED LIST\n");


    while (1) {
        char optChar[2];
        int opt;
        do {
            printf("\nChoose an option.\n[1] Insert  [2]  Find  [3]  Delete  [4]  Quit : ");
            scanf("%s", &optChar[0]);
            opt = toint(optChar[0]);

            if (opt >= Insert && opt <= Quit) { break; }
        
            printf("Invalid option.\n");
        } while(1);
        
        switch (opt) {
            case Insert:
                insert(pHead);
                break;
            case Find:
                find(pHead);
                break;
            case Delete:
                delete(pHead);
                break;
            case Quit:
                goto quit;
                break;
        }
    }


    quit:
        clear(pHead);
        printf("Quitting...\n");
        return 0;
}   

int toint(char c) { return (int) c - 48; }

// Insert: inserts the data and returns the pointer to the node
node* insert(node* HEAD) {
    printf("\nInsert string: ");

    char* newStr = malloc(sizeof(char) * STRING_SIZE_LIMIT + 1);

    if (!newStr) { return HEAD; }


    fflush(stdin);
    if (!fgets(newStr, STRING_SIZE_LIMIT, stdin)) {
        free(newStr); 
        return HEAD;
    }

    newStr[strcspn(newStr, "\n")] = '\0';

    node* newNode = malloc(sizeof(node));

    if (!newNode) {
        free(newStr);
        return HEAD;
    }

    newNode->text = newStr;
    newNode->next = NULL;
    newNode->prev = NULL;

    node* current = HEAD;

    while (current->next) {
        if (strcmp(current->text, current->next->text) < 0) {
            break;
        }

        if (!current->next) { break; }

        current = current->next;
    }

    current->next = newNode;
    current->next->prev = current;

    return current->next;
}

void get_search_input(char* buffer) {
    printf("Enter search string: ");
    fflush(stdin);
    if (fgets(buffer, STRING_SIZE_LIMIT, stdin)) {
        buffer[strcspn(buffer, "\n")] = '\0';  
    }
}

node* find(node* HEAD) {
    char lookupString[STRING_SIZE_LIMIT + 1];
    get_search_input(lookupString);
    
    node* current = HEAD->next;
    while (current) {
        int cmp = strcmp(current->text, lookupString);
        if (cmp > 0) break;  
        if (cmp == 0) {
            printf("Found string \"%s\".\n", lookupString);
            return current;
        }
        current = current->next;
    }
    
    printf("Could not find string \"%s\".\n", lookupString);
    return HEAD;
}

void delete(node* HEAD) {
    char strToDelete[STRING_SIZE_LIMIT + 1];
    get_search_input(strToDelete);
    
    node* current = HEAD->next;
    node* toDelete = NULL;
    
    while (current) {
        int cmp = strcmp(current->text, strToDelete);
        if (cmp > 0) break;
        if (cmp == 0) {
            toDelete = current;
            break;
        }
        current = current->next;
    }
    
    if (!toDelete) {
        printf("Could not delete string \"%s\".\n", strToDelete);
        return;
    }
    
    toDelete->prev->next = toDelete->next;
    if (toDelete->next) {
        toDelete->next->prev = toDelete->prev;
    }
    
    free(toDelete->text);
    free(toDelete);
    printf("Deleted \"%s\" successfully.\n", strToDelete);
}

void clear(node* HEAD) {
    node* temp;

    while (HEAD) {
        temp = HEAD;
        HEAD = temp->next;
        free(temp->text);
        free(temp);
    }
}