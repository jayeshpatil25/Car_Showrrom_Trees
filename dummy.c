#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define ORDER 5
#define MAX_KEYS (ORDER - 1)
#define MIN_KEYS (ceil(ORDER / 2) - 1)
#define MODELS 20

//Car structure
typedef struct
{
    int VIN;
    char name[50];
    char color[20];
    double price;
    char fuelType[10];
    char carType[10];
    int customer_id;
    int salesperson_id;
    char soldDate[11];
} Car;

typedef struct SoldCarTreeNode
{
    Car keys[MAX_KEYS];
    struct SoldCarTreeNode *children[ORDER];
    int num_keys;
} SoldCarTreeNode;

typedef struct AvailableCarTreeNode
{
    Car keys[MAX_KEYS];
    struct AvailableCarTreeNode *children[ORDER];
    int num_keys;
} AvailableCarTreeNode;

typedef struct
{
    int id;
    char name[50];
    char mobile[15];
    char address[100];
    int soldCarVIN;
    char registrationNumber[20];
    char payment_type[20];
    float loan_amount;
} Customer;

typedef struct CustomerTreeNode
{
    Customer keys[MAX_KEYS];
    struct CustomerTreeNode *children[ORDER];
    int num_keys;
} CustomerTreeNode;

typedef struct
{
    int id;
    char name[50];
    float salesTarget;
    float salesAchieved;
    float commission;
    SoldCarTreeNode* sold_car_root;
    CustomerTreeNode *customer_root;
} SalesPerson;

typedef struct SalesTreeNode
{
    SalesPerson keys[MAX_KEYS];
    struct SalesTreeNode *children[ORDER];
    int num_keys;
} SalesTreeNode;



typedef struct
{
    char car_model[20];
    int sold_cars;
    int available_cars;
} stock_details;

typedef struct showroom
{
    int showroom_id;
    SalesTreeNode *sales_root;
    AvailableCarTreeNode *available_car_root;
    stock_details stock[MODELS];
};

SoldCarTreeNode *create_node_sold_car()
{
    SoldCarTreeNode *node = (SoldCarTreeNode *)malloc(sizeof(SoldCarTreeNode));
    node->num_keys = 0;
    for (int i = 0; i < ORDER; i++)
    {
        node->children[i] = NULL;
    }
    return node;
}

void traverse_sold_car(SoldCarTreeNode *node)
{
    if (node == NULL)
        return;

    int i;
    for (i = 0; i < node->num_keys; i++)
    {
        if (node->children[0])
        {
            traverse_sold_car(node->children[i]);
        }
        printf("%d ", node->keys[i].VIN);
    }

    // Traverse the last child
    if (node->children[0])
    {
        traverse_sold_car(node->children[i]);
    }
}

void split_child_sold_car(SoldCarTreeNode *parent, int child_index)
{
    SoldCarTreeNode *child = parent->children[child_index];
    SoldCarTreeNode *new_child = create_node_sold_car();
    int mid_index = MAX_KEYS / 2;

    // Copy upper half of child to new_child
    new_child->num_keys = MAX_KEYS - mid_index - 1;
    for (int i = 0; i < new_child->num_keys; i++)
    {
        new_child->keys[i] = child->keys[mid_index + 1 + i];
    }

    // Copy children if not leaf
    if (child->children[0])
    {
        for (int i = 0; i <= new_child->num_keys; i++)
        {
            new_child->children[i] = child->children[mid_index + 1 + i];
        }
    }

    // Reduce keys in original child
    child->num_keys = mid_index;

    // Make space in parent for new child
    for (int i = parent->num_keys; i > child_index; i--)
    {
        parent->children[i + 1] = parent->children[i];
        parent->keys[i] = parent->keys[i - 1];
    }

    // Move middle key to parent and link new child
    parent->children[child_index + 1] = new_child;
    parent->keys[child_index] = child->keys[mid_index];
    parent->num_keys++;
}

void insert_non_full_sold_car(SoldCarTreeNode *node, Car key)
{
    int i = node->num_keys - 1;

    // leaf
    if (node->children[0] == NULL)
    {
        // Shift keys and insert
        while (i >= 0 && key.VIN < node->keys[i].VIN)
        {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->num_keys++;
    }
    else
    {
        // Find child to insert into
        while (i >= 0 && key.VIN < node->keys[i].VIN)
        {
            i--;
        }
        i++; // because you have to go to child. i was key index until now

        // Split if child is full
        if (node->children[i]->num_keys == MAX_KEYS)
        {
            split_child_sold_car(node, i);
            if (key.VIN > node->keys[i].VIN)
            {
                i++;
            }
        }
        insert_non_full_sold_car(node->children[i], key);
    }
}

void insert_sold_car(SoldCarTreeNode **root, Car key)
{
    if (*root == NULL)
    {
        *root = create_node_sold_car();
        (*root)->keys[0] = key;
        (*root)->num_keys = 1;
        return;
    }

    if ((*root)->num_keys == MAX_KEYS)
    {
        SoldCarTreeNode *new_root = create_node_sold_car();
        new_root->children[0] = *root;
        split_child_sold_car(new_root, 0);
        *root = new_root;
    }
    insert_non_full_sold_car(*root, key);
}

SoldCarTreeNode *create_node_available_car()
{
    AvailableCarTreeNode *node = (AvailableCarTreeNode *)malloc(sizeof(AvailableCarTreeNode));
    node->num_keys = 0;
    for (int i = 0; i < ORDER; i++)
    {
        node->children[i] = NULL;
    }
    return node;
}

void traverse_available_car(AvailableCarTreeNode *node)
{
    if (node == NULL)
        return;

    int i;
    for (i = 0; i < node->num_keys; i++)
    {
        if (node->children[0])
        {
            traverse_available_car(node->children[i]);
        }
        printf("%d ", node->keys[i].VIN);
    }

    // Traverse the last child
    if (node->children[0])
    {
        traverse_available_car(node->children[i]);
    }
}

void split_child_available_car(AvailableCarTreeNode *parent, int child_index)
{
    AvailableCarTreeNode *child = parent->children[child_index];
    AvailableCarTreeNode *new_child = create_node_available_car();
    int mid_index = MAX_KEYS / 2;

    // Copy upper half of child to new_child
    new_child->num_keys = MAX_KEYS - mid_index - 1;
    for (int i = 0; i < new_child->num_keys; i++)
    {
        new_child->keys[i] = child->keys[mid_index + 1 + i];
    }

    // Copy children if not leaf
    if (child->children[0])
    {
        for (int i = 0; i <= new_child->num_keys; i++)
        {
            new_child->children[i] = child->children[mid_index + 1 + i];
        }
    }

    // Reduce keys in original child
    child->num_keys = mid_index;

    // Make space in parent for new child
    for (int i = parent->num_keys; i > child_index; i--)
    {
        parent->children[i + 1] = parent->children[i];
        parent->keys[i] = parent->keys[i - 1];
    }

    // Move middle key to parent and link new child
    parent->children[child_index + 1] = new_child;
    parent->keys[child_index] = child->keys[mid_index];
    parent->num_keys++;
}

void insert_non_full_available_car(AvailableCarTreeNode *node, Car key)
{
    int i = node->num_keys - 1;

    // leaf
    if (node->children[0] == NULL)
    {
        // Shift keys and insert
        while (i >= 0 && key.VIN < node->keys[i].VIN)
        {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->num_keys++;
    }
    else
    {
        // Find child to insert into
        while (i >= 0 && key.VIN < node->keys[i].VIN)
        {
            i--;
        }
        i++; // because you have to go to child. i was key index until now

        // Split if child is full
        if (node->children[i]->num_keys == MAX_KEYS)
        {
            split_child_available_car(node, i);
            if (key.VIN > node->keys[i].VIN)
            {
                i++;
            }
        }
        insert_non_full_available_car(node->children[i], key);
    }
}

void insert_available_car(AvailableCarTreeNode **root, Car key)
{
    if (*root == NULL)
    {
        *root = create_node_available_car();
        (*root)->keys[0] = key;
        (*root)->num_keys = 1;
        return;
    }

    if ((*root)->num_keys == MAX_KEYS)
    {
        AvailableCarTreeNode *new_root = create_node_available_car();
        new_root->children[0] = *root;
        split_child_available_car(new_root, 0);
        *root = new_root;
    }
    insert_non_full_available_car(*root, key);
}

CustomerTreeNode *create_node_customer()
{
    CustomerTreeNode *node = (CustomerTreeNode *)malloc(sizeof(CustomerTreeNode));
    node->num_keys = 0;
    for (int i = 0; i < ORDER; i++)
    {
        node->children[i] = NULL;
    }
    return node;
}

void traverse_customer(CustomerTreeNode *node)
{
    if (node == NULL)
        return;

    int i;
    for (i = 0; i < node->num_keys; i++)
    {
        if (node->children[0])
        {
            traverse_customer(node->children[i]);
        }
        printf("%d ", node->keys[i].id);
    }

    // Traverse the last child
    if (node->children[0])
    {
        traverse_customer(node->children[i]);
    }
}

void split_child_customer(CustomerTreeNode *parent, int child_index)
{
    CustomerTreeNode *child = parent->children[child_index];
    CustomerTreeNode *new_child = create_node_customer();
    int mid_index = MAX_KEYS / 2;

    // Copy upper half of child to new_child
    new_child->num_keys = MAX_KEYS - mid_index - 1;
    for (int i = 0; i < new_child->num_keys; i++)
    {
        new_child->keys[i] = child->keys[mid_index + 1 + i];
    }

    // Copy children if not leaf
    if (child->children[0])
    {
        for (int i = 0; i <= new_child->num_keys; i++)
        {
            new_child->children[i] = child->children[mid_index + 1 + i];
        }
    }

    // Reduce keys in original child
    child->num_keys = mid_index;

    // Make space in parent for new child
    for (int i = parent->num_keys; i > child_index; i--)
    {
        parent->children[i + 1] = parent->children[i];
        parent->keys[i] = parent->keys[i - 1];
    }

    // Move middle key to parent and link new child
    parent->children[child_index + 1] = new_child;
    parent->keys[child_index] = child->keys[mid_index];
    parent->num_keys++;
}

void insert_non_full_customer(CustomerTreeNode *node, Customer key)
{
    int i = node->num_keys - 1;

    // leaf
    if (node->children[0] == NULL)
    {
        // Shift keys and insert
        while (i >= 0 && key.id < node->keys[i].id)
        {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->num_keys++;
    }
    else
    {
        // Find child to insert into
        while (i >= 0 && key.id < node->keys[i].id)
        {
            i--;
        }
        i++; // because you have to go to child. i was key index until now

        // Split if child is full
        if (node->children[i]->num_keys == MAX_KEYS)
        {
            split_child_customer(node, i);
            if (key.id > node->keys[i].id)
            {
                i++;
            }
        }
        insert_non_full_customer(node->children[i], key);
    }
}

void insert_customer(CustomerTreeNode **root, Customer key)
{
    if (*root == NULL)
    {
        *root = create_node_customer();
        (*root)->keys[0] = key;
        (*root)->num_keys = 1;
        return;
    }

    if ((*root)->num_keys == MAX_KEYS)
    {
        CustomerTreeNode *new_root = create_node_customer();
        new_root->children[0] = *root;
        split_child_customer(new_root, 0);
        *root = new_root;
    }
    insert_non_full_customer(*root, key);
}

SalesTreeNode *create_node_sales_person()
{
    SalesTreeNode *node = (SalesTreeNode *)malloc(sizeof(SalesTreeNode));
    node->num_keys = 0;
    for (int i = 0; i < ORDER; i++)
    {
        node->children[i] = NULL;
    }
    return node;
}

void traverse_sales_person(SalesTreeNode *node)
{
    if (node == NULL)
        return;

    int i;
    for (i = 0; i < node->num_keys; i++)
    {
        if (node->children[0])
        {
            traverse_sales_person(node->children[i]);
        }
        printf("%d ", node->keys[i].id);
    }

    // Traverse the last child
    if (node->children[0])
    {
        traverse_sales_person(node->children[i]);
    }
}

void split_child_sales_person(SalesTreeNode *parent, int child_index)
{
    SalesTreeNode *child = parent->children[child_index];
    SalesTreeNode *new_child = create_node_sales_person();
    int mid_index = MAX_KEYS / 2;

    // Copy upper half of child to new_child
    new_child->num_keys = MAX_KEYS - mid_index - 1;
    for (int i = 0; i < new_child->num_keys; i++)
    {
        new_child->keys[i] = child->keys[mid_index + 1 + i];
    }

    // Copy children if not leaf
    if (child->children[0])
    {
        for (int i = 0; i <= new_child->num_keys; i++)
        {
            new_child->children[i] = child->children[mid_index + 1 + i];
        }
    }

    // Reduce keys in original child
    child->num_keys = mid_index;

    // Make space in parent for new child
    for (int i = parent->num_keys; i > child_index; i--)
    {
        parent->children[i + 1] = parent->children[i];
        parent->keys[i] = parent->keys[i - 1];
    }

    // Move middle key to parent and link new child
    parent->children[child_index + 1] = new_child;
    parent->keys[child_index] = child->keys[mid_index];
    parent->num_keys++;
}

void insert_non_full_sales_person(SalesTreeNode *node, SalesPerson key)
{
    int i = node->num_keys - 1;

    // leaf
    if (node->children[0] == NULL)
    {
        // Shift keys and insert
        while (i >= 0 && key.id < node->keys[i].id)
        {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->num_keys++;
    }
    else
    {
        // Find child to insert into
        while (i >= 0 && key.id < node->keys[i].id)
        {
            i--;
        }
        i++; // because you have to go to child. i was key index until now

        // Split if child is full
        if (node->children[i]->num_keys == MAX_KEYS)
        {
            split_child_sales_person(node, i);
            if (key.id > node->keys[i].id)
            {
                i++;
            }
        }
        insert_non_full_sales_person(node->children[i], key);
    }
}

void insert_sales_person(SalesTreeNode **root, SalesPerson key)
{
    if (*root == NULL)
    {
        *root = create_node_sales_person();
        (*root)->keys[0] = key;
        (*root)->num_keys = 1;
        return;
    }

    if ((*root)->num_keys == MAX_KEYS)
    {
        SalesTreeNode *new_root = create_node_sales_person();
        new_root->children[0] = *root;
        split_child_sales_person(new_root, 0);
        *root = new_root;
    }
    insert_non_full_sales_person(*root, key);
}

