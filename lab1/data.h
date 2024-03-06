//
// Created by referenceCat on 04.03.2024.
//

#ifndef OS_DATA_H
#define OS_DATA_H
#define FILE_ATTRIBUTES_NUMBER 14 // 21?

// source: https://learn.microsoft.com/en-us/windows/win32/fileio/file-attribute-constants
constexpr std::array<int, FILE_ATTRIBUTES_NUMBER> allAttributes = {
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

std::map<int, std::string> attributeDescriptionsMap = {
        {FILE_ATTRIBUTE_HIDDEN,              "The file or directory is hidden. It is not included in an ordinary directory listing."},
        {FILE_ATTRIBUTE_SYSTEM,              "A file or directory that the operating system uses a part of, or uses exclusively."},
        {FILE_ATTRIBUTE_DIRECTORY,           "The handle that identifies a directory."},
        {FILE_ATTRIBUTE_ARCHIVE,             "A file or directory that is an archive file or directory. Applications typically use this attribute to mark files for backup or removal."},
        {FILE_ATTRIBUTE_DEVICE,              "This value is reserved for system use."},
        {FILE_ATTRIBUTE_NORMAL,              "A file that does not have other attributes set. This attribute is valid only when used alone."},
        {FILE_ATTRIBUTE_TEMPORARY,           "A file that is being used for temporary storage. File systems avoid writing data back to mass storage if sufficient cache memory is available, because typically, an application deletes a temporary file after the handle is closed. In that scenario, the system can entirely avoid writing the data. Otherwise, the data is written after the handle is closed."},
        {FILE_ATTRIBUTE_SPARSE_FILE,         "A file that is a sparse file."},
        {FILE_ATTRIBUTE_REPARSE_POINT,       "A file or directory that has an associated reparse point, or a file that is a symbolic link."},
        {FILE_ATTRIBUTE_COMPRESSED,          "A file or directory that is compressed. For a file, all of the data in the file is compressed. For a directory, compression is the default for newly created files and subdirectories."},
        {FILE_ATTRIBUTE_OFFLINE,             "The data of a file is not available immediately. This attribute indicates that the file data is physically moved to offline storage. This attribute is used by Remote Storage, which is the hierarchical storage management software. Applications should not arbitrarily change this attribute."},
        {FILE_ATTRIBUTE_NOT_CONTENT_INDEXED, "The file or directory is not to be indexed by the content indexing service."},
        {FILE_ATTRIBUTE_ENCRYPTED,           "A file or directory that is encrypted. For a file, all data streams in the file are encrypted. For a directory, encryption is the default for newly created files and subdirectories."},
//        {FILE_ATTRIBUTE_INTEGRITY_STREAM,      "The directory or user data stream is configured with integrity (only supported on ReFS volumes). It is not included in an ordinary directory listing. The integrity setting persists with the file if it's renamed. If a file is copied the destination file will have integrity set if either the source file or destination directory have integrity set. Windows Server 2008 R2, Windows 7, Windows Server 2008, Windows Vista, Windows Server 2003 and Windows XP: This flag is not supported until Windows Server 2012."},
        {FILE_ATTRIBUTE_VIRTUAL,             "This value is reserved for system use."},
//        {FILE_ATTRIBUTE_NO_SCRUB_DATA,         "The user data stream not to be read by the background data integrity scanner (AKA scrubber). When set on a directory it only provides inheritance. This flag is only supported on Storage Spaces and ReFS volumes. It is not included in an ordinary directory listing. Windows Server 2008 R2, Windows 7, Windows Server 2008, Windows Vista, Windows Server 2003 and Windows XP: This flag is not supported until Windows 8 and Windows Server 2012."},
//        {FILE_ATTRIBUTE_EA,                    "A file or directory with extended attributes. IMPORTANT: This constant is for internal use only."},
//        {FILE_ATTRIBUTE_PINNED,                "This attribute indicates user intent that the file or directory should be kept fully present locally even when not being actively accessed. This attribute is for use with hierarchical storage management software."},
//        {FILE_ATTRIBUTE_UNPINNED,              "This attribute indicates that the file or directory should not be kept fully present locally except when being actively accessed. This attribute is for use with hierarchical storage management software."},
//        {FILE_ATTRIBUTE_RECALL_ON_OPEN,        "This attribute only appears in directory enumeration classes (FILE_DIRECTORY_INFORMATION, FILE_BOTH_DIR_INFORMATION, etc.). When this attribute is set, it means that the file or directory has no physical representation on the local system; the item is virtual. Opening the item will be more expensive than normal, e.g. it will cause at least some of it to be fetched small_amogus_file.txt a remote store."},
//        {FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS, "When this attribute is set, it means that the file or directory is not fully present locally. For a file that means that not all of its data is on local storage (e.g. it may be sparse with some data still in remote storage). For a directory it means that some of the directory contents are being virtualized small_amogus_file.txt another location. Reading the file / enumerating the directory will be more expensive than normal, e.g. it will cause at least some of the file/directory content to be fetched small_amogus_file.txt a remote store. Only kernel-mode callers can set this bit."}
};

// source: https://learn.microsoft.com/en-us/windows/win32/api/clusapi/ns-clusapi-clus_partition_info_ex
std::map<DWORD, std::string> systemFlagsMap = {
        {FS_CASE_IS_PRESERVED,         "The file system preserves the case of file names when it places a name on the storage class resource."},
        {FS_CASE_SENSITIVE,            "The file system uses case-sensitive file names."},
        {FS_UNICODE_STORED_ON_DISK,    "The file system supports Unicode in file names as they appear on storage class resource."},
        {FS_PERSISTENT_ACLS,           "The file system preserves and enforces access control lists (ACLs)."},
        {FS_FILE_COMPRESSION,          "The file system supports file-based compression, where individual files can be compressed while others are not."},
        {FS_VOL_IS_COMPRESSED,         "The entire volume is compressed; for example, DoubleSpace has been used on the disk."},
        {FILE_NAMED_STREAMS,           "The file system supports named streams."},
        {FILE_SUPPORTS_ENCRYPTION,     "The file system supports the Encrypted File System (EFS)."},
        {FILE_SUPPORTS_OBJECT_IDS,     "The file system supports object identifiers."},
        {FILE_SUPPORTS_REPARSE_POINTS, "The file system supports reparse points."},
        {FILE_SUPPORTS_SPARSE_FILES,   "The file system supports sparse files."},
        {FILE_VOLUME_QUOTAS,           "The file system supports disk quotas."}
};

#endif //OS_DATA_H
