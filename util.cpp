#include "util.h"
#include <stdlib.h>
#include <time.h>

char *util::gen_msg_buf() {
    return (char *)calloc(1, MSG_SIZE);
}

void util::free_msg_buf(char *msg) {
    free(msg);
}

char *util::gen_name_buf() {
    return (char *)calloc(1, NAME_SIZE);
}

void util::free_name_buf(char *name) {
    free(name);
}

struct datetime util::get_current_time() {
    struct datetime dt;
    time_t now = time(0);
    struct tm tstruct;
    tstruct = *localtime(&now);

    dt.year = tstruct.tm_year + 1900;
    dt.month = tstruct.tm_mon + 1;
    dt.day = tstruct.tm_mday;
    dt.hour = tstruct.tm_hour;
    dt.minute = tstruct.tm_min;
    dt.second = tstruct.tm_sec;
    return dt;
}

std::string util::datetime_to_string(struct datetime dt) {
    char tmp[32];
    sprintf(tmp, "%04d-%02d-%02d %02d:%02d:%02d", dt.year, dt.month, dt.day, dt.hour, dt.minute, dt.second);
    return std::string(tmp);
}

struct datetime util::string_to_datetime(std::string t) {
    struct datetime dt;
    sscanf(t.c_str(),
           "%d-%d-%d %d:%d:%d",
           &dt.year,
           &dt.month,
           &dt.day,
           &dt.hour,
           &dt.minute,
           &dt.second);
    return dt;
}
