#include "./s21_cat.h"

#define NONPRINT 64
#define ASCII_TABLE 128

void process_line_v(Flags *flags, const char current_key) {
  if (flags->v) {
    if (current_key < 32 && current_key != '\t' && current_key != '\n') {
      putchar('^');
      putchar(current_key + NONPRINT);
    } else if (current_key == 127) {
      putchar('^');
      putchar('?');
    } else {
      putchar(current_key);
    }
  } else {
    putchar(current_key);
  }
}

void not_ASCII_print(Flags *flags, unsigned char current_key) {
  if (flags->v) {
    char converted_key = (signed char)current_key - ASCII_TABLE;
    putchar('M');
    putchar('-');
    if (isprint(converted_key)) {
      putchar(converted_key);
    } else {
      process_line_v(flags, converted_key);
    }
  } else {
    putchar(current_key);
  }
}

void process_char(Flags *flags, unsigned char current_key) {
  if (current_key >= ASCII_TABLE) {
    not_ASCII_print(flags, current_key);
  } else {
    process_line_v(flags, current_key);
  }
}

void process_file(FILE *file, Flags *flags) {
  int current_char;
  int previous_char = 0;
  int line_number = 1;
  int new_line = 1;
  int in_empty_sequence = 0;

  while ((current_char = fgetc(file)) != EOF) {
    unsigned char current_key = (unsigned char)current_char;

    if (flags->s && current_key == '\n') {
      if (previous_char == '\n') {
        if (!in_empty_sequence) {
          if (flags->e) putchar('$');
          putchar('\n');
          in_empty_sequence = 1;
        }
        continue;
      } else {
        in_empty_sequence = 0;
      }
    }

    if (flags->n && new_line) {
      printf("%6d\t", line_number++);
      new_line = 0;
    }

    if (flags->b && new_line && current_key != '\n') {
      printf("%6d\t", line_number++);
      new_line = 0;
    }

    if (current_key == '\n') {
      new_line = 1;
    }

    //
    if (current_key == '\n' && (flags->e || flags->E)) {
      putchar('$');
    }

    //
    if (current_key == '\t' && (flags->t || flags->T)) {
      putchar('^');
      putchar('I');
    } else {
      process_char(flags, current_key);
    }

    previous_char = current_key;
  }
}

void parse_flags(int argc, char *argv[], Flags *flags) {
  memset(flags, 0, sizeof(Flags));

  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      if (strcmp(argv[i], "--number-nonblank") == 0) {
        flags->b = 1;
        flags->n = 0;
      } else if (strcmp(argv[i], "--number") == 0) {
        flags->n = 1;
        flags->b = 0;
      } else if (strcmp(argv[i], "--squeeze-blank") == 0) {
        flags->s = 1;
      } else {
        for (int j = 1; argv[i][j] != '\0'; j++) {
          switch (argv[i][j]) {
            case 'b':
              flags->b = 1;
              flags->n = 0;
              break;
            case 'n':
              flags->n = 1;
              flags->b = 0;
              break;
            case 's':
              flags->s = 1;
              break;
            case 'e':
              flags->e = 1;
              flags->v = 1;
              break;
            case 't':
              flags->t = 1;
              flags->v = 1;
              break;
            case 'v':
              flags->v = 1;
              break;
            case 'E':
              flags->E = 1;
              break;
            case 'T':
              flags->T = 1;
              break;
            default:
              fprintf(stderr, "Unknown flag: -%c\n", argv[i][j]);
              exit(1);
          }
        }
      }
    }
  }
}

int main(int argc, char *argv[]) {
  setlocale(LC_CTYPE, "");
  Flags flags;
  parse_flags(argc, argv, &flags);

  if (argc == 1) {
    process_file(stdin, &flags);
  } else {
    for (int i = 1; i < argc; i++) {
      if (argv[i][0] != '-') {
        FILE *file = fopen(argv[i], "r");
        if (!file) {
          perror(argv[i]);
          continue;
        }
        process_file(file, &flags);
        fclose(file);
      }
    }
  }

  return 0;
}
