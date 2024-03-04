//
// Created by yawgag on 21.02.2024.
//

#include <iostream>
#include "windows.h"
#include "string"
#include <vector>
#include <map>


std::vector <std::string> drives() {
    DWORD dwSize = MAX_PATH;
    char buffer[MAX_PATH] = {0};
    std::vector <std::string> all_directory;

    if(GetLogicalDriveStrings(dwSize,buffer)){
        all_directory.clear();
        char* singleLogicalDrive = buffer;
        while(*singleLogicalDrive){
            all_directory.emplace_back(singleLogicalDrive);
            singleLogicalDrive += strlen(singleLogicalDrive) + 1;
        }
    } else {
        std::cout << "ERROR" << std::endl;
    }
    return all_directory;
}

std::vector <std::string> all_files_in_directory(LPCSTR dir){
    std::vector <std::string> allFiles;

    WIN32_FIND_DATA FindFileData;
    HANDLE tmpFile = FindFirstFile(dir, &FindFileData);

    do{
        allFiles.emplace_back(FindFileData.cFileName);
    } while(FindNextFile(tmpFile, &FindFileData) != 0 );
    FindClose(tmpFile);

    return allFiles;
}

std::string vector_to_STR(const std::vector<std::string>& dir, int key){ //rewrite this
    std::string out;
    for(auto i : dir){
        for(auto j : i) {
            out.push_back(j);
        };
    }
    if(key == 1 ){
        out.push_back('*');
    }
    return out;
}

void drive_info(LPCSTR dir){
    const std::map<DWORD, std::string> SYSFLAGS{
        {FILE_CASE_SENSITIVE_SEARCH, "FILE_CASE_SENSITIVE_SEARCH"},
        {FILE_CASE_PRESERVED_NAMES, "FILE_CASE_PRESERVED_NAMES"},
        {FILE_UNICODE_ON_DISK, "FILE_UNICODE_ON_DISK"},
        {FILE_PERSISTENT_ACLS, "FILE_PERSISTENT_ACLS"},
        {FILE_FILE_COMPRESSION, "FILE_FILE_COMPRESSION"},
        {FILE_VOLUME_QUOTAS, "FILE_VOLUME_QUOTAS"},
        {FILE_SUPPORTS_SPARSE_FILES, "FILE_SUPPORTS_SPARSE_FILES"},
        {FILE_SUPPORTS_REPARSE_POINTS, "FILE_SUPPORTS_REPARSE_POINTS"},
        {FILE_SUPPORTS_REMOTE_STORAGE, "FILE_SUPPORTS_REMOTE_STORAGE"},
        {FILE_RETURNS_CLEANUP_RESULT_INFO, "FILE_RETURNS_CLEANUP_RESULT_INFO"},
        {FILE_SUPPORTS_POSIX_UNLINK_RENAME, "FILE_SUPPORTS_POSIX_UNLINK_RENAME"},
        {FILE_VOLUME_IS_COMPRESSED, "FILE_VOLUME_IS_COMPRESSED"},
        {FILE_SUPPORTS_OBJECT_IDS, "FILE_SUPPORTS_OBJECT_IDS"},
        {FILE_SUPPORTS_ENCRYPTION, "FILE_SUPPORTS_ENCRYPTION"},
        {FILE_NAMED_STREAMS, "FILE_NAMED_STREAMS"},
        {FILE_READ_ONLY_VOLUME, "FILE_READ_ONLY_VOLUME"},
        {FILE_SEQUENTIAL_WRITE_ONCE, "FILE_SEQUENTIAL_WRITE_ONCE"},
        {FILE_SUPPORTS_TRANSACTIONS, "FILE_SUPPORTS_TRANSACTIONS"},
        {FILE_SUPPORTS_HARD_LINKS, "FILE_SUPPORTS_HARD_LINKS"},
        {FILE_SUPPORTS_EXTENDED_ATTRIBUTES, "FILE_SUPPORTS_EXTENDED_ATTRIBUTES"},
        {FILE_SUPPORTS_OPEN_BY_FILE_ID, "FILE_SUPPORTS_OPEN_BY_FILE_ID"},
        {FILE_SUPPORTS_USN_JOURNAL, "FILE_SUPPORTS_USN_JOURNAL"},
        {FILE_SUPPORTS_INTEGRITY_STREAMS, "FILE_SUPPORTS_INTEGRITY_STREAMS"},
        {FILE_SUPPORTS_BLOCK_REFCOUNTING, "FILE_SUPPORTS_BLOCK_REFCOUNTING"},
        {FILE_SUPPORTS_SPARSE_VDL, "FILE_SUPPORTS_SPARSE_VDL"},
        {FILE_DAX_VOLUME, "FILE_DAX_VOLUME"},
        {FILE_SUPPORTS_GHOSTING, "FILE_SUPPORTS_GHOSTING"}
    };

    std::string drive_type_out, sys_flags_out;
    int driveType = GetDriveType(dir);
    if(driveType == 0 || driveType == 1){
        std::cout << "ERROR" << std::endl;
        return;
    } else {
        switch (driveType) {
            case 2: drive_type_out = "The drive has removable media; for example, a floppy drive, thumb drive, or flash card reader."; break;
            case 3: drive_type_out = "The drive has fixed media; for example, a hard disk drive or flash drive."; break;
            case 4: drive_type_out = "The drive is a remote (network) drive."; break;
            case 5: drive_type_out = "The drive is a CD-ROM drive."; break;
            case 6: drive_type_out = "The drive is a RAM disk."; break;
            default: drive_type_out = "ERROR"; break;
        }
    }


    char NameBuffer[MAX_PATH];
    char SysNameBuffer[MAX_PATH];
    DWORD VSNumber;
    DWORD MCLength;
    DWORD FileSF;

    if (!GetVolumeInformation(dir,NameBuffer, sizeof(NameBuffer),&VSNumber,&MCLength,&FileSF,SysNameBuffer,sizeof(SysNameBuffer))){
        std::cout << "ERROR" << std::endl;
        return;
    }

    DWORD sectors_per_cluster, byte_per_sectors, free_clusters, total_clusters;

    if(!GetDiskFreeSpaceA(dir, &sectors_per_cluster, &byte_per_sectors, &free_clusters, &total_clusters)){
        std::cout << "ERROR" << std::endl;
        return;
    }

    unsigned long long total_disk_space = (long long)sectors_per_cluster * (long long)byte_per_sectors * (long long)total_clusters / 1024 / 1024 / 1024;
    unsigned long long free_disk_space = (long long)byte_per_sectors*(long long)sectors_per_cluster*(long long)free_clusters / 1024 / 1024 / 1024;


    std::cout << "--------------DRIVE INFO--------------" << std::endl;
    std::cout << "Drive: " << dir << std::endl;
    std::cout << "Drive type is: " << drive_type_out << std::endl;
    std::cout << "Drive file system: " << SysNameBuffer << std::endl;
    std::cout << "System flags: " << sys_flags_out << " " << FileSF << std::endl;
    std::cout << "Total drive space(Gb): " << total_disk_space << std::endl;
    std::cout << "Free drive space(Gb): " << free_disk_space  << std::endl;
    std::cout << "Drive flags: "  << std::endl;
    for(const auto& i : SYSFLAGS){
        if((FileSF & i.first) == i.first){
            std::cout << i.second << std::endl;
        }
    }
    std::cout << "--------------------------------------" << std::endl;
}

void create_directory(std::vector<std::string>& dir){
    std::string folder_name;
    std::cout << "write folder name: ";
    std::cin >> folder_name;
    dir.push_back(folder_name);
    if(!CreateDirectory(vector_to_STR(dir, 0).c_str(), NULL)){
        std::cout << "ERROR" << std::endl;
    }
    dir.pop_back();
}

void remove_directory(std::vector<std::string>& dir){
    std::string folder_name;
    std::cout << "write folder name: ";
    std::cin >> folder_name;
    dir.push_back(folder_name);
    if(!RemoveDirectory(vector_to_STR(dir, 0).c_str())){
        std::cout << "ERROR" << std::endl;
    }
    dir.pop_back();
}

void create_file(std::vector<std::string>& dir){
    std::string file_name;
    std::cout << "input file name: ";
    std::cin >> file_name;
    dir.push_back(file_name);
    HANDLE file_handle;
    if(!CreateFile(vector_to_STR(dir, 0).c_str(), (GENERIC_READ|GENERIC_WRITE), FILE_SHARE_WRITE, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, &file_handle)){
        std::cout << "ERROR" << std::endl;
    }
    dir.pop_back();
    if(!CloseHandle(file_handle)){
        std::cout << "ERROR" << std::endl;
    }
}

void copy_file(std::vector<std::string>& dir){
    std::string copied_file_name, copy_name;
    std::vector<std::string> all_directory = all_files_in_directory(vector_to_STR(dir, 1).c_str());
    int command;

    std::cout << "0 - cancel copy process\nChoose file to copy:" << std::endl;
    for(int i = 0; i < all_directory.size(); ++i){
        std::cout << i+11 << ") " << all_directory[i] << std::endl;
    }

    std::cin >> command;

    if(command == 0){
        return;
    } else if(command-11 >= all_directory.size()){
        std::cout << "invalid file";
        return;
    } else {
        dir.push_back(all_directory[command-11]);
        copied_file_name = vector_to_STR(dir, 0);
        dir.pop_back();

        std::cout << "Input copy name: ";
        std::cin >> copy_name;
        dir.push_back(copy_name);
        copy_name = vector_to_STR(dir, 0);
        dir.pop_back();

         if(!CopyFile(copied_file_name.c_str(), copy_name.c_str(), true)){
             std::cout << "File with that name already exists, overwrite the file?\n1 - Yes\n2 - No\n" << std::endl;
             std::cin >> command;
             if(command == 1){
                 if(CopyFile(copied_file_name.c_str(), copy_name.c_str(), false)){
                     std::cout << "file copied successfully" << std::endl;
                 } else {
                     std::cout << "ERROR" << std::endl;
                 }
             }
         } else {
             std::cout << "file copied successfully" << std::endl;
         }
    }



}

void move_file(std::vector<std::string> dir){
    std::vector<std::string> all_directory = all_files_in_directory(vector_to_STR(dir, 1).c_str());
    for(int i = 0; i < all_directory.size(); ++i){std::cout << i+11 << ") " << all_directory[i] << std::endl;}
    int moving_file_number;

    std::cout << "Input number of moving file: ";
    std::cin >> moving_file_number;

    if(moving_file_number-11 < 0 || moving_file_number-11 >= all_directory.size()){
        std::cout << "wrong file number" << std::endl;
        return;
    }

    std::vector<std::string> moving_file = dir;
    moving_file.emplace_back(all_directory[moving_file_number-11]);

    int command = 1;

    while(command != 0){
        std::cout << "current directory: " << vector_to_STR(dir, 0) << std::endl;
        std::cout << "0 - cancel moving process" << std::endl;
        std::cout << "1 - Back to previous directory" << std::endl;
        std::cout << "2 - move file in this directory" << std::endl;

        if(dir.size() >= 2) {
            all_directory = all_files_in_directory(vector_to_STR(dir, 1).c_str());
        } else {
            all_directory = drives();
        }

        for(int i = 0; i < all_directory.size(); ++i){std::cout << i+11 << ") " << all_directory[i] << std::endl;}

        std::cin >> command;
        if(command == 0){
            return ;
        } else if (command == 1){
            if(dir.size() > 2){
                dir.pop_back();
            }
            dir.pop_back();
            dir.pop_back();
        } else if (command > 10 && command < all_directory.size()+11){
            dir.emplace_back(all_directory[command-11]);
            if(dir.size() > 1){
                dir.emplace_back("\\");
            }
            dir.emplace_back("\\");
        } else if (command == 2){
            dir.emplace_back(moving_file[moving_file.size()-1]);
            std::cout << vector_to_STR(dir, 0) << std::endl;
            DWORD file_exist = GetFileAttributes(vector_to_STR(dir, 0).c_str());
            if(file_exist != INVALID_FILE_ATTRIBUTES){
                std::cout << "file already exist in current path" << std::endl;
                std::cout << "0 - exit from moving process" << std::endl;
                std::cout << "1 - rename moved file" << std::endl;
                std::cin>> command;
                while(command != 1 && command != 0){
                    std::cout << "invalid command. Write again";
                    std::cin >> command;
                }
                if(command == 0){return;}

                std::string new_file_name;
                std::cout << "write new file name: ";
                std::cin >> new_file_name;
                dir.pop_back();
                dir.emplace_back(new_file_name);
            }

            MoveFile(vector_to_STR(moving_file, 0).c_str(), vector_to_STR(dir, 0).c_str());
            dir.pop_back();
            return;

        } else {
            std::cout << "invalid command" << std::endl;
        }

    }
}

void file_information(std::vector<std::string> dir){

    std::string file_attributes;
    FILETIME file_creation_time, file_last_write_time, file_last_access_time;

    std::vector<std::string> all_directory = all_files_in_directory(vector_to_STR(dir, 1).c_str());
    for(int i = 0; i < all_directory.size(); ++i){std::cout << i+11 << ") " << all_directory[i] << std::endl;}
    int selected_file;

    std::cout << "Input number of file: ";
    std::cin >> selected_file;

    if(selected_file-11 < 0 || selected_file-11 >= all_directory.size()){
        std::cout << "wrong file number" << std::endl;
        return;
    }
    dir.emplace_back(all_directory[selected_file-11]);
    HANDLE file_handle;
    DWORD attr_info = GetFileAttributes(vector_to_STR(dir, 0).c_str());
    HANDLE time_info = CreateFile(vector_to_STR(dir, 0).c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, &file_handle);


    if(attr_info){
        switch(attr_info){
            case FILE_ATTRIBUTE_READONLY: file_attributes = "FILE_ATTRIBUTE_READONLY"; break;
            case FILE_ATTRIBUTE_HIDDEN: file_attributes = "FILE_ATTRIBUTE_HIDDEN"; break;
            case FILE_ATTRIBUTE_SYSTEM: file_attributes = "FILE_ATTRIBUTE_SYSTEM"; break;
            case FILE_ATTRIBUTE_DIRECTORY: file_attributes = "FILE_ATTRIBUTE_DIRECTORY"; break;
            case FILE_ATTRIBUTE_ARCHIVE: file_attributes = "FILE_ATTRIBUTE_ARCHIVE"; break;
            case FILE_ATTRIBUTE_DEVICE: file_attributes = "FILE_ATTRIBUTE_DEVICE"; break;
            case FILE_ATTRIBUTE_NORMAL: file_attributes = "FILE_ATTRIBUTE_NORMAL"; break;
            case FILE_ATTRIBUTE_TEMPORARY: file_attributes = "FILE_ATTRIBUTE_TEMPORARY"; break;
            case FILE_ATTRIBUTE_SPARSE_FILE: file_attributes = "FILE_ATTRIBUTE_SPARSE_FILE"; break;
            case FILE_ATTRIBUTE_REPARSE_POINT: file_attributes = "FILE_ATTRIBUTE_REPARSE_POINT"; break;
            case FILE_ATTRIBUTE_OFFLINE: file_attributes = "FILE_ATTRIBUTE_OFFLINE"; break;
            case FILE_ATTRIBUTE_NOT_CONTENT_INDEXED: file_attributes = "FILE_ATTRIBUTE_NOT_CONTENT_INDEXED"; break;
            case FILE_ATTRIBUTE_ENCRYPTED: file_attributes = "FILE_ATTRIBUTE_ENCRYPTED"; break;
            case FILE_ATTRIBUTE_INTEGRITY_STREAM: file_attributes = "FILE_ATTRIBUTE_INTEGRITY_STREAM"; break;
            case FILE_ATTRIBUTE_VIRTUAL: file_attributes = "FILE_ATTRIBUTE_VIRTUAL"; break;
            case FILE_ATTRIBUTE_NO_SCRUB_DATA: file_attributes = "FILE_ATTRIBUTE_NO_SCRUB_DATA"; break;
            case FILE_ATTRIBUTE_EA: file_attributes = "FILE_ATTRIBUTE_EA"; break;
            case FILE_ATTRIBUTE_PINNED: file_attributes = "FILE_ATTRIBUTE_PINNED"; break;
            case FILE_ATTRIBUTE_UNPINNED: file_attributes = "FILE_ATTRIBUTE_UNPINNED"; break;
            case FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS: file_attributes = "FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS"; break;
        }
        GetFileTime(time_info, &file_creation_time,&file_last_access_time,&file_last_write_time);
    } else {
        std::cout << "ERROR" << std::endl;
        return;
    }

    SYSTEMTIME crt, lwt, lat;
    FileTimeToSystemTime(&file_creation_time, &crt);
    FileTimeToSystemTime(&file_last_write_time, &lwt);
    FileTimeToSystemTime(&file_last_access_time, &lat);
    std::cout << "File attribute: " << file_attributes << std::endl;
    std::cout << "Creation time: " << crt.wDay << "-" << crt.wMonth << '-' << crt.wYear <<"  " << crt.wHour << ":" << crt.wMinute << ":" << crt.wSecond << std::endl;
    std::cout << "Last access time: " << lat.wDay << "-" << lat.wMonth << '-' << lat.wYear <<"  " << lat.wHour << ":" << lat.wMinute << ":" << lat.wSecond << std::endl;
    std::cout << "Last write time: " << lwt.wDay << "-" << lwt.wMonth << '-' << lwt.wYear <<"  " << lwt.wHour << ":" << lwt.wMinute << ":" << lwt.wSecond << std::endl;

    std::cout << "0 - exit from file attributes" << std::endl;
    std::cout << "1 - edit file attributes" << std::endl;

    int command;
    std::cin >> command;
    if(command == 1){
        std::cout << "0) exit from attributes editing" << std::endl;
        std::cout << "1) FILE_ATTRIBUTE_ARCHIVE" << std::endl;
        std::cout << "2) FILE_ATTRIBUTE_HIDDEN" << std::endl;
        std::cout << "3) FILE_ATTRIBUTE_NORMAL" << std::endl;
        std::cout << "4) FILE_ATTRIBUTE_NOT_CONTENT_INDEXED" << std::endl;
        std::cout << "5) FILE_ATTRIBUTE_OFFLINE" << std::endl;
        std::cout << "6) FILE_ATTRIBUTE_READONLY" << std::endl;
        std::cout << "7) FILE_ATTRIBUTE_SYSTEM" << std::endl;
        std::cout << "8) FILE_ATTRIBUTE_TEMPORARY" << std::endl;
        std::cin >> command;
        if(command == 0){
            return;
        } else {
            switch(command){
                case 1: SetFileAttributes(vector_to_STR(dir, 0).c_str(), FILE_ATTRIBUTE_ARCHIVE); break;
                case 2: SetFileAttributes(vector_to_STR(dir, 0).c_str(), FILE_ATTRIBUTE_HIDDEN); break;
                case 3: SetFileAttributes(vector_to_STR(dir, 0).c_str(), FILE_ATTRIBUTE_NORMAL); break;
                case 4: SetFileAttributes(vector_to_STR(dir, 0).c_str(), FILE_ATTRIBUTE_NOT_CONTENT_INDEXED); break;
                case 5: SetFileAttributes(vector_to_STR(dir, 0).c_str(), FILE_ATTRIBUTE_OFFLINE); break;
                case 6: SetFileAttributes(vector_to_STR(dir, 0).c_str(), FILE_ATTRIBUTE_READONLY); break;
                case 7: SetFileAttributes(vector_to_STR(dir, 0).c_str(), FILE_ATTRIBUTE_SYSTEM); break;
                case 8: SetFileAttributes(vector_to_STR(dir, 0).c_str(), FILE_ATTRIBUTE_TEMPORARY); break;
            }
        }
    }


    dir.pop_back();
    if(!CloseHandle(file_handle)){
        std::cout << "ERROR" << std::endl;
    }
}

void print_menu(){
    std::cout << "--------------MENU--------------" << std::endl;
    std::cout << "0 - EXIT" << std::endl;
    std::cout << "1 - Back to previous directory" << std::endl;
    std::cout << "2 - Get info about current directory" << std::endl;
    std::cout << "3 - Create folder" << std::endl;
    std::cout << "4 - Remove folder" << std::endl;
    std::cout << "5 - Create file" << std::endl;
    std::cout << "6 - Copy file to current directory" << std::endl;
    std::cout << "7 - Move file to another directory" << std::endl;
    std::cout << "8 - File information" << std::endl;
    std::cout << "XX - open selected directory" << std::endl;
    std::cout << "--------------------------------" << std::endl;
}


