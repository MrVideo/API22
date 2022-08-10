#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 128

struct node {
    char *data;
    struct node *next;
};

typedef struct node *list;

list add_sort(list l, char *new_data);
void print(list l);
list delete(list l, char *to_delete);
list search(list l, char *query);
void empty_buffer(char *b);

int main() {
    // Variable declarations
    char *buffer = calloc(BUFSIZE, sizeof(char));
    char *password = calloc(BUFSIZE, sizeof(char));
    char *guide = calloc(BUFSIZE, sizeof(char));
    int word_length, cmd_chk = 0, tries, ok = 0;
    list wordlist = NULL;

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
    if(scanf("%d", &tries) != 0) {
        do {
            empty_buffer(buffer);
            if(scanf("%s", buffer) != 0) {
                if(strlen(buffer) != word_length)
                    printf("not_exists");
                else {
                    if(!strcmp(buffer, password))
                        ok = 1;
                    else {
                        for(int i = 0; i < word_length; ++i)
                            guide[i] = '\0';
                        --tries;
                        for(int i = 0; i < word_length; ++i) {
                            if(password[i] == buffer[i])
                                guide[i] = '+';
                            else if(strchr(password, buffer[i]) == NULL)
                                guide[i] = '/';
                        }
                        int pwd_count, ok_count, misplaced_count, k = 0, l;
                        char tmp_chr;
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
                }
            }
            if(!ok) {
                for (int i = 0; i < word_length; ++i)
                    printf("%c", guide[i]);
                printf("\n");
            }
        } while(ok == 0 && tries > 0);
    }

    // Prints the outcome of the game.
    if(ok) printf("ok");
    if(!tries) printf("ko");

    // Terminates program.
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