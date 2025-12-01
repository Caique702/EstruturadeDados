#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_TABELA 10

// =====================================================
// ===================== ÁRVORE BINÁRIA (MAPA) =========
// =====================================================

typedef struct NoMapa {
    char nome[50];
    struct NoMapa *esq, *dir;
} NoMapa;

NoMapa* criarNoMapa(char *nome) {
    NoMapa *novo = (NoMapa*) malloc(sizeof(NoMapa));
    strcpy(novo->nome, nome);
    novo->esq = novo->dir = NULL;
    return novo;
}

void exibirMapa(NoMapa *raiz, int nivel) {
    if (raiz == NULL) return;

    for (int i = 0; i < nivel; i++)
        printf("  ");

    printf("- %s\n", raiz->nome);

    exibirMapa(raiz->esq, nivel + 1);
    exibirMapa(raiz->dir, nivel + 1);
}

void moverDetetive(NoMapa *atual) {
    if (!atual) return;
    printf("\nVocê está no cômodo: %s\n", atual->nome);

    char opcao;
    printf("Mover (E/D): ");
    scanf(" %c", &opcao);

    if (opcao == 'E' || opcao == 'e') {
        if (atual->esq)
            moverDetetive(atual->esq);
        else
            printf("Não há cômodo à esquerda.\n");
    } else if (opcao == 'D' || opcao == 'd') {
        if (atual->dir)
            moverDetetive(atual->dir);
        else
            printf("Não há cômodo à direita.\n");
    } else {
        printf("Entrada inválida.\n");
    }
}

// =====================================================
// =============== ÁRVORE DE BUSCA (PISTAS) ============
// =====================================================

typedef struct NoPista {
    char pista[100];
    struct NoPista *esq, *dir;
} NoPista;

NoPista* criarNoPista(char *texto) {
    NoPista *novo = (NoPista*) malloc(sizeof(NoPista));
    strcpy(novo->pista, texto);
    novo->esq = novo->dir = NULL;
    return novo;
}

NoPista* inserirPista(NoPista *raiz, char *texto) {
    if (raiz == NULL) return criarNoPista(texto);

    if (strcmp(texto, raiz->pista) < 0)
        raiz->esq = inserirPista(raiz->esq, texto);
    else
        raiz->dir = inserirPista(raiz->dir, texto);

    return raiz;
}

void exibirPistas(NoPista *raiz) {
    if (!raiz) return;
    exibirPistas(raiz->esq);
    printf("- %s\n", raiz->pista);
    exibirPistas(raiz->dir);
}

// =====================================================
// ===================== TABELA HASH ====================
// =====================================================

typedef struct ItemHash {
    char pista[100];
    char suspeito[50];
    struct ItemHash *prox;
} ItemHash;

ItemHash* tabela[TAM_TABELA];

unsigned int hash(char *pista) {
    unsigned int h = 0;
    for (int i = 0; pista[i] != '\0'; i++)
        h += pista[i];
    return h % TAM_TABELA;
}

void inicializarHash() {
    for (int i = 0; i < TAM_TABELA; i++)
        tabela[i] = NULL;
}

void inserirHash(char *pista, char *suspeito) {
    unsigned int indice = hash(pista);

    ItemHash *novo = (ItemHash*) malloc(sizeof(ItemHash));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->prox = tabela[indice];

    tabela[indice] = novo;

    printf("\nPista vinculada ao suspeito!\n");
}

void buscarSuspeito(char *pista) {
    unsigned int indice = hash(pista);
    ItemHash *aux = tabela[indice];

    while (aux != NULL) {
        if (strcmp(aux->pista, pista) == 0) {
            printf("\nSUSPEITO RELACIONADO: %s\n", aux->suspeito);
            return;
        }
        aux = aux->prox;
    }

    printf("\nNenhum suspeito encontrado para essa pista.\n");
}

void exibirHash() {
    printf("\n========== TABELA HASH ==========\n");
    for (int i = 0; i < TAM_TABELA; i++) {
        printf("[%d] ", i);
        ItemHash *aux = tabela[i];
        while (aux != NULL) {
            printf("(%s -> %s)  ", aux->pista, aux->suspeito);
            aux = aux->prox;
        }
        printf("\n");
    }
}

// =====================================================
// =========================== MENU ====================
// =====================================================

void menu() {
    printf("\n===== SISTEMA DE INVESTIGAÇÃO =====\n");
    printf("1 - Exibir mapa da mansão\n");
    printf("2 - Mover detetive pelo mapa\n");
    printf("3 - Adicionar nova pista\n");
    printf("4 - Exibir todas as pistas\n");
    printf("5 - Relacionar pista a suspeito\n");
    printf("6 - Buscar suspeito por pista\n");
    printf("7 - Exibir tabela hash\n");
    printf("8 - Sair\n> ");
}

int main() {
    // ------- MAPA DA MANSÃO (ÁRVORE BINÁRIA) -------
    NoMapa *mapa = criarNoMapa("Entrada");
    mapa->esq = criarNoMapa("Sala de Estar");
    mapa->dir = criarNoMapa("Biblioteca");
    mapa->esq->esq = criarNoMapa("Cozinha");
    mapa->esq->dir = criarNoMapa("Sala de Jantar");
    mapa->dir->esq = criarNoMapa("Escritório");
    mapa->dir->dir = criarNoMapa("Porão");

    // ------- ÁRVORE DE BUSCA (PISTAS) -------
    NoPista *pistas = NULL;

    // ------- TABELA HASH -------
    inicializarHash();

    int op;
    char texto[100], suspeito[50];

    while (1) {
        menu();
        scanf("%d", &op);

        switch (op) {
            case 1:
                exibirMapa(mapa, 0);
                break;

            case 2:
                moverDetetive(mapa);
                break;

            case 3:
                printf("Digite a pista: ");
                getchar();
                fgets(texto, 100, stdin);
                texto[strcspn(texto, "\n")] = 0;

                pistas = inserirPista(pistas, texto);
                printf("Pista adicionada!\n");
                break;

            case 4:
                exibirPistas(pistas);
                break;

            case 5:
                printf("Digite a pista: ");
                getchar();
                fgets(texto, 100, stdin);
                texto[strcspn(texto, "\n")] = 0;

                printf("Digite o suspeito: ");
                fgets(suspeito, 50, stdin);
                suspeito[strcspn(suspeito, "\n")] = 0;

                inserirHash(texto, suspeito);
                break;

            case 6:
                printf("Digite a pista a procurar: ");
                getchar();
                fgets(texto, 100, stdin);
                texto[strcspn(texto, "\n")] = 0;
                buscarSuspeito(texto);
                break;

            case 7:
                exibirHash();
                break;

            case 8:
                printf("Encerrando...\n");
                return 0;

            default:
                printf("Opção inválida!\n");
        }
    }

    return 0;
}
