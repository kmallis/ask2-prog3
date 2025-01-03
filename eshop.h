#ifndef ESHOP_H
#define ESHOP_H


#include "eshop.c" 

// functions

void RequestOrder(int i, Product *catalog);


void ProcessOrder(Product *catalog, int *total_requests, int *successful_orders, int *failed_orders, float *total_revenue);


void PrintReport(Product *catalog, int total_requests, int successful_orders, int failed_orders, float total_revenue);

#endif 