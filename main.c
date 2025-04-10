#include <stdio.h>
#include <stdlib.h>

#include "processo.h"

int main()
{
    Processo *processos;
    const long unsigned int MAX_DADOS_PRINT = 5;
    int id_classe = 11528;
    const int id_processo = 680402167;

    long unsigned int qnt_processos = read_csv("processo_043_202409032338.csv", &processos);
    printf("Número de processos lidos: %lu\n", qnt_processos);
    print_processos(&processos, 0, MAX_DADOS_PRINT); // Imprime os 2 primeiros processos

    printf("\n1. Ordenar, em ordem crescente, o conjunto de processos a partir do atributo “id”;\n");
    quicksort(processos, 0, qnt_processos - 1, compara_id);
    export_csv("processo_043_202409032338_ordenado_id.csv", &processos, 0, qnt_processos);
    print_processos(&processos, 0, MAX_DADOS_PRINT);

    printf("\n2. Ordenar, em ordem decrescente, o conjunto de processos a partir do atributo “data_ajuizamento”;\n");
    quicksort(processos, 0, qnt_processos - 1, compara_data);
    export_csv("processo_043_202409032338_ordenado_data.csv", &processos, 0, qnt_processos);
    print_processos(&processos, 0, MAX_DADOS_PRINT);

    printf("\n3. Contar quantos processos estão vinculados a um determinado “id_classe”;\n");
    printf("Quantidade de processos com id_classe %d: %d\n", id_classe, count_id(processos, qnt_processos, id_classe));

    printf("\n4. Identificar quantos “id_assuntos” constam nos processos presentes na base de processos;\n");
    printf("Quantidade de id_assuntos: %ld\n", count_assuntos(processos, qnt_processos));

    printf("\n5. Listar todos os processos que estão vinculados a mais de um assunto;\n");
    printf("Processos com mais de um assunto: %d\n", mais_de_um_assunto(processos, qnt_processos));

    printf("\n6. Indicar a quantos dias um processo está em tramitação na justiça;\n");
    printf("O processo %d em tramitação na justiça a %d dias\n",id_processo, count_dias(processos, qnt_processos, id_processo));


    // Libera memória alocada
    for (long unsigned int i = 0; i < qnt_processos; i++) {
        free(processos[i].data);
        free(processos[i].classe);
        free(processos[i].assunto);
    }
    free(processos);
    return 0;
}
