//
// Created by referenceCat on 03.06.2024.
//


#include <windows.h>
#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <conio.h>

#define WRONG_SYMBOL "Wrong input."

using namespace std;


// function *stolen* small_amogus_file.txt https://gist.github.com/dgoguerra/7194777
static const char *humanSize(uint64_t bytes) {
    char *suffix[] = {"B", "KB", "MB", "GB", "TB"};
    char length = sizeof(suffix) / sizeof(suffix[0]);

    int i = 0;
    double dblBytes = bytes;

    if (bytes > 1024) {
        for (i = 0; (bytes / 1024) > 0 && i < length - 1; i++, bytes /= 1024)
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

// type -> string
string TypeToString(DWORD type) {
    switch (type) {
        case MEM_PRIVATE:
            return "Private";
        case MEM_MAPPED:
            return "Mapped";
        case MEM_IMAGE:
            return "Image";
        default:
            return "Unknown";
    }
}

// protect attribute -> string
string ProtectToString(DWORD protect) {
    switch (protect) {
        case PAGE_NOACCESS:
            return "No Access";
        case PAGE_READONLY:
            return "Read Only";
        case PAGE_READWRITE:
            return "Read/Write";
        case PAGE_WRITECOPY:
            return "Write Copy";
        case PAGE_EXECUTE:
            return "Execute Only";
        case PAGE_EXECUTE_READ:
            return "Execute/Read";
        case PAGE_EXECUTE_READWRITE:
            return "Execute/Read/Write";
        case PAGE_EXECUTE_WRITECOPY:
            return "Execute/Write Copy";
        default:
            return "Unknown";
    }
}

struct ProtectionInfo {
    int number;
    std::string description;
};

map<int, DWORD> protectMap = {
        {1, PAGE_NOACCESS},
        {2, PAGE_READONLY},
        {3, PAGE_READWRITE},
        {4, PAGE_WRITECOPY},
        {5, PAGE_EXECUTE},
        {6, PAGE_EXECUTE_READ},
        {7, PAGE_EXECUTE_READWRITE},
        {8, PAGE_EXECUTE_WRITECOPY}
};

vector<ProtectionInfo> protectionInfoList = {
        {1, "PAGE_NOACCESS"},
        {2, "PAGE_READONLY"},
        {3, "PAGE_READWRITE"},
        {4, "PAGE_WRITECOPY"},
        {5, "PAGE_EXECUTE"},
        {6, "PAGE_EXECUTE_READ"},
        {7, "PAGE_EXECUTE_READWRITE"},
        {8, "PAGE_EXECUTE_WRITECOPY"}
};

string protectToString(DWORD protect) {
    switch (protect) {
        case PAGE_NOACCESS:
            return "No Access";
        case PAGE_READONLY:
            return "Read Only";
        case PAGE_READWRITE:
            return "Read/Write";
        case PAGE_WRITECOPY:
            return "Write Copy";
        case PAGE_EXECUTE:
            return "Execute Only";
        case PAGE_EXECUTE_READ:
            return "Execute/Read";
        case PAGE_EXECUTE_READWRITE:
            return "Execute/Read/Write";
        case PAGE_EXECUTE_WRITECOPY:
            return "Execute/Write Copy";
        default:
            return "Unknown";
    }
}

void printSystemInfo() {
    SYSTEM_INFO systemInfo;
    GetSystemInfo(&systemInfo);

    std::cout << "Processor Architecture: ";
    switch (systemInfo.wProcessorArchitecture) {
        case PROCESSOR_ARCHITECTURE_INTEL:
            std::cout << "x86 (32-bit)" << std::endl;
            break;
        case PROCESSOR_ARCHITECTURE_AMD64:
            std::cout << "x64 (64-bit)" << std::endl;
            break;
        case PROCESSOR_ARCHITECTURE_ARM:
            std::cout << "ARM" << std::endl;
            break;
        default:
            std::cout << "Unknown" << std::endl;
            break;
    }
}

void printGlobalMemoryStatus() {
    MEMORYSTATUSEX memoryStatus;
    memoryStatus.dwLength = sizeof(memoryStatus);
    GlobalMemoryStatusEx(&memoryStatus);

    std::cout << "Total physical memory: " << humanSizeString(memoryStatus.ullTotalPhys) << std::endl;
    std::cout << "Available physical memory: " <<  humanSizeString(memoryStatus.ullAvailPhys) << std::endl;
    std::cout << "Used memory: " <<  humanSizeString(memoryStatus.ullTotalPhys - memoryStatus.ullAvailPhys) << std::endl;
    std::cout << "Total virtual memory: " <<  humanSizeString(memoryStatus.ullTotalVirtual) << std::endl;
    std::cout << "Available virtual memory: " <<  humanSizeString(memoryStatus.ullAvailVirtual) << std::endl;

}

void determineSiteCondition() {
    PVOID address;
    std::cout << "Enter memory address (in hexadecimal): ";
    std::cin >> address;

    MEMORY_BASIC_INFORMATION memoryInfo;

    SIZE_T result = VirtualQuery(address, &memoryInfo, sizeof(memoryInfo));
    if (result == sizeof(memoryInfo)) {
        std::cout << "Region size: " << memoryInfo.RegionSize << " bytes" << std::endl;
        std::cout << "Base address: " << memoryInfo.BaseAddress << std::endl;
        std::cout << "Allocation base: " << memoryInfo.AllocationBase << std::endl;
        std::cout << "Allocation Protect: " << ProtectToString(memoryInfo.AllocationProtect) << std::endl;
        std::cout << "State: ";
        switch (memoryInfo.State) {
            case MEM_COMMIT:
                std::cout << "Committed" << std::endl;
                break;
            case MEM_FREE:
                std::cout << "Free" << std::endl;
                break;
            case MEM_RESERVE:
                std::cout << "Reserved" << std::endl;
                break;
            default:
                std::cout << "Unknown" << std::endl;
                break;
        }
        std::cout << "Protect: " << ProtectToString(memoryInfo.Protect) << std::endl;
        std::cout << "Type: " << TypeToString(memoryInfo.Type) << std::endl;
    } else {
        std::cerr << "VirtualQuery failed." << std::endl;
    }
}

void reserveAndUseVirtualMemory(bool use) {
    LPVOID lpAddress;
    SIZE_T dwSize;
    char choose;

        std::cout << "Enter size of memory region to reserve (in bytes): ";
        std::cin >> dwSize;


    std::cout << "Manual allocation? y/n: ";
    std::cin >> choose;

    if (choose == 'y') {
        std::cout << "Enter memory address to reserve: ";
        std::cin >> std::hex >> lpAddress;
        LPVOID lpMemory = VirtualAlloc(lpAddress, dwSize, MEM_COMMIT, PAGE_READWRITE);
        if (lpMemory == nullptr) {
            std::cerr << "Failed to allocate memory region." << std::endl;
            return;
        }

        std::cout << "Memory region allocated at address: " << lpMemory << std::endl;
        if (!use)
        VirtualFree(lpMemory, 0, MEM_RELEASE);
    } else if (choose == 'n') {
        LPVOID lpMemory = VirtualAlloc(nullptr, dwSize, MEM_COMMIT, PAGE_READWRITE);
        if (lpMemory == nullptr) {
            std::cerr << "Failed to allocate memory region." << std::endl;
            return;
        }

        std::cout << "Memory region allocated at address: " << lpMemory << std::endl;
        if (!use)
        VirtualFree(lpMemory, 0, MEM_RELEASE);
    } else {
        std::cerr << WRONG_SYMBOL << std::endl;
    }
}

void readData(){
    uintptr_t address;
    std::cout << "Enter memory address to read from: ";
    std::cin >> std::hex >> address;

    HANDLE hProcess = GetCurrentProcess();

    const int bufferSize = 256;
    char buffer[bufferSize];

    SIZE_T bytesRead;
    if (ReadProcessMemory(hProcess, (LPCVOID)address, buffer, bufferSize, &bytesRead) && bytesRead > 0) {
        std::cout << "Data read from memory: " << std::string(buffer, bytesRead) << std::endl;
    } else {
        std::cerr << "Failed to read data from memory." << std::endl;
    }

    CloseHandle(hProcess);
}

void writeData() {
    uintptr_t address;
    std::cout << "Enter memory address to write data to: ";
    std::cin >> std::hex >> address;

    if (std::cin.fail()) {
        std::cerr << "Invalid memory address." << std::endl;
        return;
    }

    std::string data;
    std::cout << "Enter data to write to memory: ";
    std::cin.ignore();
    std::getline(std::cin, data);

    HANDLE hProcess = GetCurrentProcess();

    auto lpAddress = (LPVOID) address;

    SIZE_T bytesWritten;
    if (WriteProcessMemory(hProcess, lpAddress, data.c_str(), data.size(), &bytesWritten) && bytesWritten == data.size()) {
        std::cout << "Data successfully written to memory." << std::endl;
    } else {
        std::cerr << "Failed to write data to memory." << std::endl;
    }

    CloseHandle(hProcess);
}

void setProtect() {
    PVOID address;
    std::cout << "Enter memory address (in hexadecimal): ";
    std::cin >> std::hex >> address;

    SIZE_T size;
    std::cout << "Enter memory size: ";
    std::cin >> size;

    std::cout << "Choose protection attribute:" << std::endl;
    for (const auto &info: protectionInfoList) {
        std::cout << info.number << ". " << info.description << std::endl;
    }

    int choice;
    std::cout << "Enter choice: ";
    std::cin >> choice;

    if (protectMap.find(choice) == protectMap.end()) {
        std::cerr << "Invalid choice." << std::endl;
        return;
    }

    DWORD protect = protectMap[choice];
    DWORD oldProtect;
    if (VirtualProtect(address, size, protect, &oldProtect)) {
        std::cout << "Protection changed successfully." << std::endl;
        std::cout << "Old protection: " << protectToString(oldProtect) << std::endl;
    } else {
        std::cerr << "VirtualProtect failed." << std::endl;

    }

    MEMORY_BASIC_INFORMATION memoryInfo;
    SIZE_T result = VirtualQuery(address, &memoryInfo, sizeof(memoryInfo));
    if (result != 0) {
        std::cout << "Current protection: " << protectToString(memoryInfo.Protect) << std::endl;
    } else {
        std::cerr << "VirtualQuery failed." << std::endl;
        return;
    }

}

void freeMemory() {
    LPVOID lpAddress;
    SIZE_T dwSize;

    std::cout << "Enter memory address to free: ";
    std::cin >> std::hex >> lpAddress;

    std::cout << "Enter size of memory region to free (in bytes): ";
    std::cin >> dwSize;

    VirtualFree(lpAddress, dwSize, MEM_DECOMMIT);
    std::cout << "Physical memory released." << std::endl;

    VirtualFree(lpAddress, dwSize, MEM_RELEASE);
    std::cout << "Address region released." << std::endl;
}

int main() {
    int menu_option;

    do {
        std::cout << " 0) exit\n"
                << " 1) get system info\n"
                << " 2) get virtual mem status\n"
                << " 3) get virtual query\n"
                << " 4) reserve mem\n"
                << " 5) reserve and use mem \n"
                << " 6) write data\n"
                << " 7) set protect\n"
                << " 8) free mem\n" << std::endl;

        std::cin >> menu_option;
        switch (menu_option) {
            case 1:
                printSystemInfo();
                getch();
                break;
            case 2:
                printGlobalMemoryStatus();
                getch();
                break;
            case 3:
                determineSiteCondition();
                getch();
                break;
            case 4:
                reserveAndUseVirtualMemory(false);
                getch();
                break;
            case 5:
                reserveAndUseVirtualMemory(true);
                getch();
                break;
            case 6:
                writeData();
                getch();
                break;
            case 7:
                setProtect();
                getch();
                break;
            case 8:
                freeMemory();
                getch();
                break;
            case 9:
                readData();
                getch();
                break;
            default:
                std::cout << WRONG_SYMBOL << std::endl;
                break;
        }
        system("cls");
    } while (menu_option != 0);
}

