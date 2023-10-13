#ifndef LISTASE_H
#define LISTASE_H

#include <stdio.h>
#include <stdlib.h>


typedef struct NO{
    int value; 
    char color; 
    struct NO* prox;    
}NO;

typedef struct NO* Lista;


Lista* criaLista();
int listaVazia(Lista *li);
NO* alocarNO();
void liberarNO(NO* q);
int insereIni(Lista* li, Card *c);
int insereFim(Lista* li, Card *c);
int removeIni(Lista* li);
int removeFim(Lista* li);
void imprimeLista(Lista* li);
void destroiLista(Lista* li);
int tamanhoLi(Lista *li);
int procuraLi(Lista *li, Card *c);
void insereOrdenado(Lista* lista, int valor);
void removerElemento(Lista* lista, int valor);

#endif