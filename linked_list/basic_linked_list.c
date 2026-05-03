#include <stdlib.h>
#include <stdio.h>


typedef struct __node_t{
    int key;
    struct __node_t *next;
} node_t;


typedef struct __list_t{
    node_t *head;
} list_t;


void init_linked_list(list_t *L){
    L->head = NULL;
}

int insert_to_linked_list(list_t *L, int key){
    node_t *new = malloc(sizeof(node_t));
    if (new == NULL) return -1;
    new->next = L->head;
    new->key = key;
    L->head = new;
    return 0;
}

int lookup_linked_list(list_t *L, int key){
    current = L->head;

    while(current!=NULL){
        if (current->key == key){
            return 0;
        }
        current = current->next;
    }

    return -1;
}

int remove_from_linked_list(list_t *L, int target){
    node_t *current = L->head;
    if (current == NULL) return -1;
    if (current->key == target){
        L->head = current->next;
        free(current);
        return 0;
    }

    while (current->next != NULL){
        if (current->next->key == target){
            node_t *temp= current->next;
            current->next = temp->next;
            free(temp);
            return 0;
        }
        current = current->next;
    }
    return -1;
}


void print_linked_list(list_t *L){
    node_t *current = L->head;
    while (current != NULL){
        printf("%d->", current->key);
        current = current->next;
    }

    if (current == NULL){
        printf("NULL\n");
    }
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

    print_linked_list(&mylist);

    return 0;
}