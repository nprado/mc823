#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h> /*perror()*/

typedef struct course {
  char *title;     /*String titulo */
  char *program;/* Número de elementos no heap */
  char *date;
  char *comment;  /* Número máximo de elementos */
} Course;



	
void insert_course(char *c);
char* r_title(char *id, FILE* f);
char* r_program(char *p, FILE* f);
char* r_date(char *id, FILE* f);
char* r_comment(char *id, FILE* f);
char* list();
void insert_comment(char *id, char *comment);
Course *info_course(char *id_course);
char* listAll();
