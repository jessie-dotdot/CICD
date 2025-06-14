#ifndef S21_CAT_H
#define S21_CAT_H

#include <ctype.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NONPRINT 64
#define ASCII_TABLE 128

typedef struct {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
  int E;
  int T;
} Flags;

void process_line_v(Flags *flags, const char current_key);
void not_ASCII_print(Flags *flags, unsigned char current_key);
void process_char(Flags *flags, unsigned char current_key);
void process_file(FILE *file, Flags *flags);
void parse_flags(int argc, char *argv[], Flags *flags);
int main(int argc, char *argv[]);

#endif
