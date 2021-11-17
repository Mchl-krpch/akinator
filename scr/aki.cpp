#define TX_USE_SPEAK
#include "C:\MinGW\TX\TXLib.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <io.h>

#include "../file/file_manager.h"
#include "windows.h"
#include "stack.h"
#include "aki.h"

//------------------------------------------------------------------------------------------------------

void select_mode (Tree* tree)
{
	assert (tree != nullptr);

	NamesLvl levels = {};
	find_lvls (STD_LVL_DIR, &levels);
	// printf ("hello");

	txSpeak ("\vvas privetstvuit inostranni pragramma Akinator! smotrite chto ya mogu!");
	printf ("(Chose mode)\n\n");
	printf ("_____[ Akinator program ]______________\n");
	printf ("[P] Play game\n");
	printf ("[G] Graph of current tree\n");
	printf ("[C] Compare two objects\n");
	printf ("[N] New tree mode\n");
	printf ("[D] Description of object\n");
	printf ("[Q] Quit\n");
	printf ("---------------------------------------\n");

	printf ("mode [Q G C N D Q]> ");
	char choice = (char)getchar ();

	if (choice == 'P') {
		printf ("Play mode\n");
		tree_ctor (tree, &levels);
		load_game (tree);
	}

	if (choice == 'G') {
		printf ("Graph mode\n");
		tree_ctor (tree, &levels);
		create_graph (tree, 0);
	}

	if (choice == 'C') {
		printf ("Compare mode\n");
	}

	if (choice == 'N') {
		printf ("Creating tree mode\n");
		create_new_tree ();
	}

	if (choice == 'D') {
		printf ("Description mode\n");
		tree_ctor (tree, &levels);

		char search_data[MAX_DATA_LEN] = {};
		fflush(stdin);
		fgets (search_data, MAX_DATA_LEN, stdin);
		search_data[strlen (search_data) - 1] = '\0';

		Stack stack = {};
		stackCtor (&stack, 16);
		Node *search_node = nullptr;

		search_object (tree->cur_node, search_data, &stack, &search_node);
		tree->cur_node = tree->root;
		print_path (&stack, tree->cur_node);
	}

	if (choice == 'Q') {
		printf ("Quit mode\n");
	}

	return;
}

//--------------------------------------------------------------------

void create_new_tree ()
{
	Tree new_tree = {};

	new_tree.root = new_node (&new_tree);
	new_tree.cur_node = new_tree.root;
	new_tree.cur_node->is_endian = 1;

	printf ("at first create one node with, write name of first object: ");
	char new_data[MAX_DATA_LEN] = {};
	fflush(stdin);
	fgets (new_data, MAX_DATA_LEN, stdin);
	new_data[strlen (new_data) - 1] = '\0';
	// scanf ("%s", new_data);
	strcpy (new_tree.cur_node->data, new_data);
	create_graph (&new_tree, 0);

	new_tree.cur_node = new_tree.root;

	printf ("___[ instruction create mode ]_______\n");
	printf ("[+] Create new Node in current position [new object have new param]\n");
	printf ("[-] Create new Node in current position [old object have new param]\n");
	printf ("[L] Move left\n");
	printf ("[R] Move right\n");
	printf ("[P] Move parent\n");
	printf ("[E] End creating\n");
	printf ("-------------------------------------\n");

	char answ = ' ';
	int active_node_index = 0;
	while (answ != 'E') {
		while (answ != '+' && answ != '-' && answ != 'L' && answ != 'R' && answ != 'P' && answ != 'E') {
			answ = (char)getchar ();
		}
		
		if (answ == '+') {
			printf ("add new: object ");
			Node *save_node = new_tree.cur_node;
			expand_tree (&new_tree, 'l');
			create_graph (&new_tree, active_node_index);
			new_tree.cur_node = save_node;
		}

		if (answ == '-') {
			printf ("add new: object ");
			Node *save_node = new_tree.cur_node;
			expand_tree (&new_tree, 'r');
			create_graph (&new_tree, active_node_index);
			new_tree.cur_node = save_node;
		}

		if (answ == 'L') {
			if (new_tree.cur_node->is_endian != 1) {
				Node *save_node = new_tree.cur_node->left;
				active_node_index =  new_tree.cur_node->left->node_index;
				// printf ("cur data: %s", new_tree.cur_node->data);
				create_graph (&new_tree, active_node_index);

				new_tree.cur_node = save_node;
			}
			else {
				printf ("You cant move here, because tree haven't continue in this node!!\n");
				answ = (char)getchar ();
				continue;
			}
		}

		if (answ == 'R') {
			if (new_tree.cur_node->is_endian != 1) {
				Node *save_node = new_tree.cur_node->right;
				active_node_index =  new_tree.cur_node->right->node_index;
				// printf ("cur data: %s", new_tree.cur_node->data);
				create_graph (&new_tree, active_node_index);

				new_tree.cur_node = save_node;
			}
			else {
				printf ("You cant move here, because tree haven't continue in this node!!\n");
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
				printf ("You cant move here, because tree haven't continue in this node!!\n");
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
		char save_ptr[MAX_DIR_PATH] = {};
		char name_of_data[MAX_DATA_LEN] = {};

		strcat (save_ptr, STD_LVL_DIR);
		printf ("enter the name of new_file: ");

		scanf ("%s", name_of_data);

		strcat (save_ptr, name_of_data);
		strcat (save_ptr, ".");
		strcat (save_ptr, AKI_EXT);

		FILE *new_file = fopen (save_ptr, "w");
		new_tree.cur_node = new_tree.root;
		print_node (new_tree.cur_node, new_file);
		fclose (new_file);
	}

	return;
}

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
		tree->cur_node->left = new_node (tree);
		strcpy (tree->cur_node->left->data, new_object);

		tree->cur_node->left->is_endian = 1;

		printf ("what feature does the new object have in contrast to %s?\n", old_object);

		fflush(stdin);
		fgets (new_data, MAX_DATA_LEN, stdin);
		new_data[strlen (new_data) - 1] = '\0';
		strcpy (tree->cur_node->data, new_data);

		tree->cur_node->right = new_node (tree);
		strcpy (tree->cur_node->right->data, old_object);
		tree->cur_node->right->is_endian = 1;
	}
	else if (side == 'r') {
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
	while (choice < 0 || choice > (int)lvls->size) {
		scanf ("%d", &choice);
	}

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
	// FILE *file = fopen (".lvls/data.tree", "r");
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

  chdir (dir);

  lvls->names = (Line *)calloc (1, sizeof (Line));
  lvls->capacity = 0;

  size_t num_sol = find_all_ext_coincidences (AKI_EXT, lvls);
  // printf ("hello1\n");

  chdir ("../");

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

  return;
}

//--------------------------------------------------------------------

void write_tree (Node *node, FILE *graph_file, int is_active)
{
	assert (graph_file != nullptr);

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

		if (node->node_index == is_active) {
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
		if (node->node_index == is_active) {
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

void load_game (Tree *tree)
{
	assert (tree != nullptr);

	while (tree->cur_node->is_endian != true) {
		txSpeak ("\vAkinator interesovatsa... neuzto %20s? [Y/N] ", tree->cur_node->data);

		if (get_ans ()) {
			tree->cur_node = tree->cur_node->left;
		}
		else {
			tree->cur_node = tree->cur_node->right;
		}
	}

	printf ("------------------------------------\n");
	txSpeak ("\vAkinator dumat, chto ito %20s? [Y/N] ", tree->cur_node->data);
	
	if (get_ans ()) {
		txSpeak ("\vleeeeeeee opyat pobedil! azazazazazaz. bylo neslozno)!\n");
		tree->cur_node = tree->root;

		return;
	}
	else {
		txSpeak ("\vleeeeeee, togda akinator ne znat, ikarni babai a chto ti zagadival?\n");
		expand_tree (tree, 'l');

		tree->cur_node = tree->root;
		FILE *file = fopen (tree->name_of_data_file, "w");
		print_node (tree->cur_node, file);
		fclose (file);

		create_graph (tree, 0);
	}

	return;
}

//--------------------------------------------------------------------

void add_node (Tree *tree)
{
	assert (tree != nullptr);

	char new_object[100] = {};
	char old_object[100] = {};
	char   new_data[100] = {};


	tree->cur_node->is_endian = 0;
	strcpy (old_object, tree->cur_node->data);

	scanf ("%s", new_object);

	tree->cur_node->left = new_node (tree);
	strcpy (tree->cur_node->left->data, new_object);
	tree->cur_node->left->is_endian = 1;


	printf ("what feature does the new object have in contrast to the old one?\n");
	scanf ("%s",  new_data);
	strcpy (tree->cur_node->data, new_data);

	tree->cur_node->right = new_node (tree);
	printf ("old: %s", old_object);
	strcpy (tree->cur_node->right->data, old_object);
	tree->cur_node->right->is_endian = 1;
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

//--------------------------------------------------------------------

void search_object (Node *node, char *data, Stack *stack, Node **saving_node)
{
	if (node->left->is_endian != 1) {
		stackPush (stack, node);
		search_object (node->left, data, stack, saving_node);
	}
	else {
		if (strcmp (node->left->data, data) == 0) {
			printf ("WE FIND: %s\n", node->left->data);
			stackPush (stack, node);

			*saving_node = node;
		}
		else {
		}
	}

	if (node->right->is_endian != 1) {
		stackPush (stack, node);
		search_object (node->right, data, stack, saving_node);
	}
	else {
		if (strcmp (node->right->data, data) == 0) {
			stackPush (stack, node);
			printf ("WE FIND: %s\n", node->right->data);

			*saving_node = node;
		}
		else {
			if (stack->size > 0) {
				stackPop (stack);
			}
		}
	}

	return;
}

//--------------------------------------------------------------------

void print_path (Stack *stack, Node *node)
{
	while (stack->size > 0) {
		Node *stack_node = nullptr;
		stackTop (stack, &stack_node);
		printf ("o: %s\n", stack_node->data);
		/*
		if (stack_node == 2) {
			printf ("ne %s\n", node->data);
		}
		else if (stack_node == 1) {
			printf ("%s\n", node->data);
		}

		if (node->parent != nullptr) {
			node = node->parent;
		}
		*/

		if (stack->size > 0) {
			stackPop (stack);
		}
	}
}

//--------------------------------------------------------------------
/*
void compare_objects (Tree *tree, Stack *stack, Stack *stack2)
{
	printf ("FIRST OBJ: ");
	char find1[32] = {};
	scanf ("%s", find1);
	Node *find_node = nullptr;
	search_object (tree->cur_node, find1, stack, &find_node);

	int ptr1[10] = {};
	int cap = stack->size;
	for (int i = 0; i < cap; i++) {
		int val = 0;
		stackTop (stack, &val);
		ptr1[cap - i - 1] = val;
		printf ("[WRITE: %d pos: %d]\n", ptr1[cap - i - 1], cap - i - 1);
		stackPop (stack);
	}

	printf ("SECOND OBJ: ");
	char find2[32] = {};
	scanf ("%s", find2);
	Node *find_node2 = nullptr;
	search_object (tree->cur_node, find2, stack2, &find_node2);

	int ptr2[10] = {};
	int cap2 = stack2->size;
	for (int i = 0; i < cap2; i++) {
		int val = 0;
		stackTop (stack2, &val);
		ptr2[cap2 - i - 1] = val;
		printf ("%d", ptr2[cap2 - i - 1]);
		stackPop (stack2);
	}

	print_difference (ptr1, ptr2, find1, tree->cur_node, find2, tree->cur_node);

	return;
}

//--------------------------------------------------------------------

void print_difference (int *ptr1, int *ptr2, char *find1, Node *node1, char *find2, Node *node2)
{
	printf ("\n____[difference]__________________________\n");
	int index = 0;
	while (ptr1[index] == ptr2[index]) {
		printf ("equel: %-20s %-20s data: %s\n", find1, find2, node1->data);
		if (ptr1[index] == 2) {
			node1 = node1->right;
			node2 = node2->right;
		}
		else {
			node1 = node1->left;
			node2 = node2->left;
		}
		index++;
	}

	while (node1->is_endian != 1 || node2->is_endian != 1) {
		if (node1->is_endian == 1 ) {
			printf ("%s %-20s ", find1, "-");
		}
		else {
			if (ptr1[index] == 2) {
				printf ("%s: NE %-20s", find1, node1->data);
				if (node1->right != nullptr) {
					node1 = node1->right;
				}
				else {

				}
			}
			else {
				printf ("%s: %-20s", find1, node1->data);
				if (node1->left != nullptr) {
					node1 = node1->left;
				}
				else {
					
				}
			}		
		}

		if (node2->is_endian == 1) {
			printf ("%s: %-20s \n", find2, "-");
		}
		else {
			if (ptr2[index] == 2) {
				printf ("%s: NE %s\n", find2, node2->data);
				if (node2->right != nullptr) {
					node2 = node2->right;
				}
				else {
					
				}

			}
			else {
				printf ("%s: %-20s\n", find2, node2->data);
				if (node2->left != nullptr) {
					node2 = node2->left;
				}
				else {
					
				}
			}
			index++;
		}
	}
}
*/