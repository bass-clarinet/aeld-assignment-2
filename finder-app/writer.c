/*
 * writer.c - write a string to a file, logging via syslog.
 *
 * Usage: writer <writefile> <writestr>
 *
 * Creates (or truncates) writefile and writes writestr into it.
 * The directory containing writefile must already exist.
 * Uses the LOG_USER syslog facility: LOG_DEBUG for normal operation,
 * LOG_ERR for errors.
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <syslog.h>
#include <unistd.h>

/* Write all of buf to fd, retrying on partial writes and EINTR (LSP ch. 2). */
static int write_to_file(int fd, const char *buf, size_t len)
{
    while (len > 0) {
        ssize_t ret = write(fd, buf, len);
        if (ret == -1) {
            if (errno == EINTR)
                continue;
            return -1;
        }
        buf += ret;
        len -= (size_t)ret;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    openlog(NULL, 0, LOG_USER);

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <writefile> <writestr>\n",
                argc > 0 ? argv[0] : "writer");
        syslog(LOG_ERR, "Invalid number of arguments: expected 2, got %d",
               argc - 1);
        closelog();
        return 1;
    }

    const char *writefile = argv[1];
    const char *writestr = argv[2];

    syslog(LOG_DEBUG, "Writing %s to %s", writestr, writefile);

    int fd = open(writefile, O_WRONLY | O_CREAT | O_TRUNC,
                  S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);   /* 0644 */
    if (fd == -1) {
        int err = errno;
        fprintf(stderr, "Error opening %s: %s\n", writefile, strerror(err));
        syslog(LOG_ERR, "Error opening %s: %s", writefile, strerror(err));
        closelog();
        return 1;
    }

    if (write_to_file(fd, writestr, strlen(writestr)) == -1) {
        int err = errno;
        fprintf(stderr, "Error writing to %s: %s\n", writefile, strerror(err));
        syslog(LOG_ERR, "Error writing to %s: %s", writefile, strerror(err));
        close(fd);
        closelog();
        return 1;
    }

    /* close() can report deferred write errors, so check it too */
    if (close(fd) == -1) {
        int err = errno;
        fprintf(stderr, "Error closing %s: %s\n", writefile, strerror(err));
        syslog(LOG_ERR, "Error closing %s: %s", writefile, strerror(err));
        closelog();
        return 1;
    }

    closelog();
    return 0;
}
