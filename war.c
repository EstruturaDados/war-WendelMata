/*
 * WAR ESTRUTURADO — 3 NÍVEIS EM UM ÚNICO ARQUIVO (war.c)
 * Menu escolhe: 1) Novato  2) Aventureiro  3) Mestre
 * Comentários focam em estrutura e fluxos.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

/* ---------- Configuração comum ---------- */
#define QTDE_TERRITORIOS 5
#define TAM_NOME 30
#define TAM_COR  10

typedef struct {
    char nome[TAM_NOME];
    char cor[TAM_COR];
    int  tropas;
} Territorio;

/* Utilidades comuns */
static void strip(char *s){ size_t n=strlen(s); if(n && s[n-1]=='\n') s[n-1]=0; }
static void limpa_stdin(void){ int c; while((c=getchar())!='\n' && c!=EOF){} }
static void pausa(void){ puts("\n<Enter> para continuar..."); limpa_stdin(); }

/* ---------- Exibição comum ---------- */
static void exibirMapa(const Territorio *m, int n){
    puts("\nMapa Atual");
    puts("Idx | Territorio          | Cor       | Tropas");
    puts("----+----------------------+-----------+-------");
    for(int i=0;i<n;i++)
        printf("%-3d | %-20s | %-9s | %d\n", i+1, m[i].nome, m[i].cor, m[i].tropas);
}

/* =========================================================
 * NÍVEL NOVATO
 * Vetor estático, cadastro e exibição.
 * ========================================================= */
static void nivel_novato(void){
    Territorio mapa[QTDE_TERRITORIOS];

    puts("\n=== Nivel Novato ===");
    for(int i=0;i<QTDE_TERRITORIOS;i++){
        printf("Territorio #%d\n", i+1);

        printf("Nome do territorio: ");
        if(!fgets(mapa[i].nome, sizeof(mapa[i].nome), stdin)) return;
        strip(mapa[i].nome);

        printf("Cor do exercito: ");
        if(!fgets(mapa[i].cor, sizeof(mapa[i].cor), stdin)) return;
        strip(mapa[i].cor);

        printf("Numero de tropas: ");
        while(scanf("%d",&mapa[i].tropas)!=1 || mapa[i].tropas<0){
            limpa_stdin(); printf("Inteiro >=0: ");
        }
        limpa_stdin();
        puts("------------------------------");
    }

    exibirMapa(mapa, QTDE_TERRITORIOS);
    pausa();
}

/* =========================================================
 * NÍVEL AVENTUREIRO
 * calloc + batalha simples 1x1 (empate favorece atacante).
 * ========================================================= */
static void simularAtaque_av(Territorio *m, int a, int d){
    int dadoA=(rand()%6)+1, dadoD=(rand()%6)+1;
    printf("Dados -> Atacante:%d  Defensor:%d\n", dadoA, dadoD);

    if(dadoA >= dadoD){
        m[d].tropas -= 1;
        puts("Atacante venceu. Defensor perde 1 tropa.");
        if(m[d].tropas <= 0){
            puts("Territorio conquistado. Transferindo 1 tropa do atacante.");
            strncpy(m[d].cor, m[a].cor, TAM_COR-1);
            m[d].tropas = 1;
            m[a].tropas -= 1;
        }
    }else{
        m[a].tropas -= 1;
        puts("Defensor venceu. Atacante perde 1 tropa.");
    }
}

static void nivel_aventureiro(void){
    Territorio *mapa = calloc(QTDE_TERRITORIOS, sizeof(Territorio));
    if(!mapa){ puts("Falha ao alocar memoria."); return; }

    puts("\n=== Nivel Aventureiro ===");
    for(int i=0;i<QTDE_TERRITORIOS;i++){
        printf("Territorio #%d\n", i+1);

        printf("Nome do territorio: ");
        if(!fgets(mapa[i].nome, sizeof(mapa[i].nome), stdin)){ free(mapa); return; }
        strip(mapa[i].nome);

        printf("Cor do exercito: ");
        if(!fgets(mapa[i].cor, sizeof(mapa[i].cor), stdin)){ free(mapa); return; }
        strip(mapa[i].cor);

        printf("Numero de tropas: ");
        while(scanf("%d",&mapa[i].tropas)!=1 || mapa[i].tropas<0){
            limpa_stdin(); printf("Inteiro >=0: ");
        }
        limpa_stdin();
        puts("------------------------------");
    }

    int opc=1;
    while(opc!=0){
        exibirMapa(mapa, QTDE_TERRITORIOS);
        puts("\n1 - Atacar");
        puts("0 - Sair");
        printf("Opcao: ");
        if(scanf("%d",&opc)!=1){ limpa_stdin(); continue; }
        limpa_stdin();
        if(opc==1){
            int a,d;
            printf("Atacante (1..5): "); if(scanf("%d",&a)!=1){ limpa_stdin(); continue; }
            printf("Defensor  (1..5): "); if(scanf("%d",&d)!=1){ limpa_stdin(); continue; }
            limpa_stdin();
            if(a<1||a>5||d<1||d>5||a==d){ puts("Indices invalidos."); continue; }
            if(mapa[a-1].tropas<2){ puts("Atacante precisa ter >=2 tropas."); continue; }
            simularAtaque_av(mapa, a-1, d-1);
        }
    }
    free(mapa);
}

/* =========================================================
 * NÍVEL MESTRE
 * Modularização + missão aleatória + verificação de vitória.
 * Regras iniciais fixas para demonstrar o fluxo.
 * ========================================================= */
enum { MISSAO_DESTRUIR_VERDE=0, MISSAO_CONQUISTAR_3=1 };

static Territorio* alocarMapa(size_t n){ return calloc(n, sizeof(Territorio)); }

static void inicializarTerritorios(Territorio *mapa, size_t n){
    const char *nomes[QTDE_TERRITORIOS] = {"Alfa","Bravo","Charlie","Delta","Echo"};
    const char *cores[QTDE_TERRITORIOS] = {"Verde","Vermelho","Azul","Amarelo","Verde"};
    const int tropas[QTDE_TERRITORIOS]  = {3,3,3,3,3};
    for(size_t i=0;i<n;i++){
        strncpy(mapa[i].nome, nomes[i], TAM_NOME-1);
        strncpy(mapa[i].cor,  cores[i], TAM_COR-1);
        mapa[i].tropas = tropas[i];
    }
}

static void exibirMenuPrincipal(void){
    puts("\nMenu Principal:");
    puts("1 - Atacar");
    puts("2 - Verificar missao");
    puts("0 - Sair");
}

static void simularAtaque_mestre(Territorio *m, int ia, int id, const char *corJogador){
    if(strcmp(m[ia].cor, corJogador)!=0){ puts("Ataque apenas de territorio seu."); return; }
    if(strcmp(m[id].cor, corJogador)==0){ puts("Nao ataque territorio seu."); return; }
    if(m[ia].tropas<2){ puts("Atacante precisa >=2 tropas."); return; }
    if(m[id].tropas<=0){ puts("Defensor vazio."); return; }

    int dadoA=(rand()%6)+1, dadoD=(rand()%6)+1;
    printf("Dados -> Atacante: %d | Defensor: %d\n", dadoA, dadoD);

    if(dadoA >= dadoD){
        m[id].tropas -= 1;
        puts("Atacante venceu a rodada.");
        if(m[id].tropas<=0){
            puts("Territorio conquistado.");
            strncpy(m[id].cor, corJogador, TAM_COR-1);
            m[id].tropas = 1;
            m[ia].tropas -= 1;
        }
    }else{
        m[ia].tropas -= 1;
        puts("Defensor resistiu.");
    }
}

static int sortearMissao(void){
    int pool[2]={MISSAO_DESTRUIR_VERDE, MISSAO_CONQUISTAR_3};
    return pool[rand()%2];
}

static void exibirMissao(int missao){
    puts("\nMissao do Jogador:");
    if(missao==MISSAO_DESTRUIR_VERDE) puts("- Destruir todos os territorios de cor Verde.");
    else if(missao==MISSAO_CONQUISTAR_3) puts("- Conquistar 3 territorios.");
}

static int verificarVitoria(const Territorio *m, size_t n, int missao, const char *corJogador){
    if(missao==MISSAO_DESTRUIR_VERDE){
        for(size_t i=0;i<n;i++)
            if(strcmp(m[i].cor,"Verde")==0 && m[i].tropas>0) return 0;
        return 1;
    }else if(missao==MISSAO_CONQUISTAR_3){
        int cnt=0;
        for(size_t i=0;i<n;i++)
            if(strcmp(m[i].cor, corJogador)==0 && m[i].tropas>0) cnt++;
        return cnt>=3;
    }
    return 0;
}

static void faseDeAtaque(Territorio *m, size_t n, const char *corJogador){
    int a,d;
    printf("Atacante (1..%zu): ", n); if(scanf("%d",&a)!=1){ limpa_stdin(); return; }
    printf("Defensor  (1..%zu): ", n); if(scanf("%d",&d)!=1){ limpa_stdin(); return; }
    limpa_stdin();
    if(a<1||a>(int)n||d<1||d>(int)n||a==d){ puts("Indices invalidos."); return; }
    simularAtaque_mestre(m, a-1, d-1, corJogador);
}

static void nivel_mestre(void){
    Territorio *mapa = alocarMapa(QTDE_TERRITORIOS);
    if(!mapa){ puts("Falha ao alocar memoria."); return; }
    inicializarTerritorios(mapa, QTDE_TERRITORIOS);

    const char corJogador[]="Azul";
    int missao=sortearMissao(), venceu=0, opc=0;

    do{
        puts("\n================== WAR ESTRUTURADO ==================");
        exibirMapa(mapa, QTDE_TERRITORIOS);
        printf("Sua cor: %s\n", corJogador);
        exibirMissao(missao);
        exibirMenuPrincipal();

        printf("Opcao: ");
        if(scanf("%d",&opc)!=1){ limpa_stdin(); continue; }
        limpa_stdin();

        if(opc==1){
            faseDeAtaque(mapa, QTDE_TERRITORIOS, corJogador);
            venceu = verificarVitoria(mapa, QTDE_TERRITORIOS, missao, corJogador);
            if(venceu) puts("\n>>> Missao concluida! Voce venceu!");
        }else if(opc==2){
            puts(verificarVitoria(mapa, QTDE_TERRITORIOS, missao, corJogador)
                 ? "Missao cumprida." : "Missao ainda nao cumprida.");
        }
    }while(opc!=0 && !venceu);

    free(mapa);
}

/* =========================================================
 * MENU PRINCIPAL DOS 3 NÍVEIS
 * ========================================================= */
int main(void){
    setlocale(LC_ALL, "");
    srand((unsigned)time(NULL));

    for(;;){
        puts("\n================= MENU GERAL =================");
        puts("1 - Nivel Novato (cadastro e exibicao)");
        puts("2 - Nivel Aventureiro (calloc + batalha)");
        puts("3 - Nivel Mestre (missoes e vitoria)");
        puts("0 - Sair");
        printf("Opcao: ");

        int op;
        if(scanf("%d",&op)!=1){ limpa_stdin(); continue; }
        limpa_stdin();

        if(op==0) break;
        if(op==1) nivel_novato();
        else if(op==2) nivel_aventureiro();
        else if(op==3) nivel_mestre();
        else puts("Opcao invalida.");
    }
    return 0;
}
