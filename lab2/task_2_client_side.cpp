//
// Created by referencecat on 03.06.24.
//

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <cstdio>
#include <cstdlib>
#define FILEPATH "/home/referencecat/CLionProjects/OS/lab2/lab2_task2_file.txt"


int main() {
    int menu_option;
    int file;
    size_t file_size;
    struct stat file_stats{};
    char *pointer_to_file_content = nullptr;
    do {
        std::cout
                << " Choose option: \n"
                << " 1) perform projection\n"
                << " 2) read data\n"
                << " 3) exit\n";
        std::cin >> menu_option;
        switch (menu_option) {
            case 1:
                file = open(FILEPATH, O_RDONLY);
                if (file == -1) {
                    perror("Error opening file");
                    exit(1);
                }


                if (fstat(file, &file_stats) == -1) {
                    perror("Errorint &file, char **pointer_to_mapped_file getting file size");
                    close(file);
                    exit(1);
                }
                file_size = file_stats.st_size;
                pointer_to_file_content = (char *)mmap(nullptr, file_stats.st_size, PROT_READ, MAP_SHARED, file,0);

                if (pointer_to_file_content == MAP_FAILED) {
                    perror("Error mapping file");
                    close(file);
                    exit(1);
                }

                std::cout << "Got mapped file successfully" << std::endl;
                break;
            case 2: {
                if (pointer_to_file_content == nullptr) {
                    std::cout<<"file isnt mapped yet" << std::endl;
                    break;
                }
                printf("Client received: %s\n", pointer_to_file_content);
                break;
            }
            case 3:
                munmap(pointer_to_file_content, file_size);
                close(file);
                printf("Exiting program.\n");
                break;
            default:
                printf("Invalid menu_option, please try again.\n");
        }
    } while (menu_option != 3);
    return 0;
}