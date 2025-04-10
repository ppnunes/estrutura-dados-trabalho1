#ifndef PROCESSO_H
#define PROCESSO_H

#define _GNU_SOURCE // para habilitar strptime (https://stackoverflow.com/questions/5378778/what-does-d-xopen-source-do-mean)
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

// Definições de padrões de formatação para análise de linhas CSV
// PARSE_1: Formato básico com campos separados por vírgulas e chaves
// PARSE_2: Formato com aspas ao redor do campo classe (multiplas classes)
// PARSE_3: Formato com aspas ao redor do campo assunto (multiplos assuntos)
// PARSE_4: Formato com aspas ao redor de múltiplos campos (classe e assunto)
#define PARSE_1 "%d,\"%[^\"]\",%[^,],{%[^}]},{%[^}]},%d" // {},{}
#define PARSE_2 "%d,\"%[^\"]\",%[^,],{%[^}]},%*1[\"]{%[^}]}%*1[\"],%d" // "{}",{}
#define PARSE_3 "%d,\"%[^\"]\",%[^,],%*1[\"]{%[^}]}%*1[\"],{%[^}]},%d" // {},"{}"
#define PARSE_4 "%d,\"%[^\"]\",%[^,],%*1[\"]{%[^}]}%*1[\"],%*1[\"]{%[^}]}%*1[\"],%d" // "{}","{}"

// Estrutura que representa os processos de um registro no CSV
typedef struct {
    int id;                     // ID do registro
    char numero[30];            // Código associado ao registro
    struct tm* data;            // Data do registro (formato estruturado)
    char data_string[30];       // Data do registro (formato string)
    char classe_string[30];     // Classe do registro (formato string)
    int* classe;                // Array de IDs de classe
    int classe_len;             // Número de classes associadas
    char assunto_string[30];    // Assunto do registro (formato string)
    int* assunto;               // Array de IDs de assunto
    int assunto_len;            // Número de assuntos associados
    int ano_eleicao;            // Ano da eleição associado ao registro
} Processo;

int parse_itens(char *processos, int** items, int *item_count);
void print_processos(Processo **processos, long unsigned int offset, long unsigned int amount);
void export_csv(const char *nome_arquivo, Processo **processos, long unsigned int offset, long unsigned int amount);
int parse_line(const char *linha, Processo *processos);
long unsigned int read_csv(const char *nome_arquivo, Processo **processos);
int compara_data(const void *a, const void *b);
int compara_id(const void *a, const void *b);
int count_id(Processo *processos, long unsigned int processos_size, int id_classe);
long unsigned int count_assuntos(Processo *processos, long unsigned int processos_size);
int mais_de_um_assunto(Processo *processos, long unsigned int processos_size);
int count_dias(Processo *processos, long unsigned int processos_size, int id);

#endif
