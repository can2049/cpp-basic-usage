// https://mp.weixin.qq.com/s/fUO4hu1QrSbCVwYCfmGnLQ

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define FILE_PATH "/tmp/test_segv_file"
// Define file size as 4096 bytes, which is a typical memory page size
#define FILE_SIZE 4096

// Create a temporary file of specified size
void create_test_file() {
  // Open file in read-write, create, and truncate mode
  int fd = open(FILE_PATH, O_RDWR | O_CREAT | O_TRUNC, (mode_t)0600);
  if (fd == -1) {
    perror("Failed to create file (open)");
    exit(EXIT_FAILURE);
  }

  // Extend file size to FILE_SIZE
  if (lseek(fd, FILE_SIZE - 1, SEEK_SET) == -1) {
    perror("Failed to set file size (lseek)");
    close(fd);
    exit(EXIT_FAILURE);
  }
  // Write one byte at the end of file to confirm file size
  if (write(fd, "", 1) == -1) {
    perror("Failed to write to file (write)");
    close(fd);
    exit(EXIT_FAILURE);
  }
  close(fd);
}

int main() {
  create_test_file();

  // Open file in read-only mode for mapping
  int fd = open(FILE_PATH, O_RDONLY);
  if (fd == -1) {
    perror("Failed to open file for mmap (open)");
    exit(EXIT_FAILURE);
  }

  // Map the first FILE_SIZE bytes of the file to the process's virtual address space
  char *ptr = mmap(NULL,         // Let kernel choose the starting address for mapping
                   FILE_SIZE,    // Size of the mapping area
                   PROT_READ,    // Protection permission of the mapping area is read-only
                   MAP_PRIVATE,  // Create a private copy-on-write mapping
                   fd,           // File descriptor
                   0);           // Start mapping from the beginning of the file

  if (ptr == MAP_FAILED) {
    perror("mmap failed");
    close(fd);
    exit(EXIT_FAILURE);
  }

  // After successful mmap, we can close the file descriptor, the mapping relationship still exists
  close(fd);

  printf("mmap succeeded. Mapped %d bytes. Address: %p\n", FILE_SIZE, ptr);
  printf("About to attempt to access address ptr[%d], which is exactly outside the mapped area...\n", FILE_SIZE);

  // This access is out of bounds. The mapped area is [ptr, ptr + 4095]
  // We try to access ptr + 4096, which is an unmapped virtual address.
  // This operation will cause SIGSEGV (segmentation fault).
  char value = ptr[FILE_SIZE + 0xfffff];

  // This line of code will never be executed
  printf("This line of code will not be printed. Read value: %c\n", value);

  // Unmap memory
  munmap(ptr, FILE_SIZE);
  // Delete temporary file
  unlink(FILE_PATH);

  return 0;
}
