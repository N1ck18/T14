#include "date.h"

date fill_date(const int *row) {
    date tmp;
    tmp.year = row[0];
    tmp.month = row[1];
    tmp.day = row[2];
    tmp.hour = row[3];
    tmp.minute = row[4];
    tmp.sec = row[5];
    tmp.status = row[6];
    tmp.code = row[7];
    return tmp;
}

void fill_date_ptr(const int *row, date *tmp) {
    tmp->year = row[0];
    tmp->month = row[1];
    tmp->day = row[2];
    tmp->hour = row[3];
    tmp->minute = row[4];
    tmp->sec = row[5];
    tmp->status = row[6];
    tmp->code = row[7];
}

void print_date(const date row) {
    printf("%d ", row.year);
    printf("%d ", row.month);
    printf("%d ", row.day);
    printf("%d ", row.hour);
    printf("%d ", row.minute);
    printf("%d ", row.sec);
    printf("%d ", row.status);
    printf("%d", row.code);
}