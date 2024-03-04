//
// Created by referenceCat on 04.03.2024.
//

#ifndef OS_DATA_H
#define OS_DATA_H
#define FILE_ATTRIBUTES_NUMBER 14 // 21?

constexpr std::array<int, FILE_ATTRIBUTES_NUMBER> attributeEnums = {
        FILE_ATTRIBUTE_HIDDEN,
        FILE_ATTRIBUTE_SYSTEM,
        FILE_ATTRIBUTE_DIRECTORY,
        FILE_ATTRIBUTE_ARCHIVE,
        FILE_ATTRIBUTE_DEVICE,
        FILE_ATTRIBUTE_NORMAL,
        FILE_ATTRIBUTE_TEMPORARY,
        FILE_ATTRIBUTE_SPARSE_FILE,
        FILE_ATTRIBUTE_REPARSE_POINT,
        FILE_ATTRIBUTE_COMPRESSED,
        FILE_ATTRIBUTE_OFFLINE,
        FILE_ATTRIBUTE_NOT_CONTENT_INDEXED,
        FILE_ATTRIBUTE_ENCRYPTED,
//        FILE_ATTRIBUTE_INTEGRITY_STREAM,
        FILE_ATTRIBUTE_VIRTUAL,
//        FILE_ATTRIBUTE_NO_SCRUB_DATA,
//        FILE_ATTRIBUTE_EA,
//        FILE_ATTRIBUTE_PINNED,
//        FILE_ATTRIBUTE_UNPINNED,
//        FILE_ATTRIBUTE_RECALL_ON_OPEN,
//        FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS
};

std::map<int, std::string> attributeMap = {
        {FILE_ATTRIBUTE_HIDDEN,                "Hidden"},
        {FILE_ATTRIBUTE_SYSTEM,                "Part of OS"},
        {FILE_ATTRIBUTE_DIRECTORY,             "Directory"},
        {FILE_ATTRIBUTE_ARCHIVE,               "Marked for backup or removal"},
        {FILE_ATTRIBUTE_DEVICE,                "Reserved for system use"},
        {FILE_ATTRIBUTE_NORMAL,                "Normal"},
        {FILE_ATTRIBUTE_TEMPORARY,             "Temporary"},
        {FILE_ATTRIBUTE_SPARSE_FILE,           "Sparse file"},
        {FILE_ATTRIBUTE_REPARSE_POINT,         "Reparse point or symbolic link"},
        {FILE_ATTRIBUTE_COMPRESSED,            "Compressed"},
        {FILE_ATTRIBUTE_OFFLINE,               "Offline"},
        {FILE_ATTRIBUTE_NOT_CONTENT_INDEXED,   "Not content indexed"},
        {FILE_ATTRIBUTE_ENCRYPTED,             "Encrypted"},
//        {FILE_ATTRIBUTE_INTEGRITY_STREAM,      "Integrity stream"},
        {FILE_ATTRIBUTE_VIRTUAL,               "Virtual"},
//        {FILE_ATTRIBUTE_NO_SCRUB_DATA,         "No scrub data"},
//        {FILE_ATTRIBUTE_EA,                    "Extended attributes"},
//        {FILE_ATTRIBUTE_PINNED,                "Pinned"},
//        {FILE_ATTRIBUTE_UNPINNED,              "Unpinned"},
//        {FILE_ATTRIBUTE_RECALL_ON_OPEN,        "Recall on open"},
//        {FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS, "Recall on data access"}
};


std::map<DWORD, std::string> systemFlagsMap = {
        {FS_CASE_IS_PRESERVED,                "The file system preserves the case of file names."},
        {FS_CASE_SENSITIVE,                "The file system uses case-sensitive file names."},
        {FS_UNICODE_STORED_ON_DISK,             "The file system supports Unicode in file names as they appear on disk."},
        {FS_PERSISTENT_ACLS,               "The file system preserves and enforces access control lists (ACLs)."},
        {FS_FILE_COMPRESSION,                "The file system supports file-based compression, where individual files can be compressed while others are not."},
        {FS_VOL_IS_COMPRESSED,                "The entire volume is compressed; for example, DoubleSpace has been used on the disk."},
        {FILE_NAMED_STREAMS,             "The file system supports named streams."},
        {FILE_SUPPORTS_ENCRYPTION,           "The file system supports the Encrypted File System (EFS)."},
        {FILE_SUPPORTS_OBJECT_IDS,         "The file system supports object identifiers."},
        {FILE_SUPPORTS_REPARSE_POINTS,            "The file system supports reparse points."},
        {FILE_SUPPORTS_SPARSE_FILES,               "The file system supports sparse files."},
        {FILE_VOLUME_QUOTAS,   "The file system supports disk quotas."}
};

#endif //OS_DATA_H
