#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 1024

// This is the start of something awful.

// Global variables
int word_length, tries;
char buffer[BUFSIZE];   // This is the string buffer to check for commands and guesses passed by the user.
int command_chk = 0;    // This value checks whether a command was passed to the program and breaks different loops.
char password[256];     // This is the word that the player has to guess.
int win = 0;            // This variable lets the game end when the player guesses the word.

// Node
struct node {
    char *word;
    int valid;
    struct node *next;
};

// List definition
typedef struct node *list;

// Empty word list
typedef struct node *list;
list wordlist = NULL;

// Filtered word list
list filtered = NULL;

// Function signatures

// List management functions

// TODO Check for unused functions
int size(list l);
list add(list l, char *new_data);
list delete(list l, char *to_delete);
list search(list l, char *query);
void print(list l);
list add_sort(list l, char *new_data);

// Support functions
// TODO Find a better alternative
void empty_buffer(char *b);

// Game functions
// TODO Implement these functions
void filtered_print(list l);
void add_new(list l);

int main() {
    // TODO Rewrite the main function

    // Initialisation
    system("clear");
    printf("Progetto Finale 2022 Politecnico di Milano - Mario Merlo\n\n");

    // Word length input
    printf("Inserisci la lunghezza delle parole: ");
    if(scanf("%d", &word_length)) {
        // Endless loop for correct words input
        printf("Inserisci le parole ammissibili di seguito\n");
        // Checks whether the user inputs "+nuova_partita" to start a new game
        while(!command_chk) {
            scanf("%s", buffer);
            if(!strcmp(buffer, "+nuova_partita"))
                command_chk = 1;
            // Otherwise adds the word to the word list
            wordlist = list_add(wordlist, buffer);
        }
    }

    // Game start
    printf("Inizio nuova partita\n");
    printf("Inserisci la parola da indovinare: ");
    // Saves the password to guess
    if(scanf("%s", password)) {
        // Asks user for tries
        printf("\nInserisci il numero dei tentativi: ");
        if(scanf("%d", &tries)) {
            // The player starts guessing
            while(tries && !win) {
                empty_buffer(buffer);
                if(scanf("%s", buffer)) {
                    if(!strcmp(buffer, password))
                        win = 1;
                    else if(!strcmp(buffer, "+stampa_filtrate"))
                        filtered_print(wordlist);
                    else if(!strcmp(buffer, "+inserisci_inizio"))
                        add_new(wordlist);
                }
            }
        }
    }
}

int size(list l) {
    if(l == NULL)
        return 0;
    else
        return 1 + size(l -> next);
}

list add(list l, char *new_data) {
    if(l == NULL) {
        struct node *tmp = malloc(sizeof(struct node));
        tmp -> data = new_data;
        tmp -> next = l;
        return tmp;
    } else {
        l -> next = add(l -> next, new_data);
        return l;
    }
}

list delete(list l, char *to_delete) {
    if(l != NULL) {
        if(l -> data == to_delete) {
            list tmp = l;
            l = l -> next;
            free(tmp);
        } else l -> next = delete(l -> next, to_delete);
    }
    return l;
}

list search(list l, char *query) {
    if(l == NULL || !strcmp(l -> data, query))
        return l;
    else
        return search(l -> next, query);
}

void print(list l) {
    if (l == NULL)
        printf("END\n");
    else {
        printf("%s -> ", l -> data);
        print(l -> next);
    }
}

list add_sort(list l, char *new_data) {
    list prev = NULL, curr = l;

    //If the list is empty, add the element on top.
    if(l == NULL) {
        list tmp = malloc(sizeof(struct node));
        tmp -> data = new_data;
        tmp -> next = l;
        return tmp;
    } else {
        //If the list is not empty
        while(curr != NULL) {
            //If new_data comes before the current node's data then add it before curr
            if(strcmp(new_data, curr -> data) < 0) {
                list tmp = malloc(sizeof(struct node));
                tmp -> data = new_data;
                tmp -> next = curr;
                //If the item goes in the first position, do not try to connect the previous node to it
                //Otherwise, do it
                if(prev != NULL) {
                    prev -> next = tmp;
                    return l;
                } else return tmp;
                //If the strings are the same, add it in the next node (it keeps the algorithm stable)
            } else if(strcmp(new_data, curr -> data) == 0) {
                prev = curr;
                curr = curr -> next;
                list tmp = malloc(sizeof(struct node));
                tmp -> data = new_data;
                tmp -> next = curr;
                prev -> next = tmp;
                return l;
                //Otherwise, just keep searching the list
            } else {
                prev = curr;
                curr = curr -> next;
            }
        }
        //If the list is over and nothing was added to it, the item goes in last
        if(curr == NULL && prev != NULL) {
            list tmp = malloc(sizeof(struct node));
            tmp -> data = new_data;
            tmp -> next = curr;
            prev -> next = tmp;
            return l;
        }
    }
    //This gets executed in case of an error
    return NULL;
}

void empty_buffer(char *b) {
    for (int i = 0; i < BUFSIZE; ++i)
        b[i] = '\0';
}

void filtered_print(list l) {

}

void add_new(list l) {

}