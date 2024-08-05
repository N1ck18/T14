#ifndef DATE_H
#define DATE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct date {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int sec;
    int status;
    int code;
} date;

date fill_date(const int *row);
void print_date(const date row);
void fill_date_ptr(const int *row, date *tmp);

#endif