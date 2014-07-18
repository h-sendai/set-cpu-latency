#include <sys/time.h>

#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "my_signal.h"
#include "set_timer.h"
#include "get_num.h"

int usage()
{
    fprintf(stderr, "Usage: set-c-state [-t timeout] c_state_num\n");
    return 0;
}

void sig_alarm(int signo)
{
    exit(0);
}

int main(int argc, char *argv[])
{
    int32_t v;
    int c;
    int fd;
    int debug = 0;
    int hold_sec = 0;

    while ( (c = getopt(argc, argv, "t:")) != -1) {
        switch (c) {
            case 't':
                hold_sec = get_num(optarg);
                break;
            default:
                break;
        }
    }
    argc -= optind;
    argv += optind;

    if (argc != 1) {
        usage();
        exit(1);
    }

    v = strtol(argv[0], NULL, 0);
    
    if (debug) {
        fprintf(stderr, "trying to write %d\n", v);
    }

    
    if (hold_sec > 0) {
        my_signal(SIGALRM, sig_alarm);
        set_timer(hold_sec, 0, 0, 0);
    }

    fd = open("/dev/cpu_dma_latency", O_WRONLY);
    if (fd < 0) {
        err(1, "open for /dev/cpu_dma_latency");
    }
    if (write(fd, &v, sizeof(v)) != sizeof(v)) {
        err(1, "write to /dev/cpu_dma_latency fail");
    }

    // keep open state /dev/cpu_dma_latency
    for ( ; ; ) {
        pause();
    }

    return 0;
}
