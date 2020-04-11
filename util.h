#pragma once

#include <string>

#define MSG_SIZE 0x4000
#define NAME_SIZE   1024

struct datetime {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
};


namespace util {
    char *gen_msg_buf();
    void free_msg_buf(char *msg);

    char *gen_name_buf();
    void free_name_buf(char *name);

    struct datetime get_current_time();
    std::string datetime_to_string(struct datetime dt);
    struct datetime string_to_datetime(std::string t);
}
