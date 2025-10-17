/* 
 * ================================================================
 * NÍVEL AVENTUREIRO - DESAFIO WAR ESTRUTURADO
 * ================================================================
 * Objetivo:
 *  - Substituir o vetor estático por alocação dinâmica (calloc)
 *  - Implementar uma fase de ataque entre dois territórios
 *  - Utilizar números aleatórios para simular dados de batalha
 *
 * Conceitos aplicados:
 *  - Struct
 *  - Ponteiros
 *  - Alocação dinâmica (calloc / free)
 *  - Aleatoriedade (rand / srand)
 *  - Funções para modularizar partes do código
 *  - Entrada e saída com scanf / fgets
 * ================================================================
 */

#include <stdio.h>   // Para entrada e saída
#include <stdlib.h>  // Para calloc, free, rand, srand
#include <string.h>  // Para manipulação de strings
#include <time.h>    // Para srand(time(NULL))

// ------------------------------------------------------------
// Constantes globais
// ------------------------------------------------------------
#define QTDE_TERRITORIOS 5
#define TAM_NOME 30
#define TAM_COR  10

// ------------------------------------------------------------
// Estrutura de dados principal: Território
// Armazena nome, cor do exército e quantidade de tropas.
// ------------------------------------------------------------
typedef struct {
    char nome[TAM_NOME];
    char cor[TAM_COR];
    int  tropas;
} Territorio;

// ------------------------------------------------------------
// Função auxiliar: strip()
// Remove o '\n' deixado pelo fgets no final da string.
// ------------------------------------------------------------
static void strip(char *s) {
    size_t n = strlen(s);
    if (n && s[n - 1] == '\n')
        s[n - 1] = 0;
}

// ------------------------------------------------------------
// Função auxiliar: limpa_stdin()
// Limpa o buffer do teclado para evitar leituras incorretas
// após um scanf, especialmente em loops.
// ------------------------------------------------------------
static void limpa_stdin(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

// ------------------------------------------------------------
// Função: exibirMapa()
// Mostra o estado atual de todos os territórios cadastrados.
// O parâmetro "const" indica que a função apenas lê os dados.
// ------------------------------------------------------------
static void exibirMapa(const Territorio *m, int n) {
    puts("\nMapa Atual");
    puts("Idx | Territorio          | Cor       | Tropas");
    puts("----+----------------------+-----------+-------");
    for (int i = 0; i < n; i++) {
        printf("%-3d | %-20s | %-9s | %d\n",
               i + 1, m[i].nome, m[i].cor, m[i].tropas);
    }
}

// ------------------------------------------------------------
// Função: simularAtaque()
// Simula uma batalha entre dois territórios.
// O ataque é decidido por rolagem de dados (1 a 6).
// Empates favorecem o atacante.
// ------------------------------------------------------------
static void simularAtaque(Territorio *m, int a, int d) {
    // Sorteia valores entre 1 e 6
    int dadoA = (rand() % 6) + 1;
    int dadoD = (rand() % 6) + 1;

    // Exibe os resultados da rolagem
    printf("Dados -> Atacante: %d  Defensor: %d\n", dadoA, dadoD);

    // Caso o atacante vença (empates contam a favor dele)
    if (dadoA >= dadoD) {
        m[d].tropas -= 1;
        puts("Atacante venceu. Defensor perde 1 tropa.");

        // Se o defensor perdeu todas as tropas, o território é conquistado
        if (m[d].tropas <= 0) {
            puts("Territorio conquistado! Transferindo 1 tropa do atacante.");
            strncpy(m[d].cor, m[a].cor, TAM_COR - 1);  // muda a cor do território
            m[d].tropas = 1;   // ocupa com 1 tropa
            m[a].tropas -= 1;  // atacante cede uma tropa
        }
    } 
    // Caso o defensor vença
    else {
        m[a].tropas -= 1;
        puts("Defensor venceu. Atacante perde 1 tropa.");
    }
}

// ------------------------------------------------------------
// Função principal: main()
// Realiza o cadastro dos territórios e controla a fase de ataque.
// ------------------------------------------------------------
int main(void) {
    // Inicializa o gerador de números aleatórios
    srand((unsigned) time(NULL));

    // Alocação dinâmica de memória para os 5 territórios
    Territorio *mapa = calloc(QTDE_TERRITORIOS, sizeof(Territorio));
    if (!mapa) {
        fprintf(stderr, "Erro: falha ao alocar memoria.\n");
        return 1;
    }

    // --------------------------------------------------------
    // Cadastro dos territórios (Nome, Cor e Tropas)
    // --------------------------------------------------------
    for (int i = 0; i < QTDE_TERRITORIOS; i++) {
        printf("Territorio #%d\n", i + 1);

        // Nome
        printf("Nome do territorio: ");
        if (!fgets(mapa[i].nome, sizeof(mapa[i].nome), stdin)) return 1;
        strip(mapa[i].nome);

        // Cor
        printf("Cor do exercito: ");
        if (!fgets(mapa[i].cor, sizeof(mapa[i].cor), stdin)) return 1;
        strip(mapa[i].cor);

        // Tropas
        printf("Numero de tropas: ");
        while (scanf("%d", &mapa[i].tropas) != 1 || mapa[i].tropas < 0) {
            limpa_stdin();
            printf("Entrada invalida. Digite um inteiro >= 0: ");
        }
        limpa_stdin();
        puts("------------------------------");
    }

    // --------------------------------------------------------
    // Loop principal de batalha
    // O jogador pode realizar ataques ou sair.
    // --------------------------------------------------------
    int opc = 1;
    while (opc != 0) {
        exibirMapa(mapa, QTDE_TERRITORIOS);
        puts("\n1 - Atacar");
        puts("0 - Sair");
        printf("Opcao: ");

        if (scanf("%d", &opc) != 1) {
            limpa_stdin();
            puts("Entrada invalida.");
            continue;
        }
        limpa_stdin();

        // Caso o jogador escolha atacar
        if (opc == 1) {
            int a, d;

            printf("Escolha atacante (1..5): ");
            if (scanf("%d", &a) != 1) { limpa_stdin(); puts("Entrada invalida."); continue; }

            printf("Escolha defensor  (1..5): ");
            if (scanf("%d", &d) != 1) { limpa_stdin(); puts("Entrada invalida."); continue; }

            limpa_stdin();

            // Validação dos índices
            if (a < 1 || a > 5 || d < 1 || d > 5 || a == d) {
                puts("Indices invalidos. Escolha novamente.");
                continue;
            }

            // O atacante precisa ter ao menos 2 tropas
            if (mapa[a - 1].tropas < 2) {
                puts("Atacante precisa ter pelo menos 2 tropas.");
                continue;
            }

            // Chama a função de simulação da batalha
            simularAtaque(mapa, a - 1, d - 1);
        }
    }

    // Liberação da memória alocada
    free(mapa);
    puts("\nMemoria liberada. Programa encerrado.");

    return 0;
}
