#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#endif

#define FILE_SIZE 1024 // Size of the memory-mapped file

int main()
{
    int fd;
    char *addr;

#ifdef _WIN32
    HANDLE fileHandle;
    DWORD fileSizeLow, fileSizeHigh;
#else
    int protection = PROT_READ | PROT_WRITE;
    int visibility = MAP_SHARED;
#endif

#ifdef _WIN32
    // Open or create a file on Windows
    fileHandle = CreateFile("mapped_file.txt", GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (fileHandle == INVALID_HANDLE_VALUE)
    {
        perror("CreateFile");
        exit(EXIT_FAILURE);
    }

    // Set the file size
    fileSizeLow = FILE_SIZE & 0xFFFFFFFF;
    fileSizeHigh = (FILE_SIZE >> 32) & 0xFFFFFFFF;
    SetFilePointer(fileHandle, fileSizeLow, (PLONG)&fileSizeHigh, FILE_BEGIN);
    SetEndOfFile(fileHandle);

    // Map the file into memory
    HANDLE mappingHandle = CreateFileMapping(fileHandle, NULL, PAGE_READWRITE, 0, 0, NULL);
    if (mappingHandle == NULL)
    {
        perror("CreateFileMapping");
        CloseHandle(fileHandle);
        exit(EXIT_FAILURE);
    }

    addr = (char *)MapViewOfFile(mappingHandle, FILE_MAP_ALL_ACCESS, 0, 0, FILE_SIZE);
    if (addr == NULL)
    {
        perror("MapViewOfFile");
        CloseHandle(mappingHandle);
        CloseHandle(fileHandle);
        exit(EXIT_FAILURE);
    }
#else
    // Open or create a file on Unix-like systems
    fd = open("mapped_file.txt", O_RDWR | O_CREAT | O_TRUNC, (mode_t)0600);
    if (fd == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Stretch the file to the desired size
    if (ftruncate(fd, FILE_SIZE) == -1)
    {
        perror("ftruncate");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Map the file into memory
    addr = mmap(NULL, FILE_SIZE, protection, visibility, fd, 0);
    if (addr == MAP_FAILED)
    {
        perror("mmap");
        close(fd);
        exit(EXIT_FAILURE);
    }
#endif

    // Write some data to the memory-mapped file
    strcpy(addr, "Hello, Memory Mapped File!");

#ifdef _WIN32
    // Unmap the memory-mapped file on Windows
    UnmapViewOfFile(addr);
    CloseHandle(mappingHandle);
    CloseHandle(fileHandle);
#else
    // Synchronize changes to the file
    if (msync(addr, FILE_SIZE, MS_SYNC) == -1)
    {
        perror("msync");
        close(fd);
        munmap(addr, FILE_SIZE);
        exit(EXIT_FAILURE);
    }

    // Unmap the memory-mapped file on Unix-like systems
    if (munmap(addr, FILE_SIZE) == -1)
    {
        perror("munmap");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Close the file
    close(fd);
#endif

    return 0;
}
