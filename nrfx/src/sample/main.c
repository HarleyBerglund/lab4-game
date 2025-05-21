#include "goodFuncs.h"
#include "list.h"
#include "lab32.h"


int main(void)
{
    init_stuff();
    int8_t clearscreen[] = CLEAR_SCREEN;
    uarte_write(clearscreen, sizeof(clearscreen));
    // Message buffer
    char msg[100];

    // Creating an empty list and array
    List *list = create_empty_list();
    int *array;
    int array_size = 0;

    while (1)
     {
        uarte_write(clearscreen, sizeof(clearscreen));
        char msg[100];
        char menu[16][50] = {
            "============ MENU ============\n\r",
            "1. Populate List with random data\n\r",
            "2. Populate Array with random data\n\r",
            "3. Clear List\n\r",
            "4. Clear Array\n\r",
            "5. Print List\n\r",
            "6. Print Array\n\r",
            "7. Is List Sorted?\n\r",
            "8. Is Array Sorted?\n\r",
            "9. Bubble Sort List\n\r",
            "10. Cocktail Sort List\n\r",
            "11. Quick Sort Array\n\r",
            "12. Merge Sort Array\n\r",
            "13. Insertion Sort Array\n\r",
            "14. Selection Sort Array\n\r",
            "==============================\n\r"
        };
        for (int i = 0; i < (*(&menu + 1) - menu); i++)
        {
            uarte_write(menu[i], strlen(menu[i]));
        }

        int choice = 0;
        strcpy(msg, "Choice: ");
        uarte_write(msg, strlen(msg));
        choice = read_int();

    switch (choice)
    {
    case 1:
    {
        uarte_write(clearscreen, sizeof(clearscreen));
        if (is_empty(list))
        {
            // Asking the user to input size of list
            int list_size;
            strcpy(msg, "Size of List: ");
            uarte_write(msg, strlen(msg));
            list_size = read_int();

            // Populate list with random values
            populate_list(&list, list_size);
        } else {
            strcpy(msg, "The List is not empty, clear them first to populate again...");
            uarte_write(msg, strlen(msg));
        }
        break;
    }

    case 2:
        uarte_write(clearscreen, sizeof(clearscreen));

        if (array_size == 0)
        {
            // Asking the user to input size of array
            strcpy(msg, "Size of Array: ");
            uarte_write(msg, strlen(msg));
            array_size = read_int();

            // Populate array with random values
            array = populate_array(array_size);
        } else {
            strcpy(msg, "The Array is not empty, clear them first to populate again...");
            uarte_write(msg, strlen(msg));
        }
        break;

    case 3:
    {
        uarte_write(clearscreen, sizeof(clearscreen));
        if (!is_empty(list))
        {
            clear_list(&list);
            strcpy(msg, "The List was cleared...");
            uarte_write(msg, strlen(msg));
        }
        else 
        {
            strcpy(msg, "The List is empty...");
            uarte_write(msg, strlen(msg));
        }
        
        break;
    }

    case 4:
    {
        uarte_write(clearscreen, sizeof(clearscreen));
        if (array_size != 0)
        {
            array_size = 0;
            free(array);
            strcpy(msg, "The Array was cleared...");
            uarte_write(msg, strlen(msg));
        }
        else 
        {
            strcpy(msg, "The List is empty...");
            uarte_write(msg, strlen(msg));
        }
        break;
    }

    case 5:
        if (!is_empty(list))
        {
            uarte_write(clearscreen, sizeof(clearscreen));
            strcpy(msg, "List: \n\r");
            uarte_write(msg, strlen(msg));
            print_list(list);
        }
        else 
        {
            strcpy(msg, "The List is empty...");
            uarte_write(msg, strlen(msg));
        }
        
        break;
    case 6:
        if (array_size != 0)
        {
            uarte_write(clearscreen, sizeof(clearscreen));
            strcpy(msg, "Array: \n\r");
            uarte_write(msg, strlen(msg));
            print_array(array, array_size);
        }
        else 
        {
            strcpy(msg, "The Array is empty...");
            uarte_write(msg, strlen(msg));
        }
        break;
    case 7:
        uarte_write(clearscreen, sizeof(clearscreen));

        if (is_sorted_list(list)) 
        {
            strcpy(msg, "The List is sorted.");
            uarte_write(msg, strlen(msg));
        }
        else
        {
            strcpy(msg, "The List is not sorted.");
            uarte_write(msg, strlen(msg));
        }

        break;
    case 8:
        uarte_write(clearscreen, sizeof(clearscreen));
        if (is_sorted_array(array, array_size)) 
        {
            strcpy(msg, "The Array is sorted.");
            uarte_write(msg, strlen(msg));
        }
        else
        {
            strcpy(msg, "The Array is not sorted.");
            uarte_write(msg, strlen(msg));
        }
        break;
    case 9:
        uarte_write(clearscreen, sizeof(clearscreen));
        strcpy(msg, "Running Bubble Sort on List...");
        uarte_write(msg, strlen(msg));

        nrf_systick_val_clear();
        uint32_t pretime1 = nrf_systick_val_get();
        bubble_sort_list(&list);
        uint32_t posttime1 = nrf_systick_val_get();

        uint32_t sorting_time1 = (pretime1 - posttime1);
        
        strcpy(msg, "\n\n\rTime to Bubble sort: ");
        uarte_write(msg, strlen(msg));
        
        send_int(sorting_time1);

        break;

    case 10:
        uarte_write(clearscreen, sizeof(clearscreen));
        strcpy(msg, "Running Cocktail Sort on List...");
        uarte_write(msg, strlen(msg));
        cocktail_shaker_sort(&list);
        break;

    case 11:
        uarte_write(clearscreen, sizeof(clearscreen));
        strcpy(msg, "Running Quick Sort on Array...");
        uarte_write(msg, strlen(msg));

        nrf_systick_val_clear();
        uint32_t pretime2 = nrf_systick_val_get();
        quick_sort(array, 0, array_size - 1);
        uint32_t posttime2 = nrf_systick_val_get();

        uint32_t sorting_time2 = (pretime2 - posttime2);
        
        strcpy(msg, "\n\n\rTime to Quick sort: ");
        uarte_write(msg, strlen(msg));
        
        send_int(sorting_time2);
        break;
    case 12:
        uarte_write(clearscreen, sizeof(clearscreen));
        strcpy(msg, "Running Merge Sort on Array...");
        uarte_write(msg, strlen(msg));
        merge_sort(array, 0, array_size - 1);
        break;
    case 13:
        uarte_write(clearscreen, sizeof(clearscreen));
        strcpy(msg, "Running Insertion Sort on Array...");
        uarte_write(msg, strlen(msg));
        
        nrf_systick_val_clear();
        uint32_t pretime3 = nrf_systick_val_get();
        insertion_sort(array, array_size);
        uint32_t posttime3 = nrf_systick_val_get();

        uint32_t sorting_time3 = (pretime3 - posttime3);
        
        strcpy(msg, "\n\n\rTime to Insertion sort: ");
        uarte_write(msg, strlen(msg));
        
        send_int(sorting_time3);

        break;
    case 14:
        uarte_write(clearscreen, sizeof(clearscreen));
        strcpy(msg, "Running Selection Sort on Array...");
        uarte_write(msg, strlen(msg));

        nrf_systick_val_clear();
        uint32_t pretime4 = nrf_systick_val_get();
        selection_sort(array, array_size);
        uint32_t posttime4 = nrf_systick_val_get();

        uint32_t sorting_time4 = (pretime4 - posttime4);
        
        strcpy(msg, "\n\n\rTime to Selection sort: ");
        uarte_write(msg, strlen(msg));
        
        send_int(sorting_time4);


        break;
    default:
        strcpy(msg, "Invalid choice, please select 1-14.");
        uarte_write(msg, strlen(msg));
        break;
    }
        strcpy(msg, "\r\n\nPress any key to continue...\n\r");
        uarte_write(msg, strlen(msg));
        printf("\n\nPress any key to continue...\n\r");
        read_int();
    }
    free(array);
    return 0;
}