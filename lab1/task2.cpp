//
// Created by referencecat on 05.03.24.
//

#include "task2.h"
#define __GNU_SOURCE

struct aio_operation {
    struct aiocb controlBlock;
    char *buffer;
    int isWriteOperation;
    bool done;
};

int fileDescriptorFrom;
int fileDescriptorTo;
unsigned long long fileSize;
unsigned blockSize;
aio_operation* operations;
int workingOperations;

void aio_completion_handler(sigval_t sigval) {
    auto *aio_op = (struct aio_operation *)sigval.sival_ptr;
    if (aio_op->done) return;

    if (aio_op->isWriteOperation) {
        aio_op->controlBlock.aio_fildes = fileDescriptorTo;
        if (aio_op->controlBlock.aio_offset + aio_op->controlBlock.aio_nbytes > fileSize) aio_op->controlBlock.aio_nbytes = fileSize - aio_op->controlBlock.aio_offset;
        aio_write(&aio_op->controlBlock);
    } else {
        aio_op->controlBlock.aio_offset += aio_op->controlBlock.aio_nbytes;
        if (aio_op->controlBlock.aio_offset >= fileSize) {
            delete aio_op->buffer;
            aio_op->done = true;
            workingOperations--;
            // if (workingOperations == 0) raise(SIGINT);
        } else {
            aio_op->controlBlock.aio_fildes = fileDescriptorFrom;
            aio_read(&aio_op->controlBlock);
        }

    }
    aio_op->isWriteOperation = !aio_op->isWriteOperation;
}

void initCopy(std::string pathFrom, std::string pathTo, int operationsNumber = 1, int blockSizeMultiplayer = 1 /*in clusters*/) {
    fileDescriptorFrom = open(pathFrom.c_str(), O_RDONLY | O_NONBLOCK, 0666);
    fileDescriptorTo = open(pathTo.c_str(), O_CREAT | O_WRONLY | O_TRUNC | O_NONBLOCK, 0666);

    struct stat statBuffer{};
    fstat(fileDescriptorFrom, &statBuffer);
    fileSize = statBuffer.st_size;
    blockSize = statBuffer.st_blksize * blockSizeMultiplayer;
    operations = new aio_operation[operationsNumber];
    workingOperations = operationsNumber;

    for (int i = 0; i < operationsNumber; i++) {
        operations[i].isWriteOperation = 1;
        operations[i].buffer = new char[blockSize]{0};

        operations[i].controlBlock = aiocb{0};
        operations[i].controlBlock.aio_fildes = fileDescriptorFrom;
        operations[i].controlBlock.aio_buf = operations[i].buffer;
        operations[i].controlBlock.aio_nbytes = blockSize;
        operations[i].controlBlock.aio_offset = i * blockSize;
        if (operations[i].controlBlock.aio_offset > fileSize) break; // stops creating new operations if existent are enough
        operations[i].controlBlock.aio_sigevent.sigev_notify = SIGEV_THREAD;
        operations[i].controlBlock.aio_sigevent.sigev_notify_function = aio_completion_handler;
        operations[i].controlBlock.aio_sigevent.sigev_value.sival_ptr = &operations[i];
        operations[i].done = false;

        // initialisation of aio operation
        aio_read(&operations[i].controlBlock);
    }

}



int main(int argc, char **argv) { // args are "from" filepath, "to" filepath, block size in cluster sizes, operations number
//    open(read_filename, O_RDONLY | O_NONBLOCK, 0666);
//    open(write_filename, O_CREAT | O_WRONLY | O_TRUNC | O_NONBLOCK, 0666);
// aio_read;
// aio_write;
// aio_return; - проверка прочитанных байтов
// aio_suspend; - ожидание асинхронных операций

    if (argc != 5) {
        std::cerr << "Wrong number of arguments. Must be 4:" << std::endl;
        return 1;
    }

    const std::string oldFilePath(argv[1]); // old file
    const std::string newFilePath(argv[2]); // new file
    // const unsigned long oldFileBytes = atoi(argv[3]); // size
    int blockSizeMultiplayer = atoi(argv[3]); // block size in clusters
    int operations = atoi(argv[4]); // operations number


    initCopy(oldFilePath, newFilePath, operations, blockSizeMultiplayer);
    // should replace it with signal handler but idk how
    while (workingOperations) { sleep(0); } // todo
    return 0;
}