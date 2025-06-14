#include "s21_grep.h"

int main(int argc, char *argv[]) {
  GrepFlags flags = {0};
  int err_code = parse_args(&flags, argc, argv);

  if (err_code != SUCCESS) {
    print_error(err_code, (err_code == NO_FILE) ? flags.pattern_file : NULL);
    cleanup(&flags);
    return err_code;
  }

  // patt should not be empty
  if (flags.num_patterns == 0) {
    fprintf(stderr, "No valid patterns provided\n");
    cleanup(&flags);
    return 1;
  }

  int file_count = 0;
  for (int i = optind; i < argc; i++) {
    file_count++;
  }

  // check if any files before process patt
  for (int j = optind; j < argc; j++) {
    if (access(argv[j], F_OK) == -1) {
      if (!flags.s) {
        print_error(NO_FILE, argv[j]);
      }
      file_count--;
    }
  }

  regex_t *regex = compile_regex(flags.combined_pattern, flags.i);
  if (!regex) {
    fprintf(stderr, "Failed to compile regex for pattern: %s\n",
            flags.combined_pattern);
    cleanup(&flags);
    return 1;
  }

  if (file_count == 0) {
    grep_file(&flags, stdin, NULL, file_count, regex);
  } else {
    for (int j = optind; j < argc; j++) {
      if (access(argv[j], F_OK) != -1) {  // if exist
        FILE *file = fopen(argv[j], "r");
        if (file) {
          grep_file(&flags, file, argv[j], file_count, regex);
          fclose(file);
        } else if (!flags.s) {
          print_error(5, argv[j]);  // err if nott
        }
      }
    }
  }
  regfree(regex);
  free(regex);

  cleanup(&flags);
  return 0;
}

void grep_file(const GrepFlags *flags, FILE *file, const char *filename,
               int file_count, regex_t *regex) {
  char buffer[BUFF_SIZE];
  int line_num = 0;
  int match_count = 0;

  while (fgets(buffer, BUFF_SIZE, file) != NULL) {
    line_num++;

    // del symb new line
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
      buffer[--len] = '\0';
    }

    if (match_line(flags, buffer, regex)) {
      match_count++;
      if (!flags->c && !flags->l) {
        print_match(flags, filename, line_num, buffer, file_count, regex);
      }
    }
  }

  if (flags->l && match_count > 0) {
    printf("%s\n", filename);
  } else if (flags->c && !flags->l) {
    if (filename != NULL && file_count > 1 && !flags->h)
      printf("%s:", filename);
    printf("%d\n", match_count);
  }
}

bool match_line(const GrepFlags *flags, const char *line, regex_t *regex) {
  int match = regexec(regex, line, 0, NULL, 0);
  return (match == 0 && !flags->v) || (match != 0 && flags->v);
}

void print_match(const GrepFlags *flags, const char *filename, int line_num,
                 const char *line, int file_count, regex_t *regex) {
  if (filename != NULL && file_count > 1 && !flags->h) {
    printf("%s:", filename);
  }
  if (flags->n) {
    printf("%d:", line_num);
  }

  if (flags->o) {
    regmatch_t match;
    size_t offset = 0;
    size_t str_len = strlen(line);
    while (regexec(regex, line + offset, 1, &match, 0) == 0) {
      int len = match.rm_eo - match.rm_so;
      printf("%.*s\n", len, line + match.rm_so + offset);
      offset += match.rm_eo;
      if (offset >= str_len) {
        break;
      }
    }
  } else {
    printf("%s\n", line);
  }
}

int parse_args(GrepFlags *flags, int argc, char *argv[]) {
  int opt;
  bool pattern_present = false;
  char *combined_pattern = NULL;
  size_t combined_pattern_size = 0;
  int err_code = SUCCESS;

  while ((opt = getopt(argc, argv, "e:f:ivclnhso")) != -1) {
    switch (opt) {
      case 'e':
        flags->e = true;
        if (combined_pattern == NULL) {
          combined_pattern = strdup(optarg);
          combined_pattern_size = strlen(optarg);
        } else {
          combined_pattern_size += strlen(optarg) + 1;  // +1 for '|'
          char *temp = realloc(combined_pattern, combined_pattern_size + 1);
          if (temp == NULL) {
            free(combined_pattern);
            return MALLOC_ERROR;
          }
          combined_pattern = temp;
          strcat(combined_pattern, "|");
          strcat(combined_pattern, optarg);
        }
        pattern_present = true;
        break;
      case 'f':
        flags->f = true;
        flags->pattern_file = strdup(optarg);
        if (!flags->pattern_file) {
          return MALLOC_ERROR;
        }

        if (access(flags->pattern_file, F_OK) == -1) {  // check if exitst
          if (!flags->s) {
            print_error(NO_FILE, flags->pattern_file);
          }
          return NO_FILE;
        }

        char *pattern_data = read_pattern_file(flags->pattern_file);
        if (!pattern_data) {
          if (!flags->s) {
            print_error(NO_FILE, flags->pattern_file);
          }
          return NO_FILE;
        } else {  // Файл успешно прочитан
          char *token = strtok(pattern_data, "\n");
          while (token != NULL) {
            if (combined_pattern == NULL) {
              combined_pattern = strdup(token);
              combined_pattern_size = strlen(token);
            } else {
              combined_pattern_size += strlen(token) + 1;
              char *temp = realloc(combined_pattern, combined_pattern_size + 1);
              if (temp == NULL) {
                free(combined_pattern);
                return MALLOC_ERROR;
              }
              combined_pattern = temp;
              strcat(combined_pattern, "|");
              strcat(combined_pattern, token);
            }
            pattern_present = true;
            token = strtok(NULL, "\n");
          }
          free(pattern_data);
        }
        break;

      case 'i':
        flags->i = true;
        break;
      case 'v':
        flags->v = true;
        break;
      case 'c':
        flags->c = true;
        break;
      case 'l':
        flags->l = true;
        break;
      case 'n':
        flags->n = true;
        break;
      case 'h':
        flags->h = true;
        break;
      case 's':
        flags->s = true;
        break;
      case 'o':
        flags->o = true;
        break;
      default:
        fprintf(stderr,
                "Usage: %s [-e pattern] [-f file] [-ivclnhso] [pattern] [file "
                "...]\n",
                argv[0]);
        return NON_EXISTENT_FLAG;
    }
  }

  if (!pattern_present) {  // if no (-e or -f)
    if (optind < argc) {   // pick 1st flag
      combined_pattern = strdup(argv[optind++]);
      pattern_present = true;
    } else {
      return NON_EXISTENT_FLAG;
    }
  }

  // add combo flags
  flags->combined_pattern = combined_pattern;
  flags->num_patterns = 1;

  return err_code;
}

char *read_pattern_file(const char *filename) {
  if (access(filename, F_OK) == -1) {  // check if exist
    perror(filename);
    return NULL;
  }

  FILE *file = fopen(filename, "r");
  if (!file) {
    perror(filename);
    return NULL;
  }

  fseek(file, 0, SEEK_END);
  long fsize = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *string = malloc(fsize + 1);
  if (!string) {
    fclose(file);
    return NULL;
  }

  fread(string, 1, fsize, file);
  fclose(file);

  string[fsize] = 0;
  return string;
}

void cleanup(GrepFlags *options) {
  if (options->patterns) {
    for (int i = 0; i < options->num_patterns; i++) {
      free(options->patterns[i]);
    }
    free(options->patterns);
  }
  free(options->pattern_file);
  free(options->combined_pattern);
}

void print_error(int err_code, const char *filename) {
  switch (err_code) {
    case NO_FILE:
      fprintf(stderr, "grep: %s: No such file or directory\n", filename);
      break;
    case NON_EXISTENT_FLAG:
      fprintf(stderr, "Usage: grep (OPTION) PATTERNS (FILE)\n");
      break;
    case MALLOC_ERROR:
      fprintf(stderr, "grep: Memory allocation failed\n");
      break;
    default:  // for err for opening file, NO_FILE
      if (filename) {
        perror(filename);
      }
      break;
  }
}

regex_t *compile_regex(const char *pattern, bool ignore_case) {
  regex_t *regex = malloc(sizeof(regex_t));
  if (!regex) {
    perror("malloc failed");
    exit(1);
  }

  int flags = REG_EXTENDED | REG_NEWLINE;
  if (ignore_case) flags |= REG_ICASE;

  if (regcomp(regex, pattern, flags)) {
    free(regex);
    return NULL;
  }

  return regex;
}
