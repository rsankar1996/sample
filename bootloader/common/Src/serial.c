#include "main.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/unistd.h>

extern UART_HandleTypeDef huart5;

int _isatty(int fd)
{
        if (fd >= STDIN_FILENO && fd <= STDERR_FILENO)
                return 1;

        errno = EBADF;
        return 0;
}

int _write(int fd, char *ptr, int len)
{
        HAL_StatusTypeDef hstatus;

        if (fd == STDOUT_FILENO || fd == STDERR_FILENO) {
                hstatus =
                    HAL_UART_Transmit(&huart5, (uint8_t *) ptr, len,
                                      HAL_MAX_DELAY);
                if (hstatus == HAL_OK)
                        return len;
                else
                        return EIO;
        }
        errno = EBADF;
        return -1;
}

int _close(int fd)
{
        if (fd >= STDIN_FILENO && fd <= STDERR_FILENO)
                return 0;

        errno = EBADF;
        return -1;
}

int _lseek(int fd, int ptr, int dir)
{
        (void) fd;
        (void) ptr;
        (void) dir;

        errno = EBADF;
        return -1;
}

int _read(int fd, char *ptr, int len)
{
        HAL_StatusTypeDef hstatus;

        if (fd == STDIN_FILENO) {
                hstatus =
                    HAL_UART_Receive(&huart5, (uint8_t *) ptr, 1,
                                     HAL_MAX_DELAY);
                if (hstatus == HAL_OK)
                        return 1;
                else
                        return EIO;
        }
        errno = EBADF;
        return -1;
}

int _fstat(int fd, struct stat *st)
{
        if (fd >= STDIN_FILENO && fd <= STDERR_FILENO) {
                st->st_mode = S_IFCHR;
                return 0;
        }

        errno = EBADF;
        return 0;
}

