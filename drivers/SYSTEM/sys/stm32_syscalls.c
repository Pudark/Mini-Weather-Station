#include <sys/stat.h>
#include <unistd.h>

int _write(int file, char *ptr, int len) { return len; }      // 禁止 printf 输出到终端
int _read(int file, char *ptr, int len)  { return 0; }        // 禁止 scanf
int _close(int file) { return -1; }
int _fstat(int file, struct stat *st) { st->st_mode = S_IFCHR; return 0; }
int _isatty(int file) { return 1; }
int _lseek(int file, int ptr, int dir) { return 0; }
void _exit(int status) { while(1); }
int _kill(int pid, int sig) { return -1; }
int _getpid(void) { return 1; }
caddr_t _sbrk(int incr) { extern char _ebss; static char *heap = &_ebss; char *prev = heap; heap += incr; return (caddr_t)prev; }
