#include "processo.h"

/**
 * parse_itens - Analisa uma string de processos separados por vírgulas e converte os valores em inteiros.
 * 
 * @processos: String contendo os processos separados por vírgulas.
 * @items: Ponteiro para um array de inteiros que será alocado dinamicamente e preenchido com os valores convertidos.
 * @item_count: Ponteiro para um inteiro que armazenará a quantidade de itens analisados.
 * 
 * Retorna o número de itens analisados.
 */
int parse_itens(char *processos, int** items, int *item_count) {
    int parsed = 0;

    // Cria uma cópia da string de entrada para evitar modificar o original
    char *processos_copy = malloc(strlen(processos) + 1);
    strcpy(processos_copy, processos);

    // Conta o número de itens divididos por vírgulas
    char *token = strtok(processos_copy, ",");
    while (token != NULL) {
        parsed++;
        token = strtok(NULL, ",");
    }
    *item_count = parsed;
    
    // Aloca memória para o array de inteiros
    *items = malloc((size_t)parsed * sizeof(int));   
    parsed = 0;

    // Reinicia a cópia da string para processar os valores
    strcpy(processos_copy, processos);
    token = strtok(processos_copy, ",");
    while (token != NULL) {
        // Converte a string para inteiro e armazena no array
        (*items)[parsed] = atoi(token);
        parsed++;
        token = strtok(NULL, ",");
    }

    // Libera a memória alocada para a cópia da string
    free(processos_copy);
    return parsed;

}

/**
 * print_processos - Imprime uma tabela formatada com os processos fornecidos.
 * 
 * @processos: Ponteiro para um array de estruturas `Processo` que contém as informações a serem exibidas.
 * @offset: Índice inicial dos processos a serem exibidos.
 * @amount: Quantidade de registros a serem exibidos a partir do índice inicial.
 * 
 * Esta função exibe os processos em formato tabular, com colunas bem definidas para cada campo.
 * Se `amount` for 0, a função retorna sem imprimir nada.
 */
void print_processos(Processo **processos, long unsigned int offset, long unsigned int amount) {

    // Imprime o cabeçalho da tabela
    printf("%-10s | %-21s | %-23s | %-15s | %-15s | %-4s\n", 
        "ID", "Código", "Data", "Classe", "Assunto", "Ano Eleição");
    printf("--------------------------------------------------------------------------------------------------------------\n");

    // Verifica se a quantidade de processos a serem impressos é zero
    // Se for, retorna sem imprimir nada
    if (amount == 0) {
        return;
    }
    
    // Itera sobre os registros e imprime cada um no formato especificado
    for (long unsigned int i = offset; i < (offset + amount); i++) {
        printf("%-10d | %-20s | %-20s | %-15s | %-15s | %-4d\n",
            (*processos)[i].id,               // ID do registro
            (*processos)[i].numero,           // Código associado
            (*processos)[i].data_string,      // Data em formato de string
            (*processos)[i].classe_string,    // Classe em formato de string
            (*processos)[i].assunto_string,   // Assunto em formato de string
            (*processos)[i].ano_eleicao);     // Ano da eleição
    }
}

void export_csv(const char *nome_arquivo, Processo **processos, long unsigned int offset, long unsigned int amount){
    FILE *file = fopen(nome_arquivo, "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return;
    }
    // Escreve o cabeçalho do arquivo CSV
    // fprintf(file, "ID,Código,Data,Classe,Assunto,Ano Eleição\n");
    fprintf(file, "id;numero;data_ajuizamento;id_classe;id_assunto;ano_eleicao\n");

    // Escreve os processos
    for (long unsigned int i = offset; i < (offset + amount); i++) {
        fprintf(file, "%d,\"%s\",%s,\"{%s}\",\"{%s}\",%d\n",
                (*processos)[i].id,
                (*processos)[i].numero,
                (*processos)[i].data_string,
                (*processos)[i].classe_string,
                (*processos)[i].assunto_string,
                (*processos)[i].ano_eleicao);
    }
}


/**
 * parse_line - Analisa uma linha de texto e preenche uma estrutura `Processo` com as informações extraídas.
 * 
 * @linha: String contendo a linha de texto a ser analisada.
 * @processos: Ponteiro para a estrutura `Processo` que será preenchida com os valores extraídos.
 * 
 * A função tenta analisar a linha usando diferentes padrões de formatação (definidos em `PARSE_1`, `PARSE_2`, etc.).
 * Se a análise for bem-sucedida, os campos da estrutura `Processo` são preenchidos, incluindo a conversão de strings
 * para inteiros e a alocação de memória para arrays dinâmicos. A data também é convertida para o formato `struct tm`.
 * 
 * Retorna o número máximo de campos analisados com sucesso.
 */
int parse_line(const char *linha, Processo *processos) {
    // Array de padrões de formatação para tentar analisar a linha
    const char* parse_patters[4] = {
        PARSE_1,
        PARSE_2,
        PARSE_3,
        PARSE_4
    };
    
    int max_parsed = 0;

    // Itera sobre os padrões de formatação para tentar analisar a linha
    for (int i = 0; i < 4; i++) {
        // Validação manual dos padrões de formatação
        // O warning é esperado e pode ser ignorado com segurança
        int parsed = sscanf(
            linha,
            parse_patters[i],
            &processos->id,                // ID do registro
            processos->numero,             // Código associado
            processos->data_string,        // Data em formato de string
            processos->classe_string,      // Classe em formato de string
            processos->assunto_string,     // Assunto em formato de string
            &processos->ano_eleicao        // Ano da eleição
        );

        // Atualiza o número máximo de campos analisados com sucesso
        if (parsed > max_parsed) {
            max_parsed = parsed;
        }

        // Se todos os campos foram analisados com sucesso, processa os processos
        if (parsed == 6) {
            // Converte strings de classe e assunto para arrays de inteiros
            parse_itens(processos->classe_string, &processos->classe, &processos->classe_len);
            parse_itens(processos->assunto_string, &processos->assunto, &processos->assunto_len);

            // Aloca memória para a data e converte a string para `struct tm`
            processos->data = malloc(sizeof(struct tm));
            strptime(processos->data_string, "%Y-%m-%d %H:%M:%S", processos->data);

            // Ajusta os campos da estrutura `tm` para o formato correto
            processos->data->tm_year += 1900; // Ajusta o ano
            processos->data->tm_mon += 1;    // Ajusta o mês

            break; // Interrompe o loop, pois a análise foi bem-sucedida
        }
    }

    return max_parsed;
}

/**
 * read_csv - Lê um arquivo CSV e preenche um array de estruturas `Processo` com as informações extraídas.
 * 
 * @nome_arquivo: Nome do arquivo CSV a ser lido.
 * @processos: Ponteiro para um array de estruturas `Processo` que será alocado dinamicamente e preenchido com os registros do arquivo.
 * 
 * A função lê o arquivo CSV linha por linha, ignora o cabeçalho e analisa cada linha para preencher as estruturas `Processo`.
 * 
 * Retorna o número de registros lidos ou 1 em caso de erro ao abrir o arquivo.
 */
long unsigned int read_csv(const char *nome_arquivo, Processo **processos) {
    // Abre o arquivo para leitura
    FILE *file = fopen(nome_arquivo, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1; // Retorna 1 em caso de erro
    }
    
    long unsigned int count = 0;

    // Variáveis para leitura de linhas
    char *linha;
    size_t len = 0;

    // Conta o número de linhas no arquivo
    while (getline(&linha, &len, file) != -1) {
        count++;
    }
    count -= 1; // Subtrai 1 para ignorar o cabeçalho

    // Aloca memória para o array de estruturas `Processo`
    *processos = malloc(count * sizeof(Processo));

    // Reinicia o ponteiro do arquivo para o início
    rewind(file);

    // Lê e ignora a primeira linha (cabeçalho)
    getline(&linha, &len, file);

    // Lê cada linha do arquivo e preenche o array de estruturas `Processo`
    int i = 0;
    while (getline(&linha, &len, file) != -1) {
        parse_line(linha, &(*processos)[i]); // Analisa a linha e preenche a estrutura
        i++;
    }

    // Libera a memória alocada para a linha e fecha o arquivo
    free(linha);
    fclose(file);

    return count; // Retorna o número de registros lidos
}

/**
 * compara_data - Compara duas estruturas `Processo` com base no campo de data.
 * 
 * @a: Ponteiro para o primeiro elemento a ser comparado.
 * @b: Ponteiro para o segundo elemento a ser comparado.
 * 
 * A função compara as datas dos dois elementos fornecidos. Se uma das datas for `NULL`, 
 * o elemento correspondente será considerado "maior" ou "menor" para fins de ordenação.
 * 
 * Retorna:
 * - Um valor negativo se `a` for menor que `b`.
 * - Zero se `a` for igual a `b`.
 * - Um valor positivo se `a` for maior que `b`.
 */
int compara_data(const Processo *a, const Processo *b) {

    // Verifica se algum dos campos 'data' é NULL
    if (b->data == NULL && a->data == NULL) {
        return 0; // Ambos são NULL, considerados iguais
    }
    if (b->data == NULL) {
        return 1; // Coloca 'b' depois de 'a'
    }
    if (a->data == NULL) {
        return -1; // Coloca 'a' depois de 'b'
    }

    return (int) difftime(mktime(b->data), mktime(a->data));

}

/**
 * compara_id - Compara duas estruturas `Processo` com base no campo de ID.
 * 
 * @a: Ponteiro para o primeiro elemento a ser comparado.
 * @b: Ponteiro para o segundo elemento a ser comparado.
 * 
 * A função compara os IDs dos dois elementos fornecidos.
 * 
 * Retorna:
 * - Um valor negativo se o ID de `a` for menor que o ID de `b`.
 * - Zero se os IDs forem iguais.
 * - Um valor positivo se o ID de `a` for maior que o ID de `b`.
 */
int compara_id(const Processo *a, const Processo *b) {

    // Retorna a diferença entre os IDs
    return a->id - b->id;
}

/**
 * count_id - Conta o número de processos que possuem uma determinada classe.
 * 
 * @processos: Ponteiro para o array de estruturas `Processo` que contém os processos.
 * @processos_size: Tamanho do array de estruturas `Processo`.
 * @id_classe: ID da classe a ser buscada.
 * 
 * A função percorre o array de processos e verifica se a classe especificada está presente
 * em cada registro. Se encontrada, incrementa o contador.
 * 
 * Retorna o número de processos que possuem a classe especificada.
 */
int count_id(Processo *processos, long unsigned int processos_size, int id_classe) {
    int count = 0;

    // Itera sobre os processos no array de processos
    for (long unsigned int i = 0; i < processos_size; i++) {
        // Verifica se a classe especificada está presente no registro atual
        for (int j = 0; j < processos[i].classe_len; j++) {
            if (processos[i].classe[j] == id_classe) {
                count++;
                break; // Evita contar várias vezes o mesmo registro
            }
        }
    }

    return count; // Retorna o número de processos encontrados
}

/**
 * count_assuntos - Conta o número de assuntos únicos presentes nos processos.
 * 
 * @processos: Ponteiro para o array de estruturas `Processo` que contém os processos.
 * @processos_size: Tamanho do array de estruturas `Processo`.
 * 
 * A função percorre o array de processos e identifica os assuntos únicos presentes.
 * Para cada registro, verifica se o assunto já foi contabilizado. Se não, adiciona-o
 * à lista de assuntos únicos e incrementa o contador.
 * 
 * Retorna o número de assuntos únicos encontrados.
 */
long unsigned int count_assuntos(Processo *processos, long unsigned int processos_size) {
    long unsigned int count = 0;
    int *assuntos = malloc(processos_size * sizeof(int)); // Aloca memória suficiente para os assuntos únicos
    int assunto_index = 0;

    // Itera sobre os processos no array de processos
    for (long unsigned int i = 0; i < processos_size; i++) {
        for (int j = 0; j < processos[i].assunto_len; j++) {
            int assunto_atual = processos[i].assunto[j];
            int encontrado = 0;

            // Verifica se o assunto já foi adicionado
            for (int k = 0; k < assunto_index; k++) {
                if (assunto_atual == assuntos[k]) {
                    encontrado = 1;
                    break;
                }
            }

            // Se não foi encontrado, adiciona ao array de assuntos únicos
            if (!encontrado) {
                assuntos[assunto_index] = assunto_atual;
                assunto_index++;
                count++;
            }
        }
    }

    free(assuntos); // Libera a memória alocada
    return count;
}

/**
 * mais_de_um_assunto - Conta o número de processos que possuem mais de um assunto associado.
 * 
 * @processos: Ponteiro para o array de estruturas `Processo` que contém os processos.
 * @processos_size: Tamanho do array de estruturas `Processo`.
 * 
 * A função percorre o array de processos e verifica quantos possuem mais de um assunto associado.
 * 
 * Retorna o número de processos que possuem mais de um assunto.
 */
int mais_de_um_assunto(Processo *processos, long unsigned int processos_size) {
    int count = 0;

    // Itera sobre os processos no array de processos
    for (long unsigned int i = 0; i < processos_size; i++) {
        // Verifica se o registro possui mais de um assunto
        if (processos[i].assunto_len > 1) {
            count++;
        }
    }

    return count; // Retorna o número de processos encontrados
}

/**
 * count_dias - Calcula a diferença em dias entre a data atual e a data de um registro específico.
 * 
 * @processos: Ponteiro para o array de estruturas `Processo` que contém os registros.
 * @processos_size: Tamanho do array de estruturas `Processo`.
 * @id: ID do registro cuja diferença de dias será calculada.
 * 
 * A função encontra o registro com o ID especificado, obtém sua data e calcula a diferença
 * em dias em relação à data atual. Se o ID não for encontrado ou a data for inválida, a função
 * retorna valores específicos para indicar essas condições.
 * 
 * Retorna:
 * - A diferença em dias entre a data atual e a data do registro.
 * - -1 se o ID não for encontrado.
 * - -2 se a data do registro for inválida.
 */
int count_dias(Processo *processos, long unsigned int processos_size, int id) {
    long int index = -1;

    // Encontra o índice do registro com o ID especificado
    for (long unsigned int i = 0; i < processos_size; i++) {
        if (processos[i].id == id) {
            index = (long int)i;
            break;
        }
    }

    // Se o ID não foi encontrado, retorna -1
    if (index == -1) {
        return -1;
    }

    // Obtém a data atual
    time_t now = time(NULL);

    // Obtém a data do registro
    struct tm *data_processos = processos[index].data;
    if (data_processos == NULL) {
        return -2; // Retorna -2 se a data for inválida
    }

    // Ajusta os campos da estrutura `tm` para o formato correto
    data_processos->tm_year -= 1900; // O ano é contado a partir de 1900
    data_processos->tm_mon -= 1;     // O mês é contado a partir de 0

    // Converte a data do registro para `time_t`
    time_t data_time = mktime(data_processos);

    // Calcula a diferença em segundos e converte para dias
    double diff_seconds = difftime(now, data_time);
    int diff_days = (int)(diff_seconds / (60 * 60 * 24));

    return diff_days; // Retorna a diferença em dias
}

/**
 * quicksort - Implementa o algoritmo de ordenação QuickSort.
 * 
 * @vetor: Ponteiro para o vetor de estruturas `Processo` que será ordenado.
 * @inf: Índice inferior do subvetor a ser ordenado.
 * @sup: Índice superior do subvetor a ser ordenado.
 * @compara: Ponteiro para a função de comparação que define a ordem dos elementos.
 *           A função deve retornar:
 *           - Valor negativo se o primeiro elemento for menor que o segundo.
 *           - Zero se os elementos forem iguais.
 *           - Valor positivo se o primeiro elemento for maior que o segundo.
 * 
 * Esta função ordena o vetor de processos utilizando o algoritmo QuickSort de forma recursiva.
 * 
 * Retorna: Nada. O vetor é ordenado diretamente na memória.
 */
void quicksort(Processo *vetor, int inf, int sup, int (*compara)(const Processo *, const Processo *)) {
    if (inf < sup) {
        // Encontra o pivô
        int p = partition(vetor, inf, sup, compara);
        // Ordena os elementos antes e depois da partição
        quicksort(vetor, inf, p - 1, compara);
        // quicksort(vetor, p + 1, sup, compara);
        quicksort(vetor, p , sup, compara);
    }
}

/**
 * partition - Particiona o vetor para o algoritmo QuickSort.
 * 
 * @vetor: Ponteiro para o vetor de estruturas `Processo` que está sendo ordenado.
 * @inf: Índice inferior do subvetor a ser particionado.
 * @sup: Índice superior do subvetor a ser particionado (o pivô está localizado neste índice).
 * @compara: Ponteiro para a função de comparação que define a ordem dos elementos.
 *           A função deve retornar:
 *           - Valor negativo se o primeiro elemento for menor que o segundo.
 *           - Zero se os elementos forem iguais.
 *           - Valor positivo se o primeiro elemento for maior que o segundo.
 * 
 * Esta função reorganiza os elementos do vetor de forma que todos os elementos menores ou iguais ao pivô
 * fiquem à esquerda e todos os elementos maiores fiquem à direita. O pivô é escolhido como o elemento
 * central do subvetor. Essa função foi adaptada da versão nos slides.
 * 
 * Retorna: O índice onde a partição foi concluída, que será usado para dividir o vetor no QuickSort.
 */
int partition(Processo *vetor, int inf, int sup, int (*compara)(const Processo *, const Processo *)) {

    Processo pivot = vetor[(inf+sup)/2];
    int i = inf; // indice para percorrer subvetor da esquerda
    int j = sup; // indice para percorrer subvetor da direita
    while(i<=j) {
        while(compara(&vetor[i], &pivot) < 0) i++;
        while(compara(&vetor[j], &pivot) > 0) j--;
        if( i<= j) {
            // realiza swap
            Processo temp = vetor[i];
            vetor[i] = vetor[j];
            vetor[j] = temp;
            i++;
            j--;
        }
    }
    return i; // retorna a posição do pivô

}
