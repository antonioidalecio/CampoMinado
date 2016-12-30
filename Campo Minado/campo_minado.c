/**
    Campo Minado Desenvolvido em C
**/

#include "campo_minado.h"

char bombas[LINHA][COLUNA]  = {};
char campo [LINHA][COLUNA]  = {};

int acertos;
int nivelEscolhido;

int main() {

	char opcao;
    bool dica;

    do {

        limpaTela();
        menu( MENUPRINCIPAL );
        lerOpcao(&opcao);
        limpaTela();

        switch(opcao) {
            case 'n': // Novo Jogo

                dica = true;
                acertos = 0;

                memset(campo , '-', LINHA*COLUNA);
                memset(bombas, '0', LINHA*COLUNA);

                imprimeCampo();
                menu( MENUJOGO );

                nivelEscolhido = selecionaNivel();
                preencheBombas( nivelEscolhido );
                preencheDicas();
                limpaTela();

                do {

                    opcao = jogar();

                    switch(opcao) {
                        case 'm': // Menu Principal
                            break;
                        case 'x': // Marcar/Desmarcar
                            marcar();
                            limpaTela();
                            menu(MENUJOGO);
                            imprimeCampo();
                            break;
                        case 't': // Dica
                            limpaTela();
                            menu(MENUJOGO);
                            imprimeCampo();
                            imprimeDica(&dica);
                            break;
                        case 'r': // Regras do Jogo
                        	limpaTela();
                            imprimeRegras();
                            pausa("\n\nTecle <enter> para voltar ao jogo!");
                            limpaTela();
                            menu( MENUJOGO );
                            imprimeCampo();
                            break;
                        case 's': // Sair
                            limpaTela();
                            break;
                        default:
                            gotoxy(1, 15); printf("Opcao invalida! Tente Novamente!\n");
                    }

                } while(opcao != 's' && opcao != 'm');

                break;
            case 'r': // Regras do Jogo
                imprimeRegras();
                pausa("\n\nTecle <enter> para voltar ao menu principal!");
                imprimeCampo();
                break;
            case 'k':
                gotoxy(1, 15); printf("Opcao ainda nao implementada =( !\n");
                pausa("\n\nTecle <enter> para voltar ao menu principal!");
                break;
            case 's': // Sair
                break;
            default:
                gotoxy(1, 15); printf("Opcao invalida! Tente novamente!\n");
        }
    } while(opcao != 's');

	return 0;
}

void imprimeRegras() {

    gotoxy(20, 2); printf("Regras do Jogo: \n");

    gotoxy(1, 4);
    printf(" - Se voce descobrir uma mina, o jogo acaba;\n\n");
    printf(" - Se descobrir um quadrado vazio, voce continua jogando;\n\n");
    printf(" - Se aparecer um numero, ele informara quantas minas estao\n");
    printf("   escondidas nos oito quadrados que o cercam;\n\n");
    printf(" - Voce usa essa informacao para deduzir em que\n");
    printf("   quadrados proximos e seguro clicar;\n\n");
}

void pausa(char* msg) {

    printf("%s!", msg);
    getchar();
    getchar();
}

void imprimeDica(bool* dica) {

    if(*dica) {

        int i, j;

        for(i = 0; i < LINHA; i++) {
            for(j = 0; j < COLUNA; j++) {
                if(campo[i][j] == '-' && bombas[i][j] != BOMBA) {
                    gotoxy(1, 18); printf("Nao tem bomba na linha '%d' coluna '%c'!\n", i, 'A' + j);
                    *dica = false;
                    return;
                }
            }
        }
    } else
        gotoxy(1, 18); printf("Voce nao pode pedir mais dicas nessa partida =( !\n");

}

void limpaTela() {

    #ifdef __linux__
        system("clear");
    #elif _WIN32
        system("cls");
    #endif
}

char jogar() {

    int lin, col;
    char aux;

    do {

        imprimeCampo();
        menu(MENUJOGO);

        do {
            gotoxy(1, 13); printf("Linha: ");
            lerOpcao(&aux);
            lin = aux;

            // Opcoes do menu: m - Menu Principal, t - Dica, r - Regras, s - Sair;
            if(lin == 'm' || lin == 'x' || lin == 't' || lin == 'r' || lin == 's')
                return lin;

            // Converte para coordenadas numericas validas;
            lin = lin - '0';
        } while(lin < 0 || lin > LINHA);

        do {
            gotoxy(1, 14); printf("Coluna: ");
            lerOpcao(&aux);
            col = aux;

            // Opcoes do menu: m - Menu Principal, t - Dica, r - Regras, s - Sair;
            if(col == 'm' || col == 'x' || col == 't' || col == 'r' || col == 's')
                return col;

            // Converte para coordenadas numericas validas;
            col = col - 'a';
        } while(col < 0 || col > COLUNA);

        if( campo[lin][col] != MARCADO && bombas[lin][col] == BOMBA ) {
            imprimeBombas();
            printf("\n\n\n\n\t\tVoce perdeu =( !\n");
            pausa("Tecle <enter> para voltar ao menu principal!");
            return 'm';
        } else if( campo[lin][col] != MARCADO && bombas[lin][col] != BOMBA )
            expande(lin, col);

        limpaTela();

    } while( !ganhou() );

    menu( MENUJOGO );
    imprimeCampo();

    printf("\n\n\n\n\t\tVoce ganhou, Parabens =) !!!\n");
    pausa("\n\nTecle <enter> para voltar ao menu principal!");
    return 'm';
}

bool ganhou() {
    return acertos == ( LINHA*COLUNA -  nivelEscolhido );
}

void preencheDicas() {

    int linha, coluna;
    int lin, col;
    int bomba;

    for(linha = 0; linha < LINHA; linha++) {
        for(coluna = 0; coluna < COLUNA; coluna++) {
            bomba = 0;
            if( bombas[linha][coluna] != BOMBA ) {
                for(lin = linha - 1; lin <= linha + 1; lin++)
                    for(col = coluna - 1; col <= coluna + 1; col++)
                        if(lin >= 0 && lin < LINHA && col >= 0 && col < COLUNA && bombas[lin][col] == BOMBA)
                            bomba++;
    
                bombas[linha][coluna] = '0' + bomba;
            }
        }
    }
}

void expande(int lin, int col) {

    if( bombas[lin][col] != '0' ) {
        campo[lin][col] = bombas[lin][col];
        acertos ++;
    } else {

        int i, j;

        campo[lin][col] = bombas[lin][col];
        acertos ++;

        for(i = lin - 1; i <= lin + 1; i++)
            for(j = col - 1; j <= col + 1; j++)
                if(i >= 0 && i < LINHA && j >= 0 && j < COLUNA && ( campo[i][j] == '-' || campo[i][j] == '?' ))
                    expande(i, j);
    }
}

void lerOpcao(char* opcao) {
    fflush(stdin);
    scanf("%c", opcao);
    *opcao = tolower( *opcao );
}

int  selecionaNivel() {

    int col = 1;
    int lin = 15;

    int nivel;

    gotoxy(col, lin++); printf(" 1 - Nivel Facil   (15 Bombas)\n");
    gotoxy(col, lin++); printf(" 2 - Nivel Medio   (20 Bombas)\n");
    gotoxy(col, lin++); printf(" 3 - Nivel Dificil (25 Bombas)\n");

    do {
    	fflush(stdin);
        gotoxy(col + 5, lin);
        scanf("%d", &nivel);
    } while( nivel < 1 || nivel > 3 );

    switch(nivel) {
        case 1:
            return FACIL;
        case 2:
            return MEDIO;
        case 3:
            return DIFICIL;
    }
}

void marcar() {

    int lin, col;
    char aux;

    limpaTela();
    imprimeCampo();
    menu( MENUJOGO );

    gotoxy(1, 19);
    printf(" - Marque (ou Desmarque) uma posicao que voce acredita ter bomba!\n");
    printf(" - Tecle 'X' e <enter> para cancelar!\n");

    do {
        gotoxy(1, 13); printf("Linha: ");
        lerOpcao(&aux);
        lin = aux;

        if(lin == 'x')
            return;
        // Converte para coordenadas numericas validas;
        lin = lin - '0';
    } while(lin < 0 || lin > LINHA);

    do {
        gotoxy(1, 14); printf("Coluna: ");
        lerOpcao(&aux);
        col = aux;

        if(col == 'x')
            return;

        // Converte para coordenadas numericas validas;
        col = col - 'a';
    } while(col < 0 || col > COLUNA);

    if( campo[lin][col] == '-' || campo[lin][col] == MARCADO )
        campo[lin][col] = ( campo[lin][col] == MARCADO ? '-' : MARCADO );
}

void menu(int m) {

    // Posição do menu
    int col;
    int lin;

    switch(m) {

        case MENUPRINCIPAL:

            col = 35;
            lin = 5;

            gotoxy(col, lin++);printf("+-- MENU PRINCIPAL --+");
            gotoxy(col, lin++);printf("|                    |");
            gotoxy(col, lin++);printf("|   N - Novo Jogo    |");
            gotoxy(col, lin++);printf("|   K - Ranking      |");
            gotoxy(col, lin++);printf("|   R - Regras       |");
            gotoxy(col, lin++);printf("|   S - Sair         |");
            gotoxy(col, lin++);printf("|                    |");
            gotoxy(col, lin++);printf("+--------------------+");

            gotoxy(25, lin + 4);
            printf("Escolha uma opcao do menu e tecle <enter>!\n");
            break;

        case MENUJOGO:

            col = 55;
            lin = 2;

            gotoxy(col, lin++);printf("+-------- MENU --------+");
            gotoxy(col, lin++);printf("|                      |");
            gotoxy(col, lin++);printf("| M - Menu Principal   |");
            gotoxy(col, lin++);printf("| X - Marcar/Desmarcar |");
            gotoxy(col, lin++);printf("| R - Regras           |");
            gotoxy(col, lin++);printf("| T - Dica             |");
            gotoxy(col, lin++);printf("| S - Sair             |");
            gotoxy(col, lin++);printf("|                      |");
            gotoxy(col, lin++);printf("+----------------------+");
            break;
    }
    gotoxy(col + 4, lin);
}

void imprimeBombas() {

    int i, j;

    gotoxy(1, 1);

    /** Cabecalho **/
    printf("   |");
    for(i = 0; i < LINHA; i++)
        printf(" %c |", 'A' + i);
    printf("\n");

    for(i = 0; i < LINHA; i++) {
        printf("%2d |", i);
        for(j = 0; j < COLUNA; j++) {
            if( bombas[i][j] != BOMBA)
                printf("    ");
            else
                printf(" %c  ", bombas[i][j]);
        }
        printf("\n");
    }
}

void imprimeCampo() {

    int i, j;

    gotoxy(1, 1);

    /** Cabecalho **/
    printf("   |");
    for(i = 0; i < LINHA; i++)
        printf(" %c |", 'A' + i);
    printf("\n");

    for(i = 0; i < LINHA; i++) {
        printf("%2d |", i);
        for(j = 0; j < COLUNA; j++)
                printf(" %c  ", campo[i][j]);
        printf("\n");
    }
}

void preencheBombas(int numBombas) {

    int lin, col;
    int cont = 0;

    srand( (unsigned int) time(NULL) );

    while( cont != numBombas ) {

        lin = rand() % LINHA;
        col = rand() % COLUNA;

        if( bombas[lin][col] != BOMBA ) {
            bombas[lin][col] = BOMBA;
            cont++;
        }
    }
}
// Posiciona o cursor do terminal nas posicoes x, y fornecidas;
void gotoxy(int x, int y) {
 	printf("\033[%d;%df", y, x);
 	fflush(stdout);
}
