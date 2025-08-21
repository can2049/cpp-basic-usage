// https://mp.weixin.qq.com/s/fUO4hu1QrSbCVwYCfmGnLQ

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#define FILE_PATH "/tmp/test_bus_file"
// Initial file size is 8KB
#define INITIAL_SIZE 8192
// Truncated file size is 4KB
#define TRUNCATE_SIZE 4096
// Byte offset to attempt access (0-indexed)
// 4999 is outside the 0-4095 range, but within the 0-8191 range
#define ACCESS_OFFSET 4999

// Create a temporary file of specified size
void create_test_file() {
  int fd = open(FILE_PATH, O_RDWR | O_CREAT | O_TRUNC, (mode_t)0600);
  if (fd == -1) {
    perror("Failed to create file (open)");
    exit(EXIT_FAILURE);
  }
  if (lseek(fd, INITIAL_SIZE - 1, SEEK_SET) == -1) {
    perror("Failed to set file size (lseek)");
    close(fd);
    exit(EXIT_FAILURE);
  }
  if (write(fd, "", 1) == -1) {
    perror("Failed to write to file (write)");
    close(fd);
    exit(EXIT_FAILURE);
  }
  close(fd);
}

int main() {
  create_test_file();

  // Open file in read-write mode
  int fd = open(FILE_PATH, O_RDWR);
  if (fd == -1) {
    perror("Failed to open file for mmap (open)");
    exit(EXIT_FAILURE);
  }

  // Map the entire 8KB file to memory
  char *ptr = mmap(NULL,          // Let kernel choose the starting address for mapping
                   INITIAL_SIZE,  // Size of the mapping area
                   PROT_READ,     // Protection permission of the mapping area is readable
                   MAP_SHARED,    // Create a shared mapping, modifications to memory will be written back to file
                   fd,            // File descriptor
                   0);            // Start mapping from the beginning of the file

  if (ptr == MAP_FAILED) {
    perror("mmap failed");
    close(fd);
    exit(EXIT_FAILURE);
  }

  printf("Parent process (PID %d): Successfully mapped %d bytes. Address: %p\n", getpid(),
         INITIAL_SIZE, ptr);

  // Create a child process
  pid_t pid = fork();

  if (pid == -1) {
    perror("fork failed");
    exit(EXIT_FAILURE);
  }

  if (pid == 0) {  // Child process code block
    printf("Child process (PID %d): Sleeping for 1 second...\n", getpid());
    sleep(1);
    printf("Child process (PID %d): Starting to truncate file '%s' to %d bytes.\n", getpid(),
           FILE_PATH, TRUNCATE_SIZE);

    // Use ftruncate to truncate the file, making it smaller
    if (ftruncate(fd, TRUNCATE_SIZE) == -1) {
      perror("ftruncate failed");
      exit(EXIT_FAILURE);
    }
    printf("Child process (PID %d): File truncated successfully. About to exit.\n", getpid());
    close(fd);
    exit(EXIT_SUCCESS);

  } else {  // Parent process code block
    printf("Parent process (PID %d): Sleeping for 2 seconds to ensure child process has completed file truncation.\n",
           getpid());
    sleep(2);

    printf("Parent process (PID %d): Preparing to access ptr[%d]...\n", getpid(), ACCESS_OFFSET);

    // ptr + ACCESS_OFFSET
    // This address is valid in virtual memory because it's within the original 8KB mapping range.
    // However, the child process has already truncated the underlying physical file to 4KB.
    // When the parent process attempts to access this address, a page fault occurs.
    // The kernel's page fault handler tries to load data from the corresponding position in the file into the physical memory page.
    // But it finds that the file is no longer that large and cannot complete the physical page filling.
    // This failure causes the kernel to send a SIGBUS signal to the process.
    char value = ptr[ACCESS_OFFSET];

    // This line of code will never be executed
    printf("Parent process (PID %d): This line of code will not be printed. Read value: %c\n", getpid(),
           value);

    wait(NULL);  // Wait for child process to finish
    munmap(ptr, INITIAL_SIZE);
    close(fd);
    unlink(FILE_PATH);  // Clean up temporary file
  }

  return 0;
}
