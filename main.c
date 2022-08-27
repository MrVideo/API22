#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 64

struct node {
    char *data;
    char *guide;
    struct node *next;
};

typedef struct node *list;

// List management functions
int list_size(list l);
list add_sort(list l, const char *new_data);
void print(list l);
list delete(list l, char *to_delete);
list search(list l, char *query);
list duplicate(list l);
list add_res(list l, char *word, char *guide);
list destroy(list l);

// Game functions
void add_new_words(list *l, list *f, list *r, int len, int postgame);
void word_check(const char *password, char *buffer, char *guide, int len, int *tp, int *pc, list *f);
void res_check(const char *word, int len, const char *guide, list *f, const char *password);
void occurrences_check(list *f, char c, int count, int len, int strict);
void new_words_check(list *r, list *f, char *new_word, int len);
void char_delete(list *f, char c, int correct);
void pwd_char_count(int *pc, const char *password, int len);
int pwd_char_check(int *pc, char c);

int main() {
    // Variable declarations
    char *buffer = calloc(BUFSIZE, sizeof(char));
    char *password = calloc(BUFSIZE, sizeof(char));
    char *guide = calloc(BUFSIZE, sizeof(char));
    int word_length, cmd_chk = 0, restart, ok = 0, tries = 0;
    int *tp = &tries;
    int pc[64] = {0};
    list wordlist = NULL;
    list filtered = NULL;
    list restrictions = NULL;

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

    // The word list is duplicated to create the filtered word list.
    filtered = duplicate(wordlist);
    // Declaration of pointer to list
    list *lw = &wordlist;
    list *lf = &filtered;
    list *lr = &restrictions;

    // If the word has been input before, the password is saved.
    memset(buffer, 0, sizeof(&buffer));
    if(scanf("%s", buffer) != 0) {
        if(search(wordlist, buffer) != NULL) {
            password = strdup(buffer);
            pwd_char_count(pc, password, word_length);
        }
    }

    // Asks the user for the number of tries in the game.
    if(scanf("%d", &tries) != 0) {

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
                    add_new_words(lw, lf, lr, word_length, 0);
                    memset(buffer, 0, sizeof(&buffer));
                    continue;
                }
                // If "+stampa_filtrate" is called, the filtered list is printed on the screen.
                else if(strcmp(buffer, "+stampa_filtrate") == 0) {
                    print(filtered);
                    memset(buffer, 0, sizeof(&buffer));
                    continue;
                }
                // If the word is not of the correct length, or it does not appear in the wordlist,
                // no tries are consumed and the user can retry.
                else if(strlen(buffer) != word_length || search(wordlist, buffer) == NULL) {
                    printf("not_exists\n");
                    memset(buffer, 0, sizeof(&buffer));
                    continue;
                }
                // If none of the checks before this get triggered, the word is checked for correctness.
                else {
                    if(strcmp(password, buffer) == 0) ok = 1;
                    else if(tries >= 0) {
                        word_check(password, buffer, guide, word_length, tp, pc, lf);
                        filtered = delete(filtered, buffer);
                        restrictions = add_res(restrictions, buffer, guide);
                        res_check(buffer, word_length, guide, lf, password);
                        memset(buffer, 0, sizeof(&buffer));
                        printf("%s\n", guide);
                        printf("%d\n", list_size(filtered));
                        if(tries != 0) continue;
                    }
                }
            }

            // Prints the outcome and resets the game.
            if(ok || tries == 0) {
                if(ok) printf("ok\n");
                if(!tries) printf("ko\n");
                ok = 0;
                memset(buffer, 0, sizeof(&buffer));
                restrictions = destroy(restrictions);
                filtered = destroy(filtered);
                // The user can manage settings while in this loop, until they decide to start a new game.
                while(!restart) {
                    if(scanf("%s", buffer) == EOF) break;
                    else {
                        if(strcmp(buffer, "+inserisci_inizio") == 0)
                            add_new_words(lw, lf, lr, word_length, 1);
                        if(strcmp(buffer, "+nuova_partita") == 0) {
                            filtered = duplicate(wordlist);
                            memset(buffer, 0, sizeof(&buffer));
                            if(scanf("%s", buffer) != 0)
                                if(search(wordlist, buffer) != NULL) {
                                    password = strdup(buffer);
                                    memset(pc, 0, BUFSIZE * sizeof(int));
                                    pwd_char_count(pc, password, word_length);
                                }
                            if(scanf("%d", &tries) != 0)
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

// List function definitions

int list_size(list l) {
    list curr = l;
    int i = 0;
    while(curr != NULL) {
        ++i;
        curr = curr -> next;
    }
    return i;
}

list add_sort(list l, const char *new_data) {
    list prev = NULL, curr = l;

    //If the list is empty, add the element on top.
    if(l == NULL) {
        list tmp = malloc(sizeof(struct node));
        tmp -> data = strdup(new_data);
        tmp -> guide = NULL;
        tmp -> next = l;
        return tmp;
    } else {
        //If the list is not empty
        while(curr != NULL) {
            //If new_data comes before the current node's data then add it before curr
            if(strcmp(new_data, curr -> data) < 0) {
                list tmp = malloc(sizeof(struct node));
                tmp -> data = strdup(new_data);
                tmp -> guide = NULL;
                tmp -> next = curr;
                //If the item goes in the first position, do not try to connect the previous node to it
                //Otherwise, do it
                if(prev != NULL) {
                    prev -> next = tmp;
                    return l;
                } else return tmp;
                //If the strings are the same, add it in the next node (it keeps the algorithm stable)
            } else {
                prev = curr;
                curr = curr -> next;
            }
        }
        //If the list is over and nothing was added to it, the item goes in last
        if(curr == NULL && prev != NULL) {
            list tmp = malloc(sizeof(struct node));
            tmp -> data = strdup(new_data);
            tmp -> guide = NULL;
            tmp -> next = curr;
            prev -> next = tmp;
            return l;
        }
    }
    //This gets executed in case of an error
    return NULL;
}

void print(list l) {
    list curr = l;
    while(curr != NULL) {
        printf("%s\n", curr -> data);
        curr = curr -> next;
    }
}

list delete(list l, char *to_delete) {
    list curr = l, prev = NULL;
    if(curr == NULL)
        return NULL;
    while(curr != NULL) {
        if(strcmp(curr -> data, to_delete) == 0) {
            list tmp = curr;
            curr = curr -> next;
            if(prev != NULL)
                prev -> next = curr;
            free(tmp);
            if(prev != NULL) return l;
            else return curr;
        }
        prev = curr;
        curr = curr -> next;
    }
    return l;
}

list search(list l, char *query) {
    list curr = l;
    while(curr != NULL) {
        if(strcmp(curr -> data, query) == 0)
            return curr;
        curr = curr -> next;
    }
    return NULL;
}

list duplicate(list l) {
    list curr = l, prev = NULL, head = NULL;
    while(curr != NULL) {
        list tmp = malloc(sizeof(struct node));
        tmp -> data = strdup(curr -> data);
        tmp -> next = NULL;
        if(head == NULL)
            head = tmp;
        else
            prev -> next = tmp;
        prev = tmp;
        curr = curr -> next;
    }
    return head;
}

list add_res(list l, char *word, char *guide) {
    list tmp = malloc(sizeof(struct node));
    tmp -> data = strdup(word);
    tmp -> guide = strdup(guide);
    tmp -> next = l;
    return tmp;
}

list destroy(list l) {
    list curr = l, tmp = NULL;
    while(curr != NULL) {
        tmp = curr -> next;
        free(curr);
        curr = tmp;
    }
    return NULL;
}

// Game function definitions

void add_new_words(list *l, list *f, list *r, int len, int postgame) {
    char *buffer = calloc(BUFSIZE, sizeof(char));
    int exit = 0;
    while(!exit) {
        if(scanf("%s", buffer) != 0) {
            if(strcmp(buffer, "+inserisci_fine") == 0)
                exit = 1;
            else if(strlen(buffer) == len) {
                *l = add_sort(*l, buffer);
                if(!postgame)
                    new_words_check(r, f, buffer, len);
            }
        }
        memset(buffer, 0, sizeof(&buffer));
    }
}

void word_check(const char *password, char *buffer, char *guide, int len, int *tp, int *pc, list *f) {
    // Empty guide buffer
    memset(guide, 0, len);
    // Wrong word: one try is subtracted to the counter.
    --*tp;
    int ok = 0; // This variable will count how many characters are correct
    for(int i = 0; i < len; ++i) {
        // If the current character is not in the password
        if(pwd_char_check(pc, buffer[i]) == 0) {
            guide[i] = '/';
            char_delete(f, buffer[i], 0);
        } else {
            // If the current character is in the password in the right place
            if(buffer[i] == password[i]) {
                guide[i] = '+';
                ++ok;
                char_delete(f, buffer[i], 1);
            } else guide[i] = '|';  // If the current character is in the password in the wrong place
        }
    }
    for(int j = 0; j < len; ++j) {
        char *bp = &buffer[j];  // Points to the string starting from the current character
        // If the guide is '|' and there is more than one instance of the character 'buffer[j]'
        if(guide[j] == '|' && strchr(bp + 1, buffer[j]) != NULL) {
            int count = 0, m = 0;       // Counter variables
            int tmpa[len];              // A temporary array that stores the positions in which the character appears
            for(int p = 0; p < len; ++p)
                tmpa[p] = -1;
            char tmp = buffer[j];       // The current character is saved for easier comparisons
            // This loops continues until there are no more instances of the character from before
            for(int k = j; k < len && strchr(bp, tmp) != NULL; ++k) {
                if (buffer[k] == tmp) {
                    ++count;        // Number of 'tmp' characters in the buffer
                    tmpa[m] = k;    // Position of the 'tmp' characters
                    ++m;            // The current index for tmpa
                    ++bp;           // The pointer to the buffer moves forward to check for more instances
                }
            }
            // If there are more 'tmp' characters in the buffer than there are in the password
            if(count > pwd_char_check(pc, tmp)) {
                int l = len - 1;
                int instances = pwd_char_check(pc, tmp);
                while(count > instances && l >= 0) {
                    // If an index is saved in the temporary array and there are still more 'tmp' characters in buffer
                    // than in the password
                    if(tmpa[l] != -1 && count > instances) {
                        if(guide[tmpa[l]] == '+' || guide[tmpa[l]] == '|') {
                            // If one of them is correct, then both counters are subtracted
                            if(guide[tmpa[l]] == '+') {
                                --count;
                                --instances;
                            }
                            // Otherwise, the rightmost '|' becomes '/'
                            else if(guide[tmpa[l]] == '|') {
                                guide[tmpa[l]] = '/';
                                --count;
                            }
                        }
                    }
                    --l;
                }
            }
        }
        ++bp;
    }
}

void res_check(const char *word, int len, const char *guide, list *f, const char *password) {
    list tmp = NULL;
    int break_chk;
    if(guide != NULL) {
        list curr = *f;
        // While the list is not over
        while(curr != NULL) {
            for(int i = 0; i < len; ++i) {
                break_chk = 0;
                // If the character does not appear in the password at all...
                if(strchr(password, word[i]) == NULL && strchr(curr -> data, word[i]) != NULL) {
                    tmp = curr -> next;
                    // ... delete the word from the filtered list.
                    *f = delete(*f, curr -> data);
                    curr = tmp;
                    break_chk = 1;
                    break;
                }
                // If the player guessed a character right but the word in the list does not have that same character
                // in the same position...
                if(guide[i] == '+' && word[i] != curr -> data[i]) {
                    tmp = curr -> next;
                    // ... delete the word from the filtered list.
                    *f = delete(*f, curr -> data);
                    curr = tmp;
                    break_chk = 1;
                    break;
                }
                // If the player guessed a character wrong and a word in the list has that same character
                // in the same position...
                if((guide[i] == '|' || guide[i] == '/') && word[i] == curr -> data[i]) {
                    tmp = curr -> next;
                    // ... delete the word from the filtered list.
                    *f = delete(*f, curr -> data);
                    curr = tmp;
                    break_chk = 1;
                    break;
                }
            }
            if(!break_chk)
                curr = curr -> next;
        }
    }
}

void occurrences_check(list *f, char c, int count, int len, int strict) {
    list curr = *f, tmp = NULL;
    int curr_count;
    while(curr != NULL) {
        curr_count = 0;
        for(int i = 0; i < len; ++i) {
            if(curr -> data[i] == c)
                ++curr_count;
        }
        if(strict) {
            if(curr_count != count) {
                tmp = curr -> next;
                *f = delete(*f, curr -> data);
                curr = tmp;
                continue;
            }
        } else {
            if(curr_count < count) {
                tmp = curr -> next;
                *f = delete(*f, curr -> data);
                curr = tmp;
                continue;
            }
        }
        curr = curr -> next;
    }
}

void new_words_check(list *r, list *f, char *new_word, int len) {
    list curr = *r;
    char tmp_chr;
    int misplaced, wrong, ok, count;
    while(curr != NULL) {
        for (int i = 0; i < len; ++i) {
            if(curr -> guide[i] == '+' && new_word[i] != curr -> data[i])
                return;
            if((curr -> guide[i] == '|' || curr -> guide[i] == '/') && new_word[i] == curr -> data[i])
                return;
            // If a character is correct in the word (even if in the wrong spot)...
            if(curr -> guide[i] == '+' || curr -> guide[i] == '|') {
                // ... mark that character and count its occurrences
                tmp_chr = curr -> data[i];
                misplaced = 0;
                wrong = 0;
                ok = 0;
                count = 0;
                for(int j = 0; j < len; ++j) {
                    if(curr -> data[j] == tmp_chr) {
                        if(curr -> guide[j] == '+')
                            ++ok;
                        if(curr -> guide[j] == '|')
                            ++misplaced;
                        if(curr -> guide[j] == '/')
                            ++wrong;
                    }
                    if(new_word[j] == tmp_chr)
                        ++count;
                }
                // If there are no wrong characters, then the check is not strict (there are *at least* 'count'
                // characters).
                if(!wrong && count < (ok + misplaced))
                    return;
                // If there are wrong characters, then the check is strict (there are *exactly* 'count' characters).
                if(wrong > 0 && count != (ok + misplaced))
                    return;
            }
        }
        curr = curr->next;
    }
    // If the function did not return, then the word can be added to the filtered list
    *f = add_sort(*f, new_word);
}

void char_delete(list *f, char c, int correct) {
    list curr = *f, tmp = NULL;
    int del;    // Checks whether there was a deletion or not and skips the reassignment of 'curr' if it happened.
    if(correct) {
        while(curr != NULL) {
            del = 0;
            if(strchr(curr->data, c) == NULL) {
                tmp = curr->next;
                *f = delete(*f, curr->data);
                curr = tmp;
                del = 1;
            }
            if(!del) curr = curr->next;
        }
    } else {
        while(curr != NULL) {
            del = 0;
            if(strchr(curr->data, c) != NULL) {
                tmp = curr->next;
                *f = delete(*f, curr->data);
                curr = tmp;
                del = 1;
            }
            if(!del) curr = curr->next;
        }
    }
}

void pwd_char_count(int *pc, const char *password, int len) {
    for(int i = 0; i < len; ++i) {
        if(password[i] == '-')
            ++pc[0];
        if(password[i] >= '0' && password[i] <= '9')
            ++pc[password[i] - 47];
        if(password[i] >= 'A' && password[i] <= 'Z')
            ++pc[password[i] - 54];
        if(password[i] == '_')
            ++pc[37];
        if(password[i] >= 'a' && password[i] <= 'z')
            ++pc[password[i] - 59];
    }
}

int pwd_char_check(int *pc, char c) {
    if(c == '-') return pc[0];
    if(c >= '0' && c <= '9') return pc[c - 47];
    if(c >= 'A' && c <= 'Z') return pc[c - 54];
    if(c == '_') return pc[37];
    if(c >= 'a' && c <= 'z') return pc[c - 59];
    return -1;
}