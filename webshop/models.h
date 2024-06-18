#ifndef MODELS_H
#define MODELS_H

typedef struct {
    int id;
    char title[50];
} Category;

typedef struct {
    int id;
    int category_id;
    char title[50];
    double price;
} Product;

typedef struct {
    char timestamp[20];
    char first_name[20];
    char last_name[20];
    char address[50];
    char products[100];
    double total_price;
} Order;

#endif // MODELS_H