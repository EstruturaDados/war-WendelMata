/* Nível Novato: cadastro e exibição de 5 territórios */
#include <stdio.h>
#include <string.h>

#define QTDE_TERRITORIOS 5
#define TAM_NOME 30
#define TAM_COR  10

typedef struct {
    char nome[TAM_NOME];
    char cor[TAM_COR];
    int  tropas;
} Territorio;

void strip(char *s){ size_t n=strlen(s); if(n && s[n-1]=='\n') s[n-1]=0; }

int main(void){
    Territorio mapa[QTDE_TERRITORIOS];
    printf("Cadastro de %d territorios\n", QTDE_TERRITORIOS);
    printf("-------------------------------------\n");

    for(int i=0;i<QTDE_TERRITORIOS;i++){
        printf("Territorio #%d\n", i+1);

        printf("Nome do territorio: ");
        if(!fgets(mapa[i].nome, sizeof(mapa[i].nome), stdin)) return 1;
        strip(mapa[i].nome);

        printf("Cor do exercito: ");
        if(!fgets(mapa[i].cor, sizeof(mapa[i].cor), stdin)) return 1;
        strip(mapa[i].cor);

        printf("Numero de tropas: ");
        while(scanf("%d", &mapa[i].tropas)!=1 || mapa[i].tropas<0){
            int c; while((c=getchar())!='\n' && c!=EOF){}
            printf("Entrada invalida. Digite um inteiro >= 0: ");
        }
        int c; while((c=getchar())!='\n' && c!=EOF){}
        printf("-------------------------------------\n");
    }

    printf("\n=== Mapa Atual ===\n");
    for(int i=0;i<QTDE_TERRITORIOS;i++){
        printf("Territorio %d:\n", i+1);
        printf("  Nome  : %s\n", mapa[i].nome);
        printf("  Cor   : %s\n", mapa[i].cor);
        printf("  Tropas: %d\n", mapa[i].tropas);
        printf("-------------------\n");
    }
    return 0;
}
