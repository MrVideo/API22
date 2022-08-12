#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 128

struct node {
    char *data;
    struct node *next;
};

struct check {
    int ok;
    int tries;
};

typedef struct node *list;

list add_sort(list l, char *new_data);
void print(list l);
list delete(list l, char *to_delete);
list search(list l, char *query);
void empty_buffer(char *b);
list add_new_words(list l, int len);
struct check word_check(char *password, char *buffer, char *guide, int word_length, struct check status);

int main() {
    // Variable declarations
    char *buffer = calloc(BUFSIZE, sizeof(char));
    char *password = calloc(BUFSIZE, sizeof(char));
    char *guide = calloc(BUFSIZE, sizeof(char));
    int word_length, cmd_chk = 0, restart = 0;
    struct check status;
    list wordlist = NULL;

    // Check struct initialisation
    status.ok = 0;
    status.tries = 0;

    // -- Game setup --
    // This is only run once.

    // Saves the word length decided by the user.
    word_length = (int)strtol(fgets(buffer, BUFSIZE, stdin), NULL, 10);

    // Starts saving new words to the main word list.
    if(word_length > 0) {
        while(!cmd_chk) {
            if(scanf("%s", buffer) != 0) {
                if(strcmp(buffer, "+nuova_partita") == 0)
                    cmd_chk = 1;
                else
                    wordlist = add_sort(wordlist, buffer);
            }
        }
    }

    // If the word has been input before, the password is saved.
    empty_buffer(buffer);
    if(scanf("%s", buffer) != 0) {
        if(search(wordlist, buffer) != NULL)
            password = strdup(buffer);
    }

    // Asks the user for the number of tries in the game.
    if(scanf("%d", &status.tries) != 0) {

        // -- Main game loop --
        // This is run until EOF is read from stdin.

        while(1) {
            // Resets the restart variable for future restarts.
            restart = 0;
            // Checks for EOF to terminate program
            if(scanf("%s", buffer) == EOF)
                break;
            else {
                // If the "+inserisci_inizio" command is passed, new words are added to the wordlist through
                // the "add_new_words" function.
                if(strcmp(buffer, "+inserisci_inizio") == 0) {
                    wordlist = add_new_words(wordlist, word_length);
                    empty_buffer(buffer);
                    continue;
                }
                // If the word is not of the correct length or it does not appear in the wordlist,
                // no tries are consumed and the user can retry.
                else if(strlen(buffer) != word_length) {
                    printf("not_exists");
                    empty_buffer(buffer);
                    continue;
                }
                // If none of the checks before this get triggered, the word is checked for correctness.
                else if(!status.ok && status.tries > 0) {
                        status = word_check(password, buffer, guide, word_length, status);
                        empty_buffer(buffer);
                        if(!status.ok && status.tries >= 0) {
                            for (int i = 0; i < word_length; ++i)
                                printf("%c", guide[i]);
                            printf("\n");
                            if(status.tries != 0) continue;
                        }
                }
            }

            // Prints the outcome and resets the game.
            if(status.ok || status.tries == 0) {
                if(status.ok) printf("ok\n");
                if(!status.tries) printf("ko\n");
                status.ok = 0;
                empty_buffer(buffer);
                // The user can manage settings while in this loop, until they decide to start a new game.
                while(!restart) {
                    if(scanf("%s", buffer) != 0) {
                        if(strcmp(buffer, "+inserisci_inizio") == 0)
                            wordlist = add_new_words(wordlist, word_length);
                        if(strcmp(buffer, "+nuova_partita") == 0) {
                            empty_buffer(buffer);
                            if(scanf("%s", buffer) != 0)
                                if(search(wordlist, buffer) != NULL)
                                    password = strdup(buffer);
                            if(scanf("%d", &status.tries) != 0)
                                restart = 1;
                        }
                    }
                }
            }
        }
    }

    // Terminates the program
    return 0;
}

list add_sort(list l, char *new_data) {
    list prev = NULL, curr = l;

    //If the list is empty, add the element on top.
    if(l == NULL) {
        list tmp = malloc(sizeof(struct node));
        tmp -> data = strdup(new_data);
        tmp -> next = l;
        return tmp;
    } else {
        //If the list is not empty
        while(curr != NULL) {
            //If new_data comes before the current node's data then add it before curr
            if(strcmp(new_data, curr -> data) < 0) {
                list tmp = malloc(sizeof(struct node));
                tmp -> data = strdup(new_data);
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
                tmp -> data = strdup(new_data);
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
            tmp -> data = strdup(new_data);
            tmp -> next = curr;
            prev -> next = tmp;
            return l;
        }
    }
    //This gets executed in case of an error
    return NULL;
}

void print(list l) {
    if (l == NULL)
        printf("END\n");
    else {
        printf("%s -> ", l -> data);
        print(l -> next);
    }
}

list delete(list l, char *to_delete) {
    if(l != NULL) {
        if(strcmp(l -> data, to_delete) == 0) {
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

void empty_buffer(char *b) {
    for(int i = 0; i < BUFSIZE; ++i)
        b[i] = '\0';
}

list add_new_words(list l, int len) {
    char *buffer = calloc(BUFSIZE, sizeof(char));
    int exit = 0;

    while(!exit) {
        if(scanf("%s", buffer) != 0) {
            if(strcmp(buffer, "+inserisci_fine") == 0)
                exit = 1;
            else if(strlen(buffer) == len)
                l = add_sort(l, buffer);
        }
        empty_buffer(buffer);
    }

    return l;
}

struct check word_check(char *password, char *buffer, char *guide, int word_length, struct check status) {
    // Correct word
    if(!strcmp(buffer, password))
        status.ok = 1;
    else {
        // Empty guide buffer
        for(int i = 0; i < word_length; ++i)
            guide[i] = '\0';
        // Wrong word
        --status.tries;
        for(int i = 0; i < word_length; ++i) {
            // Correct character check
            if(password[i] == buffer[i])
                guide[i] = '+';
            // Check for character not in string
            else if(strchr(password, buffer[i]) == NULL)
                guide[i] = '/';
        }
        // Local count variables declaration
        int pwd_count, ok_count, misplaced_count, k = 0, l;
        char tmp_chr;
        // Misplaced character check
        while(k < word_length) {
            pwd_count = 0;
            ok_count = 0;
            misplaced_count = 0;
            if(guide[k] != '+' && guide[k] != '/') {
                tmp_chr = buffer[k];
                for(int i = 0; i < word_length; ++i) {
                    if(buffer[i] == tmp_chr && password[i] != tmp_chr && guide[i] != '/')
                        ++misplaced_count;
                    if(buffer[i] == tmp_chr && guide[i] == '+')
                        ++ok_count;
                    if(password[i] == tmp_chr)
                        ++pwd_count;
                }
                l = 0;
                if(ok_count == pwd_count) {
                    while(misplaced_count > 0 && l < word_length) {
                        if(buffer[l] == tmp_chr && guide[l] != '+' && guide[l] != '/') {
                            guide[l] = '/';
                            --misplaced_count;
                        }
                        ++l;
                    }
                } else if(pwd_count > ok_count) {
                    while(misplaced_count > 0 && l < word_length) {
                        if(buffer[l] == tmp_chr && guide[l] != '+' && guide[l] != '/') {
                            if(pwd_count > ok_count) {
                                guide[l] = '|';
                                --misplaced_count;
                                --pwd_count;
                            } else if(pwd_count == ok_count) {
                                guide[l] = '/';
                                --misplaced_count;
                            }
                        }
                        ++l;
                    }
                }
            }
            ++k;
        }
    }
    return status;
}