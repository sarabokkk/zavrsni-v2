int *handeSelectProduct(bool canGoBack){
    int id_initial_capacity = 2;
    int* category_ids = (int*)malloc(id_initial_capacity * sizeof(int));
    if (category_ids == NULL) {
        perror("Failed to allocate memory for ids");
        exit(0);
    }

    int category_ids_size = 0; // Current size of the IDs list
    int id_capacity = id_initial_capacity; // Current capacity of the IDs list

    // Copy IDs from categories to the ids list
    for (int i = 0; i < category_count; i++) {
        if (category_ids_size >= id_capacity) {
            id_capacity *= 2;
            int* new_ids = (int*)realloc(category_ids, id_capacity * sizeof(int));
            if (new_ids == NULL) {
                perror("Failed to reallocate memory for ids");
                free(category_ids);
                exit(0);
            }
            category_ids = new_ids;
        }
        category_ids[category_ids_size++] = categories[i].id;
    }

    qsort(category_ids, category_ids_size, sizeof(int), compare_ints);
    int *result = (int *)malloc(sizeof(int));
    int *back_output = (int *)malloc(sizeof(int));
    char *input = (char *)malloc(3 * sizeof(char));
    int *pressedNum = (int *)malloc(sizeof(int));
    int *index;
    printf(" ");
    while (1) {

        *result = scanf("%d", pressedNum);
        if (*result == 1) {
            if (canGoBack && *pressedNum == 0) {
                int *back_code = (int *)malloc(sizeof(int));
                *back_code = -1;
                return back_code;
            }

            int *item = (int*)bsearch(&*pressedNum, category_ids, category_ids_size, sizeof(int), compare_ints);
            if (item != NULL) {
                index = item - *category_ids;
                return index;
            } else {
                printf("Category with number %d does not exists! Pleasy try again.\n", *pressedNum);
            }
        }
        else {
            printf("Wrong input! Pleasy try again and input a number.\n");
            // Clear the input buffer
            while (getchar() != '\n');
        }
    }

}