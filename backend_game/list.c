#include "lista.h"


Lista* criaLista(){
    Lista *li;
    li = (Lista*) malloc (sizeof(Lista));
    if(li != NULL){
        *li = NULL;
    }
    return li;
}

int listaVazia(Lista *li){
    if(li == NULL) return 1;
    if(*li == NULL) return 1;//True - Vazia!
    return 0;//False - tem elemento!
}

NO* alocarNO(){
    return (NO*) malloc (sizeof(NO));
}

void liberarNO(NO* q){
    free(q);
}

int insereIni(Lista* li, Card *c){
    if(li == NULL) return 0;
    NO* novo = alocarNO();
    if(novo == NULL) return 0;

    novo->c = c;
    novo->prox = *li;
    *li = novo;
    return 1;
}

int insereFim(Lista* li, int elem){
    if(li == NULL) return 0;
    NO* novo = alocarNO();
    if(novo == NULL) return 0;
    novo->info = elem;
    novo->prox = NULL;
    if(listaVazia(li)){
        *li = novo;
    }else{
        NO* aux = *li;
        while(aux->prox != NULL)
            aux = aux->prox;
        aux->prox = novo;
    }
    return 1;
}

int removeIni(Lista* li){
    if(li == NULL) return 0;
    if(listaVazia(li)) return 0;
    NO* aux = *li;
    *li = aux->prox;
    liberarNO(aux);
    return 1;
}

int removeFim(Lista* li){
    if(li == NULL) return 0;
    if(listaVazia(li)) return 0;
    NO* ant, *aux = *li;
    while(aux->prox != NULL){
        ant = aux;
        aux = aux->prox;
    }
    if(aux == *li)
        *li = aux->prox;
    else   
        ant->prox = aux->prox;
    liberarNO(aux);
    return 1;
}

void imprimeLista(Lista* li){
    if(li == NULL) return;
    if(listaVazia(li)){
        printf("Lista Vazia!\n");
        return;
    }
    printf("Elementos:\n");
    NO* aux = *li;
    while(aux != NULL){
        printf("%d ", aux->info);
        aux = aux->prox;
    }
    printf("\n");
}

void destroiLista(Lista* li){
    if(li != NULL){
        NO* aux;
        while((*li) != NULL){
            aux = *li;
            *li = (*li)->prox;
            liberarNO(aux);
        }
        free(li);
    }
}

int tamanhoLi(Lista *li){
    struct NO *corrente = (*li)->prox;
    if(corrente == NULL) return 0;
    
    int tam = 1;
    while(corrente != NULL){
        tam++;
        corrente = corrente->prox;
    }
    return tam;
}

int procuraLi(Lista *li, int x){
    struct NO *corrente = (*li)->prox;
    if(corrente == NULL) return -1;

    int i = 1;
    while(corrente != NULL){
        if(corrente->info == x) return i;
        corrente = corrente->prox;
        i++;
    }
    return -1;
}


void insereOrdenado(Lista* lista, int valor){
    NO* novo = alocarNO();

    novo->info = valor;
    novo->prox = NULL;

    NO* atual = *lista;
    NO* anterior = NULL;

    while(atual != NULL && atual->info < valor){
        anterior = atual;
        atual = atual->prox;
    }

    if(anterior == NULL){
        novo->prox = *lista;
        *lista = novo;
    } 
    
    else{
        anterior->prox = novo;
        novo->prox = atual;
    }
}


void removerElemento(Lista* lista, int valor){
    NO* atual = *lista;
    NO* anterior = NULL;

    while(atual != NULL && atual->info != valor){
        anterior = atual;
        atual = atual->prox;
    }

    if(atual == NULL) return;

    if(anterior == NULL) *lista = atual->prox;
    else anterior->prox = atual->prox;

    free(atual);
}