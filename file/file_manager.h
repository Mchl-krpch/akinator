/**
 * Additional file handling library. Originally written for the
 * Akinator project, the 6th project within the course of
 * I.R.Dedinsky, so that Akinator could work with files of its type.
 */

/**
 * the library implements the functions of searching for files with a
 * specific extension, the name of the current folder, and other
 * functions
 */
#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "../scr/aki.h"
//--------------------------------------------------------------------

/**
 * @brief special define to know max-index in word,
 * in "word" max-index = 3, 'd' = word[3]
 */
#define LEN(word) strlen(word) - 1

#define MAX_DIR_PATH 200

/**
 * @brief Looks at which folder in the program is currently active
 * 
 * @param *dir_name this array will be filled as name of
 * current directory
 */
void get_cur_dir (char *dir_name);

/**
 * @brief Separate the last word by some character
 * 
 * @param dir_path Ptr to dir in program
 * @param dir_name Name of directory
 * @param split_char Char that will be flag for func, to split
 * 
 * @return Number of splited symbols.
 */
size_t split_last_word (char *dir_path, char *dir_name,
                        char split_char, int max_index);

/**
 * @brief After separating a word, it must be written in the reverse
 * order of letters, for which this function is needed - it reverses
 * an array of letters
 * 
 * @param dir_name name of directory
 * @param len lenth of word which we need to reverse
 */
void rev_last_word (char *dir_name, size_t len);

/**
 * @brief See which extension the file has
 * 
 * @param name Name of found file
 * @param ext Extencion of file
 */
bool check_ext (char *name, const char *ext);

/**
 * @brief Finds all necessary files by extension in the selected
 * folder
 * 
 * @param file_ext Extencion which we need to find
 * @param lvls Special struct to contain names of files
 */
size_t find_all_ext_coincidences (const char *file_ext,
                                  NamesLvl *lvls);

//--------------------------------------------------------------------
#endif//FILE_MANAGER_H