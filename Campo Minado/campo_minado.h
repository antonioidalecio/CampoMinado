#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#ifdef _WIN32
    #include <windows.h>
#endif // _WIN32


#define LINHA   10
#define COLUNA	10

#define FACIL   15
#define MEDIO   20
#define DIFICIL 25

#define MENUPRINCIPAL   0
#define MENUJOGO        1

#define BOMBA   '*'
#define MARCADO '?'

char jogar();
bool ganhou();
void marcar();
void menu(int);
void limpaTela();
void pausa(char*);
void imprimeCampo();
void lerOpcao(char*);
void imprimeBombas();
void imprimeRegras();
int  selecionaNivel();
void gotoxy(int, int);
void expande(int, int);
void imprimeDica(bool*);
void preencheBombas(int);
void preencheVizinhanca(int, int);
