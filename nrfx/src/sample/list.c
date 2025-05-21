#include "list.h"
#include <assert.h>

// Returnera en tom lista - funktionen är färdig
List create_empty_list(void)
{
	return NULL;
}

// Här skapas nya noder. Denna funktion är den enda som ska använda malloc.
// Denna funktion ska i sin tur bara kallas av add_first och add_last.
// Notera att den är static och inte finns med i interfacet
static Node *create_list_node(const Data data)
{
	// glöm inte att kolla så att malloc inte returnerade NULL
	//  Allokera minne för en ny nod
	Node *new_node = (Node *)malloc(sizeof(Node));

	// Kontrollera att malloc inte returnerade NULL
	if (new_node == NULL)
	{
		return NULL;
	}

	// Tilldela data till den nya noden
	new_node->data = data;
	new_node->next = NULL;
	new_node->previous = NULL;

	return new_node;
}

// Är listan tom?
// Returnerar 1 om listan är tom, annars 0
int is_empty(const List list)
{
	if (list == NULL)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

// Lägg till en nod först i listan
void add_first(List *list, const Data data)
{
	Node *new_node = create_list_node(data);
	if (new_node == NULL)//returnar om noden är tom
	{
		return;
	}
	new_node->next = *list; // Nya nodens next-pekare pekar på originallistan
	if (*list != NULL)
	{
		(*list)->previous = new_node; // Låter previous pekaren i originallistan peka på den nya noden.
	}
	*list = new_node;
}

// Lägg till en nod sist i listan
void add_last(List *list, const Data data)
{
	Node *new_node = create_list_node(data);
	if (new_node == NULL)
	{
		return;
	}

	if (*list == NULL)
	{
		// Listan är tom, sätt den nya noden som huvud
		*list = new_node;
	}
	else
	{
		Node *current = *list;
		while (current->next != NULL) // Iterera till slutet av listan
		{
			current = current->next;
		}
		// Lägg till den nya noden sist och peka om next och previous
		current->next = new_node;
		new_node->previous = current;
	}
}

// Ta bort första noden i listan
// precondition: listan är inte tom (testa med assert)
void remove_first(List *list)
{
	assert(*list != NULL);
	Node *to_remove = *list;
	*list = (*list)->next; // Peka om listan till andra elementet
	if (*list != NULL)
	{
		(*list)->previous = NULL; // Sätter det nya första elementets previous-pekare till NULL
	}

	free(to_remove);
}

// ta bort sista noden i listan
// precondition: listan är inte tom (testa med assert)
void remove_last(List *list)
{
	assert(*list != NULL);
	Node *to_remove = *list;
	Node *new_last = NULL;

	while (to_remove->next != NULL) // Iterera till slutet på listan
	{
		to_remove = to_remove->next;
	}

	new_last = to_remove->previous;

	if (new_last != NULL)
	{
		// Uppdatera den nya sista nodens next-pekare
		new_last->next = NULL;
	}
	else
	{
		// Om det endast finns en nod blir listan tom
		*list = NULL;
	}

	free(to_remove);
}

// töm listan (ta bort alla noder ur listan)
// postcondition: Listan är tom, *list är NULL
void clear_list(List *list)
{
	// Alla noder ska frigöras
	Node *current = *list;
	Node *next_node;

	while (current != NULL) // Iterera current till lista noden och ta bort och frigör minnet för alla noder
	{
		next_node = current->next;
		free(current);
		current = next_node;
	}
	// Sätter listan till NULL
	*list = NULL;
}

// Skriv ut listan genom UART
void print_list(const List list)
{
	Node *current = list;
	static int i = 1;

    if (list != NULL) {
        char dot[] = ". ";
        char newline[] = "\n\r";

        send_int(i);
        uarte_write(dot, strlen(dot));
        send_int(list->data);
        uarte_write(newline, strlen(newline));

        i++;
        print_list(list->next);
    } else {
        i = 1;
    }
}

// returnera första datat i listan
// precondition: listan är inte tom (testa med assert)
Data get_first_element(const List list)
{
	assert(list != NULL);
	return list->data;
}

// returnera sista datat i listan.
// precondition: listan är inte tom (testa med assert)
Data get_last_element(const List list)
{
	assert(list != NULL);

	Node *current = list;

	while (current->next != NULL)
	{
		current = current->next;
	}
	return current->data;
}

// Returnera hur många noder som finns i listan
int number_of_nodes(const List list)
{
	int num_nodes = 1;	
	//basfall
	if(is_empty(list)){
		return 0;
	}
	else if(list->next == NULL){
		return num_nodes;
	}
	else if(list->next != NULL){
		num_nodes += number_of_nodes(list->next);
	}
}

// Sök efter data i listan, returnera 1 om datat finns, annars 0.
int search(const List list, const Data data)
{
	Node *current = list;

	//basfall
	if(current->data == data){
		return 1;
	}
	else if(current->next != NULL) {
		search(current->next, data);
	}
	else{
		return 0;
	}
}

// Ta bort data ur listan (första förekomsten), returnera 0 om datat inte finns, annars 1
int remove_element(List *list, const Data data)
{
	Node *current = list;
	while (current != NULL)
	{
		if (current->data == data) // Om datan i den aktuella noden är = datan vi vill ta bort (första förekomsten)
		{
			if (current == *list) // Om noden är den första i listan
			{
				// Flytta listans början till nästa nod och sätt previous till NULL (ny första nod)
				*list = current->next;
				if (*list != NULL)
				{
					(*list)->previous = NULL;
				}
			}
			else
			{
				if (current->previous != NULL)
				{
					current->previous->next = current->next; // Föregående nodens next-pekare ställs om att peka på noden efter noden som ska tas bort
				}
				if (current->next != NULL)
				{
					current->next->previous = current->previous; // Efterföljande nodens previous-pekare ställs om till att peka på den nod som ligger för noden som tas bort
				}
			}
			free(current); // Frigör minnet för den bortagna noden
			return 1;
		}
		current = current->next;
	}

	return 0;
}
