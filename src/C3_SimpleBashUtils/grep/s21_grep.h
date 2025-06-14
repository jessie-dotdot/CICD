#ifndef S21_GREP_H
#define S21_GREP_H
#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE
#define BUFF_SIZE 4096
#define SUCCESS 0
#define NO_FILE 1
#define NON_EXISTENT_FLAG 2
#define MALLOC_ERROR 3

#include <getopt.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
typedef struct {
  bool e;
  bool f;
  bool i;
  bool v;
  bool c;
  bool l;
  bool n;
  bool h;
  bool s;
  bool o;
  char **patterns;
  int num_patterns;
  char *pattern_file;
  char *combined_pattern;
} GrepFlags;

int main(int argc, char *argv[]);
void grep_file(const GrepFlags *flags, FILE *file, const char *filename,
               int file_count, regex_t *regex);
bool match_line(const GrepFlags *flags, const char *line, regex_t *regex);
void print_match(const GrepFlags *flags, const char *filename, int line_num,
                 const char *line, int file_count, regex_t *regex);
int parse_args(GrepFlags *flags, int argc, char *argv[]);
char *read_pattern_file(const char *filename);
void cleanup(GrepFlags *options);
void print_error(int err_code, const char *filename);
regex_t *compile_regex(const char *pattern, bool ignore_case);

#endif  // SRC_GREP_S21_GREP_H_
