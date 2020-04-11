#include "transmit.h"
#include <stdlib.h>
#include <string.h>

char *transmit::gen_buf() {
    return (char *)calloc(1, BUF_SIZE);
}

void transmit::free_buf(char *buf) {
    free(buf);
}

void transmit::clear_buf(char *buf) {
    memset(buf, 0, BUF_SIZE);
}
