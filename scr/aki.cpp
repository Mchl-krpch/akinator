#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <io.h>

#include "aki.h"
#include "stack.h"
#include "windows.h"
#include "../file/file_manager.h"

//------------------------------------------------------------------------------------------------------

void select_mode (Tree* tree)
{
	assert (tree != nullptr);

	NamesLvl levels = {};
	find_lvls (STD_LVL_DIR, &levels);

	char choice = ' ';

	printf ("(Chose mode)\n\n");
	printf ("_____[ Akinator program ]______________\n");
	printf ("[P] Play game\n");
	printf ("[G] Graph of current tree\n");
	printf ("[C] Compare two objects\n");
	printf ("[N] New tree mode\n");
	printf ("[D] Description of object\n");
	printf ("[Q] Quit\n");
	printf ("---------------------------------------\n");

	while (strchr ("PGCDQ", (char)choice) == 0) {
		scanf ("%c", &choice);
	}

	switch (choice) {
		case 'P':
			printf ("Play mode\n");
			tree_ctor (tree, &levels);
			load_game (tree);
			return;

		case 'G':
			printf ("Graph mode\n");
			tree_ctor (tree, &levels);
			create_graph (tree);
			return;

		case 'C':
			printf ("Compare mode\n");
			return;

		case 'D':
			printf ("Description mode\n");
			return;

		case 'Q':
			printf ("Quit mode\n");
			return;

		default:
			printf ("incorrect data\n");
	}

	return;
}

//------------------------------------------------------------------------------------------------------

void chose_data_file (Tree *tree, NamesLvl *lvls)
{
	assert (tree != nullptr);
	assert (lvls != nullptr);

	for (size_t file = 0; file < lvls->size; file++) {
		printf ("[%u] %s\n", file, lvls->names[file].ptr);
	}
	int choice = -1;
	while (choice < 0 || choice > (int)lvls->size) {
		scanf ("%d", &choice);
	}

	char name_base[MAX_DATA_LEN] = {};
	strcat (name_base, STD_LVL_DIR);
	strcat (name_base, "/");
	strcat (name_base, lvls->names[choice].ptr);
	strcpy (tree->name_of_data_file, name_base);
	printf ("chosen file: %s\n", tree->name_of_data_file);

	return;
}

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
	printf ("new data: %s", tree->cur_node->data);
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

	printf ("OST: %s %d\n", tree->cur_node->data, tree->cur_node->node_index);
	fgets (cur_data, MAX_DATA_LEN, txt);
	printf ("%s", cur_data);
	if (cur_data[0] == '{') {
		tree->cur_node->left = new_node (tree);

		tree->cur_node = tree->cur_node->left;
		get_data (tree, txt);

		fgets (cur_data, MAX_DATA_LEN, txt);
		if (tree->cur_node->parent != nullptr) {
			tree->cur_node = tree->cur_node->parent;
		}
		printf ("%s", cur_data);

		tree->cur_node->right = new_node (tree);
		tree->cur_node = tree->cur_node->right;
		get_data (tree, txt);
		fgets (cur_data, MAX_DATA_LEN, txt);
		printf ("1: %s\n", cur_data);
		if (tree->cur_node->parent != nullptr) {
			tree->cur_node = tree->cur_node->parent;
		}

	}
	else {
		tree->cur_node->is_endian = 1;
	}

	return;
}

//----------------------------------------------------------------------------------------------------------------------

void create_graph (Tree *tree)
{
  assert (tree != nullptr);

  char graph_name[MAX_DATA_LEN] = {};
  sprintf (graph_name, DUMP_FILE);
  // chdir (".visual");
  // system ("dir");
  FILE *graph_file = fopen (DUMP_FILE, "w");
  // chdir ("../");
  assert (graph_file != nullptr);

  fprintf (graph_file, "digraph G {\n");

  int index = 0;

  tree->cur_node = tree->root;
  printf ("%s\n", tree->cur_node->right->data);

  write_tree (tree->cur_node, graph_file, &index);
  index = 1;
  tree->cur_node = tree->root;
  connect_tree (tree->cur_node, graph_file, &index);

  fprintf (graph_file, "}\n");
  fclose (graph_file);

  char command[100] = {};
  sprintf (command, "dot %s -T png -o %s", DUMP_FILE, VIZ_FILE);
  system (command);

  return;
}

//----------------------------------------------------------------------------------------------------------------------

void write_tree (Node *node, FILE *graph_file, int *index)
{
	if (node == nullptr) {
		return;
	}

	if (node->is_endian != 1) {
		char message[MAX_DATA_LEN] = {};
		strcpy (message, node->data);
		strcat (message, "?");

		fprintf (graph_file, "\tL%d[shape=\"record\", label=\" <lp%d> %d | { %d  %d | %s} | <lp%d> %d\"];\n",
			node->node_index, node->node_index + 1, node->node_index + 1, node->node_index, node->is_endian, message, node->node_index + 2, node->node_index + 2);

		printf ("right: %s\n", node->data);
		write_tree (node->right, graph_file, index);
		printf ("Left:\n");
		write_tree (node->left, graph_file, index);
	}
	else {
		fprintf (graph_file, "\tL%d[shape=\"record\", label=\" { %d  %d | %s}\"];\n", node->node_index, node->node_index, node->is_endian, node->data);
	}

	return;
}

//----------------------------------------------------------------------------------------------------------------------

void connect_tree (Node *node, FILE *graph_file, int *index)
{
	assert (graph_file != nullptr);
	assert (index      != nullptr);

	if (node == nullptr) {
		return;
	}

	if (node->is_endian == 1) {
		return;
	}

	if (node->left != nullptr) {
		fprintf (graph_file, "\tL%d:<lp%d> -> L%d[color=\"blue\"];\n", node->node_index, node->node_index + 1, node->left->node_index);

		*(index) += 2;

		connect_tree (node->left, graph_file, index);
	}

	if (node->right != nullptr) {
		fprintf (graph_file, "\tL%d:<lp%d> -> L%d[color=\"blue\"];\n", node->node_index, node->node_index + 2, node->right->node_index);

		*(index) += 2;

		connect_tree (node->right, graph_file, index);
	}

	return;
}

//----------------------------------------------------------------------------------------------------------------------

void load_game (Tree *tree)
{
	assert (tree != nullptr);

	while (tree->cur_node->is_endian != true) {
		printf ("Akinator >> %s? [Y/N]\n", tree->cur_node->data);

		if (get_ans ()) {
			tree->cur_node = tree->cur_node->left;
		}
		else {
			tree->cur_node = tree->cur_node->right;
		}
	}

	printf ("Ans: %s? [Y/N]", tree->cur_node->data);
	
	if (get_ans ()) {
		printf ("ya opyat' pobegil! eto bylo neslozno)!\n");
		tree->cur_node = tree->root;

		return;
	}
	else {
		printf ("Crap! What are you guessing?\n");
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

		tree->cur_node = tree->root;
		FILE *file = fopen (tree->name_of_data_file, "w");
		print_node (tree->cur_node, file);
		fclose (file);

		create_graph (tree);
	}


	return;
}

//----------------------------------------------------------------------------------------------------------------------

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

//----------------------------------------------------------------------------------------------------------------------

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
			fprintf (file, "{\n");
			fprintf (file, "%s\n", node->left->data);
			fprintf (file, "}\n");
		}

		if (node->right->is_endian != 1) {
			fprintf (file, "{\n");
			print_node (node->right, file);
		}
		else {
			fprintf (file, "{\n");
			fprintf (file, "%s\n", node->right->data);
			fprintf (file, "}\n");
		}
	}
	if (node->parent != nullptr) {
		fprintf (file, "}\n");
	}

	return;
}

//-----------------------------------------------------------------------------

void search_object (Node *node, char *data, Stack *stack, Node **saving_node)
{
	if (node->left->is_endian != 1) {
		stackPush (stack, 1);
		search_object (node->left, data, stack, saving_node);
	}
	else {
		if (strcmp (node->left->data, data) == 0) {
			printf ("WE FIND: %s\n", node->left->data);
			stackPush (stack, 1);

			*saving_node = node;
		}
		else {
		}
	}

	if (node->right->is_endian != 1) {
		stackPush (stack, 2);
		search_object (node->right, data, stack, saving_node);
	}
	else {
		if (strcmp (node->right->data, data) == 0) {
			stackPush (stack, 2);
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

//-----------------------------------------------------------------------------

void print_path (Stack *stack, Node *node)
{
	while (stack->size > 0) {
		int val = 0;
		stackTop (stack, &val);
		printf ("o: %d", val);

		if (val == 2) {
			printf ("ne %s\n", node->data);
		}
		else if (val == 1) {
			printf ("%s\n", node->data);
		}

		if (node->parent != nullptr) {
			node = node->parent;
		}

		if (stack->size > 0) {
			stackPop (stack);
		}
	}
}

//-----------------------------------------------------------------------------

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
