#ifndef BSTREE_H
#define BSTREE_H

#include <nrfx.h>

/*******************************************************/
/* DVA270 - Interface för binärt sökträd               */
/* Anpassad efter interface från DVA244                */
/* av Stefan Bygde och Caroline Uppsäll                */
/* Vid fel eller frågor, kontakta jonas.larsson@mdu.se */
/*******************************************************/


struct treeNode
{
	int data;
	struct treeNode* left;
	struct treeNode* right;
};

typedef struct treeNode* BSTree;

/* Skapar ett tomt trad */
BSTree create_empty_tree(void);

/* Returnerar 1 ifall trädet är tomt, 0 annars */
int is_empty(const BSTree tree);

/* Sätter in 'data' sorterat i *tree*/
void insert_sorted(BSTree* tree, int data);

/* Utskriftsfunktioner*/
void print_preorder(const BSTree tree);
void print_inorder(const BSTree tree);
void print_postorder(const BSTree tree);

/* Returnerar 1 om 'data' finns i tree, 0 annars */
int find(const BSTree tree, int data);

/* Tar bort 'data' från trädet om det finns */
void remove_element(BSTree* tree, int data);

/* Returnerar hur många noder som totalt finns i trädet */
int number_of_nodes(const BSTree tree);

/* Returnerar hur djupt trädet är */
int depth(const BSTree tree);

/* Returnerar minimidjupet för trädet */
int min_depth(const BSTree tree);

/* Balansera trädet sa att depth(tree) == min_depth(tree) */
void balance_tree(BSTree* tree);

/* Töm trädet och frigör minnet för de olika noderna */
void free_tree(BSTree* tree);

#endif
