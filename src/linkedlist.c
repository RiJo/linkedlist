#include "linkedlist.h"
#include <stdlib.h>
#include <string.h>

#define SUCCESS 1
#define FAIL 0

#define COMP_CALLBACK int (*comparator)(void *a, void *b)

#ifdef DEBUG
    #define DEBUG_OUTPUT(string) (printf("[debug] %s\n",string))
#else
    #define DEBUG_OUTPUT(string)
#endif

/* PRIVATE */

void *get_item_fwd(item *list, unsigned int index) {
    if (list == NULL) {
        return NULL;
    }
    if (index == 0) {
        return list->value;
    }
    return get_item_fwd(list->next, --index);
}

void *get_item_rev(item *list, unsigned int index) {
    if (list == NULL) {
        return NULL;
    }
    if (index == 0) {
        return list->value;
    }
    return get_item_rev(list->prev, --index);
}

int item_exists(item *list, void *value, int (*comparator)(void *a, void *b)) {
    if (list == NULL) {
        return 0;
    }
    if (comparator(value, list->value) == 0) {
        return 1;
    }
    return item_exists(list->next, value, comparator);
}

item *bubble_sort(item *list, int (*comparator)(void *a, void *b)) {
    item *head = list;
    register int swapped;
    do {
        list = head;
        swapped = 0;
        while (list->next) {
            if (comparator(list->value, list->next->value) > 0) {
                void *temp = list->value;
                list->value = list->next->value;
                list->next->value = temp;
                swapped = 1;
            }
            list = list->next;
        }
    } while (swapped);
    return head;
}

item *sort_list(item *list, int (*comparator)(void *a, void *b)) {
    if (list == NULL) {
        return NULL;
    }
    return bubble_sort(list, comparator);
}

int clear_list(item *list, void (*free_value)(void *)) {
    if (list == NULL) {
        return SUCCESS;
    }
    clear_list(list->next, free_value);
    list->next = NULL;
    if (free_value != NULL) {
        free_value(list->value);
    }
    else {
        free(list->value);
    }
    list->value = NULL;
    free(list);
    DEBUG_OUTPUT("item removed");
    return SUCCESS;
}

void print_list(item *list, FILE *output, void to_string(FILE *,void *), int (*is_visible)(void *)) {
    if (list != NULL) {
        if (is_visible == NULL || is_visible(list->value)) {
            to_string(output, list->value);
        }
        print_list(list->next, output, to_string, is_visible);
    }
}

/* PUBLIC */

linkedlist *ll_create() {
    linkedlist *list = malloc(sizeof(linkedlist));
    if (list == NULL) {
        fprintf(stderr, "Error: could not allocate memory for linkedlist\n");
        exit(EXIT_FAILURE);
    }
    list->head = NULL;
    list->leaf = NULL;
    list->current = NULL;
    list->size = 0;
    DEBUG_OUTPUT("linkedlist created");
    return list;
}

int ll_push_first(linkedlist *list, void *value) {
    item *temp = malloc(sizeof(item));
    if (temp == NULL) {
        fprintf(stderr, "Error: could not allocate memory for item\n");
        exit(EXIT_FAILURE);
    }
    temp->value = value;
    temp->prev = NULL;
    temp->next = list->head;
    if (list->head == NULL) {
        list->leaf = temp;
    }
    else {
        list->head->prev = temp;
    }
    list->head = temp;
    list->size += 1;
    return SUCCESS;
}

int ll_push_last(linkedlist *list, void *value) {
    item *temp = malloc(sizeof(item));
    if (temp == NULL) {
        fprintf(stderr, "Error: could not allocate memory for item\n");
        exit(EXIT_FAILURE);
    }
    temp->value = value;
    temp->prev = list->leaf;
    temp->next = NULL;
    if (list->head == NULL) {
        list->head = temp;
        list->leaf = temp;
    }
    else {
        list->leaf->next = temp;
        list->leaf = temp;
    }
    list->size += 1;
    return SUCCESS;
}

void *ll_get_first(linkedlist *list) {
    return list->head->value;
}

void *ll_get_last(linkedlist *list) {
    return list->leaf->value;
}

void *ll_pop_first(linkedlist *list) {
    item *temp = list->head;
    if (temp == NULL) {
        DEBUG_OUTPUT("nothing to pop from beginning of list");
        return NULL;
    }
    list->head = temp->next;
    if (list->head != NULL) {
        list->head->prev = NULL;
    }
    void *value = temp->value;
    free(temp);
    list->size--;
    DEBUG_OUTPUT("first item popped from list");
    return value;
}

void *ll_pop_last(linkedlist *list) {
    item *temp = list->leaf;
    if (temp == NULL) {
        DEBUG_OUTPUT("nothing to pop from end of list");
        return NULL;
    }
    list->leaf = temp->prev;
    if (list->leaf != NULL) {
        list->leaf->next = NULL;
    }
    void *value = temp->value;
    free(temp);
    list->size--;
    DEBUG_OUTPUT("last item popped from list");
    return value;
}

void *ll_get_index(linkedlist *list, unsigned int index) {
    if (index >= list->size) {
        fprintf(stderr, "Warning: index out of bounds\n");
        return NULL;
    }
    if (index < list->size / 2) {
        DEBUG_OUTPUT("get item by index, starting from head");
        return get_item_fwd(list->head, index);
    }
    else {
        DEBUG_OUTPUT("get item by index, starting from leaf");
        return get_item_rev(list->leaf, list->size - 1 - index);
    }
}

void *ll_get_next(linkedlist *list) {
    if (list->current == NULL) {
        if (list->head == NULL) {
            return NULL;
        }
        else {
            list->current = list->head;
            return list->current->value;
        }
    }
    else {
        list->current = list->current->next;
        if (list->current == NULL) {
            return NULL;
        }
        else {
            return list->current->value;
        }
    }
}

void *ll_get_prev(linkedlist *list) {
    if (list->current == NULL) {
        if (list->leaf == NULL) {
            return NULL;
        }
        else {
            list->current = list->leaf;
            return list->current->value;
        }
    }
    else {
        list->current = list->current->prev;
        if (list->current == NULL) {
            return NULL;
        }
        else {
            return list->current->value;
        }
    }
}

int ll_exists(linkedlist *list, void *value, int (*comparator)(void *, void *)) {
    return item_exists(list->head, value, comparator);
}

int ll_sort(linkedlist *list, int (*comparator)(void *a, void *b)) {
    if (comparator == NULL) {
        fprintf(stderr, "Warning: comparator function is NULL, cannot sort\n");
        return FAIL;
    }
    else {
        list->head = sort_list(list->head, comparator);
        DEBUG_OUTPUT("linkedlist sorted");
        return SUCCESS;
    }
}

int ll_clear(linkedlist *list, void (*free_value)(void *)) {
    if (list->head != NULL) {
        clear_list(list->head, free_value);
        list->head = NULL;
    }
    list->size = 0;
    DEBUG_OUTPUT("linkedlist cleared");
    return SUCCESS;
}

int ll_delete(linkedlist *list, void (*free_value)(void *)) {
    ll_clear(list, free_value);
    list->head = NULL;
    free(list);
    DEBUG_OUTPUT("linkedlist deleted");
    return SUCCESS;
}

void ll_print(linkedlist *list, FILE *output, void (*to_string)(FILE *, void *)) {
    print_list(list->head, output, to_string, NULL);
}

void ll_print_filter(linkedlist *list, FILE *output, void (*to_string)(FILE *, void *), int (*is_visible)(void *)) {
    print_list(list->head, output, to_string, is_visible);
}

void ll_dump(linkedlist *list, FILE *output, void (*to_string)(FILE *, void *)) {
    fprintf(output, "\n### linkedlist dump ############################################################\n\n");
    fprintf(output, " Version: %s\n", LL_VERSION);
    fprintf(output, " Head: %lX\n", (long)list->head);
    fprintf(output, " Leaf: %lX\n", (long)list->leaf);
    fprintf(output, " Current: %lX\n", (long)list->current);
    fprintf(output, " Items: %d\n", list->size);
    fprintf(output, "\n################################################################################\n\n");
    if (list->size > 0) {
        item *next = list->head;
        unsigned int index = 0;
        while (next != NULL) {
            fprintf(output, " index %d: %-8.lX prev: %-8.lX next: %-8.lX ", index++, (long)next, (long)next->prev, (long)next->next);
            if (to_string != NULL) {
                to_string(output, next->value);
            }
            else {
                fprintf(output, "value: %-8.lX", (long)next->value);
            }
            fprintf(output, "\n");
            next = next->next;
        }
    }
    else {
        fprintf(output, " (no items)\n");
    }
    fprintf(output, "\n################################################################################\n\n");
}