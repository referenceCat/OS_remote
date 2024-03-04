//
// Created by referencecat on 04.03.24.
//

#include <list>
#include <sstream>
#include <chrono>
#include <iomanip>
#include "task1.h"
#include "iostream"
#define SEPARATOR(); std::cout << "-------------------------------------------------------------------------" << std::endl;
#define NEXT_LINE(); std::cout << std::endl;
#define DRIVES_NAMES_BUFFER_SIZE 1000
#define DRIVES_NAMES_MAX 100
#define DRIVE_VOLUME_NAME_BUFFER_SIZE 256
#define DRIVE_FILESYSTEM_NAME_BUFFER_SIZE 256


// source: https://gist.github.com/dgoguerra/7194777
// only for this function
static const char *humanSize(uint64_t bytes)
{
    char *suffix[] = {"B", "KB", "MB", "GB", "TB"};
    char length = sizeof(suffix) / sizeof(suffix[0]);

    int i = 0;
    double dblBytes = bytes;

    if (bytes > 1024) {
        for (i = 0; (bytes / 1024) > 0 && i<length-1; i++, bytes /= 1024)
            dblBytes = bytes / 1024.0;
    }

    static char output[200];
    sprintf(output, "%.02lf %s", dblBytes, suffix[i]);
    return output;
}

std::string humanSizeString(uint64_t bytes) {
    std::string result = humanSize(bytes);
    return result;
}

void newDir() {
    std::cout << "Enter directory path: ";
    std::string path;
    std::cin >> path;
    if (!CreateDirectoryA(path.c_str(), nullptr)) {
        std::cout << "Could not create new directory." << std::endl;
        return;
    }
    std::cout << "Directory successfully created." << std::endl;
}

void deleteDir() {
    std::cout << "Enter directory path: ";
    std::string path;
    std::cin >> path;
    if (!RemoveDirectoryA(path.c_str())) {
        std::cout << "Could not delete directory." << std::endl;
        return;
    }
    std::cout << "Directory successfully deleted." << std::endl;
}

void newFile() {
    std::cout << "Enter file path: ";

    std::string path;
    std::cin >> path;
    HANDLE f = CreateFileA(path.c_str(),
                           GENERIC_READ | GENERIC_WRITE,
                           0, nullptr,
                           CREATE_NEW,
                           FILE_ATTRIBUTE_NORMAL,
                           nullptr);
    if (f == nullptr || !CloseHandle(f)) {
        std::cout << "Could not create new file." << std::endl;
        return;
    }

    std::cout << "File successfully created." << std::endl;
};

bool yesNoMenu() {
    int option = -1;
    bool result;
    while (option != 1 && option != 2) {
        std::cout << "1. Yes" << std::endl;
        std::cout << "2. No" << std::endl;

        std::cin >> option;
        if (option == 1) result = true;
        else if (option == 2) result = false;
        else {
            std::cout << "Wrong input. Try again: " << std::endl;
        }
    }

    return result;
}

void copyFile() {
    std::cout << "Enter old file path:" << std::endl;

    std::string oldPath;
    std::cin >> oldPath;

    std::cout << "Enter new file path: " << std::endl;

    std::string newPath;
    std::cin >> newPath;

    std::cout << "Overwrite existent file if needed?" << std::endl;
    bool overwrite = yesNoMenu();

    if (!CopyFileA(oldPath.c_str(), newPath.c_str(), !overwrite)) {
        std::cout << "Could not copy file." << std::endl;
        return;
    }

    std::cout << "File successfully copied." << std::endl;
};

void moveFile() {
    std::cout << "Enter old file path:" << std::endl;

    std::string oldPath;
    std::cin >> oldPath;

    std::cout << "Enter new file path: " << std::endl;

    std::string newPath;
    std::cin >> newPath;

    if (!MoveFileA(oldPath.c_str(), newPath.c_str())) {
        std::cout << "Could not move file." << std::endl;
        return;
    }
    std::cout << "File successfully moved." << std::endl;
};

void drivesInfo() {
    char buffer[DRIVES_NAMES_BUFFER_SIZE];
    LPTSTR buffer_ptr = buffer;
    DWORD bufferSize = DRIVES_NAMES_BUFFER_SIZE;
    std::list<char*> drivesNames;
    assert(DRIVES_NAMES_BUFFER_SIZE > GetLogicalDriveStrings(bufferSize, buffer_ptr));

    if (buffer[0] == '\0') {
        std::cout << "Could not find any logical drives." << std::endl;
        return;
    }
    drivesNames.insert(drivesNames.end(), buffer);
    for (int cur = 0; cur < DRIVES_NAMES_BUFFER_SIZE - 1; cur++) {
        if (buffer[cur] == '\0') {
            if (buffer[cur + 1] == '\0') break;
            else drivesNames.insert(drivesNames.end(), buffer + cur + 1);
        }
    }
    NEXT_LINE();
    std::cout << "List of logical drives:" << std::endl;

    int counter = 0;
    for (char* driveName: drivesNames) {
        std::cout << ++counter << ". "<< driveName << std::endl;
    }
    NEXT_LINE();
    int option;
    std::cout << "Enter number of drive or 0 to go to main menu: ";
    std::cin >> option;

    if (option == 0) return;

    while (option - 1 > counter) {
        std::cout << "Wrong input. Try again: " << std::endl;
        std::cin >> option;
    }

    if (option == 0) return;
    NEXT_LINE();

    auto driveName = drivesNames.front();
    std::advance(driveName, option - 1);
    auto type = GetDriveTypeA(driveName);

    std::cout << "Type: ";
    switch (type) {
        case DRIVE_NO_ROOT_DIR:
            std::cout << "NO ROOT DIR";
            break;
        case DRIVE_REMOVABLE:
            std::cout << "REMOVABLE";
            break;
        case DRIVE_FIXED:
            std::cout << "FIXED";
            break;
        case DRIVE_REMOTE:
            std::cout << "REMOTE";
            break;
        case DRIVE_CDROM:
            std::cout << "CDROM";
            break;
        case DRIVE_RAMDISK:
            std::cout << "RAMDISK";
            break;

            case DRIVE_UNKNOWN: default:
            std::cout << "UNKNOWN";
            break;
    }

    NEXT_LINE();
    NEXT_LINE();

    char VolumeNameBuffer[DRIVE_VOLUME_NAME_BUFFER_SIZE];
    char FileSystemNameBuffer[DRIVE_FILESYSTEM_NAME_BUFFER_SIZE];
    unsigned long VolumeSerialNumber;
    unsigned long systemFlags;

    BOOL GetVolumeInformationFlag = GetVolumeInformationA(
            driveName,
            VolumeNameBuffer,
            DRIVE_VOLUME_NAME_BUFFER_SIZE,
            &VolumeSerialNumber,
            nullptr,
            &systemFlags,
            FileSystemNameBuffer,
            DRIVE_FILESYSTEM_NAME_BUFFER_SIZE);

    if (GetVolumeInformationFlag != 0) {
        if (VolumeNameBuffer[0] != '\0') std::cout << "Volume: " << VolumeNameBuffer <<std:: endl;
        std::cout << "Volume Serial Number: " << VolumeSerialNumber << std::endl;
        if (FileSystemNameBuffer[0] != '\0') std::cout << "File System: " << FileSystemNameBuffer << std::endl;
    } else
        std::cout << "Could not get information about this drive." << std::endl;

    NEXT_LINE();

    std::cout << "System flags: " << std::endl;
    for (auto& flag: systemFlagsMap) {
        if (flag.first & systemFlags) {
            std::cout << flag.second << std::endl;
        }
    }

    NEXT_LINE();

    ULARGE_INTEGER FreeBytesAvailable = {0};
    ULARGE_INTEGER TotalNumberOfBytes = {0};
    ULARGE_INTEGER TotalNumberOfFreeBytes = {0};

    BOOL GetDiskFreeSpaceFlag = GetDiskFreeSpaceExA(
            driveName,
            &FreeBytesAvailable,
            &TotalNumberOfBytes,
            &TotalNumberOfFreeBytes
    );

    if (GetDiskFreeSpaceFlag != 0) {
        auto availableSpace = humanSizeString(FreeBytesAvailable.QuadPart);
        auto totalSpace = humanSizeString(TotalNumberOfBytes.QuadPart);
        auto totalFreeSpace =humanSizeString( TotalNumberOfFreeBytes.QuadPart);

        std::cout << "Available space: " << availableSpace << std::endl;
        std::cout << "Total space: " << availableSpace << std::endl;
        std::cout << "Total free space: " << availableSpace << std::endl;
    } else
        std::cout << "Could not get drive space info." << std::endl;


};

void fileAttributes() {
    std::cout << "Enter file path: ";
    std::string path;
    std::cin >> path;

    std::list<std::string> attributesDescriptions;

    auto mask = GetFileAttributesA(path.c_str());
    if (mask == INVALID_FILE_ATTRIBUTES) {
        std::cout << "Invalid file attributesDescriptions!." << std::endl;
    }

    for (auto attribute: allAttributes) {
        if (mask & attribute) {
            attributesDescriptions.push_back(attributeMap.at(attribute));
        }
    }

    for (auto &description: attributesDescriptions) {
        std::cout << description << std::endl;
    }
};

std::string filetimeToHRF(FILETIME filetime) {
    SYSTEMTIME systemTime;
    FileTimeToSystemTime(&filetime, &systemTime);
    std::stringstream result;
    result << systemTime.wDay << "." << systemTime.wMonth << "." << systemTime.wYear << " "
                         << systemTime.wHour + 3 << ":" << systemTime.wMinute << std::endl;
    return result.str();

}

void readFileTime(FILETIME* filetime) {
    std::cout << "(dd.mm.yyyy-H:M): ";
    std::string input;
    std::cin >> input;
    std::tm tm = {};
    std::stringstream stream(input);
    stream >> std::get_time(&tm, "%d.%m.%Y-%H:%M");
    std::mktime(&tm);

    SYSTEMTIME systemTime;
    GetSystemTime(&systemTime);
    systemTime.wDay = tm.tm_mday;
    systemTime.wMonth = tm.tm_mon + 1;
    systemTime.wYear = tm.tm_year + 1900;
    systemTime.wHour = tm.tm_hour;
    systemTime.wMinute = tm.tm_min;

    SystemTimeToFileTime(&systemTime, filetime);
}

void fileTime() {
    std::string path;
    std::cout << "Enter file path: ";
    std::cin >> path;

    FILETIME createTime, lastAccessTime, lastWriteTime;

    auto file = CreateFile(path.c_str(),
                      GENERIC_READ | FILE_WRITE_ATTRIBUTES,
                      FILE_SHARE_READ | FILE_SHARE_WRITE,
                      nullptr,
                      OPEN_EXISTING,
                      FILE_ATTRIBUTE_NORMAL,
                      nullptr);

    if (file == INVALID_HANDLE_VALUE) {
        std::cout << "Failed to open file." << std::endl;
        return;
    }

    if (!GetFileTime(file, &createTime, &lastAccessTime, &lastWriteTime)) {
        std::cout << "Failed to get file time." << std::endl;
        CloseHandle(file);
        return;
    }

    std::cout << "File created at: " << filetimeToHRF(createTime);
    std::cout << "File accessed at: " << filetimeToHRF(lastAccessTime);
    std::cout << "File altered at: " << filetimeToHRF(lastWriteTime);

    std::cout << "Do you want to change file time?" << std::endl;
    bool changeFileTime = yesNoMenu();
    if (changeFileTime) {
        std::cout << "Enter time file created/accessed/altered at:" << std::endl;
        readFileTime(&createTime);
        readFileTime(&lastAccessTime);
        readFileTime(&lastWriteTime);
        SetFileTime(file,&createTime,&lastAccessTime,&lastWriteTime);
        std::cout << "File time successfully changed." << std::endl;
    }

    CloseHandle(file);
};

int mainMenu() {
    SEPARATOR();
    std::cout << "Options: " << std::endl;
    std::cout << "0. Quit" << std::endl;
    std::cout << "1. Data drives info" << std::endl;
    std::cout << "2. New folder" << std::endl;
    std::cout << "3. Delete folder" << std::endl;
    std::cout << "4. New file" << std::endl;
    std::cout << "5. Copy file" << std::endl;
    std::cout << "6. Move file" << std::endl;
    std::cout << "7. File attributes" << std::endl;
    std::cout << "8. File time" << std::endl;

    NEXT_LINE();
    std::cout << "Choose option: ";

    int option;
    std::cin >> option;

    switch (option) {
        case 0:
            return 0;

        case 1:
            drivesInfo();
            break;

        case 2:
            newDir();
            break;

        case 3:
            deleteDir();
            break;

        case 4:
            newFile();
            break;

        case 5:
            copyFile();
            break;

        case 6:
            moveFile();
            break;

        case 7:
            fileAttributes();
            break;

        case 8:
            fileTime();
            break;

        default:
            std::cout << "Wrong input. Try again" << std::endl;

    }
    return 1;
}

int main() {
    while (mainMenu());
    return 0;
}