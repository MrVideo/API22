#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// This is the start of something awful

// Global variables
int word_length, tries;
char buffer[1024];      // This is the string buffer to check for commands passed from the user
int command_chk = 0;    // This value checks whether a command was passed to the program and breaks different loops
char password[256];     // This is the word that the player has to guess

// Node
struct node {
    char *word;
    int valid;
    struct node *next;
};

// Empty word list
typedef struct node *list;
list wordlist = NULL;

// Function signatures

// List functions
list list_add(list l, char *string);
list search(list l, char *string);

int main() {
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
    if(scanf("%s", password)) {
        if(search(wordlist, password) == NULL)
            printf("La parola da indovinare dev'essere inclusa tra quelle ammissibili.\n");
    }
}

list list_add(list l, char *string) {
    if(l == NULL) {
        struct node *tmp = malloc(sizeof(struct node));
        tmp -> word = string;
        tmp -> valid = 1;
        tmp -> next = l;
        return tmp;
    }
    else {
        l -> next = list_add(l -> next, string);
        return l;
    }
}

list search(list l, char *string) {
    if(l == NULL || l -> word == string)
        return l;
    else
        return search(l -> next, string);
}