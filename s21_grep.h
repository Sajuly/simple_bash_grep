#ifndef SRC_GREP_S21_GREP_H_
#define SRC_GREP_S21_GREP_H_

char str[200][255] = {0};    // file в которых ищем
char str_e[200][255] = {0};  // shablons
char str_f[200][255] = {0};  // file with shablons

int cnt = 0;
int cnt_e = 0;  //  str_spisok_shblons
int cnt_f = 0;  // str_file_shablons

int flag_e = 0;  // boolean true or false
int flag_i = 0;
int flag_v = 0;
int flag_c = 0;
int flag_l = 0;
int flag_n = 0;

int flag_h = 0;
int flag_s = 0;
int flag_f = 0;
int flag_o = 0;

#endif  // SRC_GREP_S21_GREP_H_
