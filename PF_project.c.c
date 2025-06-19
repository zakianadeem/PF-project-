#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure for customer
struct customer {
    char name[30];
    char contact_number[20];
    char purchasing_date[20];
};

// Function for reading data from customer
void read_data(const char *filename, struct customer *info) {
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        printf("Error: Could not open file %s for writing.\n", filename);
        return;
    }
    //Validate customer name
    while (1) {
        int valid = 1, i;
        printf("Enter customer name (alphabets only, max 30 characters): ");
        scanf(" %[^\n]", info->name); // Allows spaces in name

        // Validate that the name contains only alphabets or spaces
        for (i = 0; info->name[i] != '\0'; i++) {
            if ((info->name[i] < 'A' || (info->name[i] > 'Z' && info->name[i] < 'a') || info->name[i] > 'z') && info->name[i] != ' ') {
                valid = 0;
                break;
            }
        }

        if (valid) break; // Name is valid
        printf("Invalid name. Please enter a valid name with alphabets only.\n");
    }
   // Validate contact number
    while (1) {
        int valid = 1, i;
        printf("Enter contact number (digits only, max 15 characters): ");
        scanf("%s", info->contact_number);

        // Check if the contact number contains only digits
        for (i = 0; info->contact_number[i] != '\0'; i++) {
            if (info->contact_number[i] < '0' || info->contact_number[i] > '9') {
                valid = 0;
                break;
            }
        }

        if (valid && strlen(info->contact_number) <= 15) break; // Contact number is valid
        printf("Invalid contact number. Please enter digits only.\n");
    }
    // Validate purchasing date
    while (1) {
        int valid = 1, i;
        printf("Enter purchasing date (YYYY-MM-DD): ");
        scanf("%s", info->purchasing_date);

        // Check if the date format is correct
        if (strlen(info->purchasing_date) != 10 ||
            info->purchasing_date[4] != '-' ||
            info->purchasing_date[7] != '-') {
            valid = 0;
        } else {
            // Check each character in the YYYY-MM-DD format
            for (i = 0; i < 10; i++) {
                if ((i != 4 && i != 7) && (info->purchasing_date[i] < '0' || info->purchasing_date[i] > '9')) {
                    valid = 0;
                    break;
                }
            }
        }

        if (valid) break; // Purchasing date is valid
        printf("Invalid date. Please enter the date in YYYY-MM-DD format.\n");
    }

    fwrite(info, sizeof(struct customer), 1, file);
    printf("\nCustomer record saved successfully.\n");
    fclose(file);
}

// Function for generating a discount code
char *discount_code(char name[], int key) {
    int len = strlen(name), i;
    char *discount_code = (char *)malloc((len + 1) * sizeof(char));
    if (discount_code == NULL) {
        printf("Memory allocation failed");
        return NULL;
    }
    for (i = 0; i < len; i++) {
        discount_code[i] = (((name[i] ^ key) + 95) % 95) + 32;
    }
    discount_code[len] = '\0';
    return discount_code;
}

// Function for printing the bill
void print_bill(FILE *file, struct customer info, float bill, float discounted_price, float saved) {
    fseek(file, 0, SEEK_SET);
    if (file == NULL) {
        printf("Error: Could not open file for reading.\n");
        return;
    }
    fread(&info, sizeof(struct customer), 1, file);
    printf("Customer Name   : %s\n", info.name);
    printf("Contact Number  : %s\n", info.contact_number);
    printf("Purchasing Date : %s\n", info.purchasing_date);
    printf("Total Bill      : %.2f\n", bill);
    printf("Discounted Price: %.2f\n", discounted_price);
    printf("Saved Amount    : %.2f\n", saved);
}

// Function for displaying menu
void menu() {
    printf("-------Select from menu and enter a-h---------\n");
    printf("Fruits\n");
    printf("a. Banana      -- 100/kg\n");
    printf("b. Apples      -- 200/kg\n");
    printf("c. Oranges     -- 100/kg\n");
    printf("d. Strawberry  -- 150/kg\n");
    printf("Vegetables\n");
    printf("e. Tomatoes    -- 100/kg\n");
    printf("f. Potatoes    -- 150/kg\n");
    printf("g. Ladyfinger  -- 200/kg\n");
    printf("h. Cauliflower -- 200/kg\n");
    printf("\n................................................\n");
}

// Structure for order
struct order {
    int order_id;
    char order_name[50];
    char order_date[20];
    float total_amount;
    char order_status[20];
};

// Function for searching order ID
void searchOrder(struct order data[], int size, int id, int index) {
    if (index >= size) {
        printf("Invalid ID!!\n");
        return;
    }
    if (data[index].order_id == id) {
        printf("\nOrder Details:\n");
        printf("Order ID      : %d\n", data[index].order_id);
        printf("Name          : %s\n", data[index].order_name);
        printf("Order Date    : %s\n", data[index].order_date);
        printf("Total Amount  : %.2f\n", data[index].total_amount);
        printf("Order Status  : %s\n", data[index].order_status);
        printf("--------------------------------------\n");
        return;
    }
    searchOrder(data, size, id, index + 1); //Recursive function call
}

int main() {
    struct customer info;
    int quantity, i, n, bill = 0;
    int a = 100, b = 200, c = 100, d = 150, e = 100, f = 150, g = 200, h = 200;
    char *discount, code[20];

    printf(".............Welcome to Customer-Seller Services..............\n");
    printf("Enter 1 if you are a customer and 2 if you are a seller:\n"); //mode selection for seller and customer
    scanf("%d", &n);

    switch (n) {
        case 1: {
            menu();
            printf("Enter number of items to purchase: ");
            scanf("%d", &quantity);

            if(quantity <= 0) //if no items are purchased
            {
            	printf("No items to purchase!Exiting customer mode!!\n");
            	break;
			}

            char items[quantity];
            printf("Enter a-h of respective product:\n");
            for (i = 0; i < quantity; i++) {
                printf("Enter item %d: ", i + 1);
                scanf(" %c", &items[i]);
                if (!(items[i] >= 'a' && items[i] <= 'h')) { //Validate correct selection from menu
                    printf("Invalid selection of menu! Enter again\n");
                    i--;
                    continue;
                }
            }

            const char *filename = "sales.txt";
            read_data(filename, &info); //function call read data

            printf("\nWe are offering 50 percent discount by giving a discount code!\n");
            int key = 50;
            discount = discount_code(info.name, key); //function call for discount code
            if (discount != NULL) {
                printf("%s", discount);
            }

            while ((getchar()) != '\n');
            printf("\nEnter discount code: ");
            fgets(code, sizeof(code), stdin);
            code[strcspn(code, "\n")] = '\0';

            if (strcmp(code, discount) == 0) { //Validate discount code
                for (i = 0; i < quantity; i++) {
                    switch (items[i]) {
                        case 'a': bill += a; break;
                        case 'b': bill += b; break;
                        case 'c': bill += c; break;
                        case 'd': bill += d; break;
                        case 'e': bill += e; break;
                        case 'f': bill += f; break;
                        case 'g': bill += g; break;
                        case 'h': bill += h; break;
                        default: printf("Invalid item: %c\n", items[i]); break;
                    }
                }
                float saved = 0.50 * bill;
                float discounted_price = bill - saved;
                printf("\n--------Bill---------\n");
                FILE *file = fopen(filename, "w+");
                if (file == NULL) {
                    printf("Error");
                    return 0;
                }
                print_bill(file, info, bill, discounted_price, saved);
                fclose(file);
            } else {
                printf("Incorrect discount code!\n"); //Prompt for incorrect discount code
            }
            free(discount);
            break;
        }
        case 2: {
            int id, n;
            struct order data[] = { //initializing order data
                {1001, "Ali", "20-11-2024", 1000, "In process"},
                {1006, "Ahmed", "23-11-2024", 2000, "Delivered"},
                {1008, "Sara", "25-11-2024", 1500, "Dispatched"},
                {1004, "John", "10-11-2024", 900, "Delivered"},
                {1007, "Ayesha", "15-11-2024", 600, "In process"}
            };
            while (1) {
            	printf("\n--------------SELECT MODE-------------\n"); //Displaying modes for sellers
                printf("Enter 1-3\n1. Searching order ID\n2. Calculating profit/loss\n3. Exit\n");
                scanf("%d", &n);

                if (n == 1) { //searching order by ID
                    while (1) {
                        printf("Enter order ID (1001-1010)(0 for exit): ");
                        scanf("%d", &id);
                        if (id == 0) break;
                        else if (id < 1001 || id > 1010) { //Validate correct ID
                            printf("Order ID does not match!\n");
                            continue;
                        }
                        searchOrder(data, 5, id, 0);
                    }
                } else if (n == 2) { //calculating profit in sales of fruit/vegetables:
                    float fruits[2], vegetables[2], sum1 = 0, sum2 = 0;
                    for (i = 0; i < 2; i++) {
                        printf("Enter sales of year %d of fruits: ", i + 1);
                        scanf("%f", &fruits[i]);
                        sum1 += fruits[i];
                        printf("Enter sales of year %d of vegetables: ", i + 1);
                        scanf("%f", &vegetables[i]);
                        sum2 += vegetables[i];
                    }
                    float diff, total = sum1 + sum2, percent;
                    if (sum1 > sum2) { //for fruits:
                        printf("Sales of fruits remain profitable among both years!\n");
                        diff = sum1 - sum2;
                        percent = (diff / sum1) * 100;
                        printf("Loss in fruits as compared to vegetables: %.2f\n", diff);
                        printf("Profit in fruits is %.2f%%\n", percent);
                    }
					else if (sum2 > sum1) { //for vegetables:
                        printf("Sales of vegetables remain profitable among both years!\n");
                        diff = sum2 - sum1;
                        percent = (diff / sum2) * 100;
                        printf("Loss in vegetables as compared to fruits: %.2f\n", diff);
                        printf("Profit in vegetables is %.2f%%\n", percent);
                    }
                    else if(sum1 == sum2) //to check equal profit
                    printf("Sales of fruits and vegetables are equal!\nNo profit margin!!\n");

                    if(sum1==0 || sum2==0) //to check profit when one quantity is zero
                    {
                    	printf("1 category had zero sales!!\n");
                    	continue;
					}

                } else if (n == 3) { //exiting from program:
                    printf("Exiting program!!\n");
                    break;
                } else {
                    printf("Invalid input!! Select a valid option\n");
                }
            }
            break;
        }
        default:
        printf("Invalid selection of mode!!\n"); //when mode other than 1 and 2 are selected
        break;
    }
    return 0;
}
