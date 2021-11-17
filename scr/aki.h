/**
 * Akinator game library. Created as part of the course of
 * I.R.Dedinsky as the 6th project in 2021. The game is a
 * clone of the original game [https://ru.akinator.com/]
 * where the genie guesses the hidden character
 */

/**
 * The library implements such functions as finding the desired
 * element in the akinator tree, expanding the tree, saving
 * progress.
 * 
 * in the future, we will probably create a window in which a
 * dialogue with this akinator will be implemented
 */
#ifndef AKI_H
#define AKI_H

#include <stdio.h>

#include "stack.h"

/**
 * @brief akinator tree node it contains
 * akinator's node data
 * 
 * @param parent Ptr to previous Akinator node
 * @param left   Ptr to left Ak. node
 * @param right  Ptr to right Ak. node
 */
#define MAX_DATA_LEN 100
#define MAX_WORD_LEN  32

const char STD_DATA_FILE[] = "data.txt";
const char STD_LVL_DIR[]   = ".lvls";
const char DUMP_FILE[]     = ".visual/dump_file.dot";
const char VIZ_FILE[]      = ".visual/graph.png";
const char AKI_EXT[]       = "tree";


struct Line {
  char *ptr = nullptr;
};

struct NamesLvl {
  Line *names = nullptr;
  size_t capacity = 0;
  size_t size     = 0;
};

struct Node {
    char data[MAX_DATA_LEN]  = "";

    int node_index = 0;

    Node* parent   = nullptr;

    Node* left     = nullptr;
    Node* right    = nullptr;

    bool is_endian = 0;
};

/**
 * @brief Main structure of the akinator tree
 * 
 * @param root Ptr of the root Akinator's tree
 */
struct Tree {
	Node *root      = nullptr;

    int index = 0;
    FILE *dump_file = nullptr;
    FILE *data_file = nullptr;
    char name_of_data_file[32] = {};

	Node *cur_node  = nullptr;
};

void chose_data_file (Tree *tree, NamesLvl *lvls);

size_t find_lvls (const char *dir, NamesLvl *Levels);

/**
 * @brief Selects mode of Aki-program
 * 
 * @param tree ptr to Akinator's tree
 */
void select_mode (Tree* tree);

/**
 * @brief Creates New node
 * 
 * @param tree Ptr to Akinator's tree
 */
Node *new_node (Tree* tree);

/**
 * @brief Creates Akinator's tree to use it
 * for questions
 * 
 * @param tree Ptr to Akinator's tree
 */
void tree_ctor (Tree *tree, NamesLvl *lvls);

/**
 * @brief Reads data from file
 * 
 * @param tree Ptr to Akinator's tree
 * @param txt  Ptr to file with data
 */
void get_data (Tree *tree, FILE *file);

/**
 * @brief Create akinator's graph
 * 
 * @param tree Ptr to Akinator's tree
 */
void create_graph (Tree *tree);

/**
 * @brief Gets player's answer to Akinator's
 * question
 */
bool get_ans ();

/**
 * @brief Loads Akinator's main game
 * 
 * @param tree Ptr to Akinator's tree
 */
void load_game (Tree *tree);

/**
 * @brief Save recursively player's and akinators game progress
 * 
 * @param tree     Ptr to akinator's tree
 * @param node     Ptr to akinator's last node in it's tree
 * @param data_tx  Ptr to file that contains akinator's tree data
 */
void print_node (Node *node, FILE *file);

/**
 * @brief Recoursively write one node of akinator's tree
 * 
 * @param tree Ptr to Akinator's tree
 * @param graph_file file.dot
 * @param index - node index to know how to connect nodes in graph
 */
void write_tree (Node *node, FILE *graph_file, int *index);

/**
 * @brief Recoursively connect three nodes of akinator's tree
 * 
 * @param tree Ptr to Akinator's tree
 * @param graph_file file.dot
 * @param index - node index to know how to connect nodes in graph
 */
void connect_tree (Node *node, FILE *graph_file, int *index);

/**
 * @brief search node in tree which contains needed data
 * 
 * @param tree Ptr to Akinator's tree
 * @param saving_node contains ptr to needed node
 */
void search_object (Node *node, char *data, Stack *stack, Node **saving_node);

/**
 * @brief Create path of object's data
 * 
 * @param tree Ptr to Akinator's tree
 * @param saving_node contains ptr to needed node
 */
void create_path (Node *node, Stack *stack);

/**
 * @brief Write data, which belong to object
 * 
 * @param stack Ptr to stack with data
 * @param node contains ptr to node
 */
void print_path (Stack *stack, Node *node);

/**
 * @brief Compare two objects, create which way is the node
 * 
 * @param tree Ptr to Akinator's tree
 * @param saving_node contains ptr to needed node
 */
void compare_objects (Tree *tree, Stack *stack, Stack *stack2);

/**
 * @brief Printf difference between two objects in tree
 * 
 * @param tree Ptr to Akinator's tree
 * @param saving_node contains ptr to needed node
 */
void print_difference (int *ptr1, int *ptr2, char *find1, Node *node1, char *find2, Node *node2);



#endif//AKI_H