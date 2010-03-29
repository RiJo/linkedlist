#include "linkedlist.h"
#include <stdlib.h>

int compare(void *item1, void *item2) {
    int x = *((int *)item1);
    int y = *((int *)item2);
    return (x > y) - (y > x);
}

int natural_number(void *item) {
    int x = *((int *)item);
    return (x >= 0);
}

void to_string_dump(FILE * output, void *item) {
    int x = *((int *)item);
    fprintf(output, "value: %d", x);
}

void to_string_print(FILE * output, void *item) {
    int x = *((int *)item);
    fprintf(output, "value: %d\n", x);
}


int *new_int(int value) {
    int *x = malloc(sizeof(int));
    *x = value;
    return x;
}

int main(void) {
    linkedlist *list = ll_create();

    ll_push_last(list, new_int(5));
    ll_push_last(list, new_int(8));
    ll_push_last(list, new_int(21));
    ll_push_last(list, new_int(-3));
    ll_push_last(list, new_int(18));
    ll_push_last(list, new_int(-12));
    ll_push_last(list, new_int(0));
    ll_push_last(list, new_int(8));
    
    FILE *output = stdout;
    
    printf("\n -->  INITIAL LIST\n");
    ll_dump(list, output, to_string_dump);

    ll_sort(list, compare);
    printf("\n -->  SORTED LIST\n");
    ll_dump(list, output, to_string_dump);

    printf("\n -->  UNFILTERED LIST\n\n");
    ll_print(list, output, to_string_print);
    printf("\n\n");

    printf("\n -->  FILTERED LIST (NATURAL NUMBERS)\n\n");
    ll_print_filter(list, output, to_string_print, natural_number);
    printf("\n\n");

    ll_delete(list, NULL);
    return EXIT_SUCCESS;
}
