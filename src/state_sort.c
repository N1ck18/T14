#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "binary_files_common_functions.h"
#include "date.h"
#include "io.h"

void read_file(FILE *fp, char *path, int *error);
void sort_file(FILE *fp, char *path, int *error);
int compare_date(date *lhs, date *rhs);
void add_record_in_file(FILE *fp, char *path, int *error);
void check_for_valid_input(const int *row, int *error);
int *input_date_record(int *error);

#ifdef FULL_COMMAND
#define COMMAND 3
void find_in_file(FILE *fp, char *path, int *error);
void find_data(FILE *fp, char *path, date *input_record, int *error);
void parse_date(int *row, const char *data, int *error);
#else
#define COMMAND 2
#endif

int main() {
    int stop = 0;
    int error = 0;
    int command = 0;
    char *path = (char *)malloc(sizeof(char) * 256);
    char ch;
    input(path);
    if (strlen(path) == 0) {
        error = 1;
    }
    while (error != 1 && stop != 1) {
        if ((scanf("%d", &command) != 1) || command < -1 || command > COMMAND ||
            ((ch = getchar()) != '\n' && ch != ' ')) {
            stop = 1;
            error = 1;
        }
        FILE *fp;
        if (error != 1 && (fp = fopen(path, "rb")) && get_records_count_in_file(fp) > 0) {
            fclose(fp);
            switch (command) {
                case -1:
                    stop = 1;
                    break;
                case 0:
                    read_file(fp, path, &error);
                    break;
                case 1:
                    sort_file(fp, path, &error);
                    if (error != 1) {
                        read_file(fp, path, &error);
                    }
                    break;
                case 2:
                    add_record_in_file(fp, path, &error);
                    if (error != 1) {
                        sort_file(fp, path, &error);
                    }
                    if (error != 1) {
                        read_file(fp, path, &error);
                    }
                    break;
#ifdef FULL_COMMAND
                case 3:
                    find_in_file(fp, path, &error);
                    break;
#endif
                default:
                    error = 1;
                    break;
            }
        } else {
            error = 1;
        }
    }
    if (error == 1) {
        printf("n/a");
    }
    free(path);
}

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
                if (compare_date(&lhs, &rhs)) {
                    swap_records_in_file(fp, j - 1, j);
                }
            }
        }
        fclose(fp);
    } else {
        *error = 1;
    }
}

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

void add_record_in_file(FILE *fp, char *path, int *error) {
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
}

void check_for_valid_input(const int *row, int *error) {
    *error = (row[1] < 1 || row[1] > 12)   ? 1
             : (row[2] > 32 || row[2] < 1) ? 1
             : (row[3] < 0 || row[3] > 24) ? 1
             : (row[4] < 0 || row[4] > 60) ? 1
             : (row[5] < 0 || row[5] > 60) ? 1
             : (row[6] < 0 || row[6] > 1)  ? 1
                                           : 0;
}

#ifdef FULL_COMMAND

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

void find_data(FILE *fp, char *path, date *input_record, int *error) {
    if ((fp = fopen(path, "rb"))) {
        for (int i = 0; i < get_records_count_in_file(fp); i++) {
            date tmp = read_record_from_file(fp, i);
            if (input_record->year == tmp.year && input_record->month == tmp.month &&
                input_record->day == tmp.day) {
                printf("%d\n", tmp.code);
                break;
            }
        }
    } else {
        *error = 1;
    }
}

void find_in_file(FILE *fp, char *path, int *error) {
    char data[11];
    if ((scanf("%s", data)) && strlen(data) == 10) {
        int *row = malloc(sizeof(int) * 8);
        parse_date(row, data, error);
        if (*error != 1) {
            date data_ = fill_date(row);
            find_data(fp, path, &data_, error);
        }
        free(row);
    } else {
        *error = 1;
    }
}

#endif