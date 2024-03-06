//
// Created by referencecat on 05.03.24.
//

#include "task2.h"

struct aio_operation {
    struct aiocb aio;
    char *buffer;
    int write_operation;
    void* next_operation;
};

void aio_completion_handler(sigval_t sigval) {
    auto *aio_op = (struct aio_operation *)sigval.sival_ptr;
    if (aio_op->write_operation) {
// операция записи
    } else {
//операция чтения
    }
}

void copyFile(std::string pathFrom, std::string pathTo) {
    int fdFrom = open(pathFrom.c_str(), O_RDONLY | O_NONBLOCK, 0666);
    int fdTo = open(pathTo.c_str(), O_CREAT | O_WRONLY | O_TRUNC | O_NONBLOCK, 0666);

    struct stat statBuffer{};
    fstat(fdFrom, &statBuffer);
    long long size = statBuffer.st_size;

    char buffer [size];

    aiocb readcb{};
    readcb.aio_fildes = fdFrom;
    readcb.aio_buf = buffer;
    readcb.aio_nbytes = size;
    readcb.aio_offset = 0;
    aio_read(&readcb);

    sleep(1);

    aiocb writecb{};
    writecb.aio_fildes = fdTo;
    writecb.aio_buf = buffer;
    writecb.aio_nbytes = size;
    aio_write(&writecb);
    std::cout << buffer << std::endl;
}



int main(int argc, char **argv) { //
//    open(read_filename, O_RDONLY | O_NONBLOCK, 0666);
//    open(write_filename, O_CREAT | O_WRONLY | O_TRUNC | O_NONBLOCK, 0666);
// aio_read;
// aio_write;
// aio_return; - проверка прочитанных байтов
// aio_suspend; - ожидание асинхронных операций

    if (argc != 7) {
        std::cerr << "Wrong number of arguments. Must be 7:" << std::endl;
        return 1;
    }

    const std::string oldFilePath(argv[1]); // old file
    const std::string newFilePath(argv[2]); // new file
    const unsigned long oldFileBytes = atoi(argv[3]); // size
    long bs = atoi(argv[4]); // multiplitions
    long oios = atoi(argv[5]); // operations
//    std::string gen = "gen";
//    if (strcmp(argv[6], gen.c_str()) == 0)
//    {
//        LocalFileGenerator(oldFilePath, oldFileBytes);
//    }
    copyFile(oldFilePath, newFilePath);
    return 0;
}