/*
 * ===============================================================
 * NÍVEL MESTRE - DESAFIO WAR ESTRUTURADO
 * ===============================================================
 * Objetivo:
 *  - Modularizar completamente o código em funções
 *  - Criar sistema de missões (aleatórias)
 *  - Verificar vitória conforme a missão sorteada
 *  - Utilizar passagem por referência e const correctness
 *
 * Conceitos:
 *  - Funções
 *  - const
 *  - srand/rand
 *  - ponteiros
 *  - organização modular
 * ===============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

/* --------------------------------------------------------------- */
/* Constantes globais */
#define MAX_TERRITORIOS 5
#define TAM_NOME 30
#define TAM_COR 10

#define MISSAO_DESTRUIR_VERDE 0
#define MISSAO_CONQUISTAR_3   1

/* --------------------------------------------------------------- */
/* Estrutura que representa cada território */
typedef struct {
    char nome[TAM_NOME];
    char cor[TAM_COR];
    int tropas;
} Territorio;

/* --------------------------------------------------------------- */
/* Protótipos de funções */
Territorio* alocarMapa(size_t n);
void inicializarTerritorios(Territorio *mapa, size_t n);
void liberarMemoria(Territorio **mapa);
void exibirMapa(const Territorio *mapa, size_t n);
void exibirMenuPrincipal(void);
void exibirMissao(int missao);
void faseDeAtaque(Territorio *mapa, size_t n, const char *corJogador);
void simularAtaque(Territorio *mapa, int ia, int id, const char *corJogador);
int  sortearMissao(void);
int  verificarVitoria(const Territorio *mapa, size_t n, int missao, const char *corJogador);
void limparBufferEntrada(void);

/* --------------------------------------------------------------- */
/* Função principal */
int main(void) {
    setlocale(LC_ALL, "");
    srand((unsigned) time(NULL));

    Territorio *mapa = alocarMapa(MAX_TERRITORIOS);
    if (!mapa) {
        fprintf(stderr, "Erro: falha ao alocar memória.\n");
        return 1;
    }

    inicializarTerritorios(mapa, MAX_TERRITORIOS);
    const char corJogador[] = "Azul";
    int missao = sortearMissao();
    int opcao, venceu = 0;

    do {
        puts("\n================== WAR ESTRUTURADO ==================");
        exibirMapa(mapa, MAX_TERRITORIOS);
        printf("Sua cor: %s\n", corJogador);
        exibirMissao(missao);
        exibirMenuPrincipal();

        printf("Opção: ");
        if (scanf("%d", &opcao) != 1) {
            limparBufferEntrada();
            puts("Entrada inválida.");
            continue;
        }
        limparBufferEntrada();

        switch (opcao) {
            case 1:
                faseDeAtaque(mapa, MAX_TERRITORIOS, corJogador);
                venceu = verificarVitoria(mapa, MAX_TERRITORIOS, missao, corJogador);
                if (venceu) puts("\n>>> Missão concluída! Você venceu!");
                break;

            case 2:
                if (verificarVitoria(mapa, MAX_TERRITORIOS, missao, corJogador))
                    puts("Missão cumprida!");
                else
                    puts("Missão ainda não concluída.");
                break;

            case 0:
                puts("Encerrando jogo...");
                break;

            default:
                puts("Opção inválida.");
        }

    } while (opcao != 0 && !venceu);

    liberarMemoria(&mapa);
    return 0;
}

/* --------------------------------------------------------------- */
/* Implementações das funções */

Territorio* alocarMapa(size_t n) {
    return (Territorio*) calloc(n, sizeof(Territorio));
}

void inicializarTerritorios(Territorio *mapa, size_t n) {
    const char *nomes[MAX_TERRITORIOS] = {"Alfa","Bravo","Charlie","Delta","Echo"};
    const char *cores[MAX_TERRITORIOS] = {"Verde","Vermelho","Azul","Amarelo","Verde"};
    const int tropasIniciais[MAX_TERRITORIOS] = {3,3,3,3,3};
    for (size_t i = 0; i < n; i++) {
        strncpy(mapa[i].nome, nomes[i], TAM_NOME - 1);
        strncpy(mapa[i].cor, cores[i], TAM_COR - 1);
        mapa[i].tropas = tropasIniciais[i];
    }
}

void liberarMemoria(Territorio **mapa) {
    if (mapa && *mapa) {
        free(*mapa);
        *mapa = NULL;
    }
}

void exibirMenuPrincipal(void) {
    puts("\nMenu Principal:");
    puts("1 - Atacar");
    puts("2 - Verificar missão");
    puts("0 - Sair");
}

void exibirMapa(const Territorio *mapa, size_t n) {
    puts("\nMapa Atual");
    puts("Idx | Território          | Cor       | Tropas");
    puts("----+----------------------+-----------+-------");
    for (size_t i = 0; i < n; i++)
        printf("%-3zu | %-20s | %-9s | %d\n", i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
}

void exibirMissao(int missao) {
    puts("\nMissão do Jogador:");
    if (missao == MISSAO_DESTRUIR_VERDE)
        puts("- Destruir todos os territórios de cor Verde.");
    else if (missao == MISSAO_CONQUISTAR_3)
        puts("- Conquistar 3 territórios.");
    else
        puts("- Missão desconhecida.");
}

void faseDeAtaque(Territorio *mapa, size_t n, const char *corJogador) {
    int a, d;
    printf("Escolha atacante (1..%zu): ", n);
    if (scanf("%d", &a) != 1) { limparBufferEntrada(); return; }
    printf("Escolha defensor (1..%zu): ", n);
    if (scanf("%d", &d) != 1) { limparBufferEntrada(); return; }
    limparBufferEntrada();

    if (a < 1 || a > (int)n || d < 1 || d > (int)n || a == d) {
        puts("Índices inválidos.");
        return;
    }
    if (mapa[a-1].tropas < 2) {
        puts("Atacante precisa de pelo menos 2 tropas.");
        return;
    }
    if (strcmp(mapa[a-1].cor, corJogador) != 0) {
        puts("Você só pode atacar a partir de um território seu.");
        return;
    }

    simularAtaque(mapa, a-1, d-1, corJogador);
}

void simularAtaque(Territorio *mapa, int ia, int id, const char *corJogador) {
    int dadoA = (rand() % 6) + 1;
    int dadoD = (rand() % 6) + 1;
    printf("Dados -> Atacante: %d | Defensor: %d\n", dadoA, dadoD);

    if (dadoA >= dadoD) {
        mapa[id].tropas -= 1;
        puts("Atacante venceu a rodada!");
        if (mapa[id].tropas <= 0) {
            puts("Território conquistado!");
            strncpy(mapa[id].cor, corJogador, TAM_COR - 1);
            mapa[id].tropas = 1;
            mapa[ia].tropas -= 1;
        }
    } else {
        mapa[ia].tropas -= 1;
        puts("Defensor resistiu ao ataque.");
    }
}

int sortearMissao(void) {
    int missoes[2] = {MISSAO_DESTRUIR_VERDE, MISSAO_CONQUISTAR_3};
    return missoes[rand() % 2];
}

int verificarVitoria(const Territorio *mapa, size_t n, int missao, const char *corJogador) {
    if (missao == MISSAO_DESTRUIR_VERDE) {
        for (size_t i = 0; i < n; i++)
            if (strcmp(mapa[i].cor, "Verde") == 0 && mapa[i].tropas > 0)
                return 0;
        return 1;
    } else if (missao == MISSAO_CONQUISTAR_3) {
        int cont = 0;
        for (size_t i = 0; i < n; i++)
            if (strcmp(mapa[i].cor, corJogador) == 0 && mapa[i].tropas > 0)
                cont++;
        return cont >= 3;
    }
    return 0;
}

void limparBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}
