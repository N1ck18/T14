#ifndef BINARY_H
#define BINARY_H

#include <stdio.h>
#include <stdlib.h>

#include "date.h"

date read_record_from_file(FILE *pfile, int index);
void write_record_in_file(FILE *pfile, const date *record_to_write, int index);
void swap_records_in_file(FILE *pfile, int record_index1, int record_index2);
int get_file_size_in_bytes(FILE *pfile);
int get_records_count_in_file(FILE *pfile);

#endif