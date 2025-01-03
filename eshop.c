#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>

//static variables
#define CATALOG_SIZE 10
#define NUM_CUSTOMERS 5
#define NUM_ORDERS 10

//product structure
typedef struct {
    char description[50];
    float price;
    int item_count;
    int requests;
    int sold;
    char failed_customers[NUM_CUSTOMERS * 10]; // 50 in total orders
} Product;

//declare pipes globally so they are accessible by all functions
int SERVER_PIPE[NUM_CUSTOMERS][2];
int CLIENT_PIPE[NUM_CUSTOMERS][2];

//---------------------------------------------------------------------------------------------------------------------------------------------------------

void RequestOrder(int i, Product *catalog) {

    close(SERVER_PIPE[i][0]); //close read from server
    close(CLIENT_PIPE[i][1]); //close write from client

    srand(time(NULL) ^ (getpid() << 16)); //initialize srand

    //for the total number of orders
    for (int j = 0; j < NUM_ORDERS; j++) {

        int product_index = rand() % CATALOG_SIZE;  //random selection for the product of the order based on the time scale
        write(SERVER_PIPE[i][1], &product_index, sizeof(product_index));  //send the product of the order to server

        char response[100];
        read(CLIENT_PIPE[i][0], response, sizeof(response));  //read the response from server
        printf("Customer %d: %s\n", i + 1, response);

        sleep(1); //wait 1 second until the next order
    }

    close(SERVER_PIPE[i][1]);  //close write from server
    close(CLIENT_PIPE[i][0]);  //close read from client
    exit(EXIT_SUCCESS); // Success
}


void ProcessOrder(Product *catalog, int *total_requests, int *successful_orders, int *failed_orders, float *income) {

    for (int i = 0; i < NUM_CUSTOMERS; i++) {
        for (int j = 0; j < NUM_ORDERS; j++) {

            //read the product of the order from client
            int product_index;
            read(SERVER_PIPE[i][0], &product_index, sizeof(product_index));

            char response[100];
            (*total_requests)++;   //a request was made

            //if there is quantity of this product, the order is successful
            if (product_index >= 0 && product_index < CATALOG_SIZE) {

                catalog[product_index].requests++;  //increase the requests of this product

                if (catalog[product_index].item_count > 0) {

                    //this product has availability
                    //decrease the quantity
                    catalog[product_index].item_count--;

                    //increase sold and add the product price to summary
                    catalog[product_index].sold++;
                    *income += catalog[product_index].price;

                    (*successful_orders)++;

                    //update the client for the result of his request
                    snprintf(response, sizeof(response), "Order successful: %s - €%.2f", catalog[product_index].description, catalog[product_index].price);

                }
                else {   //order failed

                    // update the client for the result of his request
                    snprintf(response, sizeof(response), "Order failed: Product unavailable");

                    (*failed_orders)++;

                    //create an array for the customers that their order was failed 
                    char failed_entry[10];
                    snprintf(failed_entry, sizeof(failed_entry), "C%d ", i + 1); // +1 so the clients' id will start from 1 and not 0

                    //for this spesific product in failed customers field add the customer that failed to order 
                    //(cut -> so the client will be added to the string) there is a chance that will be more than one clients that failed to order
                    //we keep in mind that strings have /0 at the end so we need to increase 1 from the total size (overflow)
                    strncat(catalog[product_index].failed_customers, failed_entry, sizeof(catalog[product_index].failed_customers) - strlen(catalog[product_index].failed_customers) - 1);
                }

            }
            else {  // product does not exist

                snprintf(response, sizeof(response), "Order failed: Invalid product");
                (*failed_orders)++;
            }

            //send the request response to client
            write(CLIENT_PIPE[i][1], response, strlen(response) + 1);
            sleep(1); //wait 1 second
        }
    }
}


void PrintReport(Product *catalog, int total_requests, int successful_orders, int failed_orders, float income) {

    printf("\n-------- Report --------\n");

    for (int i = 0; i < CATALOG_SIZE; i++) {    //for all the products of the catalog, print the statistics

        printf("-> %s\n", catalog[i].description);
        printf("  Requests: %d\n", catalog[i].requests);
        printf("  Sold: %d\n", catalog[i].sold);

        if (strlen(catalog[i].failed_customers) > 0) {
            printf("  Unserved Customers: %s\n", catalog[i].failed_customers);
        } else {
            printf("  Unserved Customers: None\n");
        }
    }

    //print the summary report of the e-shop
    printf("\nTotal Requests: %d\n", total_requests);
    printf("Successful Orders: %d\n", successful_orders);
    printf("Failed Orders: %d\n", failed_orders);
    printf("Total Revenue: €%.2f\n", income);
}



int main() {

    Product catalog[CATALOG_SIZE];

    //initialize products
    snprintf(catalog[0].description, sizeof(catalog[0].description), "Product 1");
    catalog[0].price = 20.00;
    catalog[0].item_count = 2;

    snprintf(catalog[1].description, sizeof(catalog[1].description), "Product 2");
    catalog[1].price = 17.99;
    catalog[1].item_count = 2;

    snprintf(catalog[2].description, sizeof(catalog[2].description), "Product 3");
    catalog[2].price = 8.50;
    catalog[2].item_count = 2;

    snprintf(catalog[3].description, sizeof(catalog[3].description), "Product 4");
    catalog[3].price = 12.30;
    catalog[3].item_count = 2;

    snprintf(catalog[4].description, sizeof(catalog[4].description), "Product 5");
    catalog[4].price = 25.00;
    catalog[4].item_count = 2;

    snprintf(catalog[5].description, sizeof(catalog[5].description), "Product 6");
    catalog[5].price = 5.00;
    catalog[5].item_count = 2;

    snprintf(catalog[6].description, sizeof(catalog[6].description), "Product 7");
    catalog[6].price = 30.00;
    catalog[6].item_count = 2;

    snprintf(catalog[7].description, sizeof(catalog[7].description), "Product 8");
    catalog[7].price = 45.50;
    catalog[7].item_count = 2;

    snprintf(catalog[8].description, sizeof(catalog[8].description), "Product 9");
    catalog[8].price = 22.75;
    catalog[8].item_count = 2;

    snprintf(catalog[9].description, sizeof(catalog[9].description), "Product 10");
    catalog[9].price = 18.99;
    catalog[9].item_count = 2;

    //--------------------------------------------------------------------------------

    //initialize summary variables for each product
    for (int i = 0; i < CATALOG_SIZE; i++) {
        catalog[i].requests = 0;
        catalog[i].sold = 0;
        catalog[i].failed_customers[0] = '\0';
    }

    //exit if there is a problem with creating pipes
    for (int i = 0; i < NUM_CUSTOMERS; i++) {

        if (pipe(SERVER_PIPE[i]) == -1 || pipe(CLIENT_PIPE[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }


    //create each customer and fork()
    for (int i = 0; i < NUM_CUSTOMERS; i++) {

        pid_t pid = fork();

        //if pid is negative, we have an error so exit the program
        if (pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        //client side - pid is 0 -> child process
        if (pid == 0) {
            RequestOrder(i, catalog);
        }
    }


    //again close the connections for the customers
    for (int i = 0; i < NUM_CUSTOMERS; i++) {
        close(SERVER_PIPE[i][1]); // Close write
        close(CLIENT_PIPE[i][0]); // Close read
    }

    //initialize the other summary variables
    int total_requests = 0;
    int successful_orders = 0;
    int failed_orders = 0;
    float income = 0.0;


    //Server process
    ProcessOrder(catalog, &total_requests, &successful_orders, &failed_orders, &income);

    //close all connections
    for (int i = 0; i < NUM_CUSTOMERS; i++) {
        close(SERVER_PIPE[i][0]);
        close(CLIENT_PIPE[i][1]);
    }

    //print the report
    PrintReport(catalog, total_requests, successful_orders, failed_orders, income);

    return 0;
}