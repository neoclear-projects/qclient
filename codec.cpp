#include "codec.h"
#include <string.h>

void encode::gen_msg(char *buffer, const char *msg) {
    *(buffer++) = 0x0;
    strcpy(buffer, msg);
}

void encode::gen_name(char *buffer, const char *name) {
    *(buffer++) = 0x1;
    strcpy(buffer, name);
}

void fill_datetime(char *buffer, struct datetime dt) {
    *((int *)buffer) = dt.year;
    buffer += 4;
    *(buffer++) = (char)dt.month;
    *(buffer++) = (char)dt.day;
    *(buffer++) = (char)dt.hour;
    *(buffer++) = (char)dt.minute;
    *(buffer++) = (char)dt.second;
}

void encode::gen_sync(char *buffer, struct datetime dt) {
    *(buffer++) = 0x3;
    fill_datetime(buffer, dt);
}

void encode::gen_usr_sts(char *buffer) {
    buffer[0] = 0x4;
}

void extractDatetime(struct datetime *dt, const char *buffer) {
    dt->year = *(int *)(buffer);
    buffer += 4;
    dt->month = *(buffer++);
    dt->day= *(buffer++);
    dt->hour = *(buffer++);
    dt->minute = *(buffer++);
    dt->second = *(buffer++);
}

void decode::decMessage(const char *buffer, char *sender, struct datetime *dt, char *msg) {
    buffer++;
    extractDatetime(dt, buffer);
    buffer += 9;
    strcpy(sender, buffer);
    buffer += 1024;
    strcpy(msg, buffer);
}

void decode::decUser(const char *buffer, char *name) {
    strcpy(name, buffer + 1);
}
