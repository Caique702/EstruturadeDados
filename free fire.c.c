#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INV 20
#define STRING 50

// ------------------------------
// STRUCTS
// ------------------------------
typedef struct {
    char nome[STRING];
    char tipo[STRING];
    int prioridade;
} Item;

typedef struct Node {
    Item item;
    struct Node *next;
} Node;

// ------------------------------
// INVENTÁRIO - LISTA SEQUENCIAL (VETOR)
// ------------------------------
Item mochila[MAX_INV];
int qtdItens = 0;

// ------------------------------
// LISTA ENCADEADA
// ------------------------------
Node* head = NULL;

// ------------------------------
// FUNÇÕES AUXILIARES
// ------------------------------
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// ------------------------------
// ADICIONAR ITEM AO VETOR
// ------------------------------
void adicionarItemVetor() {
    if (qtdItens >= MAX_INV) {
        printf("\nA mochila está cheia!\n");
        return;
    }

    Item novo;
    printf("\n--- Adicionar Item (Vetor) ---\n");
    printf("Nome: ");
    scanf("%s", novo.nome);

    printf("Tipo (alimento/arma/ferramenta): ");
    scanf("%s", novo.tipo);

    printf("Prioridade (1-100): ");
    scanf("%d", &novo.prioridade);

    mochila[qtdItens++] = novo;
    printf("Item adicionado ao inventário (vetor)!\n");
}

// ------------------------------
// ADICIONAR ITEM NA LISTA ENCADEADA
// ------------------------------
void adicionarItemLista() {
    Node *novo = (Node*)malloc(sizeof(Node));

    printf("\n--- Adicionar Item (Lista Encadeada) ---\n");
    printf("Nome: ");
    scanf("%s", novo->item.nome);

    printf("Tipo: ");
    scanf("%s", novo->item.tipo);

    printf("Prioridade: ");
    scanf("%d", &novo->item.prioridade);

    novo->next = head;
    head = novo;

    printf("Item adicionado à lista encadeada!\n");
}

// ------------------------------
// EXIBIR VETOR
// ------------------------------
void exibirVetor() {
    printf("\n--- Inventário (Vetor) ---\n");

    if (qtdItens == 0) {
        printf("Mochila vazia.\n");
        return;
    }

    for (int i = 0; i < qtdItens; i++) {
        printf("[%d] %s | Tipo: %s | Prioridade: %d\n",
            i, mochila[i].nome, mochila[i].tipo, mochila[i].prioridade);
    }
}

// ------------------------------
// EXIBIR LISTA ENCADEADA
// ------------------------------
void exibirLista() {
    printf("\n--- Inventário (Lista Encadeada) ---\n");

    if (head == NULL) {
        printf("Lista vazia.\n");
        return;
    }

    Node *ptr = head;
    while (ptr != NULL) {
        printf("%s | Tipo: %s | Prioridade: %d\n",
            ptr->item.nome, ptr->item.tipo, ptr->item.prioridade);
        ptr = ptr->next;
    }
}

// ------------------------------
// SELECTION SORT (por nome)
// ------------------------------
void ordenarPorNome() {
    for (int i = 0; i < qtdItens - 1; i++) {
        int menor = i;

        for (int j = i + 1; j < qtdItens; j++) {
            if (strcmp(mochila[j].nome, mochila[menor].nome) < 0) {
                menor = j;
            }
        }

        if (menor != i) {
            Item temp = mochila[i];
            mochila[i] = mochila[menor];
            mochila[menor] = temp;
        }
    }
    printf("\nInventário ordenado alfabeticamente!\n");
}

// ------------------------------
// BUSCA BINÁRIA (NOME)
// ------------------------------
int buscaBinaria(char chave[]) {
    int inicio = 0;
    int fim = qtdItens - 1;

    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;
        int cmp = strcmp(mochila[meio].nome, chave);

        if (cmp == 0) return meio;
        else if (cmp < 0) inicio = meio + 1;
        else fim = meio - 1;
    }
    return -1;
}

// ------------------------------
// MENU
// ------------------------------
void menu() {
    int op;
    do {
        printf("\n=============================\n");
        printf("     MINI GAME - SOBREVIDA\n");
        printf("=============================\n");
        printf("1. Adicionar item (vetor)\n");
        printf("2. Adicionar item (lista encadeada)\n");
        printf("3. Exibir inventário (vetor)\n");
        printf("4. Exibir inventário (lista encadeada)\n");
        printf("5. Ordenar inventário (nome)\n");
        printf("6. Buscar item (nome)\n");
        printf("0. Sair\n");
        printf("Escolha: ");
        scanf("%d", &op);

        switch (op) {
            case 1: adicionarItemVetor(); break;
            case 2: adicionarItemLista(); break;
            case 3: exibirVetor(); break;
            case 4: exibirLista(); break;
            case 5: ordenarPorNome(); break;
            case 6: {
                char nomeBusca[STRING];
                printf("Digite o nome para busca: ");
                scanf("%s", nomeBusca);
                int pos = buscaBinaria(nomeBusca);
                if (pos >= 0)
                    printf("Item encontrado no vetor: %s (posição %d)\n", mochila[pos].nome, pos);
                else
                    printf("Item NÃO encontrado.\n");
                break;
            }
            case 0: printf("Encerrando...\n"); break;
            default: printf("Opção inválida!\n");
        }

    } while (op != 0);
}

// ------------------------------
// MAIN
// ------------------------------
int main() {
    menu();
    return 0;
}
