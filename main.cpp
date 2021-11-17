#include <stdlib.h>

#include <stdio.h>

#include "windows.h"
#include "scr/aki.h"
#include "scr/stack.h"


int main ()
{
	Tree tree = {};
	select_mode (&tree);
	
	return 0;

	/*
	tree_ctor (&tree);
	create_graph (&tree);

	printf ("lets find objct: ");
	char find[32] = {};
	scanf ("%s", find);

	Stack stack = {};
	
	stackCtor (&stack, 16);
	Node *find_node = nullptr;
	search_object (tree.cur_node, find, &stack, &find_node);
	print_path (&stack, find_node);

	printf ("find data: %s", find_node->data );
	// print_path (&stack, tree.cur_node);
	stackDtor (&stack);
	
	tree.cur_node = tree.root;

	stack = {};
	stackCtor (&stack, 16);
	Stack stack2 = {};
	stackCtor (&stack2, 16);
	compare_objects (&tree, &stack, &stack2);
	load_game (&tree);
	*/
	return 0;
}