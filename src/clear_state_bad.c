#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "binary_files_common_functions.h"
#include "date.h"
#include "io.h"

void read_file(FILE *fp, char *path, int *error);
void sort_file(FILE *fp, char *path, int *error);
int compare_date(date *lhs, date *rhs);
// void add_record_in_file(FILE *fp, char *path, int *error);
void check_for_valid_input(const int *row, int *error);
int *input_date_record(int *error);

// void find_in_file(FILE *fp, char *path, int *error);
// void find_data(FILE *fp, char *path, date *input_record, int *error);
void parse_date(int *row, const char *data, int *error);

int copy_data(char *from_path, char *to_path, int index_start, int index_end, int *error);
void clear_process(FILE *fp, char *path, int *error);
int compare_first(const date *lhs, const date *rhs);
int compare_last(const date *lhs, const date *rhs);
int find_index(FILE *fp, char *path, date *input_record, int index, int *error,
               int (*comp)(const date *, const date *));

int main() {
    int error = 0;
    char *path = (char *)malloc(sizeof(char) * 256);
    input(path);
    if (strlen(path) == 0) {
        error = 1;
    }
    FILE *fp;
    if (error != 1 && (fp = fopen(path, "rb")) && get_records_count_in_file(fp) > 0) {
        fclose(fp);
        clear_process(fp, path, &error);
        if (error != 1) {
            read_file(fp, path, &error);
        }
    } else {
        error = 1;
    }

    if (error == 1) {
        printf("n/a");
    }
    free(path);
}

// read and write
void read_file(FILE *fp, char *path, int *error) {
    if ((fp = fopen(path, "rb"))) {
        int row[8];
        while (fread(row, sizeof(int), 8, fp)) {
            date row_ = fill_date(row);
            print_date(row_);
            printf("\n");
        }
        fclose(fp);
        // read_record_from_file(fp, 0);
    } else {
        *error = 1;
    }
}

// delete bad
void clear_process(FILE *fp, char *path, int *error) {
    char data_1[10];
    char data_2[10];
    if (((scanf("%s", data_1)) && strlen(data_1) == 10) && ((scanf("%s", data_2)) && strlen(data_2) == 10)) {
        int *row_1 = (int *)malloc(sizeof(int) * 8);
        int *row_2 = (int *)malloc(sizeof(int) * 8);
        parse_date(row_1, data_1, error);
        parse_date(row_2, data_2, error);
        if (*error != 1) {
            date data_1 = fill_date(row_1);
            date data_2 = fill_date(row_2);
            if (compare_date(&data_1, &data_2)) {
                *error = 1;
            }
            int index_start = find_index(fp, path, &data_1, 0, error, compare_first);
            int index_end = find_index(fp, path, &data_2, index_start, error, compare_last);
            char *to_path = (char *)malloc(sizeof(char) * 256);
            strcpy(to_path, path);
            strcat(to_path, ".tmp");
            if (*error != 1) {
                int rec_count = copy_data(path, to_path, index_start, index_end, error);
                copy_data(to_path, path, rec_count, rec_count, error);
                remove(to_path);
            }
            free(to_path);
        }
        free(row_1);
        free(row_2);
    } else {
        *error = 1;
    }
}

int copy_data(char *from_path, char *to_path, int index_start, int index_end, int *error) {
    FILE *fp;
    FILE *cp;
    int record_count = 0;
    if ((fp = fopen(from_path, "rb")) && (cp = fopen(to_path, "wb"))) {
        int i = 0;
        for (; i < index_start; i++) {
            date tmp = read_record_from_file(fp, i);
            write_record_in_file(cp, &tmp, i);
        }
        int file_size = get_records_count_in_file(fp);
        if (index_end + 1 < file_size) {
            for (; i < (file_size - index_end + index_start - 1); i++) {
                date tmp = read_record_from_file(fp, index_end + 1 + i - index_start);
                write_record_in_file(cp, &tmp, i);
            }
        }
        record_count = get_records_count_in_file(cp);
        fclose(cp);
        fclose(fp);
    } else {
        *error = 1;
    }
    return record_count;
}

// record < value
int compare_first(const date *lhs, const date *rhs) {
    return lhs->year < rhs->year     ? 1
           : lhs->year > rhs->year   ? 0
           : lhs->month < rhs->month ? 1
           : lhs->month > rhs->month ? 0
           : lhs->day < rhs->day     ? 1
           : lhs->day > rhs->day     ? 0
                                     : 1;
}

// record > value
int compare_last(const date *lhs, const date *rhs) {
    return !(lhs->year > rhs->year     ? 1
           : lhs->year < rhs->year   ? 0
           : lhs->month > rhs->month ? 1
           : lhs->month < rhs->month ? 0
           : lhs->day > rhs->day     ? 1
           : lhs->day < rhs->day     ? 0
                                     : 1);
}

int find_index(FILE *fp, char *path, date *input_record, int index, int *error,
               int (*comp)(const date *, const date *)) {
    if ((fp = fopen(path, "rb"))) {
        for (; index < get_records_count_in_file(fp); index++) {
            date tmp = read_record_from_file(fp, index);
            if (!comp(&tmp, input_record)) {
                // printf("%d\n", tmp.code);
                break;
            }
        }
        /* if (index == get_records_count_in_file(fp)) {
            *error = 1;
        } */
        fclose(fp);
    } else {
        *error = 1;
    }
    return index;
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

// input data
int *input_date_record(int *error) {
    int *row = (int *)malloc(sizeof(date));
    int i = 0;
    for (; i < 7; i++) {
        if (scanf("%d", &row[i]) && getchar() == ' ') {
        } else {
            *error = 1;
        }
    }
    if (scanf("%d", &row[i]) && getchar() == '\n') {
    } else {
        *error = 1;
    }
    return row;
}

/* void add_record_in_file(FILE *fp, char *path, int *error) {
    if ((fp = fopen(path, "ab"))) {
        int *ptr_row = input_date_record(error);
        check_for_valid_input(ptr_row, error);
        if (*error != 1) {
            date ptr_record = fill_date(ptr_row);
            write_record_in_file(fp, &ptr_record, 0);
        }
        fclose(fp);
        free(ptr_row);
    } else {
        *error = 1;
    }
} */

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