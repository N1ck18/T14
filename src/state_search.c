#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "binary_files_common_functions.h"
#include "date.h"
#include "io.h"

void check_for_valid_input(const int *row, int *error);
void find_in_file(FILE *fp, char *path, int *error);
void find_data(FILE *fp, char *path, date *input_record, int *error);
void parse_date(int *row, const char *data, int *error);

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
        find_in_file(fp, path, &error);
        if (error == 1) {
            printf("n/a");
        }
    } else {
        printf("n/a");
    }
    free(path);
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

void find_data(FILE *fp, char *path, date *input_record, int *error) {
    if ((fp = fopen(path, "rb"))) {
        int result = 0;
        for (int i = 0; i < get_records_count_in_file(fp); i++) {
            date tmp = read_record_from_file(fp, i);
            if (input_record->year == tmp.year && input_record->month == tmp.month &&
                input_record->day == tmp.day) {
                result = 1;
                printf("%d", tmp.code);
                break;
            }
        }
        if (!result) {
            printf("n/a");
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

void check_for_valid_input(const int *row, int *error) {
    *error = (row[1] < 1 || row[1] > 12)   ? 1
             : (row[2] > 32 || row[2] < 1) ? 1
             : (row[3] < 0 || row[3] > 24) ? 1
             : (row[4] < 0 || row[4] > 60) ? 1
             : (row[5] < 0 || row[5] > 60) ? 1
             : (row[6] < 0 || row[6] > 1)  ? 1
                                           : 0;
}