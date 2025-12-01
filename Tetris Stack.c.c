#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ---------------------
// STRUCT DA PEÇA
// ---------------------
typedef struct {
    char nome[20];
} Peca;

// ---------------------
// FILA (Peças Futuras)
// ---------------------
#define TAM_FILA 10

typedef struct {
    Peca itens[TAM_FILA];
    int inicio, fim, quantidade;
} Fila;

// Inicializa a fila
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->quantidade = 0;
}

// Verifica se fila está cheia
int filaCheia(Fila *f) {
    return f->quantidade == TAM_FILA;
}

// Verifica se fila está vazia
int filaVazia(Fila *f) {
    return f->quantidade == 0;
}

// Insere peça na fila
void enfileirar(Fila *f, Peca p) {
    if (filaCheia(f)) {
        printf("\n[FILA] A fila está cheia! Não é possível adicionar mais peças.\n");
        return;
    }
    f->itens[f->fim] = p;
    f->fim = (f->fim + 1) % TAM_FILA;
    f->quantidade++;
}

// Remove peça da fila
Peca desenfileirar(Fila *f) {
    Peca vazia = {"NULL"};
    if (filaVazia(f)) {
        printf("\n[FILA] A fila está vazia!\n");
        return vazia;
    }
    Peca removida = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % TAM_FILA;
    f->quantidade--;
    return removida;
}

// Exibe a fila
void exibirFila(Fila *f) {
    printf("\n--- FILA DE PECAS FUTURAS ---\n");
    if (filaVazia(f)) {
        printf("A fila está vazia.\n");
        return;
    }

    int i = f->inicio;
    for (int c = 0; c < f->quantidade; c++) {
        printf("%d: %s\n", c + 1, f->itens[i].nome);
        i = (i + 1) % TAM_FILA;
    }
}

// ---------------------
// PILHA (Peça Reservada)
// ---------------------
typedef struct {
    Peca topo;
    int cheio;
} Pilha;

// Inicializa pilha
void inicializarPilha(Pilha *p) {
    p->cheio = 0;
}

// Verifica se pilha está vazia
int pilhaVazia(Pilha *p) {
    return p->cheio == 0;
}

// Insere na pilha
void push(Pilha *p, Peca x) {
    if (p->cheio) {
        printf("\n[PILHA] A pilha já está ocupada! Só é possível trocar.\n");
        return;
    }
    p->topo = x;
    p->cheio = 1;
}

// Remove da pilha
Peca pop(Pilha *p) {
    Peca vazia = {"NULL"};
    if (pilhaVazia(p)) {
        printf("\n[PILHA] Não há peça reservada.\n");
        return vazia;
    }
    p->cheio = 0;
    return p->topo;
}

// Exibe pilha
void exibirPilha(Pilha *p) {
    printf("\n--- PECA RESERVADA (HOLD) ---\n");
    if (pilhaVazia(p)) {
        printf("Nenhuma peça reservada.\n");
    } else {
        printf("Reservada: %s\n", p->topo.nome);
    }
}

// ---------------------
// SISTEMA DO JOGO
// ---------------------

char pecasTetris[7][20] = {
    "I", "O", "T", "L", "J", "S", "Z"
};

// Cria uma peça aleatória
Peca gerarPeca() {
    Peca nova;
    strcpy(nova.nome, pecasTetris[rand() % 7]);
    return nova;
}

// Preenche a fila inicial
void preencherFila(Fila *f) {
    while (!filaCheia(f)) {
        enfileirar(f, gerarPeca());
    }
}

// ---------------------
// MENU
// ---------------------

void menu() {
    printf("\n===== TETRIS STACK - CONTROLE DE PEÇAS =====\n");
    printf("1. Jogar próxima peça (remove da fila)\n");
    printf("2. Reservar peça (fila -> pilha)\n");
    printf("3. Recuperar peça reservada (pilha -> fila)\n");
    printf("4. Exibir fila de peças futuras\n");
    printf("5. Exibir peça reservada\n");
    printf("6. Sair\n> ");
}

int main() {
    Fila fila;
    Pilha pilha;
    int opcao;

    srand(time(NULL));

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    preencherFila(&fila);

    while (1) {
        menu();
        scanf("%d", &opcao);

        if (opcao == 6) {
            printf("\nEncerrando...\n");
            break;
        }

        switch (opcao) {
            case 1: {
                Peca jogada = desenfileirar(&fila);
                if (strcmp(jogada.nome, "NULL") != 0) {
                    printf("\nVocê jogou a peça: %s\n", jogada.nome);
                    enfileirar(&fila, gerarPeca());
                }
            } break;

            case 2: {
                if (filaVazia(&fila)) {
                    printf("\nNão há peças na fila.\n");
                    break;
                }

                Peca proxima = fila.itens[fila.inicio];

                if (pilhaVazia(&pilha)) {
                    push(&pilha, proxima);
                    desenfileirar(&fila);
                    enfileirar(&fila, gerarPeca());
                    printf("\nPeça %s reservada!\n", proxima.nome);
                } else {
                    Peca temp = pilha.topo;
                    pop(&pilha);

                    push(&pilha, proxima);
                    desenfileirar(&fila);
                    enfileirar(&fila, gerarPeca());

                    printf("\nTrocou a peça reservada %s pela peça %s!\n",
                           temp.nome, proxima.nome);
                }
            } break;

            case 3: {
                if (pilhaVazia(&pilha)) {
                    printf("\nNenhuma peça para recuperar.\n");
                    break;
                }
                Peca recuperada = pop(&pilha);
                printf("\nVocê recuperou a peça reservada: %s\n", recuperada.nome);
                enfileirar(&fila, recuperada);
            } break;

            case 4:
                exibirFila(&fila);
                break;

            case 5:
                exibirPilha(&pilha);
                break;

            default:
                printf("\nOpção inválida!\n");
        }
    }

    return 0;
}
