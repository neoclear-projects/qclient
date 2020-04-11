#ifndef TRANSMIT_H
#define TRANSMIT_H

#define BUF_SIZE 0x1000

namespace transmit {
    char *gen_buf();
    void free_buf(char *buf);
    void clear_buf(char *buf);
}

#endif // TRANSMIT_H
