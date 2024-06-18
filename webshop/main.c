#include "screens.h"
#include "crud.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    // load data from files
    loadCategories();
    loadProducts();
    loadOrders();

    // enter menu
    showShopWelcomeScreen();

    // free memory
    free(categories);
    free(products);
    return 0;
}