#ifndef PROCESSO_H
#define PROCESSO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define PARSE_1 "%d,\"%[^\"]\",%[^,],{%[^}]},{%[^}]},%d" // {},{}
#define PARSE_2 "%d,\"%[^\"]\",%[^,],{%[^}]},%*1[\"]{%[^}]}%*1[\"],%d" // "{}",{}
#define PARSE_3 "%d,\"%[^\"]\",%[^,],%*1[\"]{%[^}]}%*1[\"],{%[^}]},%d" // {},"{}"
#define PARSE_4 "%d,\"%[^\"]\",%[^,],%*1[\"]{%[^}]}%*1[\"],%*1[\"]{%[^}]}%*1[\"],%d" // "{}","{}"

// Definições de padrões de formatação para análise de linhas CSV
// PARSE_1: Formato básico com campos separados por vírgulas e chaves
// PARSE_2: Formato com aspas ao redor do campo classe (multiplas classes)
// PARSE_3: Formato com aspas ao redor do campo assunto (multiplos assuntos)
// PARSE_4: Formato com aspas ao redor de múltiplos campos (classe e assunto)

typedef struct {
    int id;
    char numero[20];
    struct tm* data;
    char data_string[30];
    int* id_classe;
    char classe_string[30];
    int id_classe_len;
    int* id_assunto;
    char assunto_string[30];
    int id_assunto_len;
    int ano_eleicao;
} Processo;

Processo* abreArquivo (const char* NomeArquivo, int *N);

void ordenaPorId ();

void imprimeDados ();


#endif