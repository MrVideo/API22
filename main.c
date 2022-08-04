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

// Empty word list
typedef struct node *list;
list wordlist = NULL;

// Function signatures

// List functions
list list_add(list l, char *string);
list search(list l, char *string);

// Support functions
void empty_buffer(char *b);

// Game functions
void filtered_print(list l);
void add_new(list l);

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

void empty_buffer(char *b) {
    for (int i = 0; i < BUFSIZE; ++i)
        b[i] = '\0';
}

void filtered_print(list l) {

}

void add_new(list l) {

}