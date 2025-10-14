#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ======================================================================
// DESAFIO DETECTIVE QUEST - NÍVEL NOVATO: MAPA DA MANSÃO
//
// Implementa um mapa da mansão utilizando uma Árvore Binária, onde 
// cada nó (Sala) representa um cômodo. A navegação utiliza recursão 
// para simular a ação de "voltar" ao sair de uma sala.
// ======================================================================

// --- ESTRUTURAS ---

/**
 * @brief Estrutura que representa um cômodo (nó) na mansão.
 * Contém o nome da sala e ponteiros para os caminhos à esquerda e direita.
 */
typedef struct Sala {
    char nome[50];
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

// --- FUNÇÕES AUXILIARES DE GERENCIAMENTO ---

/**
 * @brief Cria e aloca memória para uma nova sala.
 * @param nome O nome do cômodo.
 * @return Ponteiro para a nova Sala criada.
 */
Sala* criarSala(const char* nome) {
    Sala* novaSala = (Sala*)malloc(sizeof(Sala));
    strcpy(novaSala->nome, nome);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

/**
 * @brief Limpa o buffer de entrada para evitar problemas com scanf.
 */
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * @brief Libera a memória alocada para as salas da mansão.
 * @param sala O nó (Sala) atual a ser liberado.
 */
void liberarSalas(Sala* sala) {
    if (sala != NULL) {
        liberarSalas(sala->esquerda);
        liberarSalas(sala->direita);
        free(sala);
    }
}

// --- FUNÇÃO PRINCIPAL DE NAVEGAÇÃO (RECURSIVA) ---

/**
 * @brief Permite ao jogador explorar as salas recursivamente.
 * A recursão é usada para simular o "voltar" à sala anterior.
 * * @param sala O nó (Sala) atual onde o jogador se encontra.
 */
void explorarSalas(Sala* sala) {
    if (sala == NULL) {
        printf("Caminho sem saida. Voltando a sala anterior...\n");
        return;
    }
    char escolha;
    
    do
    {
        printf("\n=============================================\n");
        printf("Você está na sala: %s\n", sala->nome);
        printf("=============================================\n");

        // Exibição das opções disponíveis para guiar o usuário
        printf("Opções de Caminho:\n");
        
        if (sala->esquerda != NULL) {
            printf("  [e] Ir para: %s\n", sala->esquerda->nome);
        }
        
        if (sala->direita != NULL) {
            printf("  [d] Ir para: %s\n", sala->direita->nome);
        }
        
        printf("  [s] Voltar à sala anterior/sair da mansão\n"); 
        printf("\nPara onde deseja ir? (e: esquerda, d: direita, s: sair/voltar): ");

        scanf(" %c", &escolha);
        limparBuffer();
        
        if (escolha == 'e') {
            if (sala->esquerda != NULL) {
                explorarSalas(sala->esquerda);
            } else {
                printf("Caminho 'e' bloqueado ou inexistente. Tente novamente.\n");
            }
        } else if (escolha == 'd') {
            if (sala->direita != NULL) {
                explorarSalas(sala->direita);
            } else {
                printf("Caminho 'd' bloqueado ou inexistente. Tente novamente.\n");
            }
        } else if (escolha == 's') {
            printf("Saindo da exploração.\n");
        } else {
            printf("Opção inválida. Tente novamente.\n");
        }
    } while (escolha != 's');
    
}

// --- FUNÇÃO PRINCIPAL ---
int main() {
    // --- CONSTRUÇÃO DO MAPA (ÁRVORE FIXA/ESTÁTICA) ---
    // Nível 0 (Raiz)
    Sala* hall = criarSala("Hall de Entrada"); 
    
    // Nível 1
    Sala* biblioteca = criarSala("Biblioteca");
    Sala* cozinha = criarSala("Cozinha");
    hall->esquerda = biblioteca;
    hall->direita = cozinha;
    
    // Nível 2
    Sala* sotao = criarSala("Sótão");
    Sala* quarto = criarSala("Quarto Principal");
    biblioteca->esquerda = sotao;
    cozinha->direita = quarto;
    
    // Nível 3 (Folhas - Fim do Caminho)
    Sala* banheiro = criarSala("Banheiro");
    sotao->esquerda = banheiro;
    
    // --- EXPLORAÇÃO ---
    explorarSalas(hall);
    
    // --- LIMPEZA ---
    liberarSalas(hall);
    
    return 0;
}
