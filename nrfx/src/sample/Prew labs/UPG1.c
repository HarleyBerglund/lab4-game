#include "goodFuncs.h"
#include "list.h"

int main(void)
{
	init_stuff();
	int8_t clearscreen[] = CLEAR_SCREEN;
	uarte_write(clearscreen, sizeof(clearscreen));

	List *head = create_empty_list();

	while (1)
	{	
		char msg[100];
		char menu[14][50] = {"=== MENU ===\n\r", "1. Insert at beginning\n\r", "2. Insert at end\n\r", "3. Check if list is empty\n\r", "4. Remove first element\n\r", "5. Remove last element\n\r","6. Clear list\n\r", "7. Print list\n\r", "8. Get first element\n\r","9. Get last element\n\r", "10. Get number of nodes\n\r", "11. Search for element\n\r","12. Remove specific element\n\r", "Enter your choice: "};
  
		for (int i = 0; i < 14; i++)
		{
			uarte_write(menu[i], strlen(menu[i]));
		}
			
		int choice = read_int();
		

		switch (choice)
		{
		case 1:
			uarte_write(clearscreen, sizeof(clearscreen));
			strcpy(msg, "Enter data to insert at the beginning: ");
			uarte_write(msg, strlen(msg));
			Data new_data1 = read_int();
			add_first(&head, new_data1);
			strcpy(msg, "Data added at the beginning.\n\r");
			uarte_write(msg, strlen(msg));
			break;
		case 2:
			uarte_write(clearscreen, sizeof(clearscreen));
			strcpy(msg, "Enter data to insert at the end: ");
			uarte_write(msg, strlen(msg));
			Data new_data2 = read_int();
			add_last(&head, new_data2);
			strcpy(msg, "Data added at the end.\n\r");
			uarte_write(msg, strlen(msg));
			break;
		case 3:
			uarte_write(clearscreen, sizeof(clearscreen));
			if (is_empty(head))
				strcpy(msg, "The list is empty.\n\r");
			else
				strcpy(msg, "The list is not empty.\n\r");
			uarte_write(msg, strlen(msg));
			break;
		case 4:
			uarte_write(clearscreen, sizeof(clearscreen));
			remove_first(&head);
			strcpy(msg, "First element removed.\n\r");
			uarte_write(msg, strlen(msg));
			break;
		case 5:
			uarte_write(clearscreen, sizeof(clearscreen));
			remove_last(&head);
			strcpy(msg, "Last element removed.\n\r");
			uarte_write(msg, strlen(msg));
			break;
		case 6:
			uarte_write(clearscreen, sizeof(clearscreen));
			clear_list(&head);
			strcpy(msg, "List cleared.\n\r");
			uarte_write(msg, strlen(msg));
			break;
		case 7:
			uarte_write(clearscreen, sizeof(clearscreen));
			strcpy(msg, "Current list contents:\n\r");
			uarte_write(msg, strlen(msg));
			print_list(head);
			break;

		case 8:
			uarte_write(clearscreen, sizeof(clearscreen));
			Data first_element = get_first_element(head);
			char buffer1[64];
			snprintf(buffer1, sizeof(buffer1), "First element: %d\n", first_element);
			uarte_write(buffer1, strlen(buffer1));
			break;
		case 9:
			uarte_write(clearscreen, sizeof(clearscreen));
			Data last_element = get_last_element(head);
			char buffer2[64];
			snprintf(buffer2, sizeof(buffer2), "Last element: %d\n", last_element);
			uarte_write(buffer2, strlen(buffer2));
			break;
		case 10:
			uarte_write(clearscreen, sizeof(clearscreen));
			int num_nodes = number_of_nodes(head);
			char buffer3[64];
			snprintf(buffer3, sizeof(buffer3), "Number of nodes: %d\n", num_nodes);
			uarte_write(buffer3, strlen(buffer3));
			break;
		case 11:
			uarte_write(clearscreen, sizeof(clearscreen));
		strcpy(msg, "Enter data to search for: ");
			uarte_write(msg, strlen(msg));
			Data search_data = read_int();
			if (search(head, search_data))
				strcpy(msg, "Data found in the list.\n\r");
			else
				strcpy(msg, "Data not found in the list.\n\r");
			uarte_write(msg, strlen(msg));
			break;

		case 12:
			uarte_write(clearscreen, sizeof(clearscreen));
			strcpy(msg, "Enter data to remove: ");
			uarte_write(msg, strlen(msg));
			Data index_to_remove = read_int();
			int removed = remove_element(&head, index_to_remove);
			if (removed)
				strcpy(msg, "Data removed from the list.\n\r");
			else
			strcpy(msg, "Data not found in the list.\n\r");
			uarte_write(msg, strlen(msg));
			break;

		default:
			uarte_write(clearscreen, sizeof(clearscreen));
			strcpy(msg, "Invalid choice.\n\r");
			uarte_write(msg, strlen(msg));
			break;
		}
		char temp[100];
		read_string(temp, sizeof(temp));
		uarte_write(clearscreen, sizeof(clearscreen));
	}

	return 0;
}
