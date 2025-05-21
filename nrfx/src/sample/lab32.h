#ifndef LAB32_H
#define LAB32_H

#include "lab32.h"

#include "list.h"
void populate_list(List *list, int size);
int *populate_array(int size);
int is_sorted_array(int *arr, int size);
int is_sorted_list(const List list);
void swap_nodes(List *head, Node *first_node);
void bubble_sort_list(List *list);
void cocktail_shaker_sort(List *list);
void quick_sort(int arr, int lower, int upper);
void merge_sort(int array[], int lower, int upper);
void insertion_sort(int arr[], int size);
void selection_sort(int arr[], int size);

#endif