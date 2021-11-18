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

//--------------------------------------------------------------------
#ifndef AKI_H
#define AKI_H

#include <stdio.h>

#include "stack.h"
//--------------------------------------------------------------------
#define MAX_DATA_LEN 100
#define MAX_WORD_LEN  32

/**
 * @brief Print's needed data in func: print_node()
 */
#define NODE_DATA(data)         \
  fprintf (file, "{\n");        \
  fprintf (file, "%s\n", data); \
  fprintf (file, "}\n");

//--------------------------------------------------------------------

const char DUMP_FILE[]     = ".visual/dump_file.dot";
const char STD_DATA_FILE[] = "data.txt";
const char STD_LVL_DIR[]   = ".lvls/";
const char AKI_EXT[]       = "tree";
const char VIZ_FILE[]      = ".visual/graph.png";

//--------------------------------------------------------------------
/**
 * @brief Stucture to contain name of file for program
 */
struct Line {
  char *ptr = nullptr;
};

/**
 * @brief Contains levels in directory for akinator's
 * progaram
 * 
 * @param names Ptr to structure that will contain 
 * name of file
 * 
 * @param capacity Max size of storage for names
 * @param size current number of contained files
 */
struct NamesLvl {
  Line *names     = nullptr;
  size_t capacity = 0;
  size_t size     = 0;
};

/**
 * @brief Contain's node data in itself
 * 
 * @param data Content of node data [questinon / answer]
 * 
 * @param node_index Index of node
 * @param parent Ptr to parent node
 * @param left Ptr to left node
 * @param right Ptr to right node
 * @param is_endian Bool param. to undertand does this
 * node have followers
 */
struct Node {
  Node* parent             = nullptr;
  Node* left               = nullptr;
  Node* right              = nullptr;

  char data[MAX_DATA_LEN]  = "";

  int node_index           = 0;

  bool is_endian           = 0;
  bool is_active           = 0;
};

/**
 * @brief Main structure of the akinator tree
 * 
 * @param root Ptr of the root Akinator's tree
 * @param dump_file Ptr to file with graph
 * @param data_file Ptr to file with Akinator's base
 * @param name_of_data_file name of data_file
 * @param cur_node Ptr to current node in tree
 */
struct Tree {
  int index                  = 0;
  FILE *dump_file            = nullptr;
  FILE *data_file            = nullptr;
  char name_of_data_file[32] = {};

	Node *root                 = nullptr;
	Node *cur_node             = nullptr;
};


//-------- Modes -----------------------------------------------------
/**
 * @brief Selects mode of Aki-program
 * 
 * @param tree ptr to Akinator's tree
 */
void select_mode (Tree* tree);

/**
 * @brief User can chose data_base to use it in Akinator
 * 
 * @param tree Ptr to empty created tree
 * @param lvls Ptr to created lvls data
 */
void chose_data_file (Tree *tree, NamesLvl *lvls);

/**
 * @brief Play description (search mode)
 * 
 * @param levels Ptr to NamesLvl data [names of levels]
 * @param tree   Ptr to tree
 */
void description_mode (NamesLvl *levels, Tree *tree);

/**
 * @brief Play description (search mode)
 * 
 * @param levels Ptr to NamesLvl data [names of levels]
 * @param tree   Ptr to tree
 */
void compare_mode (Tree *tree, NamesLvl *levels);


//-------- Axilary to modes ------------------------------------------
/**
 * @brief Creates new tree in mode New tree   [for new tree mode]
 */
void create_new_tree ();

/**
 * @brief Push all parents node in stack      [for compare mode]
 */
void push_nodes_in_stack (Node *node, Stack *stack);

/**
 * @brief Skip own nodes in tree              [for compare mode]
 */
void skip_own_nodes (Node *print_node11, Node *print_node21,
                     Stack *stack1, Stack *stack2);

/**
 * @brief Skip own nodes in tree              [for compare mode]
 */
void skip_own_nodes (Node **print_node11, Node **print_node21,
                     Stack *stack1, Stack *stack2);

/**
 * @brief Print own nodes in tree             [for compare mode]
 */
void print_own_nodes (Stack *stack1, Stack *stack2,
                      Node *print_node11, Node *print_node12,
                      Node *print_node21, Node *print_node22);

/**
 * @brief Print remaining nodes               [for compare mode]
 */
void print_last_nodes (Stack *stack,
                       Node *node1, Node *node2,
                       int index);

/**
 * @brief Find all levels with special extencion in
 * derectory                                  [for P, C, D mode]
 * 
 * @param Levels Ptr to NamesLvl strycture
 * @param dir name of directory
 * 
 * @return number of found levels
 */
size_t find_lvls (const char *dir, NamesLvl *Levels);

/**
 * @brief Save file with akinator's data     [for compare mode]
 */
void save_new_aki_data_file (Tree *tree);

//-------- Main functions --------------------------------------------

/**
 * @brief Adds node to tree                   [for P, N, mode]
 * 
 * @param tree ptr to Akinator's tree
 * @param size [l,r - value] add node in
 * yes/no-direction
 */
void expand_tree (Tree *tree, char side);

/**
 * @brief Change current node data in tree
 * 
 * @param tree        Ptr to Akinator's tree node
 */
void change_node_data (Node *node);

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
 * @brief Gets player's answer to Akinator's
 * question
 */
bool get_ans ();

/**
 * @brief Loads Akinator's main game
 * 
 * @param tree Ptr to Akinator's tree
 */
void load_game (Tree *tree, Stack *stack);

/**
 * @brief Ask user's all question in game
 * 
 * @param tree Ptr to Akinator's tree
 * @param answer_with_assumption_correct Special const to know
 * is answer with IDN answer was right
 * @param stack Ptr to stack with IDN nodes
 */
void answer_aki_question (Tree *tree, Stack *stack,
                          bool *answer_with_assumption_correct);

/**
 * @brief Bad endin of the game
 * 
 * @param tree Ptr to Akinator's tree
 */
void BAD_END (Tree *tree);

/**
 * @brief Happy endin of the game
 * 
 * @param tree Ptr to Akinator's tree
 */
void happy_end (Tree *tree);

/**
 * @brief Save recursively player's and akinators game progress
 * 
 * @param tree     Ptr to akinator's tree
 * @param node     Ptr to akinator's last node in it's tree
 * @param data_tx  Ptr to file that contains akinator's tree data
 */
void print_node (Node *node, FILE *file);

//-------- Graph funcs -----------------------------------------------

/**
 * @brief Create akinator's graph
 * 
 * @param tree Ptr to Akinator's tree
 */
void create_graph (Tree *tree, int active_node);

/**
 * @brief Recoursively write one node of akinator's tree
 * 
 * @param tree Ptr to Akinator's tree
 * @param graph_file file.dot
 * @param index - node index to know how to connect nodes in graph
 */
void write_tree (Node *node, FILE *graph_file, int is_active);

/**
 * @brief Recoursively connect three nodes of akinator's tree
 * 
 * @param tree       Ptr to Akinator's tree
 * @param graph_file file.dot
 * @param index      Node index to know how to connect nodes in graph
 */
void connect_tree (Node *node, FILE *graph_file);

/**
 * @brief Make current node the last one [New tree mode function]
 * 
 * @param tree        Ptr to Akinator's tree node
 */
void tide_node (Node *node);

//-------- Compare, search -------------------------------------------

/**
 * @brief search node in tree which contains needed data
 * 
 * @param tree        Ptr to Akinator's tree
 * @param saving_node Contains ptr to needed node
 */
void search_object (Node *node, char *data, Stack *stack, Node **saving_node);

/**
 * @brief Create path of object's data
 * 
 * @param tree        Ptr to Akinator's tree
 * @param saving_node Contains ptr to needed node
 */
void create_path (Node *node, Stack *stack);

/**
 * @brief Write data, which belong to object
 * 
 * @param stack Ptr to stack with data
 * @param node  Contains ptr to node
 */
void print_path (Stack *stack, Node *searched_node);

/**
 * @brief Compare two objects, create which way is the node
 * 
 * @param tree        Ptr to Akinator's tree
 * @param saving_node contains ptr to needed node
 */
void compare_objects (Tree *tree, Stack *stack, Stack *stack2);

/**
 * @brief Printf difference between two objects in tree
 * 
 * @param tree        Ptr to Akinator's tree
 * @param saving_node contains ptr to needed node
 */
void print_difference (Stack *stack, Node *node1, Stack *stack2, Node *node2);

/**
 * @brief Creates first node in new tree [create new tree mode]
 * 
 * @param tree        Ptr to Akinator's tree
 */
void first_node_in_new_tree (Tree *tree);

/**
 * @brief Expand node to left side in new tree
 * 
 * @param tree        Ptr to Akinator's tree
 * @param new_object  Ptr to new object buffer
 * @param old_object  Ptr to old object that was in tree
 * @param new_data    Ptr to new qestion that will be inserted in tree
 */
void expand_left (Tree *tree, char *new_object, char *old_object, char *new_data);

/**
 * @brief Expand node to right side in new tree
 * 
 * @param tree        Ptr to Akinator's tree
 * @param new_object  Ptr to new object buffer
 * @param old_object  Ptr to old object that was in tree
 * @param new_data    Ptr to new qestion that will be inserted in tree
 */
void expand_right (Tree *tree, char *new_object, char *old_object, char *new_data);

//-------- End of .h file! -------------------------------------------

#endif//AKI_H