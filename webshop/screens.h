#ifndef SCREENS_H
#define SCREENS_H

#include <stdbool.h>
#include "models.h"

// screen navigation handler
int *handleStaticMenuInput(int max, bool canGoBack);
int *handeSelectProduct(bool canGoBack);
int *handeSelectCategory();

//home menu
void showShopWelcomeScreen();
void showAdministrationLoginScreen();

// customer menu
void showShopProductsMenuScreen();
void showCart(int *selected_product_ids, int *selected_product_size);

//administration menu
void showMainAdministrationPage();
void showAddProductScreen();
void showOrdersScreen();
void showUpdateProductScreen();
void showEditProductTitle(int product_id);
void showDeleteProductScreen();
void showEditProductCategory(int product_id);
void showEditProductPrice(int product_id);

#endif // SCREENS_H