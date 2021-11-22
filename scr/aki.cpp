/**
 * The program implements many functions, for example, searching for
 * files with the desired extension, playing akinator itself, adding
 * nodes to the tree, searching and much more
 */

/**
 * The library implements such functions as finding the desired
 * element in the akinator tree, expanding the tree, saving
 * progress.
 * 
 * in the future, we will probably create a window in which a
 * dialogue with this akinator will be implemented
 */

/**
 * TXLibushka: with voice!
 */
#define TX_USE_SPEAK
#include "C:\MinGW\TX\TXLib.h"

//--------------------------------------------------------------------
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <io.h>

#include "../file/file_manager.h"
#include "windows.h"
#include "stack.h"
#include "aki.h"
//--------------------------------------------------------------------

static void introduction ()
{
	txSpeak ("vas privetstvuit inostranni pragramma Akinator! ",
	         "Smotrite!");

	printf ("(Chose mode)\n\n");
	printf ("================\n");
	printf ("Akinator program\n");
	printf ("=========================\n");
	printf ("[P] Play game\n");
	printf ("[G] Graph of current tree\n");
	printf ("[C] Compare two objects\n");
	printf ("[N] New tree mode\n");
	printf ("[D] Description of object\n");
	printf ("[Q] Quit\n");
	printf ("=========================\n\n");

	txSpeak ("Akinator mozet otgadat to chto ti dumal, risovat ",
	         "graf,  sravnit, sozdat derevo, nahodit cho-to");
	txSpeak ("esli Akinator vas uze dostal wi mozete vibrat Kwit");

	return;
}

//--------------------------------------------------------------------

void select_mode (Tree* tree)
{
	assert (tree != nullptr);

	NamesLvl levels = {};

	if (find_lvls (STD_LVL_DIR, &levels) == 0) {
		printf ("ERROR! NO AKI LEVELS IN FOLDER\n");

		return;
	}

	introduction ();

	char choice = ' '; 

	while (choice != 'Q') {
		printf ("mode [Q G C N D Q]> ");

		fflush (stdin);
		choice = (char) getchar ();

		switch (choice) {
			case 'P': {
				printf ("Play mode\n");
				printf ("vouvouvouvouvou Akinator nasel takovi urovni. ",
					      "viberete kde akinator budet hotet vas nahitrit");
				
				tree_ctor (tree, &levels);
				create_graph (tree, 0);

				Stack stack = {};
				stack_ctor (&stack, 16);
				load_game (tree, &stack);

				break;
			}

			case 'G': {
				printf ("Graph mode\n");
				tree_ctor (tree, &levels);
				create_graph (tree, 0);

				break;
			}

			case 'C': {
				printf ("Compare mode\n");
				compare_mode (tree, &levels);

				break;
			}

			case 'N': {
				printf ("Creating tree mode\n");
				create_new_tree ();

				break;
			}

			case 'D': {
				printf ("Description mode\n");
				description_mode (&levels, tree);

				break;
			}

			case 'Q': {
				printf ("Bye!\n");

				break;
			}

			default: {
				printf ("incorrect data, try again> ");

				break;
			}
		}
	}

	return;
}

//--------------------------------------------------------------------

void description_mode (NamesLvl *levels, Tree *tree)
{
	assert (levels != nullptr);
	assert (tree   != nullptr);

	tree_ctor (tree, levels);

	create_graph (tree, 0);
	char search_data[MAX_DATA_LEN] = "";

	printf ("we search> ");

	fflush(stdin);
	fgets (search_data, MAX_DATA_LEN, stdin);
	search_data[strlen (search_data) - 1] = '\0';

	Stack stack = {};
	stack_ctor (&stack, 16);

	Node *search_node = nullptr;
	search_object (tree->cur_node, search_data, &stack, &search_node);
	create_graph (tree, 0);
	printf ("\n____[ Desctiption ]___________\n");
	print_path (&stack, search_node);

	stack_dtor (&stack);

	return;
}

//--------------------------------------------------------------------

void search_object (Node *node, char *data, Stack *stack, Node **saving_node)
{
	assert (saving_node != nullptr);
	assert (node        != nullptr);
	assert (data 		!= nullptr);
	assert (stack       != nullptr);

	if (node->left->is_endian != 1) {
		search_object (node->left, data, stack, saving_node);
	}
	else if (strcmp (node->left->data, data) == 0) {
		node->left->is_active = 1;

		stack_push (stack, node->left);
		*saving_node = node->left;

		return;
	}

	if (node->right->is_endian != 1) {
		search_object (node->right, data, stack, saving_node);
	}
	else if (strcmp (node->right->data, data) == 0) {
		node->right->is_active = 1;

		stack_push (stack, node->right);
		*saving_node = node->right;

		return;
	}

	return;
}

//--------------------------------------------------------------------

void print_path (Stack *stack, Node *search_node)
{
	assert (stack != nullptr);

	if (search_node == nullptr) {
		printf ("ERROR! WE HAVEN'T THIS OBJECT IN TREE\n");

		return;
	}

	while (search_node->parent != nullptr) {
		stack_push (stack, search_node->parent);

		search_node = search_node->parent;
	}

	Node *print_node  = nullptr;
	Node *print_node2 = nullptr;
	stack_top (stack, &print_node);
	stack_pop (stack);

	while (stack->size > 0) {
		stack_top (stack, &print_node2);
		if ( strcmp (print_node->right->data, print_node2->data) == 0) {
			printf ("Ne %s ", print_node->data);
		}
		else {
			printf ("%s ", print_node->data);
		}

		print_node = print_node2;

		stack_pop (stack);
	}

	printf ("\n");
}

//--------------------------------------------------------------------

void compare_mode (Tree *tree, NamesLvl *levels)
{
	assert (levels != nullptr);
	assert (tree   != nullptr);

	tree_ctor (tree, levels);
	create_graph (tree, 0);

	char search_data[MAX_DATA_LEN] = "";

	printf ("Object 1]> ");
	fflush (stdin);
	fgets (search_data, MAX_DATA_LEN, stdin);
	search_data[strlen (search_data) - 1] = '\0';

	Stack stack = {};
	stack_ctor (&stack, 16);
	Node *search_node = nullptr;
	search_object (tree->cur_node, search_data, &stack, &search_node);

	printf ("Object 2]> ");
	fflush(stdin);
	fgets (search_data, MAX_DATA_LEN, stdin);
	search_data[strlen (search_data) - 1] = '\0';

	Stack stack2 = {};
	stack_ctor (&stack2, 16);
	Node *search_node2 = nullptr;
	search_object (tree->cur_node, search_data, &stack2, &search_node2);

	create_graph (tree, 0);
	print_difference (&stack, search_node, &stack2, search_node2);

	return; 
}

// PIPETS)) \/ NO COMMENTS PLEASE I WANT TO FIX THIS FUNCTION BUT I
// SHOULD LEARN MATH...
//--------------------------------------------------------------------

void push_nodes_in_stack (Node *node, Stack *stack)
{


	while (node->parent != nullptr) {
		stack_push (stack, node->parent);

		node = node->parent;
	}

	return;
}

//--------------------------------------------------------------------

void skip_own_nodes
	(Node **print_node11, Node **print_node21, Stack *stack1, Stack *stack2)
{
	assert (stack1 != nullptr);
	assert (stack2 != nullptr);

	stack_top (stack1, print_node11);
	stack_pop (stack1);
	stack_top (stack2, print_node21);
	stack_pop (stack2);

	while (strcmp ((*print_node11)->data, (*print_node21)->data) == 0) {
		stack_top (stack1, print_node11);
		stack_pop (stack1);
		stack_top (stack2, print_node21);
		stack_pop (stack2);
	}

	printf ("\ndif. start at node with data: %s\n",
		     (*print_node11)->parent->data);

	return;
}

//--------------------------------------------------------------------

void print_own_nodes (Stack *stack1, Stack *stack2,
											Node *print_node11, Node *print_node12,
											Node *print_node21, Node *print_node22)
{
	assert (stack1       != nullptr);
	assert (stack2       != nullptr);

	while (stack1->size > 0 && stack2->size > 0) {
		stack_top (stack1, &print_node12);

		if (strcmp (print_node11->right->data, print_node12->data) == 0) {
			printf ("1] ne %-20s ", print_node11->data);
		}
		else {
			printf ("1] %-20s    ", print_node11->data);
		}

		print_node11 = print_node12;

		stack_top (stack2, &print_node22);
		if (strcmp (print_node21->right->data, print_node22->data) == 0) {
			printf ("2] ne %-20s\n", print_node21->data);
		}
		else {
			printf ("2] %-20s   \n", print_node21->data);
		}

		print_node21 = print_node22;

		stack_pop (stack1);
		stack_pop (stack2);
	}

	return;
}

//--------------------------------------------------------------------

void print_last_nodes (Stack *stack, Node *node1, Node *node2, int index)
{
	assert (index < 3 && index > 0);
	assert (stack != nullptr);

	char special_insert[] = "\t\t\t   ";

	while (stack->size > 0) {
		stack_top (stack, &node2);
		if ( strcmp (node1->right->data, node2->data) == 0) {
			if (index == 2) {
				printf ("%s%d] ne %-20s\n", special_insert, index, node1->data);
			}
			else {
				printf ("%d] ne %-20s\n", index, node1->data);
			}
		}
		else {
			if (index == 2) {
				printf ("%s%d] %-20s   \n", special_insert, index, node1->data);
			}
			else {
				printf ("%d] %-20s   \n", index, node1->data);
			}
		}

		node1 = node2;

		stack_pop (stack);
	}

	return;
}

//--------------------------------------------------------------------

void print_difference (Stack *stack1, Node *search_node1,
											 Stack *stack2, Node *search_node2)
{
	assert (stack1 != nullptr);
	assert (stack2 != nullptr);

	if (search_node1 == nullptr || search_node2 == nullptr) {
		printf ("ERROR! WE HAVEN'T THIS DATA IN CURRENT TREE\n");
		return;
	}

	push_nodes_in_stack (search_node1, stack1);
	push_nodes_in_stack (search_node2, stack2);

// [ #### SKIP OWN NODES ################################# ]
	Node *print_node11 = nullptr;
	Node *print_node12 = nullptr;

	Node *print_node21 = nullptr;
	Node *print_node22 = nullptr;

	skip_own_nodes (&print_node11, &print_node21, stack1, stack2);

	printf ("\n____[difference]__________________________\n");

	// printf ("%d %d\n", stack2->size, stack1->size);
	print_own_nodes (stack1, stack2,
		               print_node11, print_node12,
		               print_node21, print_node22);

	print_last_nodes (stack1, print_node11, print_node12, 1);
	print_last_nodes (stack2, print_node21, print_node22, 2);

	printf ("__________________________________________\n");

	return;
}

//--------------------------------------------------------------------

void first_node_in_new_tree (Tree *tree)
{
	assert (tree != nullptr);

	tree->root = new_node (tree);
	tree->cur_node = tree->root;
	tree->cur_node->is_endian = 1;

	printf ("at first create one node with, ",
					"write name of first object: ");

	char new_data[MAX_DATA_LEN] = "";
	fflush(stdin);
	fgets (new_data, MAX_DATA_LEN, stdin);
	new_data[strlen (new_data) - 1] = '\0';

	strcpy (tree->cur_node->data, new_data);
	create_graph (tree, 0);

	return;
}

//--------------------------------------------------------------------

static void create_new_tree_intro ()
{
	printf ("\n___[ instruction create mode ]_______\n");
	printf ("[+] Create new Node in current position ",
				  "[new object have new param]\n");

	printf ("[-] Create new Node in current position ",
		      "[old object have new param]\n");

	printf ("[L] Move left\n");
	printf ("[R] Move right\n");
	printf ("[P] Move parent\n");
	printf ("[E] End creating\n");
	printf ("[C] Change cur data\n");
	printf ("[T] Tide this node [make this node the last one]\n");
	printf ("-------------------------------------\n");

	return;
}

//--------------------------------------------------------------------

void tide_node (Node *node)
{
	assert (node != nullptr);

	printf ("tide dat: %s\n", node->data);
	if (node->is_endian != 1) {
		node->is_endian = 1;

		free (node->right);
		free (node->left);
	}
	else {
		printf ("this node is already last!\n");
	}

	return;
}

//--------------------------------------------------------------------

void change_node_data (Node *node)
{
	assert (node != nullptr);

	char new_data[MAX_DATA_LEN] = "";

	fflush (stdin);
	fgets (new_data, MAX_DATA_LEN, stdin);
	new_data[strlen (new_data) - 1] = '\0';

	strcpy (node->data, new_data);

	return;
}

//--------------------------------------------------------------------

void create_new_tree ()
{
	Tree new_tree = {};

	first_node_in_new_tree (&new_tree);

	create_new_tree_intro();

	char answ = ' ';
	int active_node_index = 0;

	while (answ != 'E') {
		while (answ != '+' && answ != '-' && answ != 'L'
				&& answ != 'R' && answ != 'P' && answ != 'E'
				&& answ != 'T' && answ != 'C') {
			answ = (char)getchar ();
		}
		
		if (answ == 'T') {
			Node *save_node = new_tree.cur_node;
			tide_node (new_tree.cur_node);
			create_graph (&new_tree, active_node_index);
			new_tree.cur_node = save_node;
		}

		if (answ == 'C') {
			Node *save_node = new_tree.cur_node;
			change_node_data (new_tree.cur_node);
			create_graph (&new_tree, active_node_index);
			new_tree.cur_node = save_node;
		}

		if (answ == '+') {
			if (new_tree.cur_node->is_endian != 0) {
				printf ("add new: object ");
				Node *save_node = new_tree.cur_node;
				expand_tree (&new_tree, 'l');
				create_graph (&new_tree, active_node_index);
				new_tree.cur_node = save_node;
			}
			else {
				printf ("You can't add tree in filled (not endian node)\n");
			}
		}

		if (answ == '-') {
			if (new_tree.cur_node->is_endian != 0) {
				printf ("add new: object ");
				Node *save_node = new_tree.cur_node;
				expand_tree (&new_tree, 'r');
				create_graph (&new_tree, active_node_index);
				new_tree.cur_node = save_node;
			}
			else {
				printf ("You can't add tree in filled (not endian node)\n");
			}
		}

		if (answ == 'L') {
			if (new_tree.cur_node->is_endian != 1) {
				Node *save_node = new_tree.cur_node->left;
				active_node_index =  new_tree.cur_node->left->node_index;
				create_graph (&new_tree, active_node_index);

				new_tree.cur_node = save_node;
			}
			else {
				printf ("You cant move here, because tree haven't continue ",
								"in this node!!\n");

				answ = (char)getchar ();
				continue;
			}
		}

		if (answ == 'R') {
			if (new_tree.cur_node->is_endian != 1) {
				Node *save_node = new_tree.cur_node->right;
				active_node_index =  new_tree.cur_node->right->node_index;
				create_graph (&new_tree, active_node_index);

				new_tree.cur_node = save_node;
			}
			else {
				printf ("You cant move here, because tree haven't ",
								"continue in this node!!\n");

				answ = (char)getchar ();
				continue;
			}
		}

		if (answ == 'P') {
			if (new_tree.cur_node->parent != nullptr) {
				Node *save_node = new_tree.cur_node->parent;
				active_node_index =  new_tree.cur_node->parent->node_index;
				create_graph (&new_tree, active_node_index);
				new_tree.cur_node = save_node;
			}
			else {
				printf ("You cant move here, because tree haven't ",
								"continue in this node!!\n");

				answ = (char)getchar ();
				continue;
			}
		}

		if (answ == 'E') {
			break;
		}
		answ = (char)getchar ();
	}

	printf ("do you want save that tree? [Y/N]");
	if (get_ans ()) {
		save_new_aki_data_file (&new_tree);
	}
	else {
		return;
	}
}

//--------------------------------------------------------------------

void save_new_aki_data_file (Tree *tree)
{
	assert (tree != nullptr);

	char save_ptr[MAX_DIR_PATH] = "";
	char name_of_data[MAX_DATA_LEN] = "";

	strcat (save_ptr, STD_LVL_DIR);
	printf ("enter the name of new_file: ");

	scanf ("%s", name_of_data);

	strcat (save_ptr, name_of_data);
	strcat (save_ptr, ".");
	strcat (save_ptr, AKI_EXT);

	FILE *new_file = fopen (save_ptr, "w");
	tree->cur_node = tree->root;

	print_node (tree->cur_node, new_file);

	fclose (new_file);

	return;
}

//--------------------------------------------------------------------

void expand_right (Tree *tree, char *new_object, char *old_object, char *new_data)
{
	assert (old_object != nullptr);
	assert (tree       != nullptr);
	assert (new_data   != nullptr);
	assert (new_object != nullptr);

	tree->cur_node->left = new_node (tree);
	strcpy (tree->cur_node->left->data, new_object);

	tree->cur_node->left->is_endian = 1;

	printf ("what feature does the new object ",
					"have in contrast to %s?\n", old_object);

	fflush(stdin);
	fgets (new_data, MAX_DATA_LEN, stdin);
	new_data[strlen (new_data) - 1] = '\0';
	strcpy (tree->cur_node->data, new_data);

	tree->cur_node->right = new_node (tree);
	strcpy (tree->cur_node->right->data, old_object);
	tree->cur_node->right->is_endian = 1;

	return;
}

//--------------------------------------------------------------------

void expand_left (Tree *tree, char *new_object, char *old_object, char *new_data)
{
	assert (old_object != nullptr);
	assert (tree       != nullptr);
	assert (new_data   != nullptr);
	assert (new_object != nullptr);

	tree->cur_node->right = new_node (tree);
	strcpy (tree->cur_node->right->data, new_object);

	tree->cur_node->right->is_endian = 1;

	printf ("what feature does the OLD object have in contrast to %s?\n", new_object);

	fflush(stdin);
	fgets (new_data, MAX_DATA_LEN, stdin);
	new_data[strlen (new_data) - 1] = '\0';
	strcpy (tree->cur_node->data, new_data);

	tree->cur_node->left = new_node (tree);
	strcpy (tree->cur_node->left->data, old_object);
	tree->cur_node->left->is_endian = 1;

	return;
}

//--------------------------------------------------------------------

void expand_tree (Tree *tree, char side)
{
	assert (tree != nullptr);

	char new_object[100] = {};
	char old_object[100] = {};
	char   new_data[100] = {};

	tree->cur_node->is_endian = 0;
	strcpy (old_object, tree->cur_node->data);

	fflush(stdin);
	fgets (new_object, MAX_DATA_LEN, stdin);
	new_object[strlen (new_object) - 1] = '\0';

	if (side == 'l') {
		expand_right (tree, new_object, old_object, new_data);
	}
	else if (side == 'r') {
		expand_left (tree, new_object, old_object, new_data);
	}
	else {
		printf ("function ran with bad param\n");
	}

	return;
}

//--------------------------------------------------------------------

void chose_data_file (Tree *tree, NamesLvl *lvls)
{
	assert (tree != nullptr);
	assert (lvls != nullptr);

	for (size_t file = 0; file < lvls->size; file++) {
		printf ("[%zu] %s\n", file, lvls->names[file].ptr);
	}

	int choice = -1;

	printf ("\n========\n");
	printf ("choice> ");
	while (choice < 0 || choice > (int)lvls->size) {
		scanf ("%d", &choice);
	}
	printf ("========\n");

	char name_base[MAX_DATA_LEN] = {};
	strcat (name_base, STD_LVL_DIR);
	strcat (name_base, lvls->names[choice].ptr);
	strcpy (tree->name_of_data_file, name_base);

	return;
}

//--------------------------------------------------------------------

void tree_ctor (Tree *tree, NamesLvl *lvls)
{
	assert (tree != nullptr);
	assert (lvls != nullptr);

	tree->index = 0;

	tree->root     = new_node (tree);
	tree->cur_node = tree->root;

	chose_data_file (tree, lvls);
	FILE *file = fopen (tree->name_of_data_file, "r");

	get_data (tree, file);

	tree->cur_node = tree->root;
	fclose (file);

	return;
}

//--------------------------------------------------------------------

size_t find_lvls (const char *dir, NamesLvl *lvls)
{
  assert (lvls != nullptr);
  assert (dir  != nullptr);

  lvls->names = (Line *)calloc (MAX_DATA_LEN, sizeof (Line));
  lvls->capacity = 0;

  size_t num_sol = find_all_ext_coincidences (AKI_EXT, lvls);

  return num_sol;
}

//--------------------------------------------------------------------

Node* new_node (Tree* tree)
{
  assert (tree != nullptr);

  Node* node = (Node *)calloc (1, sizeof (Node) );

  node->parent = tree->cur_node;
  node->node_index = tree->index;
  tree->index++;

  return node;
}

//--------------------------------------------------------------------

void get_data (Tree *tree, FILE* txt)
{
	assert (tree != nullptr);
	assert (txt  != nullptr);

	char cur_data[MAX_DATA_LEN] = {};
	fgets (cur_data, MAX_DATA_LEN, txt);

	strcpy (tree->cur_node->data, cur_data);

	char *sym_n = strchr (tree->cur_node->data, '\n');

	if (sym_n != nullptr) {
		*sym_n = '\0';
	}

	fgets (cur_data, MAX_DATA_LEN, txt);

	if (cur_data[0] == '{') {
		tree->cur_node->left = new_node (tree);
		tree->cur_node = tree->cur_node->left;
		get_data (tree, txt);

		fgets (cur_data, MAX_DATA_LEN, txt);
		if (tree->cur_node->parent != nullptr) {
			tree->cur_node = tree->cur_node->parent;
		}

		tree->cur_node->right = new_node (tree);
		tree->cur_node = tree->cur_node->right;
		get_data (tree, txt);

		fgets (cur_data, MAX_DATA_LEN, txt);
		if (tree->cur_node->parent != nullptr) {
			tree->cur_node = tree->cur_node->parent;
		}

	}
	else {
		tree->cur_node->is_endian = 1;
	}

	return;
}

//--------------------------------------------------------------------

void create_graph (Tree *tree, int active_node_index)
{
  assert (tree != nullptr);

  char graph_name[MAX_DATA_LEN] = {};
  sprintf (graph_name, DUMP_FILE);

  FILE *graph_file = fopen (DUMP_FILE, "w");
  assert (graph_file != nullptr);
  fprintf (graph_file, "digraph G {\n");

  tree->cur_node = tree->root;
  write_tree (tree->cur_node, graph_file, active_node_index);

  tree->cur_node = tree->root;
  connect_tree (tree->cur_node, graph_file);

  fprintf (graph_file, "}\n");
  fclose (graph_file);

  char command[100] = {};
  sprintf (command, "dot %s -T png -o %s", DUMP_FILE, VIZ_FILE);
  system (command);

  tree->cur_node = tree->root;

  return;
}

//--------------------------------------------------------------------

void write_tree (Node *node, FILE *graph_file, int is_active)
{
	assert (graph_file != nullptr);
	assert (node       != nullptr);

	if (node == nullptr) {
		return;
	}

	char yes_a[]  = "yes";
	char not_a[]  = "no";
	char ind[]    = "index";
	char is_end[] = "is_end" ;

	if (node->is_endian != 1) {
		char message[MAX_DATA_LEN] = {};
		strcpy (message, node->data);
		strcat (message, "?");

		if (node->node_index == is_active || node->is_active == 1) {
			fprintf (graph_file, "\tL%d[shape=\"record\",style=\"filled\",fillcolor=\"burlywood1\", label=\" <lp%d> %s | { %s: %d %s: %d | %s} | <lp%d> %s\"];\n",
				node->node_index, node->node_index + 1, yes_a, ind,
				node->node_index, is_end, node->is_endian, message,
				node->node_index + 2, not_a);
		}
		else {	
			fprintf (graph_file, "\tL%d[shape=\"record\", label=\" <lp%d> %s | { %s: %d %s: %d | %s} | <lp%d> %s\"];\n",
				node->node_index, node->node_index + 1, yes_a, ind,
				node->node_index, is_end, node->is_endian, message,
				node->node_index + 2, not_a);
		}

		write_tree (node->right, graph_file, is_active);
		write_tree (node->left, graph_file, is_active);
	}
	else {
		if (node->node_index == is_active || node->is_active == 1) {
			fprintf (graph_file, "\tL%d[shape=\"record\",style=\"filled\",fillcolor=\"burlywood1\", label=\" { %s: %d %s: %d | %s}\"];\n",
				node->node_index, ind, node->node_index, is_end, node->is_endian, node->data);
		}
		else {
			fprintf (graph_file, "\tL%d[shape=\"record\", label=\" { %s: %d %s: %d | %s}\"];\n",
				node->node_index, ind, node->node_index, is_end, node->is_endian, node->data);
		}
	}

	return;
}

//--------------------------------------------------------------------

void connect_tree (Node *node, FILE *graph_file)
{
	assert (graph_file != nullptr);
	assert (node       != nullptr);

	if (node == nullptr) {
		return;
	}

	if (node->is_endian == 1) {
		return;
	}

	if (node->left != nullptr) {
		fprintf (graph_file, "\tL%d:<lp%d> -> L%d[color=\"blue\"];\n",
			node->node_index, node->node_index + 1, node->left->node_index);

		connect_tree (node->left, graph_file);
	}

	if (node->right != nullptr) {
		fprintf (graph_file, "\tL%d:<lp%d> -> L%d[color=\"blue\"];\n",
			node->node_index, node->node_index + 2, node->right->node_index);

		connect_tree (node->right, graph_file);
	}

	return;
}

//--------------------------------------------------------------------

void answer_aki_question (Tree *tree, Stack *stack,
													bool *answer_with_assumption_correct)
{
	while (tree->cur_node->is_endian != true || stack->size != 0) {
		
		txSpeak ("\vAkinator interesovatsa... ito %20s? [Y/N/IDN] ",
						tree->cur_node->data);

		char answ[4] = {};

// [ ### IS IDN WAS PRINTED ################## ]
		if (tree->cur_node->is_endian == 1) {
			fflush (stdin);
			fgets (answ, 4, stdin);

			if (answ[0] == 'Y') {
				*answer_with_assumption_correct = true;

				break;
			}
			else if (answ[0] == 'N') {
				stack_top (stack, &tree->cur_node);
				stack_pop (stack);
				
				tree->cur_node = tree->cur_node->right;
				
				continue;
			}
		}

// [ ### CUR QUESTION ###################### ]
		fflush (stdin);
		fgets (answ, 4, stdin);

		if (answ[0] == 'I') {
			stack_push (stack, tree->cur_node);
			tree->cur_node = tree->cur_node->left;

			continue;
		}
		else if (answ[0] == 'Y') {
			tree->cur_node = tree->cur_node->left;

			continue;
		}
		else if (answ[0] == 'N') {
			tree->cur_node = tree->cur_node->right;

			continue;
		}
		else {
			printf ("incorrect data, try again> ");
		}
	}
}

//--------------------------------------------------------------------

void BAD_END (Tree *tree)
{
	assert (tree != nullptr);

	txSpeak ("\vleeeeeee, togda akinator ne znat, ikarni babai a chto ti zagadival?\n");
	expand_tree (tree, 'l');

	tree->cur_node = tree->root;
	FILE *file = fopen (tree->name_of_data_file, "w");
	print_node (tree->cur_node, file);
	fclose (file);

	create_graph (tree, 0);

	return;
}

//--------------------------------------------------------------------

void happy_end (Tree *tree)
{
	assert (tree != nullptr);

	printf ("\n=============================\n");

	txSpeak ("\vleeeeeeee opyat pobedil! azazazazazaz. bylo neslozno)!\n");
	txSpeak ("\vza takov prekrasni pobeda wi platit akinatoru sto rublei\n");
	printf ("zaplatit ");
	int sallary = 0;
	scanf ("%d", &sallary);
	if (sallary >= 100) {
		txSpeak ("Akinator dovooolen, prihodite escho!");
	}
	else {
		txSpeak ("Leeee nu kak tak, togda ya s vami ne igrau");

		printf ("[ Akinator obidelsa ]\n");
	}
	tree->cur_node = tree->root;

	return;
}

//--------------------------------------------------------------------

void load_game (Tree *tree, Stack *stack)
{
	assert (tree  != nullptr);
	assert (stack != nullptr);

	bool answer_with_assumption_correct = false;

	answer_aki_question (tree, stack, &answer_with_assumption_correct);

	if (answer_with_assumption_correct == false) {
		printf ("------------------------------------\n");
		txSpeak ("\vAkinator dumat, chto ito %20s? [Y/N] ", tree->cur_node->data);
	}
	
	if (answer_with_assumption_correct || get_ans ()) {
		happy_end (tree);
	}
	else {
		BAD_END (tree);
	}

	return;
}

//--------------------------------------------------------------------

bool get_ans ()
{
	char choice = ' ';

	while (choice != 'Y' && choice != 'N') {
		scanf ("%c", &choice);
	}

	if (choice == 'Y') {
		return true;
	}
	else {
		return false;
	}
}

//--------------------------------------------------------------------

void print_node (Node *node, FILE *file)
{
	assert (file != nullptr);
	assert (node != nullptr);

	if (node->is_endian != 1) {
		fprintf (file, "%s\n", node->data);

		if (node->left->is_endian != 1) {
			fprintf (file, "{\n");
			print_node (node->left, file);
		}
		else {
			NODE_DATA(node->left->data)
		}

		if (node->right->is_endian != 1) {
			fprintf (file, "{\n");
			print_node (node->right, file);
		}
		else {
			NODE_DATA(node->right->data)
		}
	}

	if (node->parent != nullptr) {
		fprintf (file, "}\n");
	}

	return;
}

//-------- End of .cpp file! -----------------------------------------

/*
void dtor ()
{


	return;
}
*/