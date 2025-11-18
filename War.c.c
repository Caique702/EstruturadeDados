#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// -------------------------------------------
// STRUCTS
// -------------------------------------------

typedef struct {
    char nome[30];
    char cor[15];
    int tropas;
} Jogador;

typedef struct {
    char nome[30];
    Jogador *dono;
    int tropas;
} Territorio;

typedef struct {
    char descricao[100];
    int concluida;
} Missao;


// -------------------------------------------
// FUNÇÕES DO JOGO
// -------------------------------------------

void cadastrarJogadores(Jogador *players, int qtd) {
    char cores[5][15] = {"Verde", "Azul", "Vermelho", "Amarelo", "Branco"};

    for (int i = 0; i < qtd; i++) {
        printf("\n--- Cadastro do Jogador %d ---\n", i + 1);

        printf("Nome: ");
        scanf(" %[^\n]", players[i].nome);

        strcpy(players[i].cor, cores[i]);
        players[i].tropas = 10;

        printf("Cor atribuída: %s\n", players[i].cor);
    }
}

void inicializarTerritorios(Territorio *mapa) {
    char nomes[5][30] = {
        "America", "Europa", "Asia", "Africa", "Oceania"
    };

    for (int i = 0; i < 5; i++) {
        strcpy(mapa[i].nome, nomes[i]);
        mapa[i].dono = NULL;
        mapa[i].tropas = 0;
    }
}

void distribuirTerritorios(Territorio *mapa, Jogador *players, int qtdJogadores) {
    for (int i = 0; i < 5; i++) {
        mapa[i].dono = &players[i % qtdJogadores];
        mapa[i].tropas = 3;

        printf("%s recebeu %s com 3 tropas.\n",
               mapa[i].dono->nome, mapa[i].nome);
    }
}

void mostrarMapa(Territorio *mapa) {
    printf("\n========== MAPA DO JOGO ==========\n");

    for (int i = 0; i < 5; i++) {
        printf("%d) %-10s | Dono: %-10s | Tropas: %d\n",
               i,
               mapa[i].nome,
               mapa[i].dono->nome,
               mapa[i].tropas);
    }
}

void ataque(Territorio *atacante, Territorio *defensor) {
    printf("\n====================================\n");
    printf("           BATALHA INICIADA\n");
    printf("====================================\n");

    if (atacante->tropas < 2) {
        printf("O território atacante não possui tropas suficientes!\n");
        return;
    }

    printf("%s (%s) ATACA %s (%s)\n",
           atacante->nome, atacante->dono->cor,
           defensor->nome, defensor->dono->cor);

    int dadoAtq = rand() % 6 + 1;
    int dadoDef = rand() % 6 + 1;

    printf("\nDado do atacante: %d\n", dadoAtq);
    printf("Dado do defensor: %d\n", dadoDef);

    if (dadoAtq > dadoDef) {
        defensor->tropas--;
        printf("Defensor perdeu 1 tropa!\n");

        if (defensor->tropas == 0) {
            printf("\nTerritório CONQUISTADO!\n");
            defensor->dono = atacante->dono;
            defensor->tropas = 1;
            atacante->tropas--;
        }
    } else {
        atacante->tropas--;
        printf("Atacante perdeu 1 tropa!\n");
    }
}

void inicializarMissoes(Missao *missoes) {
    strcpy(missoes[0].descricao, "Conquistar 3 territorios.");
    strcpy(missoes[1].descricao, "Eliminar o jogador Azul.");
    strcpy(missoes[2].descricao, "Controlar America e Europa.");

    for (int i = 0; i < 3; i++)
        missoes[i].concluida = 0;
}

void mostrarMissoes(Missao *missoes) {
    printf("\n========== MISSOES ==========\n");

    for (int i = 0; i < 3; i++) {
        printf("%d) %s  [%s]\n",
               i,
               missoes[i].descricao,
               missoes[i].concluida ? "CONCLUIDA" : "PENDENTE");
    }
}

void mostrarJogadores(Jogador *players, int qtd) {
    printf("\n========== JOGADORES ==========\n");

    for (int i = 0; i < qtd; i++) {
        printf("%d) %-10s | Cor: %-10s | Tropas: %d\n",
               i, players[i].nome, players[i].cor, players[i].tropas);
    }
}


// -------------------------------------------
// MENU INTERATIVO
// -------------------------------------------

void menu(Territorio *mapa, Jogador *players, Missao *missoes, int qtdJogadores) {
    int opc;

    do {
        printf("\n\n====================================\n");
        printf("              MENU DO JOGO\n");
        printf("====================================\n");
        printf("1 - Mostrar mapa\n");
        printf("2 - Atacar território\n");
        printf("3 - Mostrar jogadores\n");
        printf("4 - Mostrar missões\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opc);

        switch (opc) {

            case 1:
                mostrarMapa(mapa);
                break;

            case 2: {
                int atq, def;
                mostrarMapa(mapa);

                printf("\nEscolha o número do território atacante: ");
                scanf("%d", &atq);

                printf("Escolha o número do território defensor: ");
                scanf("%d", &def);

                if (atq >= 0 && atq < 5 && def >= 0 && def < 5 && atq != def)
                    ataque(&mapa[atq], &mapa[def]);
                else
                    printf("Opção inválida!\n");
                break;
            }

            case 3:
                mostrarJogadores(players, qtdJogadores);
                break;

            case 4:
                mostrarMissoes(missoes);
                break;

            case 0:
                printf("\nEncerrando o jogo...\n");
                break;

            default:
                printf("Opção inválida!\n");
        }

    } while (opc != 0);
}


// -------------------------------------------
// MAIN
// -------------------------------------------

int main() {
    srand(time(NULL));

    int qtdJogadores = 5;

    Jogador *jogadores = malloc(qtdJogadores * sizeof(Jogador));
    Territorio *mapa = malloc(5 * sizeof(Territorio));
    Missao *missoes = malloc(3 * sizeof(Missao));

    if (!jogadores || !mapa || !missoes) {
        printf("Erro de alocação de memória!\n");
        return 1;
    }

    cadastrarJogadores(jogadores, qtdJogadores);
    inicializarTerritorios(mapa);
    distribuirTerritorios(mapa, jogadores, qtdJogadores);
    inicializarMissoes(missoes);

    menu(mapa, jogadores, missoes, qtdJogadores);

    free(jogadores);
    free(mapa);
    free(missoes);

    return 0;
}
