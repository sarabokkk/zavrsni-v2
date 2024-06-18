#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "screens.h"
#include "crud.h"

const char *ADMIN_USER = "admin";
const char *ADMIN_PASS = "1234";

int *handleStaticMenuInput(int max, bool canGoBack){
    int *pressedNum = (int *)malloc(sizeof(int));
    int *result = (int *)malloc(sizeof(int));
    int *min = (int *)malloc(sizeof(int));
    *min = (canGoBack) ? 0 : 1;
    printf(" ");
    while (1) {
        *result = scanf("%d", pressedNum);

        if (*result == 1) {
            if (*pressedNum >= *min && *pressedNum <= max) {
                return pressedNum;
            } else {
                printf("Wrong input! Pleasy try again.\n");
            }
        } else {
            printf("Wrong input! Pleasy try again and input a number.\n");
            // Clear the input buffer
            while (getchar() != '\n');
        }
    }
}

int *handeSelectProduct(bool canGoBack){
    int *pressedNum = (int *)malloc(sizeof(int));
    int *result = (int *)malloc(sizeof(int));
    while (1) {
        printf("\nEnter an product iD: ");
        *result = scanf("%d", pressedNum);

        if (*result == 1) {
            if (canGoBack && *pressedNum == 0) {
                int *discard_code = (int *)malloc(sizeof(int));
                *discard_code = 0;
                return discard_code;
            }
            else if (*pressedNum == -1) {
                int *stop_code = (int *)malloc(sizeof(int));
                *stop_code = -1;
                return stop_code;
            }

            bool *product_found = (bool *)malloc(sizeof(bool));
            *product_found = false;
            for (int i = 0; i < product_count; i++) {
                if (products[i].id == *pressedNum) {
                    *product_found = true;
                    break;
                }
            }

            if (*product_found) {
                return pressedNum;
            }
            else {
                printf("\nProduct with ID %d not found.\n", *pressedNum);
            }
        }
        else {
            printf("Wrong input! Pleasy try again and input a number.\n");
            // Clear the input buffer
            while (getchar() != '\n');
        }
    }
}

int *handeSelectCategory(){
    int *pressedNum = (int *)malloc(sizeof(int));
    int *result = (int *)malloc(sizeof(int));
    while (1) {
        printf("\nCategory ID: ");
        *result = scanf("%d", pressedNum);

        if (*result == 1) {
            bool *category_found = (bool *)malloc(sizeof(bool));
            *category_found = false;
            for (int i = 0; i < category_count; i++) {
                if (categories[i].id == *pressedNum) {
                    *category_found = true;
                    break;
                }
            }

            if (*category_found) {
                return pressedNum;
            }
            else {
                printf("\nCategory with ID %d not found.\n", *pressedNum);
            }
        }
        else {
            printf("Wrong input! Pleasy try again and input a number.\n");
            // Clear the input buffer
            while (getchar() != '\n');
        }
    }
}

void showShopWelcomeScreen() {
    printf(":::::::::::::::::::Welcome to webshop:::::::::::::::::::\n\n");

    printf("Select number of menu item to continue: \n");
    printf("1 - Administrator Login\n");
    printf("2 - Browse shop\n");
    printf("------------------------\n");
    printf("0 - Close app\n\n");
    printf("Select item:");
    WelcomeMenu welcomeMenu;
    char entry[10];
    fgets(entry, sizeof(entry), stdin);
    int option;
    if (sscanf(entry, "%d", &option) == 1) {
        welcomeMenu = (WelcomeMenu)option;
        switch (welcomeMenu) {
        case EXIT:
            printf("Closing app");
            break;
        case showAdministrationLoginScreen:
            showAdministrationLoginScreen();
            break;

        case showShopProductsMenuScreen:
            showShopProductsMenuScreen();
            break;
        }
    }
}

void showAdministrationLoginScreen() {
    printf(":::::::::::::::::::Administrator login:::::::::::::::::::\n\n");

    char *username = (char *)malloc(1000 * sizeof(char));
    char *password = (char *)malloc(1000 * sizeof(char));

    while (1) {
        printf("Username: ");
        scanf("%s", username);
        printf("\nPassword: ");
        scanf("%s", password);


        if (strcmp(username, ADMIN_USER) == 0 && strcmp(password, ADMIN_PASS) == 0) {
            free(username);
            free(password);
            showMainAdministrationPage();
            break;
        }
        else {
            printf("Authentication failed, try again!\n\n");
        }
    }
}

void showCart(int *selected_product_ids, int *selected_product_size) {
    printf(":::::::::::::::::::Shopping cart:::::::::::::::::::\n\n");

    double *total_price;
    total_price = (double *)malloc(sizeof(double));
    char selected_products_titles[300];

    printf("\nSelected products:\n");

    for (int i = 0; i < *selected_product_size; i++) {
        Product product = getProductById(selected_product_ids[i]);
        strcat(selected_products_titles, product.title);
        if (i<*selected_product_size-1) {
            strcat(selected_products_titles, " | ");
        }
        printf("%d. %s - %.2fE\n", i+1, product.title, product.price);
        *total_price += product.price;
    }
    printf("----------------------------------\n");
    printf("Total price: %.2fE", *total_price);

    printf("\nPlease enter your order data");

    char *first_name;
    char *last_name;
    char *address;
    char *timestamp;

    size_t buffer_size = 100;

    // Allocate memory for the address
    first_name = (char *)malloc(buffer_size * sizeof(char));
    last_name = (char *)malloc(buffer_size * sizeof(char));
    address = (char *)malloc(buffer_size * sizeof(char));

    printf("\nFirst name: ");
    scanf("%49s", *&first_name);

    printf("\nLast name: ");
    scanf("%49s", *&last_name);

    getchar();

    printf("\nAddress: ");
    scanf("%99[^\n]", *&address);

    timestamp = get_current_timestamp();

    Order* order = (Order*)malloc(sizeof(Order));
    if (order == NULL) {
        perror("Memory allocation failed");
        exit(0);
    }
    strcpy(order->timestamp, timestamp);
    strcpy(order->first_name, first_name);
    strcpy(order->last_name, last_name);
    strcpy(order->address, address);
    strcpy(order->products, selected_products_titles);
    order->total_price = *total_price;


    createOrder(order);

    // Free allocated memory
    free(first_name);
    free(last_name);
    free(address);
    free(timestamp);
    free(total_price);

    printf("\nOrder successfully created!\n");
    showShopWelcomeScreen();
}

void showShopProductsMenuScreen() {
    printf(":::::::::::::::::::Webshop products:::::::::::::::::::\n\n");

    if (product_count == 0) {
        printf("All products are sold, nothing to show!\n");
        showShopWelcomeScreen();
    }

    printf("Select products ids to put them in the cart: \n");
    for (int i = 0; i < product_count; i++) {
        Category category = getCategoryById(products[i].category_id);
        printf("%d - [%s] %s - %.2fE\n", products[i].id, category.title, products[i].title, products[i].price);
    }
    printf("------------------------\n");
    printf("0 - Go back\n\n");
    printf("Select items (type -1 to stop or 0 to discard):");

    int *selected_product_ids = NULL;
    int *selected_product_size = (int *)malloc(sizeof(int));
    int capacity = 1;
    int num;
    *selected_product_size = 0;

    // Allocate initial memory
    selected_product_ids = (int *)malloc(capacity * sizeof(int));
    if (selected_product_ids == NULL) {
        perror("Memory allocation failed");
        exit(0);
    }

    while(1) {
        int *result = (int *)malloc(sizeof(int));
        result = handeSelectProduct(true);

        if (*result == 0) {
            free(result);
            showShopWelcomeScreen();
        }
        else if (*result == -1) {
            free(result);
            showCart(selected_product_ids, selected_product_size);
            break;
        }

       if (*selected_product_size == capacity) {
            capacity *= 2;
            int *temp = (int *)realloc(selected_product_ids, capacity * sizeof(int));
            if (temp == NULL) {
                perror( "Memory reallocation failed\n");
                free(selected_product_ids); // Free the previously allocated memory before exiting
                exit(0);
            }
            selected_product_ids = temp;
        }

        selected_product_ids[*selected_product_size] = *result;
        (*selected_product_size)++;
    }

}

void showMainAdministrationPage() {
    printf(":::::::::::::::::::Administration area:::::::::::::::::::\n\n");

    printf("Select number of menu item to continue: \n");
    printf("1 - View orders\n");
    printf("2 - Add new product\n");
    printf("3 - Update product\n");
    printf("4 - Delete product\n");
    printf("------------------------\n");
    printf("0 - Logout\n\n");
    printf("Select item:");
    AdminMenu adminMenu;
    char entry[10];
    fgets(entry, sizeof(entry), stdin);
    int option;
    if (sscanf(entry, "%d", &option) == 1) {
        adminMenu = (AdminMenu)option;
        switch (adminMenu) {
        case logOut:
            printf("You are successfully logged out!\n");
            showShopWelcomeScreen();
            break;

        case showOrdersScreen:
            showOrdersScreen();
            break;

        case showAddProductScreen:
            showAddProductScreen();
            break;

        case showUpdateProductScreen:
            showUpdateProductScreen();
            break;

        case showDeleteProductScreen:
            showDeleteProductScreen();
            break;
        }
    }
}

void showOrdersScreen() {
    printf(":::::::::::::::::::Placed orders:::::::::::::::::::\n\n");

    if (order_count == 0) {
        printf("There are no orders!\n");
    }
    else {
        for (int i = 0; i < order_count; i++) {
            printf(
                "%d. %s %s %s, %s, Products: %s, Total price - %.2fE\n",
                i+1,
                orders[i].timestamp,
                orders[i].first_name,
                orders[i].last_name,
                orders[i].address,
                orders[i].products,
                orders[i].total_price
            );
        }
    }
    showMainAdministrationPage();
}

void showAddProductScreen() {
    printf(":::::::::::::::::::Add new product:::::::::::::::::::\n\n");
    // Allocate memory for the address
    char *product_title = (char *)malloc(100 * sizeof(char));
    int *product_category_id = (int *)malloc(sizeof(int));
    double *price = (double *)malloc(sizeof(double));

    getchar();
    printf("\nProduct title: ");
    scanf("%99[^\n]", *&product_title);
    getchar();
    printf("\nCategories:\n");
    for (int i = 0; i < category_count; i++) {
        printf("%d - %s\n", categories[i].id, categories[i].title);
    }
    product_category_id = handeSelectCategory();
    getchar();

    int *price_validation_result = (int *)malloc(sizeof(int));
    while (1) {
        printf("\nPrice: ");
        *price_validation_result = scanf("%lf",*&price);

        if (*price_validation_result == 1) {
            break;
        } else {
            printf("Wrong input! Pleasy try again and input a valid price.\n");
            // Clear the input buffer
            while (getchar() != '\n');
        }
    }

    Product* product = (Product*)malloc(sizeof(Product));
    if (product == NULL) {
        perror("Memory allocation failed");
        exit(0);
    }

    product->id = getNewProductId();
    strcpy(product->title, product_title);

    product->category_id = *product_category_id;
    product->price = *price;

    createProduct(product);

    printf("\nProduct successfully created!\n");
    showMainAdministrationPage();
}

void showEditProductTitle(int product_id) {
    printf(":::::::::::::::::::Edit product title:::::::::::::::::::\n\n");

    char *product_title = (char *)malloc(100 * sizeof(char));
    printf("New product title: ");
    getchar();
    scanf("%99[^\n]", *&product_title);

    for (int i = 0; i < product_count; i++) {
        if (products[i].id == product_id) {
            strncpy(products[i].title, product_title, sizeof(products[i].title) - 1);
            // Ensure null-termination
            products[i].title[sizeof(products[i].title) - 1] = '\0';
        }
    }

    updateProductsInFile();
    free(product_title);
    printf("\nProduct title succesfully updated!\n");
    showMainAdministrationPage();
}

void showEditProductCategory(int product_id) {
    printf(":::::::::::::::::::Edit product category:::::::::::::::::::\n\n");

    printf("\nAvailable categories:\n");
    for (int i = 0; i < category_count; i++) {
        printf("%d - %s\n", categories[i].id, categories[i].title);
    }

    int *product_category_id = (int *)malloc(100 * sizeof(int));
    getchar();
    product_category_id = handeSelectCategory();

    for (int i = 0; i < product_count; i++) {
        if (products[i].id == product_id) {
            products[i].category_id = *product_category_id;
        }
    }

    updateProductsInFile();
    free(product_category_id);
    printf("\nProduct title succesfully updated!\n");
    showMainAdministrationPage();
}

void showEditProductPrice(int product_id) {
    printf(":::::::::::::::::::Edit product Price:::::::::::::::::::\n\n");

    double *price = (double *)malloc(sizeof(double));
    int *price_validation_result = (int *)malloc(sizeof(int));

    while (1) {
        printf("\nPrice: ");
        getchar();
        *price_validation_result = scanf("%lf",*&price);

        if (*price_validation_result == 1) {
            break;
        } else {
            printf("Wrong input! Pleasy try again and input a valid price.\n");
            // Clear the input buffer
            while (getchar() != '\n');
        }
    }

    for (int i = 0; i < product_count; i++) {
        if (products[i].id == product_id) {
            products[i].price = *price;
        }
    }

    updateProductsInFile();
    free(price);
    free(price_validation_result);
    printf("\nProduct price succesfully updated!\n");
    showMainAdministrationPage();
}

void showDeleteProductScreen() {
    printf(":::::::::::::::::::Delete product:::::::::::::::::::\n\n");

    if (product_count == 0) {
        printf("No products are added!\n");
        showMainAdministrationPage();
    }

    printf("Select product ID you want to delete \n");
    for (int i = 0; i < product_count; i++) {
        Category category = getCategoryById(products[i].category_id);
        printf("%d - [%s] %s - %.2fE\n", products[i].id, category.title, products[i].title, products[i].price);
    }

    int *selected_product_id = (int *)malloc(sizeof(int));
    selected_product_id = handeSelectProduct(false);

    Product selected_product = getProductById(*selected_product_id);
    int *pressedNum = (int *)malloc(sizeof(int));
    int *result = (int *)malloc(sizeof(int));

    printf("\nSelected product to delete: %d - %s\n", selected_product.id, selected_product.title);
    printf("Press 1 to delete or type anything else to discard ");

    getchar();
    *result = scanf("%d", pressedNum);

    if (*result == 1) {
        if (*pressedNum == 1) {
            // delete product
            for (int i = 0; i < product_count; i++) {
                if (products[i].id == *selected_product_id) {
                    deleteProduct(i);
                }
            }
        }
    }
    else {
        showMainAdministrationPage();
        while (getchar() != '\n');
    }
    free(pressedNum);
    free(result);
    updateProductsInFile();
    printf("\nProduct succesfully deleted!\n");
    showMainAdministrationPage();
}

void showUpdateProductScreen(){
    printf(":::::::::::::::::::Update product:::::::::::::::::::\n\n");

    if (product_count == 0) {
        printf("No products are added!\n");
        showMainAdministrationPage();
    }

    printf("Select product ID you want to edit \n");
    for (int i = 0; i < product_count; i++) {
        Category category = getCategoryById(products[i].category_id);
        printf("%d - [%s] %s - %.2fE\n", products[i].id, category.title, products[i].title, products[i].price);
    }

    int *selected_product_id = (int *)malloc(sizeof(int));
    selected_product_id = handeSelectProduct(false);

    Product selected_product = getProductById(*selected_product_id);

    printf("\nSelected product: %d - %s\n", selected_product.id, selected_product.title);
    printf("Select what you want to update\n");
    printf("1 - Title\n");
    printf("2 - Category\n");
    printf("3 - Price\n");
    printf("------------------------\n");
    printf("0 - Exit\n\n");

    printf("Select item:");

    UpdateProductMenu updateProductMenu;
    char entry[10];
    fgets(entry, sizeof(entry), stdin);
    int option;
    if (sscanf(entry, "%d", &option) == 1) {
        updateProductMenu = (UpdateProductMenu)option;
    switch(updateProductMenu) {
        case backToAdminPage:
            showMainAdministrationPage();
            break;

        case showEditProductTitle:
            showEditProductTitle(*selected_product_id);
            free(selected_product_id);
            break;

        case showEditProductCategory:
            showEditProductCategory(*selected_product_id);
            free(selected_product_id);
            break;

        case showEditProductPrice:
            showEditProductPrice(*selected_product_id);
            free(selected_product_id);
            break;
    }

}