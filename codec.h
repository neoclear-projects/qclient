#ifndef CODEC_H
#define CODEC_H

#include "util.h"

namespace encode {
    void gen_msg(char *buffer, const char *msg);
    void gen_name(char *buffer, const char *name);
    void gen_sync(char *buffer, struct datetime dt);
    void gen_usr_sts(char *buffer);
}

namespace decode {
    void decMessage(const char *buffer, char *sender, struct datetime *dt, char *msg);
    void decUser(const char *buffer, char *name);
}

#endif // CODEC_H
