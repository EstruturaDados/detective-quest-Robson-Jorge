#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ======================================================================
// DESAFIO DETECTIVE QUEST - ALGORITMOS AVANÇADOS
// ======================================================================

// Define o tamanho da tabela hash
#define TAMANHO_TABELA 10

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

/**
 * @brief Estrutura que representa uma pista (nó) em uma árvore binária.
 * Contém a descrição da pista e ponteiros para os nós esquerdo e direito.
 */
typedef struct Pista {
    char descricao[50];
    struct Pista* esquerda;
    struct Pista* direita;
} Pista;

/**
 * @brief Estrutura que representa um suspeito.
 * Contém o nome do suspeito, uma árvore de pistas associadas e um ponteiro para o próximo suspeito na lista.
 */
typedef struct Suspeito {
    char nome[50];
    Pista* pistas; // Árvore de pistas associadas ao suspeito
    struct Suspeito* proximo; // Ponteiro para o próximo suspeito na lista
} Suspeito;

/**
 * @brief Estrutura da Tabela Hash.
 * É um array de ponteiros para o início das Listas Encadeadas (Buckets).
 */
typedef struct {
    Suspeito* buckets[TAMANHO_TABELA];
} TabelaHash;

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

/**
 * @brief Pesquisa uma pista na árvore de pistas.
 * @param arvorePistas Ponteiro para a raiz da árvore de pistas.
 * @param descricao A descrição da pista a ser pesquisada.
 * @return 1 se a pista for encontrada, 0 caso contrário.
 */
int pesquisarPista(Pista* arvorePistas, const char* descricao) {
    if (arvorePistas == NULL) {
        return 0; // Pista não encontrada
    }
    if (strcmp(descricao, arvorePistas->descricao) == 0) {
        return 1; // Pista encontrada
    } else if (strcmp(descricao, arvorePistas->descricao) < 0) {
        return pesquisarPista(arvorePistas->esquerda, descricao);
    } else {
        return pesquisarPista(arvorePistas->direita, descricao);
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

// --- TABELA HASH ---

/**
 * @brief Função de Espalhamento (Hash Function).
 * Mapeia a chave (string) para um índice (0 a TAMANHO_TABELA - 1).
 * * @param chave A string a ser convertida.
 * @return O índice (bucket) no array.
 */
int calcularHash(const char* chave) {
    int soma = 0;
    
    // 1. Soma os valores ASCII de todos os caracteres
    for (int i = 0; chave[i] != '\0'; i++) {
        soma = soma + chave[i];
    }
    
    // 2. Aplica o MÓDULO para garantir que o índice caia dentro do tamanho da tabela.
    // Esta é a parte crucial do mapeamento.
    return soma % TAMANHO_TABELA;
}


/**
 * @brief Inicializa a Tabela Hash com todos os buckets vazios (NULL).
 * @param th O ponteiro para a Tabela Hash.
 */
void inicializarTabela(TabelaHash* th) {
    for (int i = 0; i < TAMANHO_TABELA; i++) {
        th->buckets[i] = NULL;
    }
}

/**
 * @brief Insere um suspeito e uma pista na Tabela Hash.
 * @param th O ponteiro para a Tabela Hash.
 * @param nomeSuspeito O nome do suspeito.
 * @param descricaoPista A descrição da pista.
 */
void inserirHash(TabelaHash* th, const char* nomeSuspeito, const char* descricaoPista) {
    int indice = calcularHash(nomeSuspeito);
    
    // Verifica se o suspeito já existe na lista do bucket
    Suspeito* atual = th->buckets[indice];
    while (atual != NULL) {
        if (strcmp(atual->nome, nomeSuspeito) == 0) {
            // Suspeito encontrado, insere a pista na árvore de pistas
            inserirPista(&(atual->pistas), descricaoPista);
            return;
        }
        atual = atual->proximo;
    }
    
    // Suspeito não encontrado, cria um novo e insere no início da lista
    Suspeito* novoSuspeito = (Suspeito*)malloc(sizeof(Suspeito));
    strcpy(novoSuspeito->nome, nomeSuspeito);
    novoSuspeito->pistas = NULL;
    inserirPista(&(novoSuspeito->pistas), descricaoPista);
    novoSuspeito->proximo = th->buckets[indice];
    th->buckets[indice] = novoSuspeito;
}

/**
 * @brief Conta recursivamente o número total de nós (pistas) em uma BST.
 */
int contarPistasBST(Pista* raiz) {
    if (raiz == NULL) {
        return 0; // Caso base: a subárvore está vazia
    }
    // Retorna a contagem da esquerda + contagem da direita + 1 (para o nó atual)
    return 1 + contarPistasBST(raiz->esquerda) + contarPistasBST(raiz->direita);
}

/**
 * @brief Exibe os suspeitos e suas pistas na Tabela Hash.
 * @param th O ponteiro para a Tabela Hash.
 */
void exibirSuspeitos(TabelaHash* th) {
    printf("\n--- Suspeitos e suas Pistas ---\n");
    Suspeito* suspeitoMaisCitado = NULL;
    int maxPistas = 0;

    for (int i = 0; i < TAMANHO_TABELA; i++) {
        Suspeito* atual = th->buckets[i];
        while (atual != NULL) {
            printf("Suspeito: %s\n", atual->nome);
            printf("Pistas:\n");
            exibirPistas(atual->pistas);
            
            // Contar o número de pistas para determinar o mais citado
            int contadorPistas = contarPistasBST(atual->pistas);
            printf("Total de pistas: %d\n\n", contadorPistas);

            if (contadorPistas > maxPistas) {
                maxPistas = contadorPistas;
                suspeitoMaisCitado = atual;
            }
            
            atual = atual->proximo;
        }
    }
        if (suspeitoMaisCitado != NULL) {
        printf("\nO Suspeito mais citado é: %s (com %d pistas).\n", suspeitoMaisCitado->nome, maxPistas);
    } else {
        printf("\nNenhum suspeito encontrado.\n");
    }
}

/**
 * @brief Busca um suspeito na Tabela Hash com base na descrição da pista.
 * @param th O ponteiro para a Tabela Hash.
 * @param descricaoPista A descrição da pista.
 * @return O ponteiro para o suspeito encontrado, ou NULL se não for encontrado.
 */
Suspeito* buscarSuspeito(TabelaHash* th, const char* descricaoPista) {
    for (int i = 0; i < TAMANHO_TABELA; i++) {
        Suspeito* atual = th->buckets[i];
        while (atual != NULL) {
            if (pesquisarPista(atual->pistas, descricaoPista)) {
                return atual;
            }
            atual = atual->proximo;
        }
    }
    printf("Nenhum suspeito encontrado com a pista '%s'.\n", descricaoPista);
    return NULL;
}

/**
 * @brief Libera a memória alocada para a Tabela Hash.
 * @param th O ponteiro para a Tabela Hash.
*/
void liberarTabela(TabelaHash* th) {
    for (int i = 0; i < TAMANHO_TABELA; i++) {
        Suspeito* atual = th->buckets[i];
        while (atual != NULL) {
            Suspeito* temp = atual;
            atual = atual->proximo;
            liberarPistas(temp->pistas); // Libera a árvore de pistas associada
            free(temp);
        }
    }
}

// --- FUNÇÃO PRINCIPAL DE NAVEGAÇÃO (RECURSIVA) ---

/**
 * @brief Permite ao jogador explorar os cômodos da mansão recursivamente.
 * A recursão é usada para simular o "voltar" à sala anterior.
 * * @param sala O nó (Comodo) atual onde o jogador se encontra.
 */
void explorarComodos(Comodo* comodo, Pista** arvorePistas, TabelaHash* tabelaSuspeitos) {
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
            Suspeito* suspeito = buscarSuspeito(tabelaSuspeitos, comodo->pista);
            if (suspeito != NULL) {
                printf("Suspeito associado a essa pista: %s\n", suspeito->nome);
            }
        } else {
            printf("Nenhuma pista encontrada aqui.\n");
        }
        printf("=============================================\n");

        // Exibição das opções disponíveis para guiar o usuário
        printf("\n=============================================\n");
        printf("Ações e Opções de Caminho:\n");

        if (comodo->esquerda != NULL) {
            printf("  [e] Ir para: %s\n", comodo->esquerda->nome);
        }
        
        if (comodo->direita != NULL) {
            printf("  [d] Ir para: %s\n", comodo->direita->nome);
        }
        
        printf("  [p] Ver todas as pistas coletadas (em ordem)\n");
        printf("  [s] Voltar ao cômodo anterior / Sair da mansão\n");
        printf("=============================================\n");
        printf("Para onde deseja ir? (e: esquerda, d: direita, s: sair/voltar, p: ver pistas): ");

        scanf(" %c", &escolha);
        limparBuffer();

        switch (escolha) {
            case 'e':
                if(comodo->esquerda == NULL) {
                    printf("Caminho 'e' bloqueado ou inexistente. Tente novamente.\n");
                } else {
                    explorarComodos(comodo->esquerda, arvorePistas, tabelaSuspeitos);
                }
                break;
            case 'd':
                if(comodo->direita == NULL) {
                    printf("Caminho 'd' bloqueado ou inexistente. Tente novamente.\n");
                } else {
                    explorarComodos(comodo->direita, arvorePistas, tabelaSuspeitos);
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

    // --- INICIALIZAÇÃO DA ÁRVORE DE PISTAS ---
    Pista* arvorePistas = NULL;

    // --- INICIALIZAÇÃO DA TABELA HASH ---
    TabelaHash tabelaSuspeitos;
    inicializarTabela(&tabelaSuspeitos);

    // --- INSERÇÃO DE SUSPEITOS E PISTAS ---
    inserirHash(&tabelaSuspeitos, "Sr. Verde", "Diário faltando páginas.");
    inserirHash(&tabelaSuspeitos, "Sr. Verde", "Faca com manchas de sangue.");
    inserirHash(&tabelaSuspeitos, "Sra. Rosa", "Telefone quebrado.");
    inserirHash(&tabelaSuspeitos, "Sra. Rosa", "Toalha molhada.");
    inserirHash(&tabelaSuspeitos, "Sr. Azul", "Garrafas de produtos de limpeza abertas.");
    inserirHash(&tabelaSuspeitos, "Sra. Rosa", "Bilhete encontrado: 'Libro azul na estante.'");

    // --- MENU PRINCIPAL ---
    char comandoPrincipal;
    printf("\n--- BEM-VINDO AO DETECTIVE QUEST (NÍVEL MESTRE) ---\n");

    do {
        printf("\n=============================================\n");
        printf("       MENU DE INVESTIGAÇÃO PRINCIPAL\n");
        printf("=============================================\n");
        printf(" [1] EXPLORAR a Mansão (Buscar Pistas)\n");
        printf(" [2] ANALISAR Evidências e Mostrar Suspeitos(Dica: Explore primeiro a Mansão)\n");
        printf(" [3] SAIR do Jogo\n");
        printf("=============================================\n");
        printf("Escolha uma opção: ");
        scanf(" %c", &comandoPrincipal);
        limparBuffer();

        switch (comandoPrincipal) {
            case '1':
                printf("\n>>> Iniciando Exploração (Volte com 's' para o Menu Principal).\n");
                explorarComodos(hall, &arvorePistas, &tabelaSuspeitos);
                break;
            case '2':
                printf("\n>>> FASE DE ANÁLISE: Verificando as associações de pistas...\n");
                exibirSuspeitos(&tabelaSuspeitos);
                break;
            case '3':
                printf("\nEncerrando o jogo. Adeus, detetive.\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }

    } while (comandoPrincipal != '3'); 

    // --- LIMPEZA DE MEMÓRIA ---
    liberarComodos(hall);
    liberarPistas(arvorePistas);
    liberarTabela(&tabelaSuspeitos);
    
    return 0;
}
