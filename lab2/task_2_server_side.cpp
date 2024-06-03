//
// Created by referencecat on 03.06.24.
//

#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>

#define FILEPATH "/home/referencecat/CLionProjects/OS/lab2/lab2_task2_file.txt"
#define FILESIZE (sizeof(char) * 1000)


int main() {
    int menu_option;
    int file;
    char *pointer_to_file_content = nullptr;
    do {
        std::cout
                << " Choose option: \n"
                << " 1) create file\n"
                << " 2) write data to mapped file\n"
                << " 3) delete file and exit\n";
        std::cin >> menu_option;
        switch (menu_option) {
            case 1:
                file = open(FILEPATH, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR); // create file to rw with user permission to rw
                if (file == -1) {
                    perror("Error opening file");
                    exit(1);
                }
                if (ftruncate(file, FILESIZE) == -1) {
                    perror("Error setting file size");
                    close(file);
                    exit(1);
                }
                pointer_to_file_content = (char *) mmap(nullptr, FILESIZE, PROT_READ | PROT_WRITE, MAP_SHARED, file,
                                                        0); // map entire file to rw and opens it for other processes
                if (pointer_to_file_content == MAP_FAILED) {
                    perror("Error mapping file");
                    close(file);
                    exit(1);
                }
                memset(pointer_to_file_content, 0, FILESIZE); // clearing file to 0
                break;
            case 2:
                if (pointer_to_file_content != nullptr) {
                    std::string input;
                    std::cout << "Enter message: ";
                    std::getchar();
                    std::getline(std::cin, input);

                    sprintf(pointer_to_file_content, "%s", input.c_str());
                } else {
                    printf("Memory is not mapped yet.\n");
                }
                break;
            case 3:
                if (pointer_to_file_content != nullptr) {
                    munmap(pointer_to_file_content, FILESIZE); // unmaps file
                    close(file);
                    unlink(FILEPATH); // removes file from file system
                    pointer_to_file_content = nullptr;
                }
                printf("Exiting program.\n");
                break;
            default:
                printf("Invalid menu_option, please try again.\n");
        }
    } while (menu_option != 3);

    return 0;
}