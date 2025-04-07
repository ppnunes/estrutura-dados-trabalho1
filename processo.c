#include "processo.h"

Processo* abreArquivo (const char* NomeArquivo, int *N) {
    FILE *fp = fopen(NomeArquivo, "r");
    if(fp == NULL) {
        printf("ERRO: O arquivo não pode ser aberto!\n");
        exit(1);
    }

    Processo temp;

    char Cabecalho[1000];
    fscanf(fp, "%s", Cabecalho);
    *N = 0;

    while (fscanf(fp, "%s", Cabecalho) != EOF)
    {
        (*N)++;
    }

    Processo *X = malloc((*N)* sizeof(Processo));
    if(X == NULL) {
        printf("ERRO: Não foi possível alocar memória para a tabela!\n");
        exit(1);
    }
    
    // reinicia o arquivo
    fclose(fp);
    fp = fopen(NomeArquivo, "r");
    if(fp == NULL) {
        printf("ERRO: O arquivo não pode ser aberto!\n");
        exit(1);
    }

    fscanf(fp, "%[^\n]\n", Cabecalho);
    const char* parses[4] = {
        PARSE_1,
        PARSE_2,
        PARSE_3,
        PARSE_4
    };

    for (int i = 0; i < (*N); i++)
    {
        char linha[3000];
        fscanf(fp, "%[^\n]\n", linha);
        
        for (int j = 0; j < 4; j++)
        {
            int parsed = sscanf(
                linha,
                parses[j],
                &X[i].id,
                X[i].numero,
                X[i].data_string,
                X[i].classe_string,
                X[i].assunto_string,
                &X[i].ano_eleicao
            );
            if (parsed == 6) {
                //DEU CERTO
                break;
            }
        }
        

    }

    return X;
};

void ordenaPorId () {
    printf("Conteudo ordenado por Id");
}

void imprimeDados (int start, int end) {
    printf("Dados impressos");
}