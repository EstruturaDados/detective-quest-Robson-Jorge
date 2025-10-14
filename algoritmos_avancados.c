#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ======================================================================
// DESAFIO DETECTIVE QUEST - ALGORITMOS AVANÇADOS
// ======================================================================

// --- ESTRUTURAS ---

/**
 * @brief Estrutura que representa um cômodo (nó) na mansão.
 * Contém o nome da comodo e ponteiros para os caminhos à esquerda e direita.
 */
typedef struct Comodo {
    char nome[50];
    char pista[50]; 
    struct Comodo* esquerda;
    struct Comodo* direita;
} Comodo;

typedef struct Pista {
    char descricao[50];
    struct Pista* esquerda;
    struct Pista* direita;
} Pista;

// --- FUNÇÕES AUXILIARES DE GERENCIAMENTO ---

/**
 * @brief Cria e aloca memória para um novo cômodo.
 * @param nome O nome do cômodo.
 * @return Ponteiro para a nova Comodo criada.
 */
Comodo* criarComodo(const char* nome) {
    Comodo* novoComodo = (Comodo*)malloc(sizeof(Comodo));
    strcpy(novoComodo->nome, nome);
    strcpy(novoComodo->pista, "");
    novoComodo->esquerda = NULL;
    novoComodo->direita = NULL;
    return novoComodo;
}

/**
 * @brief Cria e aloca memória para uma nova pista.
 * @param descricao A descrição da pista.
 * @return Ponteiro para a nova Pista criada.
 */
Pista* criarPista(const char* descricao) {
    Pista* novaPista = (Pista*)malloc(sizeof(Pista));
    strcpy(novaPista->descricao, descricao);
    novaPista->esquerda = NULL;
    novaPista->direita = NULL;
    return novaPista;
}

/**
 * @brief Insere uma nova pista na árvore de pistas.
 * @param pista Ponteiro duplo para a raiz da árvore de pistas.
 * @param nome A descrição da nova pista a ser inserida.
 */
void inserirPista(Pista** arvorePistas, const char* pista) {
    if (*arvorePistas == NULL) {
        *arvorePistas = criarPista(pista);
    } else if (strcmp(pista, (*arvorePistas)->descricao) < 0) {
        inserirPista(&((*arvorePistas)->esquerda), pista);
    } else if(strcmp(pista, (*arvorePistas)->descricao) > 0) {
        inserirPista(&((*arvorePistas)->direita), pista);
    }
}

/**
 * @brief Exibe todas as pistas armazenadas na árvore de pistas (em ordem).
 * @param arvorePistas Ponteiro para a raiz da árvore de pistas.
 */
void exibirPistas(Pista* arvorePistas) {
    if (arvorePistas != NULL) {
        exibirPistas(arvorePistas->esquerda);
        printf("- %s\n", arvorePistas->descricao);
        exibirPistas(arvorePistas->direita);
    }
}

// --- FUNÇÕES AUXILIARES ---

/**
 * @brief Limpa o buffer de entrada para evitar problemas com scanf.
 */
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * @brief Libera a memória alocada para as salas da mansão.
 * @param comodo O nó (Comodo) atual a ser liberado.
 */
void liberarComodos(Comodo* comodo) {
    if (comodo != NULL) {
        liberarComodos(comodo->esquerda);
        liberarComodos(comodo->direita);
        free(comodo);
    }
}

/**
 * @brief Libera a memória alocada para as pistas.
 * @param arvorePistas Ponteiro para a raiz da árvore de pistas.
 */
void liberarPistas(Pista* arvorePistas) {
    if (arvorePistas != NULL) {
        liberarPistas(arvorePistas->esquerda);
        liberarPistas(arvorePistas->direita);
        free(arvorePistas);
    }
}

// --- FUNÇÃO PRINCIPAL DE NAVEGAÇÃO (RECURSIVA) ---

/**
 * @brief Permite ao jogador explorar os cômodos da mansão recursivamente.
 * A recursão é usada para simular o "voltar" à sala anterior.
 * * @param sala O nó (Comodo) atual onde o jogador se encontra.
 */
void explorarComodos(Comodo* comodo, Pista** arvorePistas) {
    if (comodo == NULL) {
        printf("Caminho sem saída. Voltando o cômodo anterior...\n");
        return;
    }
    char escolha;
    
    do
    {
        printf("\n=============================================\n");
        printf("Você está no cômodo: %s\n", comodo->nome);
        if (strlen(comodo->pista) > 0) {
            printf("Pista encontrada nesse cômodo: %s\n", comodo->pista);
            inserirPista(arvorePistas, comodo->pista);
        } else {
            printf("Nenhuma pista encontrada aqui.\n");
        }
        printf("=============================================\n");
        // pausarExecucao();

        // Exibição das opções disponíveis para guiar o usuário
        printf("\n=============================================\n");
        printf("Ações e Opções de Caminho:\n");

        // 1. Exibe as opções de Caminho (e, d) com o nome do cômodo
        if (comodo->esquerda != NULL) {
            printf("  [e] Ir para: %s\n", comodo->esquerda->nome);
        }
        
        if (comodo->direita != NULL) {
            printf("  [d] Ir para: %s\n", comodo->direita->nome);
        }
        
        // 2. Exibe as opções de Menu (p, s)
        printf("  [p] Ver todas as pistas coletadas (em ordem)\n");
        printf("  [s] Voltar ao cômodo anterior / Sair da mansão\n"); // Mensagem consolidada
        printf("=============================================\n");
        printf("Para onde deseja ir? (e: esquerda, d: direita, s: sair/voltar, p: ver pistas): ");

        scanf(" %c", &escolha);
        limparBuffer();

        switch (escolha) {
            case 'e':
                if(comodo->esquerda == NULL) {
                    printf("Caminho 'e' bloqueado ou inexistente. Tente novamente.\n");
                } else {
                    explorarComodos(comodo->esquerda, arvorePistas);
                }
                break;
            case 'd':
                if(comodo->direita == NULL) {
                    printf("Caminho 'd' bloqueado ou inexistente. Tente novamente.\n");
                } else {
                    explorarComodos(comodo->direita, arvorePistas);
                }
                break;
            case 'p':
                printf("\nPistas coletadas até agora:\n");
                if (arvorePistas != NULL) {
                    exibirPistas(*arvorePistas);
                } else {
                    printf("Nenhuma pista coletada ainda.\n");
                }
                break;
            case 's':
                printf("Saindo da exploração.\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
        
    } while (escolha != 's');
    
}

// --- FUNÇÃO PRINCIPAL ---
int main() {
    // --- CONSTRUÇÃO DO MAPA (ÁRVORE FIXA/ESTÁTICA) ---
    // Nível 0 (Raiz)
    Comodo* hall = criarComodo("Hall de Entrada"); 
    
    // Nível 1
    Comodo* biblioteca = criarComodo("Biblioteca");
    Comodo* salaDeEstar = criarComodo("Sala de Estar");
    hall->esquerda = biblioteca;
    hall->direita = salaDeEstar;
    
    // Nível 2
    Comodo* cozinha = criarComodo("Cozinha");
    Comodo* quarto = criarComodo("Quarto Principal");
    Comodo* banheiro = criarComodo("Banheiro");
    biblioteca->direita = quarto;
    biblioteca->esquerda = banheiro;
    salaDeEstar->esquerda = cozinha;
    
    // Nível 3 (Folhas - Fim do Caminho)
    Comodo* escritorio = criarComodo("Escritório");
    Comodo* areaDeServico = criarComodo("Área de Serviço");
    cozinha->direita = areaDeServico;
    quarto->direita = escritorio;

    // Adicionando pistas
    strcpy(biblioteca->pista, "Diário faltando páginas.");
    strcpy(cozinha->pista, "Faca com manchas de sangue.");
    strcpy(quarto->pista, "Telefone quebrado.");
    strcpy(banheiro->pista, "Toalha molhada.");
    strcpy(areaDeServico->pista, "Garrafas de produtos de limpeza abertas.");
    strcpy(escritorio->pista, "Bilhete encontrado: 'Libro azul na estante.'");

    Pista* arvorePistas = NULL;
    
    // --- EXPLORAÇÃO ---
    explorarComodos(hall, &arvorePistas);
    
    // --- LIMPEZA DA ÁRVORE DE COMODOS ---
    liberarComodos(hall);
    
    // --- LIMPEZA DA ÁRVORE DE PISTAS ---
    liberarPistas(arvorePistas);
    
    return 0;
}
