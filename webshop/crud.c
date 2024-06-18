#include "crud.h"
#include "models.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Category* categories = NULL;
Product* products = NULL;
Order* orders = NULL;
static int category_count = 0;
static int product_count = 0;
static int order_count = 0;

int compare_ints(const void *a, const void *b) {
    int int_a = *((int*)a);
    int int_b = *((int*)b);

    if (int_a < int_b) return -1;
    else if (int_a > int_b) return 1;
    else return 0;
}

void loadCategories() {
    const char* filename = "categories.csv";
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    char line[256];
    int capacity = 10;
    categories = (Category*)malloc(capacity * sizeof(Category));

    while (fgets(line, sizeof(line), file)) {
        if (category_count >= capacity) {
            capacity *= 2;
            categories = (Category*)realloc(categories, capacity * sizeof(Category));
        }

        sscanf(line, "%d,%99[^\n]", &categories[category_count].id, categories[category_count].title);
        category_count++;
    }

    fclose(file);
}

void loadProducts() {
    const char* filename = "products.csv";
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    char line[256];
    int capacity = 10;
    products = (Product*)malloc(capacity * sizeof(Product));

    while (fgets(line, sizeof(line), file)) {
        if (product_count >= capacity) {
            capacity *= 2;
            products = (Product*)realloc(products, capacity * sizeof(Product));
        }

        sscanf(
            line,
            "%d,%d,%99[^,],%lf",
            &products[product_count].id,
            &products[product_count].category_id,
            products[product_count].title,
            &products[product_count].price
        );
        product_count++;
    }

    fclose(file);
}

void loadOrders() {
    const char* filename = "orders.csv";
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    char line[256];
    int capacity = 10;
    orders = (Order*)malloc(capacity * sizeof(Order));

    while (fgets(line, sizeof(line), file)) {
        if (order_count >= capacity) {
            capacity *= 2;
            orders = (Order*)realloc(products, capacity * sizeof(Order));
        }

        sscanf(
            line,
            "%99[^,],%99[^,],%99[^,],%99[^,],%99[^,],%lf",
            orders[order_count].timestamp,
            orders[order_count].first_name,
            orders[order_count].last_name,
            orders[order_count].address,
            orders[order_count].products,
            &orders[order_count].total_price
        );
        order_count++;
    }

    fclose(file);
}

Category getCategoryById(int category_id) {
    for (int i = 0; i < category_count; i++) {
        if (categories[i].id == category_id) {
            return categories[i];
        }
    }
}

Product getProductById(int product_id) {
    for (int i = 0; i < product_count; i++) {
        if (products[i].id == product_id) {
            return products[i];
        }
    }
}

char* get_current_timestamp() {
    time_t rawtime;
    struct tm *timeinfo;
    char *buffer = (char *)malloc(20 * sizeof(char)); // Allocate memory for the timestamp

    if (buffer == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, 20, "%Y-%m-%d %H:%M:%S", timeinfo);

    return buffer;
}

void createOrder(Order *order) {
    const char* filename = "orders.csv";
    FILE* file = fopen(filename, "a");
    if (!file) {
        perror("Error opening file");
        return;
    }

    fprintf(
        file,
        "%s,%s,%s,%s,%s,%.2f\n",
        order->timestamp,
        order->first_name,
        order->last_name,
        order->address,
        order->products,
        order->total_price
    );

    int capacity = 20;
    orders = (Order*)realloc(orders, capacity * sizeof(Order));
    orders[order_count++] = *order;

    fclose(file);
    free(order);
}

void createProduct(Product *product) {
    const char* filename = "products.csv";
    FILE* file = fopen(filename, "a");
    if (!file) {
        perror("Error opening file");
        return;
    }

    fprintf(file, "%d,%d,%s,%.2f\n", product->id, product->category_id, product->title, product->price);

    rewind(file);
    fclose(file);

    int capacity = 10*2;
    products = (Product*)realloc(products, capacity * sizeof(Product));
    products[product_count++] = *product;

    free(product);
}

inline int findBiggestNumber(int* array, int size) {
    if (size <= 0) {
        perror("Array size must be positive\n");
        exit(0); // Indicate an error
    }

    int max = array[0];
    for (int i = 1; i < size; i++) {
        if (array[i] > max) {
            max = array[i];
        }
    }
    return max;
}

int getNewProductId() {
    int id_initial_capacity = 2;
    int* product_ids = (int*)malloc(id_initial_capacity * sizeof(int));
    if (product_ids == NULL) {
        perror("Failed to allocate memory for ids");
        exit(0);
    }

    int product_ids_size = 0; // Current size of the IDs list
    int id_capacity = id_initial_capacity; // Current capacity of the IDs list

    // Copy IDs from categories to the ids list
    for (int i = 0; i < product_count; i++) {
        if (product_ids_size >= id_capacity) {
            id_capacity *= 2;
            int* new_ids = (int*)realloc(product_ids, id_capacity * sizeof(int));
            if (new_ids == NULL) {
                perror("Failed to reallocate memory for ids");
                free(product_ids);
                exit(0);
            }
            product_ids = new_ids;
        }
        product_ids[product_ids_size++] = products[i].id;
    }

    return findBiggestNumber(product_ids, product_ids_size) + 1;
}

void updateProductsInFile() {
    // first delete content in file
    const char* filename = "products.csv";
    remove(filename);
    FILE* file_write = fopen(filename, "w");
    if (!file_write) {
        perror("Error opening file");
        return;
    }
    fclose(file_write);


    FILE* file_append = fopen(filename, "a");
    if (!file_append) {
        perror("Error opening file");
        return;
    }

    // update file content
    for (int i = 0; i < product_count; i++) {
        fprintf(
            file_append,
            "%d,%d,%s,%.2f\n",
            products[i].id,
            products[i].category_id,
            products[i].title,
            products[i].price
        );
    }
    fclose(file_append);

}

void deleteProduct(int index) {
    if (index < 0 || index >= product_count) {
        perror("Index out of bounds\n");
        return;
    }

    // Shift elements to the left
    for (int i = index; i < product_count - 1; i++) {
        (products)[i] = (products)[i + 1];
    }

    product_count--;

    products = realloc(products, (product_count) * sizeof(Product));
    if (products == NULL && product_count > 0) {
        perror("Failed to reallocate memory");
        exit(0);
    }
}

void deleteOrder() {
    const char* filename = "orders.csv";
    char* name = (char*)calloc(100, sizeof(char));
    printf("Enter the last name of the order:\n");
    fgets(name, 99, stdin);
    FILE* file = fopen(filename, "r");
    FILE* temp = fopen("temp.csv", "w");

    if (file == NULL || temp == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char line[1000];

    while (fgets(line, sizeof(line), file) != NULL) {
        if (strstr(line, name) == NULL) {
            fputs(line, temp);
        }
    }

    fclose(file);
    fclose(temp);

    remove(filename);
    rename("temp.csv", filename);
}