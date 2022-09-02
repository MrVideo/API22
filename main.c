#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 64

struct node {
    struct node *next;
    char data[];
};

struct res_node {
    char c;
    int index;
    int correct;
    int count;
    int exact;
    struct res_node *next;
};

typedef struct node *list;
typedef struct res_node *res_list;

// List management functions
int list_size(list l);
list add_sort(list l, const char *new_data, int len);
list add(list l, const char *new_data, int len);
void print(list l);
list search(list l, char *query);
res_list add_res(res_list r, char c, int index, int correct, int count, int exact);
list destroy(list l);
res_list res_destroy(res_list r);
list duplicate_sort(list f, int len, int overwrite);

// Game functions
void add_new_words(list *l, list *f, res_list *r, int len, int postgame, int *sorted, int f_created);
void word_check(char *password, char *buffer, char *guide, int len, int *tp, short *pc, short *wc, list *f, res_list *r);
void occurrences_check(list *f, char c, int count, int strict);
void new_words_check(res_list *r, list *f, char *new_word, const int *sorted, int len);
void char_delete(list *f, char c, int correct);
void char_count(short *bc, const char *word, int len);
short char_check(short *bc, char c);
void delete_index(int i, char c, list *f, int correct);
void first_add(list *l, list *f, res_list *r, int len, int sorted);

int main() {
    // Variable declarations
    char *buffer = calloc(BUFSIZE, sizeof(char));
    char *password = NULL, *guide = NULL;
    int word_length, cmd_chk = 0, restart, ok = 0, tries = 0, sorted = 0, f_created = 0;
    int *tp = &tries, *fs = &sorted;
    short pc[64] = {0}, wc[64] = {0};
    list wordlist = NULL;
    list filtered = NULL;
    res_list restrictions = NULL;

    // -- Game setup --
    // This is only run once.

    // Saves the word length decided by the user.
    word_length = (int)strtol(fgets(buffer, BUFSIZE, stdin), NULL, 10);

    // Starts saving new words to the main word list.
    if(word_length > 0) {
        password = malloc(sizeof(char) * (word_length + 1));
        guide = malloc(sizeof(char) * (word_length + 1));
        while(!cmd_chk) {
            if(scanf("%s", buffer) != 0) {
                if(strcmp(buffer, "+nuova_partita") == 0)
                    cmd_chk = 1;
                else wordlist = add(wordlist, buffer, word_length);
                memset(buffer, 0, BUFSIZE);
            }
        }
    }

    // Declaration of pointer to list
    list *lw = &wordlist;
    list *lf = &filtered;
    res_list *lr = &restrictions;

    // If the word has been input before, the password is saved.
    if(scanf("%s", buffer) != 0) {
        strncpy(password, buffer, word_length + 1);
        char_count(pc, password, word_length);
    }

    memset(buffer, 0, BUFSIZE);

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
                    add_new_words(lw, lf, lr, word_length, 0, fs, f_created);
                    memset(buffer, 0, BUFSIZE);
                    continue;
                }
                // If "+stampa_filtrate" is called, the filtered list is printed on the screen.
                else if(strcmp(buffer, "+stampa_filtrate") == 0) {
                    if(!f_created) {
                        filtered = duplicate_sort(wordlist, word_length, 0);
                        f_created = 1;
                        sorted = 1;
                    }
                    else if(!sorted) {
                        filtered = duplicate_sort(filtered, word_length, 1);
                        sorted = 1;
                    }
                    print(filtered);
                    memset(buffer, 0, BUFSIZE);
                    continue;
                }
                // If the word is not of the correct length, or it does not appear in the wordlist,
                // no tries are consumed and the user can retry.
                else if(strlen(buffer) != word_length || search(wordlist, buffer) == NULL) {
                    printf("not_exists\n");
                    memset(buffer, 0, BUFSIZE);
                    continue;
                }
                // If none of the checks before this get triggered, the word is checked for correctness.
                else {
                    if(strcmp(password, buffer) == 0) ok = 1;
                    else if(tries >= 0) {
                        memset(wc, 0, BUFSIZE * sizeof(short));
                        char_count(wc,buffer, word_length);
                        word_check(password, buffer, guide, word_length, tp, pc, wc, lf, lr);
                        if(!f_created) {
                            first_add(lw, lf, lr, word_length, sorted);
                            f_created = 1;
                        }
                        memset(buffer, 0, BUFSIZE);
                        for(int i = 0; i < word_length; ++i)
                            printf("%c", guide[i]);
                        printf("\n%d\n", list_size(filtered));
                        if(tries != 0) continue;
                    }
                }
            }

            // Prints the outcome and resets the game.
            if(ok || tries == 0) {
                if(ok) printf("ok\n");
                if(!tries) printf("ko\n");
                ok = 0;
                memset(buffer, 0, BUFSIZE);
                restrictions = res_destroy(restrictions);
                filtered = destroy(filtered);
                sorted = 0;
                f_created = 0;
                // The user can manage settings while in this loop, until they decide to start a new game.
                while(!restart) {
                    if(scanf("%s", buffer) == EOF) break;
                    else {
                        if(strcmp(buffer, "+inserisci_inizio") == 0)
                            add_new_words(lw, lf, lr, word_length, 1, fs, f_created);
                        if(strcmp(buffer, "+nuova_partita") == 0) {
                            memset(buffer, 0, BUFSIZE);
                            if(scanf("%s", buffer) != 0) {
                                strncpy(password, buffer, word_length + 1);
                                memset(pc, 0, BUFSIZE * sizeof(short));
                                char_count(pc, password, word_length);
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
    free(buffer);
    free(password);
    free(guide);
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

list add_sort(list l, const char *new_data, int len) {
    list prev = NULL, curr = l;

    //If the list is empty, add the element on top.
    if(l == NULL) {
        list tmp = malloc(sizeof(struct node) + sizeof(char) * (len + 1));
        strcpy(tmp->data, new_data);
        tmp -> next = l;
        return tmp;
    } else {
        //If the list is not empty
        while(curr != NULL) {
            //If new_data comes before the current node's data then add it before curr
            if(strcmp(new_data, curr -> data) < 0) {
                list tmp = malloc(sizeof(struct node) + sizeof(char) * (len + 1));
                strcpy(tmp->data, new_data);
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
            list tmp = malloc(sizeof(struct node) + sizeof(char) * (len + 1));
            strcpy(tmp->data, new_data);
            tmp -> next = curr;
            prev -> next = tmp;
            return l;
        }
    }
    //This gets executed in case of an error
    return NULL;
}

list add(list l, const char *new_data, int len) {
    list tmp = malloc(sizeof(struct node) + sizeof(char) * (len + 1));
    strcpy(tmp->data, new_data);
    tmp->next = l;
    return tmp;
}

void print(list l) {
    list curr = l;
    while(curr != NULL) {
        printf("%s\n", curr -> data);
        curr = curr -> next;
    }
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

res_list add_res(res_list r, char c, int index, int correct, int count, int exact) {
    res_list tmp = malloc(sizeof(struct res_node));
    tmp->c = c;
    tmp->index = index;
    tmp->correct = correct;
    tmp->count = count;
    tmp->exact = exact;
    tmp->next = r;
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

res_list res_destroy(res_list r) {
    res_list curr = r, tmp = NULL;
    while(curr != NULL) {
        tmp = curr->next;
        free(curr);
        curr = tmp;
    }
    return NULL;
}

list duplicate_sort(list f, int len, int overwrite) {
    list curr = f, head = NULL;
    while(curr != NULL) {
        head = add_sort(head, curr->data, len);
        curr = curr->next;
    }
    if(overwrite) destroy(f);
    return head;
}

// Game function definitions

void add_new_words(list *l, list *f, res_list *r, int len, int postgame, int *sorted, int f_created) {
    char *buffer = calloc(BUFSIZE, sizeof(char));
    int exit = 0;
    while(!exit) {
        if(scanf("%s", buffer) != 0) {
            if(strcmp(buffer, "+inserisci_fine") == 0)
                exit = 1;
            else if(strlen(buffer) == len) {
                *l = add(*l, buffer, len);
                if (f_created && r != NULL)
                    new_words_check(r, f, buffer, sorted, len);
            }
        }
        memset(buffer, 0, BUFSIZE);
    }
    free(buffer);
}

void word_check(char *password, char *buffer, char *guide, int len, int *tp, short *pc, short *wc, list *f, res_list *r) {
    // Empty guide buffer
    memset(guide, 0, len);
    // Wrong word: one try is subtracted to the counter.
    --*tp;
    short pwd, count;
    char tmp;
    for (int i = 0; i < len; ++i) {
        if(guide[i] != '+' && guide[i] != '|' && guide[i] != '/') {
            tmp = buffer[i];
            pwd = char_check(pc, tmp);
            count = char_check(wc, tmp);
            if(pwd != -1 && count != -1) {
                char *b = buffer, *p = password;
                long index, p_index;
                // If these two values are the same or there are more 'tmp' characters in the password, the symbols
                // must be either '+' or '|'.
                if(pwd >= count) {
                    // While there are more 'tmp' characters in both strings, check whether they are in the same index
                    // position. If so, mark '+', else mark '|'.
                    while(b != NULL) {
                        b = strchr(b, tmp);
                        if (b != NULL) {
                            p = password;
                            int found = 0;
                            index = b - buffer;
                            if (index < len) {
                                while (p != NULL && !found) {
                                    p = strchr(p, tmp);
                                    if (p != NULL) {
                                        p_index = p - password;
                                        if (p_index < len) {
                                            if (index == p_index) {
                                                guide[index] = '+';
                                                delete_index((int) index, tmp, f, 1);
                                                *r = add_res(*r, tmp, (int) index, 1, -1, -1);
                                                found = 1;
                                            }
                                        }
                                        ++p;
                                    }
                                }
                                if (!found) {
                                    guide[index] = '|';
                                    delete_index((int) index, tmp, f, 0);
                                    *r = add_res(*r, tmp, (int) index, 0, -1, -1);
                                }
                            }
                            ++b;
                        }
                    }
                    occurrences_check(f, tmp, count, 0);
                    *r = add_res(*r, tmp, -1, -1, count, 0);
                }
                // If pwd < count, then some characters in the guide will be '/'.
                else {
                    // If the password does not contain that character then every instance of it will be '/'.
                    if(pwd == 0) {
                        while (b != NULL) {
                            b = strchr(b, tmp);
                            if (b != NULL) {
                                index = b - buffer;
                                if (index < len)
                                    guide[index] = '/';
                                ++b;
                            }
                        }
                        // This function deletes all the words that contain characters that do not appear in the
                        // password anywhere.
                        char_delete(f, tmp, 0);
                        *r = add_res(*r, tmp, -1, -1, 0, -1);
                    } else {
                        // While there are more 'tmp' characters in the password, check whether they are in the
                        // correct position in the buffer.
                        int pwd_tmp = pwd, count_tmp = count;
                        // This loop checks for correct characters in the correct places by "syncing" the two char
                        // pointers. This goes on until there are no more occurrences of a given character or until
                        // all of the characters in the password are found to be correct.
                        while(b != NULL && p != NULL && pwd_tmp > 0) {
                            b = strchr(b, tmp);
                            p = strchr(p, tmp);
                            if(p == NULL || b == NULL)
                                break;
                            index = b - buffer;
                            p_index = p - password;
                            if(index < len && p_index < len) {
                                if(index == p_index) {
                                    guide[index] = '+';
                                    delete_index((int)index, tmp, f, 1);
                                    *r = add_res(*r, tmp, (int)index, 1, -1, -1);
                                    --pwd_tmp;
                                    --count_tmp;
                                    ++b;
                                    ++p;
                                } else {
                                    if(index < p_index)
                                        ++b;
                                    if(index > p_index)
                                        ++p;
                                }
                            }
                        }
                        // If pwd_temp is not 0 yet, then there are more characters that are correct but not
                        // in the right place.
                        // The b pointer is reset and the check starts from the beginning.
                        b = buffer;
                        while(b != NULL && pwd_tmp > 0) {
                            b = strchr(b, tmp);
                            if(b == NULL) break;
                            index = b - buffer;
                            if(index < len) {
                                if (guide[index] != '+') {
                                    guide[index] = '|';
                                    delete_index((int) index, tmp, f, 0);
                                    *r = add_res(*r, tmp, (int) index, 0, -1, -1);
                                    --pwd_tmp;
                                }
                                ++b;
                            }
                        }
                        // If pwd_temp is 0 here, then all that are left are wrong characters.
                        // The b pointer is reset if the loop above run.
                        if(b != buffer) b = buffer;
                        while(b != NULL && count_tmp > 0) {
                            b = strchr(b, tmp);
                            if(b == NULL) break;
                            index = b - buffer;
                            if(index < len) {
                                if(guide[index] != '+' && guide[index] != '|') {
                                    guide[index] = '/';
                                    delete_index((int) index, tmp, f, 0);
                                    *r = add_res(*r, tmp, (int) index, 0, -1, -1);
                                    --count_tmp;
                                }
                                ++b;
                            }
                        }
                        occurrences_check(f, tmp, pwd, 1);
                        *r = add_res(*r, tmp, -1, -1, pwd, 1);
                    }
                }
            }
        }
    }
}

void occurrences_check(list *f, char c, int count, int strict) {
    list curr = *f, tmp = NULL, prev = NULL;
    char *w;
    int curr_count, del;
    while(curr != NULL) {
        del = 0;
        w = curr->data;
        curr_count = 0;
        // This loop counts the number of occurrences of the character passed to occurrences_check in the current
        // word in the filtered list.
        while(w != NULL && curr_count <= count) {
            w = strchr(w, c);
            if(w != NULL) {
                ++curr_count;
                ++w;
            }
        }
        // If the check must be strict, if the current word in filtered has a different number of characters than
        // the one passed to the function, it is deleted.
        if(strict) {
            if(curr_count != count) {
                tmp = curr;
                curr = curr->next;
                if(prev != NULL) prev->next = curr;
                else *f = curr;
                free(tmp);
                del = 1;
            }
        }
        // If the check is not strict, if the current word does not have at least 'count' characters, it is deleted.
        else {
            if(curr_count < count) {
                tmp = curr;
                curr = curr->next;
                if(prev != NULL) prev->next = curr;
                else *f = curr;
                free(tmp);
                del = 1;
            }
        }
        // If no deletion occurred, the curr pointer is updated
        if(!del) {
            prev = curr;
            curr = curr->next;
        }
    }
}

void new_words_check(res_list *r, list *f, char *new_word, const int *sorted, int len) {
    res_list curr = *r;
    while(curr != NULL) {
        if(curr->index != -1) {
            if(curr->correct && new_word[curr->index] != curr->c) return;
            if(!curr->correct && new_word[curr->index] == curr->c) return;
        }
        if(curr->count != -1) {
            if (curr->count == 0) {
                if (strchr(new_word, curr->c) != NULL) return;
            }
            else {
                char *w = new_word;
                int count = 0;
                while(w != NULL) {
                    w = strchr(w, curr->c);
                    if(w != NULL) {
                        ++count;
                        ++w;
                    }
                }
                if(curr->exact && count != curr->count) return;
                if(!curr->exact && count < curr->count) return;
            }
        }
        curr = curr->next;
    }
    // If the function did not return, the word can be added.
    if(sorted)
        *f = add_sort(*f, new_word, len);
    else
        *f = add(*f, new_word, len);
}

void char_delete(list *f, char c, int correct) {
    list curr = *f, tmp = NULL, prev = NULL;
    int del;    // Checks whether there was a deletion or not and skips the reassignment of 'curr' if it happened.
    if(correct) {
        while(curr != NULL) {
            del = 0;
            if(strchr(curr->data, c) == NULL) {
                tmp = curr;
                curr = curr->next;
                if(prev != NULL) prev->next = curr;
                else *f = curr;
                free(tmp);
                del = 1;
            }
            if(!del) {
                prev = curr;
                curr = curr->next;
            }
        }
    } else {
        while(curr != NULL) {
            del = 0;
            if(strchr(curr->data, c) != NULL) {
                tmp = curr;
                curr = curr->next;
                if(prev != NULL) prev->next = curr;
                else *f = curr;
                free(tmp);
                del = 1;
            }
            if(!del) {
                prev = curr;
                curr = curr->next;
            }
        }
    }
}

void char_count(short *bc, const char *word, int len) {
    for(int i = 0; i < len; ++i) {
        if(word[i] == '-')
            ++bc[0];
        if(word[i] >= '0' && word[i] <= '9')
            ++bc[word[i] - 47];
        if(word[i] >= 'A' && word[i] <= 'Z')
            ++bc[word[i] - 54];
        if(word[i] == '_')
            ++bc[37];
        if(word[i] >= 'a' && word[i] <= 'z')
            ++bc[word[i] - 59];
    }
}

short char_check(short *bc, char c) {
    if(c == '-') return bc[0];
    if(c >= '0' && c <= '9') return bc[c - 47];
    if(c >= 'A' && c <= 'Z') return bc[c - 54];
    if(c == '_') return bc[37];
    if(c >= 'a' && c <= 'z') return bc[c - 59];
    return -1;
}

void delete_index(int i, char c, list *f, int correct) {
    list curr = *f, tmp = NULL, prev = NULL;
    int del;
    while(curr != NULL) {
        del = 0;
        if(correct) {
            if(curr->data[i] != c) {
                tmp = curr;
                curr = curr->next;
                if(prev != NULL) prev->next = curr;
                else *f = curr;
                free(tmp);
                del = 1;
            }
        } else {
            if(curr->data[i] == c) {
                tmp = curr;
                curr = curr->next;
                if(prev != NULL) prev->next = curr;
                else *f = curr;
                free(tmp);
                del = 1;
            }
        }
        if(!del) {
            prev = curr;
            curr = curr->next;
        }
    }
}

void first_add(list *l, list *f, res_list *r, int len, int sorted) {
    list curr_l = *l;
    res_list curr_r = *r;
    int to_add;
    while(curr_l != NULL) {
        to_add = 1;
        curr_r = *r;
        while(curr_r != NULL) {
            if(curr_r->index != -1) {
                if(curr_r->correct && curr_l->data[curr_r->index] != curr_r->c) {
                    to_add = 0;
                    break;
                }
                if(!curr_r->correct && curr_l->data[curr_r->index] == curr_r->c) {
                    to_add = 0;
                    break;
                }
            }
            if(curr_r->count != -1) {
                if (curr_r->count == 0) {
                    if (strchr(curr_l->data, curr_r->c) != NULL) {
                        to_add = 0;
                        break;
                    }
                }
                else {
                    char *w = curr_l->data;
                    int count = 0;
                    while(w != NULL) {
                        w = strchr(w, curr_r->c);
                        if(w != NULL) {
                            ++count;
                            ++w;
                        }
                    }
                    if(curr_r->exact && count != curr_r->count) {
                        to_add = 0;
                        break;
                    }
                    if(!curr_r->exact && count < curr_r->count) {
                        to_add = 0;
                        break;
                    }
                }
            }
            curr_r = curr_r->next;
        }
        if(to_add) {
            if(sorted) *f = add_sort(*f, curr_l->data, len);
            else *f = add(*f, curr_l->data, len);
        }
        curr_l = curr_l->next;
    }
}
