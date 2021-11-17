/**
 * Additional file handling library. Originally written for the Akinator
 * project, the 6th project within the course of I.R.Dedinsky, so that
 * Akinator could work with files of its type.
 */

/**
 * the library implements the functions of searching for files with a
 * specific extension, the name of the current folder, and other
 * functions
 */
#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "../scr/aki.h"

/**
 * @brief special define to know max-index in word,
 * in "word" max-index = 3, 'd' = word[3]
 */
#define LEN(word) strlen (word) - 1

#define MAX_DIR_PATH 200

/**
 * @brief 
 */
void get_cur_dir (char *dir_name);

/**
 * @brief 
 */
size_t split_last_word (char *dir_path, char *dir_name, char split_char);

/**
 * @brief 
 */
void rev_last_word (char *dir_name, size_t len);

/**
 * @brief 
 */
bool check_ext (char *name, const char *ext);

/**
 * @brief 
 */
size_t find_all_ext_coincidences (const char *file_ext, NamesLvl *lvls);

#endif//FILE_MANAGER_H