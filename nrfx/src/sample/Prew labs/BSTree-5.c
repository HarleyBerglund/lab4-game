#include <assert.h>
#include "BSTree-5.h"
#include <stdlib.h>
#include <math.h>

/*Det är helt tillåtet att lägga till egna hjälpfunktioner men de befintliga funktionerna ska implementeras enligt instruktionerna*/

/* Statiska hjälpfunktioner används av andra funktioner i trädet och ska inte ligga i interfacet (användaren behöver inte känna till dessa) */

/* Skapar en trädnod med det givna datat. Denna funktion bör vara den enda som använder malloc för att skapa en ny treeNode*/
static struct treeNode* create_tree_node(int data)
{
    // glöm inte att kolla så att malloc inte returnerade NULL
	//  Allokera minne för en ny nod
    struct treeNode* new_tree_node = (struct treeNode*)malloc(sizeof(struct treeNode));

	// Kontrollera att malloc inte returnerade NULL
    if (!new_tree_node) return NULL;

	// Tilldela data till den nya noden
	new_tree_node->data = data;
	new_tree_node->left = NULL;
	new_tree_node->right = NULL;

	return new_tree_node;
}



static void write_sorted_helper(const BSTree tree, int* array, int* index) {
    if (tree != NULL) {
		// Rekursivt traversera vänster delträd
        write_sorted_helper(tree->left, array, index);
		// Spara nodens data i arrayen
        array[(*index)++] = tree->data;
		// Rekursivt traversera höger delträd
        write_sorted_helper(tree->right, array, index);
    }
}

/* Returnerar en dynamiskt allokerad array som innehåller trädets data sorterat */
static int* write_sorted_to_array(const BSTree tree)
{
    /* Skapa en dynamisk array med rätt storlek
    
    Skriv datat från trädet sorterat till arrayen (minsta till största)
       - till detta kanske du behöver en hjälpfunktion */
	// Allokerar minne för en array av storlek number_of_nodes(tree)

    if (is_empty(tree)) return NULL;
    int* array = (int*)malloc(sizeof(int) * number_of_nodes(tree));
	// Kontrollera att malloc inte returnerade NULL
    if (!array) return NULL;
    int index = 0;
    write_sorted_helper(tree, array, &index);
    return array;

}

/* Bygger upp ett sorterat, balanserat träd från en sorterad array */
static void build_tree_sorted_from_array(BSTree* tree, const int arr[], int size)
{
    /* Bygg rekursivt från mitten.
    Mittenelementet i en delarray skapar rot i delträdet
    Vänster delarray bygger vänster delträd
    Höger delarray bygger höger delträd*/
    if (size <= 0) return;
    int mid = size/2;

    insert_sorted(tree, arr[mid]);

	build_tree_sorted_from_array(&((*tree)->left), arr, mid);
	build_tree_sorted_from_array(&((*tree)->right), arr + mid + 1, size - mid - 1);
}

/* Implementation av trädet, funktionerna i interfacet */

/* Skapar ett tomt träd - denna funktion ar färdig */
BSTree create_empty_tree(void)
{
	return NULL;
}

/* Returnerar 1 ifall trädet är tomt, 0 annars */
int is_empty(const BSTree tree)
{
    return (tree == NULL);
}

/* Sätter in 'data' sorterat i *tree */
void insert_sorted(BSTree* tree, int data)
{
	/*Tänk på att trädet kan vara tomt vid insättning
    I vanliga fall kan man lösa dubletter på olika sätt. Eftersom vi vill att era lösningar
    fungerar likadant inför examinationen så bestämmer jag att dubletter ej ska tillåtas i trädet.
    Post-condition kan verifieras med hjälp av find(...)*/
    if (*tree == NULL) {
        *tree = create_tree_node(data);
        return;
    }
    if (data < (*tree)->data) insert_sorted(&((*tree)->left), data);
    else if (data > (*tree)->data) insert_sorted(&((*tree)->right), data);
}

/* Utskriftsfunktioner
Det räcker att ni implementerar LR-ordningarna*/
void print_preorder(const BSTree tree)
{
    char msg[100];
    if (is_empty(tree)) {
        strcpy(msg, "Empty");
        uarte_write(msg, strlen(msg));
        return;
    }

    // Otherwise, tree is not NULL
    send_int(tree->data);
    strcpy(msg, " ");
    uarte_write(msg, strlen(msg));
    if (tree->left != NULL) print_preorder(tree->left);
    if (tree->right != NULL) print_preorder(tree->right);
}


void print_inorder(const BSTree tree)
{
    char msg[100];
    if (is_empty(tree)) {
        strcpy(msg, "Empty");
        uarte_write(msg, strlen(msg));
        return;
    }


    // Otherwise, tree is not NULL
    if (tree->left != NULL) print_inorder(tree->left);
    send_int(tree->data);
    strcpy(msg, " ");
    uarte_write(msg, strlen(msg));
    if (tree->right != NULL) print_inorder(tree->right);
}

void print_postorder(const BSTree tree)
{
    char msg[100];
    if (is_empty(tree)) {
        strcpy(msg, "Empty");
        uarte_write(msg, strlen(msg));
        return;
    }

    if (tree->left != NULL) print_postorder(tree->left);
    if (tree->right != NULL) print_postorder(tree->right);
    send_int(tree->data);
    strcpy(msg, " ");
    uarte_write(msg, strlen(msg));
}


/* Returnerar 1 om 'data' finns i tree, 0 annars */
int find(const BSTree tree, int data)
{
    // Tänk pa att trädet kan vara tomt
    if (is_empty(tree)) return 0;
    if (tree->data == data) return 1;
    else if (data < tree->data) return find(tree->left, data);
    else return find(tree->right, data);
}

/* Tar bort 'data' från trädet om det finns */
void remove_element(BSTree* tree, int data)
{
    /* Inget data ska/kan tas bort från ett tomt träd
    Tre fall: Ett löv (inga barn), ett barn (vänster eller höger), två barn

    Glöm inte att frigöra noden när den länkats ur trädet'*/

    if (is_empty(tree)) return;
    // Om datan är mindre än nodens data, gå till vänster barn
    if (data < (*tree)->data) {
        remove_element(&((*tree)->left), data);
    }
    // Om datan är större än nodens data, gå till höger barn
    else if (data > (*tree)->data) {
        remove_element(&((*tree)->right), data);
    }
    // Datan hittades i trädet
    else {
        // Noden är ett löv, frigör den
        if ((*tree)->left == NULL && (*tree)->right == NULL) {
            free(*tree);
            *tree = NULL;
        }
        // Noden har ett barn i höger ben
        else if ((*tree)->left == NULL) {
            BSTree temp = *tree;
            *tree = (*tree)->right;
            free(temp);
        }
        // Noden har ett barn i vänster ben
        else if ((*tree)->right == NULL) {
            BSTree temp = *tree;
            *tree = (*tree)->left;
            free(temp);
        }
		else {
			// Noden har två barn
			// Hitta den minsta noden i höger delträd
			BSTree min_node = (*tree)->right;
			while (min_node->left != NULL) {
				min_node = min_node->left;
			}
			// Ersätt nodens data med den minsta nodens data
			(*tree)->data = min_node->data;
			// Ta bort den minsta noden från höger delträd
			remove_element(&((*tree)->right), min_node->data);
		}
        
    }
}


/* Returnerar hur många noder som totalt finns i trädet */
int number_of_nodes(const BSTree tree)
{   
    if (is_empty(tree)) return 0;
	// Beräknar antalet noder i vänster och höger delträd rekursivt till basfallet uppnås
    return 1 + number_of_nodes(tree->left) + number_of_nodes(tree->right);

}

/* Returnerar hur djupt trädet är */
int depth(const BSTree tree)
{
    if (is_empty(tree)) return 0;
	// Beräknar djupet av vänster och höger delträd rekursivt till basfallet uppnås
    int left_depth = depth(tree->left);
    int right_depth = depth(tree->right);
	// Returnerar djupet av trädet som är max av vänster eller höger delträd + 1
    return 1 + fmax(left_depth,right_depth);
}

/* Returnerar minimidjupet för trädet
Se math.h för användbara funktioner*/
int min_depth(const BSTree tree)
{
    if (is_empty(tree)) return 0;
    // Noden är ett löv
    if (tree->left == NULL && tree->right == NULL) return 1;
    // Vänster barn är NULL -> minimum beror då på höger barn
    if (tree->left == NULL) return 1 + min_depth(tree->right);
    // Höger barn är NULL -> minimum beror då på vänster barn
    if (tree->right == NULL) return 1 + min_depth(tree->left);
	// Båda barnen finns -> minimum beror på det minsta av vänster och höger barn
    int left_min = min_depth(tree->left);
    int right_min = min_depth(tree->right);
    // Väljer det minsta av de två
    return 1 + fmin(left_min,right_min);

}

/* Balansera trädet så att depth(tree) == minDepth(tree) */
void balance_tree(BSTree* tree)
{
    /* Förslag på algoritm:
    - överfor trädet till en dynamiskt allokerad array med writeSortedToArray()
    - töm trädet med freeTree()
    - bygg upp trädet rekursivt från arrayen med buildTreeSortedFromArray()
    - frigör minne för den dynamiskt allokerade arrayen

    Post-conditions:
    - tree har lika många noder som tidigare
      - djupet för trädet är samma som minimumdjupet för trädet */
	int n = number_of_nodes(*tree);

	// Skapa en sorterad array från trädet
	int* array = write_sorted_to_array(*tree);  
	// Friar det gamla trädet
    free_tree(tree);
    *tree = NULL;
	// Bygger ett nytt träd från den sorterade arrayen
	build_tree_sorted_from_array(tree, array, n);
	// Frigör minnet för den sorterade arrayen
    free(array);
}



/* Töm trädet och frigör minnet för de olika noderna */
void free_tree(BSTree* tree)
{
    if (*tree != NULL) {
		// Rekursivt frigör vänster och höger delträd
        free_tree(&((*tree)->left));
        free_tree(&((*tree)->right));
		// Frigör minnet för den aktuella noden
        free(*tree);
        *tree = NULL;
    }
}


