#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "binary_files_common_functions.h"
#include "date.h"
#include "io.h"

void read_file(FILE *fp);
void sort_file(FILE *fp, char *path, int *error);
int compare_date(date *lhs, date *rhs);
void check_for_valid_input(const int *row, int *error);
void parse_date(int *row, const char *data, int *error);

int compare(const date *lhs, const date *rhs);
void input_date(date *data, int *error);
void copy(FILE *fp, FILE *cp, int *error);
void copy_with_comp(FILE *fp, FILE *cp, date *start, date *end, int *error);
char *make_tmp(char *path);

int main() {
    int error = 0;
    char *path = (char *)malloc(sizeof(char) * 256);
    input(path);
    if (strlen(path) == 0) {
        error = 1;
    }
    FILE *fp;
    if (error != 1 && (fp = fopen(path, "rb")) && get_records_count_in_file(fp) > 0) {
        FILE *cp;
        char *to_path = make_tmp(path);  // temp file
        date *first_date = malloc(sizeof(date));
        date *second_date = malloc(sizeof(date));
        input_date(first_date, &error);
        input_date(second_date, &error);
        if ((cp = fopen(to_path, "wb")) && !compare(second_date, first_date) &&
            error != 1) {  // first > second = error
            copy_with_comp(fp, cp, first_date, second_date, &error);
            fclose(cp);
        } else {
            error = 1;
        }
        fclose(fp);
        if (error != 1 && (cp = fopen(to_path, "rb")) && get_records_count_in_file(cp) > 0) {
            if ((fp = fopen(path, "wb"))) {
                copy(cp, fp, &error);
                fclose(cp);
                remove(to_path);
            } else {
                error = 1;
            }
        } else {
            error = 1;
        }
        if (error != 1 && (fp = fopen(path, "rb"))) {
            read_file(fp);
            fclose(fp);
        } else {
            error = 1;
        }
        free(to_path);
        free(first_date);
        free(second_date);
    } else {
        error = 1;
    }
    if (error == 1) {
        printf("n/a");
    }
    free(path);
}

char *make_tmp(char *path) {
    char *to_path = (char *)malloc(sizeof(char) * 256);  // temp file
    to_path[0] = '\0';
    strcpy(to_path, path);
    strcat(to_path, ".tmp");
    return to_path;
}

void input_date(date *data, int *error) {
    char date[11];
    if (((scanf("%s", date)) && strlen(date) == 10)) {
        int row[8] = {0};
        parse_date(row, date, error);
        if (*error != 1) {
            fill_date_ptr(row, data);
        }
    } else {
        *error = 1;
    }
}

void copy_with_comp(FILE *fp, FILE *cp, date *start, date *end, int *error) {
    if (get_records_count_in_file(fp) > 0) {
        int index = 0;
        for (int i = 0; i < get_records_count_in_file(fp); i++) {
            date tmp = read_record_from_file(fp, i);
            if (compare(&tmp, start) || compare(end, &tmp)) {
                write_record_in_file(cp, &tmp, index);
                index++;
            }
        }
    } else {
        *error = 1;
    }
}

void copy(FILE *fp, FILE *cp, int *error) {
    if (get_records_count_in_file(fp) > 0) {
        for (int i = 0; i < get_records_count_in_file(fp); i++) {
            date tmp = read_record_from_file(fp, i);
            write_record_in_file(cp, &tmp, i);
        }
    } else {
        *error = 1;
    }
}

// read and write
void read_file(FILE *fp) {
    int row[8];
    while (fread(row, sizeof(int), 8, fp)) {
        date row_ = fill_date(row);
        print_date(row_);
        printf("\n");
    }
}

// return: lhs < rhs
int compare(const date *lhs, const date *rhs) {
    return lhs->year < rhs->year     ? 1
           : lhs->year > rhs->year   ? 0
           : lhs->month < rhs->month ? 1
           : lhs->month > rhs->month ? 0
                                     : lhs->day < rhs->day;
}

// sort
// true: left > right - swap
int compare_date(date *lhs, date *rhs) {
    return lhs->year > rhs->year       ? 1
           : lhs->year < rhs->year     ? 0
           : lhs->month > rhs->month   ? 1
           : lhs->month < rhs->month   ? 0
           : lhs->day > rhs->day       ? 1
           : lhs->day < rhs->day       ? 0
           : lhs->hour > rhs->hour     ? 1
           : lhs->hour < rhs->hour     ? 0
           : lhs->minute > rhs->minute ? 1
           : lhs->minute < rhs->minute ? 0
           : lhs->sec > rhs->sec       ? 1
           : lhs->sec < rhs->sec       ? 0
                                       : 1;
}

void sort_file(FILE *fp, char *path, int *error) {
    if ((fp = fopen(path, "r+b")) && get_records_count_in_file(fp) > 1) {
        for (int i = 0; i < get_records_count_in_file(fp); i++) {
            for (int j = 1; j < get_records_count_in_file(fp); j++) {
                date rhs = read_record_from_file(fp, j);
                date lhs = read_record_from_file(fp, j - 1);
                if (!compare_date(&lhs, &rhs)) {
                    swap_records_in_file(fp, j - 1, j);
                }
            }
        }
        fclose(fp);
    } else {
        *error = 1;
    }
}

// find
void check_for_valid_input(const int *row, int *error) {
    *error = (row[1] < 1 || row[1] > 12)   ? 1
             : (row[2] > 32 || row[2] < 1) ? 1
             : (row[3] < 0 || row[3] > 24) ? 1
             : (row[4] < 0 || row[4] > 60) ? 1
             : (row[5] < 0 || row[5] > 60) ? 1
             : (row[6] < 0 || row[6] > 1)  ? 1
                                           : 0;
}

void parse_date(int *row, const char *data, int *error) {
    int shift = 1000;
    for (size_t i = 0; i < 8; i++) {
        row[i] = 0;
    }
    // year
    for (int i = 6; i < 10; i++) {
        row[0] += (data[i] - 48) * shift;
        shift /= 10;
    }
    // month
    row[1] = (data[3] - 48) * 10;
    row[1] += data[4] - 48;
    // day
    row[2] = (data[0] - 48) * 10;
    row[2] += data[1] - 48;

    check_for_valid_input(row, error);
}