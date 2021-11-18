#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include <assert.h>
#include <sys/types.h>
#include <io.h>

#include <stdlib.h>

#include "file_manager.h"

// [ #### DIRECTORY NAME ############################################## ] 
void get_cur_dir (char *dir_name)
{
	char dir_path[MAX_DIR_PATH] = {};
	getcwd (dir_path, MAX_DIR_PATH);

	split_last_word (dir_path, dir_name, '\\', 5);
}

//-----------------------------------------------------------------------

size_t split_last_word (char *path, char *dir, char split_char, int max_index)
{
	assert (dir != nullptr);
	assert (path != nullptr);


	char cur_ch = ' ';
	size_t cur_index = LEN(path);
	size_t sym_counter = 0;

	for (int index = 0; index < max_index; index++) {
		if (cur_ch != split_char) {
			dir[sym_counter] = path[cur_index];

			sym_counter++;
			cur_index--;

			cur_ch = path[cur_index];
		}
	}

	dir[sym_counter] = '\0';

	rev_last_word (dir, sym_counter);

	return sym_counter;
}

//-----------------------------------------------------------------------

void rev_last_word (char *dir_name, size_t len)
{
	char dir_name_copy[32] = "";
	strcpy (dir_name_copy, dir_name);

	for (size_t i = 0; i < len + 1; i++) {
		dir_name[i] = dir_name_copy[len - i - 1];
	}
}

// [ #### READ EXTENCION ############################################### ] 
size_t find_all_ext_coincidences (const char *ext, NamesLvl *lvls)
{
	assert (ext != nullptr);

	size_t find_counter = 0;

	DIR *dir;
  struct dirent *de;

  char dir_path[MAX_DIR_PATH] = {};
	getcwd (dir_path, MAX_DIR_PATH);

  dir = opendir (STD_LVL_DIR);

  // system ("dir");

  while (dir) {
  	de = readdir (dir);
    
    if (!de) {
    	break;
    }

  	if (strchr (de->d_name, '.') != NULL && check_ext (de->d_name, ext)){
  		lvls->size++;

  		lvls->names[find_counter].ptr = (char *)calloc (32, sizeof (char));
    	// printf ("n find: %s\n", de->d_name);

  		strcpy (lvls->names[find_counter].ptr, de->d_name);
  		printf ("n find: %s\n", lvls->names[find_counter].ptr);

  		find_counter++;
  	}

  	/*
  	if (readdir (dir) == nullptr) {
    	continue;
    }
    de = readdir (dir);
    if (!de) {
    	break;
    }

    if (check_ext (de->d_name, ext)){
  		lvls->size++;

  		lvls->names[find_counter].ptr = (char *)calloc (32, sizeof (char));

  		strcpy (lvls->names[find_counter].ptr, de->d_name);
  		printf ("n find: %s\n", lvls->names[find_counter].ptr);

  		find_counter++;
    }
    */
    
  }
  closedir (dir);

  return find_counter;
}

//-----------------------------------------------------------------------

bool check_ext (char *name, const char *ext)
{
	assert (name != nullptr);
	assert (ext  != nullptr);

	// printf ("hello3\n");

	char file_extencion[10] = {};
	split_last_word (name, file_extencion, '.', 5);

	if (strcmp (file_extencion, ext) == 0) {
		return true;
	}

	return false;
}