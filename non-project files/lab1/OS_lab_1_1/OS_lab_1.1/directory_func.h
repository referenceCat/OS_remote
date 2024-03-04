//
// Created by yawgag on 21.02.2024.
//

#ifndef OS_LAB_1_DIRECTORY_FUNC_H
#define OS_LAB_1_DIRECTORY_FUNC_H


#include <vector>
#include <string>

std::vector <std::string> drives(); // search drives
std::vector <std::string> all_files_in_directory(const char* dir); // search files and folders in directory
std::string vector_to_STR(const std::vector<std::string>& dir, int key); // key=1 - LPCSTR for search files, key=0 for others functions
void create_directory(std::vector<std::string>& dir); // creating directory
void remove_directory(std::vector<std::string>& dir); // removing directory
void copy_file(std::vector<std::string>& dir); // coping file
void print_menu(); // print menu in cmd
void drive_info(const char* dir); // print info about drive
void create_file(std::vector<std::string>& dir); // creating file
void move_file(std::vector<std::string> dir); // moving file to another directory
void file_information(std::vector<std::string> dir);

#endif //OS_LAB_1_DIRECTORY_FUNC_H
