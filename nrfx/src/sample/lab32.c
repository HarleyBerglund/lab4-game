#include "lab32.h"


void populate_list(List *list, int size)
{
    set_random_seed();

    for (int i = 0; i < size; i++)
    {
        int random = get_random_number(1, size);
        add_last(list, random);
    }
}

int *populate_array(int size)
{
    set_random_seed();

    int *arr = malloc(size * sizeof(int));
    for (int i = 0; i < size; i++)
    {
        arr[i] = get_random_number(1, size);
    }

    return arr;
}

int is_sorted_array(int *arr, int size)
{
    for (int i = 0; i < size - 1; i++)
    {
        if (arr[i] >= arr[i + 1] && arr[i] != arr[i + 1] )
        {
            return 0;
        }
    }
    return 1;
}

int is_sorted_list(const List list)
{
    // The list is empty or the end is reached and the list is sorted
    if (list == NULL || list->next == NULL)
        return 1;
    // Checking the upcomming values in the list recursively
    if (list->data <= list->next->data)
        return is_sorted_list(list->next);
    // If the values arent sorted, return 0
    else
        return 0;
}

void print_array(int *arr, int size)
{
    char dot[] = ". ";
    char newline[] = "\n\r";
    for (int i = 0; i < size; i++)
    {
        send_int(i + 1);
        uarte_write(dot, strlen(dot));
        send_int(arr[i]);
        uarte_write(newline, strlen(newline));
    }
}

void swap_nodes(List *head, Node *first_node)
{
    if (first_node == NULL || first_node->next == NULL)
    {
        return;
    }

    Node *second_node = first_node->next;
    Node *prev_node = first_node->previous;
    Node *next_node = second_node->next;

    // Länka prev_node till second_node
    if (prev_node != NULL)
    {
        prev_node->next = second_node;
    }


    // Länka next_node till first_node
    if (next_node != NULL)
    {
        next_node->previous = first_node;
    }

    // Byt first_node och second_node
    second_node->previous = prev_node;
    second_node->next = first_node;
    first_node->previous = second_node;
    first_node->next = next_node;

    if (*head == first_node) 
    {
        *head = second_node;
    }
}

void bubble_sort_list(List *list)
{
    // Checking if the list is empty
    if (list == NULL || *list == NULL)
        return;

    int swapped;
    // Marks the end of the unsorted region of the list.
    Node *end = NULL;

    // Loop that runs untill the list is sorted (nothing swapped)
    do
    {
        swapped = 0;           // Resetting swap var
        Node *current = *list; // List head

        // Inner loop goes through the unsorted portion of the list
        while (current->next != end)
        {
            // If current node data > next node data, swap them
            if (current->data > current->next->data)
            {
                swap_nodes(list, current);
                swapped = 1; // Swap was made
            }
            else
            {
                current = current->next; // The nodes where already in the correct order, move on to next
            }
        }
        // The largest element of the current unsorted list is now in its correct position (smaller then last end), so mark it as new end
        end = current;
    } while (swapped);
}

void cocktail_shaker_sort(List *list)
{
    // Checking if the list is empty
    if (list == NULL || *list == NULL)
        return;

    int swapped;
    // Marks the end and start of the unsorted region of the list.
    Node *start = *list;
    Node *end = NULL;

    do
    {
        swapped = 0;
        Node *current = start;

        // Forward pass
        while (current->next != end)
        {
            // If current node data > next node data, swap them
            if (current->data > current->next->data)
            {
                swap_nodes(list, current);
                swapped = 1; // Swap was made
            }
            else
            {
                current = current->next; // The nodes where already in the correct order, move on to next node
            }
        }

        // If no swap was made during the forward pass -> the list is sorted
        if (!swapped)
            break;

        swapped = 0; // Reset for backward pass
        // The largest element of the current unsorted list is now in its correct position (smaller then last end), so mark it as new end
        end = current;

        // Backward pass
        while (current->previous != NULL && current != start)
        {
            // If current node data < previous node data, swap them
            if (current->data < current->previous->data)
            {
                swap_nodes(list, current->previous);
                swapped = 1; // Swap was made
            }
            else
            {
                current = current->previous; // The nodes where already in the correct order, move on to previous node
            }
        }

        // After the backward pass, the first sorted node is 'current', mark this as the start of the unsorted portion
        start = current;

    } while (swapped);
}

void quick_sort(int arr, int lower, int upper)
{
    if (lower < upper)
    {
        int pivot_location = quick_sort_helper(arr, lower, upper);//chooses pivot location
        quick_sort(arr, lower, pivot_location - 1);//swaps places on the left side
        quick_sort(arr, pivot_location + 1, upper);//swaps places on the right side
    }
}

int quick_sort_helper(int arr[], int lower, int upper)
{
    int pivot_point = arr[upper];//defines the pivot point
    int j = lower - 1;
    for (int i = lower; i < upper; i++)
    {
        if (arr[i] <= pivot_point)
        {
            //swaps places
            j++;
            int temp = arr[j];
            arr[j] = arr[i];
            arr[i] = temp;
        }
    }

    //swaps places
    int temp = arr[j + 1];
    arr[j + 1] = arr[upper];
    arr[upper] = temp;
    return j + 1;
}



void merge_sort(int array[], int lower, int upper)
{
    if (lower < upper) {
        int mid = lower + (upper - lower) / 2;
        // Sort first and second halves
        merge_sort(array, lower, mid);
        merge_sort(array, mid + 1, upper);
        merge_helper(array, lower, mid, upper);
    }
}

void merge_helper(int arr[], int lower, int mid, int upper)
{
    int i, j, k;
    int lower_size = mid - lower + 1;
    int upper_size = upper - mid;

    // Createing temp arrays
    int lower_array[lower_size], upper_array[upper_size];

    // Copy to temp arrays
    for (i = 0; i < lower_size; i++)
    {
        lower_array[i] = arr[lower + i];
    }
        
    for (j = 0; j < upper_size; j++)
    {
        upper_array[j] = arr[mid + 1 + j];
    }
        
    // Merge the temp arrays back into array[lower..upper]
    i = 0;
    j = 0;
    k = lower;
    while (i < lower_size && j < upper_size) 
    {
        if (lower_array[i] <= upper_array[j]) 
        {
            arr[k] = lower_array[i];
            i++;
        }
        else {
            arr[k] = upper_array[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of lower_array
    while (i < lower_size) 
    {
        arr[k] = lower_array[i];
        i++;
        k++;
    }

    // Copy the remaining elements of upper_array
    while (j < upper_size) 
    {
        arr[k] = upper_array[j];
        j++;
        k++;
    }
}

void insertion_sort(int arr[], int size)
{
    for (int i = 1; i < size; i++)
    {
       int current = arr[i];//defines which element we are on
       int previous = i - 1;//defines the previous element

       while (previous >= 0 && arr[previous] > current) //loops thorugh all the elemets untill the unsorted pile is empty and the sorted pile is full
       {
           //swaps place of the data
           arr[previous + 1] = arr[previous];
           previous = previous - 1;
       }
       //goes  to next element
       arr[previous + 1] = current;

    }
}

void selection_sort(int arr[], int size) 
{
    for (int current = 0; current < size - 1; current++) 
    {
        int minimum_value = current;//making assumtion that our current value is the smallest

        for (int next = current + 1; next < size; next++) 
        {
            //checks if the next value is bigger than the current minimum value
            if (arr[next] < arr[minimum_value]) 
            {
                minimum_value = next;
            }
        }
        //sets the correct positions
        int temp = arr[current];
        arr[current] = arr[minimum_value];
        arr[minimum_value] = temp;
    }
}


