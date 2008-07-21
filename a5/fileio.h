// RTS Aufgabe 1   -- 2008-07-21
// Levin Alexander -- 744463

// open a file for reading
//
int file_open_read(char* path);

// open a file for writing
//
int file_open_write(char* path);

// close a file
//
void file_close(int fd);

// wrapper around read(2) with minimal error handling
//
ssize_t file_read(int fd, void* buf, size_t count);

// wrapper around write(2) with minimal error handling
// ensures `count` bytes are actually written
//
ssize_t file_write(int fd, const void *buf, size_t count);
