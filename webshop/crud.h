#ifndef CRUD_H
#define CRUD_H
#include "models.h"

typedef enum {
	EXIT=0,
	showAdministrationLoginScreen,
	showShopProductsMenuScreen
} WelcomeMenu;

typedef enum {
	logOut = 0,
	showOrdersScreen,
	ShowAddProductScreen,
	showUpdateProductScreen,
	showDeleteProductScreen
} AdminMenu;

typedef enum {
	backToAdminPage = 0,
	showEditProductTitle,
	showEditProductCategory,
	showEditProductPrice,
} UpdateProductMenu;

extern Category* categories;
extern Product* products;
extern Order* orders;
extern int category_count;
extern int product_count;
extern int order_count;

int compare_ints(const void *a, const void *b);
Category getCategoryById(int category_id);
Product getProductById(int product_id);
char* get_current_timestamp();
void loadCategories();
void loadProducts();
void loadOrders();
void createOrder(Order *order);
void createProduct(Product *product);
char* list_to_string(int* list, int size);
int getNewProductId();
inline int findBiggestNumber(int*, int);
void updateProductsInFile();
void deleteProduct(int index);
void deleteOrder();

#endif // CRUD_H