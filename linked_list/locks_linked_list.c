#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>


typedef struct __node_t{
    int key;
    struct __node_t *next;
} node_t;


typedef struct __list_t{
    node_t *head;
    pthread_mutex_t lock;
} list_t;


void init_linked_list(list_t *L){
    L->head = NULL;
    pthread_mutex_init(&L->lock, NULL);
}

int insert_to_linked_list(list_t *L, int key){
    node_t *new = malloc(sizeof(node_t));

    if (new==NULL) return -1;

    new->key = key;
    pthread_mutex_lock(&L->lock);
    new->next = L->head;
    L->head = new;
    pthread_mutex_unlock(&L->lock);
    return 0;
}

int lookup_linked_list(list_t *L, int key){
    int rv = -1;
    pthread_mutex_lock(&L->lock);
    node_t *current = L->head;
    while(current!=NULL){
        if (current->key == key){
            rv = 1;
            break;
        }
        current = current->next;
    }
    pthread_mutex_unlock(&L->lock);
    return rv;
}


int remove_from_linked_list(list_t *L, int target){
    int do_loop = 1;
    int rv = -1;
    pthread_mutex_lock(&L->lock);
    node_t *current = L->head;
    if (current == NULL) {
        rv = -1;
        do_loop = 0;
    } else if (current->key == target){
        L->head = current->next;
        free(current);
        do_loop = 0;
        rv = 0;
    }

    if (do_loop == 1) {
        while (current->next != NULL){
            if (current->next->key == target){
                node_t *temp= current->next;
                current->next = temp->next;
                free(temp);
                rv = 0;
                break;
            }
            current = current->next;
        }
    }
    pthread_mutex_unlock(&L->lock);
    return rv;
}



int main (int argc, char *argv[]){
    list_t mylist;
    init_linked_list(&mylist);
    insert_to_linked_list(&mylist, 5);
    insert_to_linked_list(&mylist, 6);
    insert_to_linked_list(&mylist, 7);
    insert_to_linked_list(&mylist, 8);
    insert_to_linked_list(&mylist, 9);
    insert_to_linked_list(&mylist, 10);
    insert_to_linked_list(&mylist, 11);
    insert_to_linked_list(&mylist, 12);

    int x = lookup_linked_list(&mylist, -5);

    printf("%d", x);
    return 0;
}