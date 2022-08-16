#include "s21_grep.h"

#include <errno.h>
#include <regex.h>
#include <stdio.h>
#include <string.h>

void flag_up(int argc, char *argv[]);
void print_str();
void print_with_namefile(int flag, int flag_h, int count_str_n,
                         char *str_from_file, char *str);

int main(int argc, char *argv[]) {
  flag_up(argc, argv);
  print_str();

  return 0;
}

void flag_up(int argc, char *argv[]) {
  char str_args[200] = {0};
  int arg_len = 0;
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      strcpy(str_args, argv[i]);
      arg_len = strlen(str_args);
      for (int j = 1; j < arg_len; j++) {
        switch (str_args[j]) {
          case 'e':
            if (str_args[j + 1] == '\0' && ++i < argc) {
              strcpy(str_e[cnt_e], argv[i]);
              flag_e = 1;
              cnt_e++;
            }
            break;
          case 'i': flag_i = 1; break;
          case 'v': flag_v = 1; break;
          case 'c': flag_c = 1; break;
          case 'l': flag_l = 1; break;
          case 'n': flag_n = 1; break;
          case 'h': flag_h = 1; break;
          case 's': flag_s = 1; break;
          case 'f':
            if (str_args[j + 1] == '\0' && ++i < argc) {
              strcpy(str_f[cnt_f], argv[i]);
              flag_f = 1;
              cnt_f++;
            }
            break;
          case 'o':
            flag_o = 1;
            break;
        }
      }
    } else {
      strcpy(str[cnt], argv[i]);
      cnt++;
    }
  }
  if (!(flag_e || flag_f)) {
    strcpy(str_e[0], str[0]);
    cnt_e = 1;
    str[0][0] = '\0';

    for (int i = 1; i < cnt; i++) {
      strcpy(str[i - 1], str[i]);
    }
    cnt = cnt - 1;
  }
}

void print_str() {
  FILE *my_file;
  char str_from_file[500];
  char str_match[500];

  regex_t regex;
  int ret;
  int num_str = 0;
  int flag_regex = 0;
  int for_flag_l = 0;
  int count_str_n = 0;
  int flag_found = 0;
  regmatch_t pmatch[1];

  if (flag_i) {
    flag_regex = REG_ICASE;
  }

  for (int i = 0; i < cnt_f; i++) {
    my_file = fopen(str_f[i], "r");

    if (my_file != NULL) {
      while (fgets(str_from_file, 500, my_file) != NULL) {
        strncpy(str_e[cnt_e], str_from_file, strlen(str_from_file) - 1);
        cnt_e++;
      }
    }
  }

  for (int i = 0; i < cnt; i++) {
    my_file = fopen(str[i], "r");
    for_flag_l = 0;
    if (my_file != NULL) {
      while (fgets(str_from_file, 500, my_file) != NULL) {
        count_str_n++;
        flag_found = 0;

        for (int j = 0; j < cnt_e; j++) {
          ret = regcomp(&regex, str_e[j], flag_regex);
          ret = regexec(&regex, str_from_file, 1, pmatch, 0);
          if (flag_v) {
            flag_o = 0;
          }
          if (flag_o && !flag_c && !flag_l && !flag_v) {
            if (ret == 0 && pmatch[0].rm_so != -1) {
              int ln = pmatch[0].rm_eo - pmatch[0].rm_so;
              strncpy(str_match, &str_from_file[pmatch[0].rm_so], ln);
              str_match[ln] = '\n';
              str_match[ln + 1] = '\0';
              print_with_namefile(flag_n, flag_h, count_str_n, str_match,
                                  str[i]);
            }
          }
          if (!ret) {
            flag_found = 1;
          }
          regfree(&regex);
        }

        if (flag_found) {
          for_flag_l = 1;
        }

        if (flag_v) {
          flag_found = !flag_found;
          for_flag_l = 1;
        }
        if (flag_found) {
          if (flag_c) {
            num_str++;
            continue;
          }
          if (flag_l) {
            continue;
          } else {
            if (!flag_o) {
              print_with_namefile(flag_n, flag_h, count_str_n, str_from_file,
                                  str[i]);
            } else {
              continue;
            }
          }
        }
      }
      fclose(my_file);
      count_str_n = 0;

      if (flag_c) {
        if (!flag_l) {
          if (cnt < 2 || flag_h) {
            printf("%d\n", num_str);
          } else {
            printf("%s:%d\n", str[i], num_str);
          }
          num_str = 0;
        } else {
          if (cnt < 2 || flag_h) {
            if (num_str == 0) {
              printf("0\n");
            } else {
              printf("1\n");
            }
          } else {
            if (num_str == 0) {
              printf("%s:0\n", str[i]);
            } else {
              printf("%s:1\n", str[i]);
            }
          }
          num_str = 0;
        }
      }
    } else {
      if (!flag_s) {
        fprintf(stderr, "s21_grep: %s: %s\n", str[i], strerror(errno));
      } else {
        continue;
      }
    }

    if (flag_l && !flag_v) {
      if (flag_l && (for_flag_l ^ flag_v)) {
        printf("%s\n", str[i]);
      }
    } else {
      if (flag_l) {
        printf("%s\n", str[i]);
      }
    }
  }
}

void print_with_namefile(int flag_n, int flag_h, int count_str_n,
                         char *str_from_file, char *str) {
  if (cnt < 2 || flag_h) {
    if (flag_n) {
      printf("%d:%s", count_str_n, str_from_file);
    } else {
      printf("%s", str_from_file);
    }
  } else {
    if (flag_n) {
      printf("%s:%d:%s", str, count_str_n, str_from_file);
    } else {
      if (flag_f) {
        printf("%s:%s", str, str_from_file);
      } else {
        printf("%s:%s", str, str_from_file);
      }
    }
  }
  if (str_from_file[strlen(str_from_file) - 1] != '\n') {
    printf("\n");
  }
}
