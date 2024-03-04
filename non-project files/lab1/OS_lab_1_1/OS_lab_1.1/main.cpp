#include <iostream>
#include "string"
#include <vector>
#include "directory_func.h"

int main() {

    std::vector<std::string> current_directory;
    std::vector<std::string> all_directory;
    int command;

    while(true){
        // printing current directory
        if(current_directory.empty()){
            std::cout << "current directory is empty" << std::endl;
        } else {
            std::cout << "current directory is: " << vector_to_STR(current_directory, 0) << std::endl;
        }

        print_menu();


        //printing all folders to choose. current_directory.empty() -> choose drive, other -> choose folder
        if(current_directory.empty()){
            all_directory = drives();
        } else {
            all_directory = all_files_in_directory(vector_to_STR(current_directory, 1).c_str());
        }
        //printing all folder-file to choose. If empty folder list, ERROR break;
        if(all_directory.empty()){ return 0;}
        for(int i = 0; i < all_directory.size(); ++i){
            std::cout << i+11 << ") " << all_directory[i] << std::endl;
        }

        std::cin >> command;

        if(command == 0){
            break;
        } else if (command == 1 && !current_directory.empty()){
            if(current_directory.size() > 2){
                current_directory.pop_back();
            }
            current_directory.pop_back();
            current_directory.pop_back();
        } else if (command == 2) {
            if(current_directory.size() == 2){
                drive_info(vector_to_STR(current_directory, 0).c_str());
            }  else {
                std::cout << "invalid command" << std::endl;
            }

        } else if(command == 3){
            if(current_directory.size() < 2){
                std::cout << "invalid command" << std::endl;
            } else {
                create_directory(current_directory);
            }

        } else if(command == 4){
            if(current_directory.size() < 2){
                std::cout << "invalid command" << std::endl;
            } else {
                remove_directory(current_directory);
            }

        } else if(command == 5){
            if(current_directory.size() < 2){
                std::cout << "invalid command" << std::endl;
            } else {
                create_file(current_directory);
            }

        } else if(command == 6){
            if(current_directory.size() < 2){
                std::cout << "invalid command" << std::endl;
            } else {
                copy_file(current_directory);
            }

        } else if(command == 7){
            if(current_directory.size() < 2){
                std::cout << "invalid command" << std::endl;
            } else {
                move_file(current_directory);
            }

        } else if(command == 8){
            if(current_directory.size() < 2){
                std::cout << "invalid command" << std::endl;
            } else {
                file_information(current_directory);
            }

        } else if (command > 10){ // add folder to current directory
            if (command-11 >= all_directory.size()){
                std::cout << "wrong folder number" << std::endl;
                continue;
            }
            current_directory.emplace_back(all_directory[command-11]);
            if(current_directory.size() > 1){
                current_directory.emplace_back("\\");
            }
            current_directory.emplace_back("\\");
        } else {
            std::cout << "invalid command" << std::endl;
        }
    }

    return 0;
}
