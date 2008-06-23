// RTS Aufgabe 1   -- 2008-06-23
// Levin Alexander -- 744463

// copy data from a source file to a destination
//
//   +src+: file descriptor, must be open for reading
//   +dst+: file descriptor, must be open for writing
//
// returns a true value on success, and a negative 
// value on failure
//
int filecpy(int src, int dst);

// open a file for reading
//
int file_open_read(char* path);

// open a file for writing
//
int file_open_write(char* path);

// close a file
//
void file_close(int fd);