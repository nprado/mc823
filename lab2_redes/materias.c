#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h> /*perror()*/
#include "materias.h"

void insert_course(char *c){
	FILE *f;
	f = fopen("courses","r+");
	if (f==NULL) {printf("File error\n"); exit (1);}
	fseek(f, 0, SEEK_END);
	fprintf(f, "%s\n", c); 	
}
/*--------------------Auxiliares de Leitura de Dados --------------*/

char* r_title(char *id, FILE* f){
	char *title;
	char c,c_aux;
	int lSize = 0;
	size_t result;
	
	f = fopen(id,"r");
	if (f==NULL) {printf("File error\n"); exit (1);}
	
	c = fgetc(f);

	if(c=='+'){		
		while(c_aux!= '+'){
			c_aux = fgetc(f);
			lSize++;
		}
	}
	
	fseek (f, -lSize  , SEEK_CUR );
	title = (char*) malloc (sizeof(char)*lSize);
	result = fread(title,1,lSize -1,f);

	return title;
}

char* r_program(char *p, FILE* f){
	char *program;
	char c,c_aux;
	int lSize = 0;
	size_t result;
	
	f = fopen(p,"r");
	if (f==NULL) {printf("File error\n"); exit (1);}
	
	c = fgetc(f);
		
	while(c!= '#'){c = fgetc(f);}
	if(c=='#'){
		while(c_aux!='#'){
			c_aux = fgetc(f);
			lSize++;
		}
	}
	
	fseek (f, -lSize  , SEEK_CUR );
	program = (char*) malloc (sizeof(char)*lSize);
	result = fread(program,1,lSize -1,f);

	return program;
}


char* r_date(char *id, FILE* f){
	char *date;
	char c,c_aux;
	int lSize = 0;
	size_t result;
	
	f = fopen(id,"r");
	if (f==NULL) {printf("File error\n"); exit (1);}
	
	c = fgetc(f);
		
	while(c!= '&'){c = fgetc(f);}
	if(c=='&'){
		while(c_aux!='&'){
			c_aux = fgetc(f);
			lSize++;
		}
	}
	
	fseek (f, -lSize  , SEEK_CUR );
	date = (char*) malloc (sizeof(char)*lSize);
	result = fread(date,1,lSize -1,f);

	return date;
}

char* r_comment(char *id, FILE* f){
	char *comment;
	char c,c_aux;
	int lSize = 0;
	size_t result;
	
	f = fopen(id,"r");
	if (f==NULL) {printf("File error\n"); exit (1);}
	
	c = fgetc(f);
		
	while(c!= '|'){c = fgetc(f);}
	if(c=='|'){
		while(c_aux!='|'){
			c_aux = fgetc(f);
			lSize++;
		}
	}
	
	fseek (f, -lSize  , SEEK_CUR );
	comment = (char*) malloc (sizeof(char)*lSize);
	result = fread(comment,1,lSize -1,f);

	return comment;
}

/*-----------------------------------------------------------------*/
/*------------MANIPULACAO DAS MATERIAS ----------------------------*/

Course* info_course(char *id_course){
	FILE *f_course;
	

	Course *course = (Course*) malloc (sizeof (Course));

	course->title = r_title(id_course, f_course);
	course->program =  r_program(id_course, f_course);
	course->date = r_date(id_course, f_course);
	course->comment = r_comment(id_course, f_course);


	return course;		
}

char* list(){
	int lSize;
	size_t result;
	char *l; 

	FILE *f = fopen("courses", "r");
	if (f==NULL) {printf("File error\n"); exit (1);}
	// ve o tamanho do arquivo
	fseek (f , 0 , SEEK_END);
 	lSize = ftell (f);
	rewind(f);

	// allocate memory to contain the whole file:
  	l = (char*) malloc (sizeof(char)*lSize);
	
 	result = fread (l,1,lSize,f);
  	if (result != lSize) {fputs ("Reading error",stderr); exit (3);}
	
	fclose(f);
	
	return l;
}

void insert_comment(char *id, char *comment){
	char c;
	FILE * f;
	
	f = fopen(id,"r+");
	if (f==NULL) {printf("File error\n"); exit (1);}
	
	c = fgetc(f);	
	while(c!= '|'){c = fgetc(f);}
	if(c=='|'){
		c = fgetc(f); // anda mais um carater
		fputs(comment,f);
		fputc('|',f); // insere delimitador no final
	}
	
	fclose(f);
}

char* listAll(){
	int lSize;
	size_t result;
	char *l; 

	FILE *f = fopen("allinfo", "r");
	if (f==NULL) {printf("File error\n"); exit (1);}
	// ve o tamanho do arquivo
	fseek (f , 0 , SEEK_END);
 	lSize = ftell (f);
	rewind(f);

	// allocate memory to contain the whole file:
  	l = (char*) malloc (sizeof(char)*lSize);
	
 	result = fread (l,1,lSize,f);
  	if (result != lSize) {fputs ("Reading error",stderr); exit (3);}
	
	fclose(f);
	
	return l;
}
/*
int main(){
	
	FILE *f;
	Course* c;
	
	//printf("%s\n", r_title("mc102",f));
	
	c = info_course("mc102");
	printf("Disciplinas:\n%s\n\n",list());
	printf("Titulo:\n%s\n\n",c->title);
	printf("Programa:\n%s\n\n",c->program);
	printf("Horarios:\n%s\n\n", c->date);
	printf("Comentario:\n%s\n\n", c->comment);
	insert_comment("mc102", "blavlakoekskldsosdiosj");
	return 0;	
}	*/
