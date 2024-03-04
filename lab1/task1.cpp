//
// Created by referencecat on 04.03.24.
//

#include "task1.h"
#include "iostream"
#define SEPARATOR(); std::cout << "----------------------------------------------" << std::endl;
#define EMPTY_LINE(); std::cout << std::endl;

struct Directory {

};

void printCurrentDir() {};

void changeCurrentDir() {};

void goToParentDir() {};

void printCurrentDirInfo() {};

void newDir() {};

void deleteDir() {};

void newFile() {};

void copyFile() {};

void moveFile() {};

void printFileInfo() {};

void printDrivesList() {};

void driveInfo() {};

int mainMenu() {
    SEPARATOR();
    std::cout << "Options: " << std::endl;
    std::cout << "0. Quit" << std::endl;
    std::cout << "1. Current directory" << std::endl;
    std::cout << "2. Change current directory" << std::endl;
    std::cout << "3. Go to parent directory" << std::endl;
    std::cout << "4. Current directory info" << std::endl;
    std::cout << "5. New folder" << std::endl;
    std::cout << "6. Delete folder" << std::endl;
    std::cout << "7. New file" << std::endl;
    std::cout << "8. Copy file to current directory" << std::endl;
    std::cout << "9. Move file to another directory" << std::endl;
    std::cout << "10. File info" << std::endl;
    std::cout << "11. Data drives info" << std::endl;
    EMPTY_LINE();
    std::cout << "Choose option: ";

    int option;
    std::cin >> option;

    switch (option) {
        case 0:
            return 0;

        case 1:
            printCurrentDir();
            break;

        case 2:
            changeCurrentDir();
            break;

        case 3:
            goToParentDir();
            break;

        case 4:
            printCurrentDirInfo();
            break;

        case 5:
            newDir();
            break;

        case 6:
            deleteDir();
            break;

        case 7:
            newFile();
            break;

        case 8:
            copyFile();
            break;

        case 9:
            moveFile();
            break;

        case 10:
            printFileInfo();
            break;

        case 11:
            printFileInfo();
            break;

        case 12:
            printDrivesList();
            driveInfo();
            break;

        default:
            std::cout << "Wrong input. Try again" << std::endl;

    }
    return 1;
}

int main() {
    bool running = true;
    while (running) {
        running = mainMenu();
    }
    return 0;
}