#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define TAM 30

#ifdef NULL
	#undef NULL
	#define NULL 0
#else
	#define NULL 0
#endif

int idNos = 1;

typedef struct NO_GRAFO {
	int id;
	char info[TAM];
	struct LISTA * adjacentes;
} NO_GRAFO;

typedef struct GRAFO {
	struct LISTA * nos;
} GRAFO;

typedef struct NO_LISTA {
	NO_GRAFO * no_grafo;
	struct NO_LISTA * ant;
	struct NO_LISTA * prox;
} NO_LISTA;

typedef struct LISTA {
	struct NO_LISTA * ini;
	struct NO_LISTA * fim;
} LISTA;

//ALOCA MEMÓRIA

LISTA * aloca_lista() {
	LISTA * lista = (LISTA *) malloc(sizeof(LISTA));
	lista->ini = lista->fim = NULL;
	return lista;
}

NO_GRAFO * aloca_no_grafo(char valor[TAM]) {
	NO_GRAFO * no_grafo = (NO_GRAFO *) malloc(sizeof(NO_GRAFO));
	no_grafo->id = idNos;
	idNos++;
	strcpy(no_grafo->info, valor);
	no_grafo->adjacentes = aloca_lista();
	return no_grafo;
}

GRAFO * aloca_grafo() {
	GRAFO * grafo = (GRAFO *) malloc(sizeof(GRAFO));
	grafo->nos = aloca_lista();
	grafo->nos->ini = grafo->nos->fim = NULL;
	return grafo;
}

NO_LISTA * aloca_no_lista(NO_GRAFO * no_grafo) {
	NO_LISTA * no_lista = (NO_LISTA *) malloc(sizeof(NO_LISTA));
	no_lista->no_grafo = no_grafo;
	no_lista->ant = no_lista->prox = NULL;
	return no_lista;
}

//LISTA

void inserir_lista_inicio(LISTA * lista, char valor[TAM]) {
	if(lista == NULL) {
		return;
	} else if(lista->ini == NULL) {
		lista->ini = lista->fim = aloca_no_lista(aloca_no_grafo(valor));
		return;
	} else {
		lista->ini->ant = aloca_no_lista(aloca_no_grafo(valor));
		lista->ini->ant->prox = lista->ini;
		lista->ini = lista->ini->ant;
		return;
	}
}

void inserir_lista_fim(LISTA * lista, char valor[TAM]) {
	if(lista == NULL) {
		return;
	} else if(lista->fim == NULL) {
		lista->fim = lista->ini = aloca_no_lista(aloca_no_grafo(valor));
		return;
	} else {
		lista->fim->prox = aloca_no_lista(aloca_no_grafo(valor));
		lista->fim->prox->ant = lista->fim;
		lista->fim = lista->fim->prox;
		return;
	}
}

NO_LISTA * getAux(NO_LISTA * no, int n) {
	if(n == 0) {
		return no;
	} else {
		getAux(no->prox, n-1);
	}
}

NO_LISTA * get(LISTA * lista, int n) {
	return getAux(lista->ini, n);
}

NO_LISTA * getIgualInfoAux(NO_LISTA * no, char info[TAM]) {
	if(no == NULL) {
		return NULL;
	} else if(strcmp(no->no_grafo->info, info) == 0) {
		return no;
	} else {
		getIgualInfoAux(no->prox, info);
	}
}

NO_LISTA * getIgualInfo(LISTA * lista, char info[TAM]) {
	return getIgualInfoAux(lista->ini, info);
}

int tamListaAux(NO_LISTA * no, int n) {
	if(no == NULL) {
		return n;
	} else {
		return tamListaAux(no->prox, n+1);
	}
}

int tamLista(LISTA * lista) {
	return tamListaAux(lista->ini, 0);
}

void imprimeListaAux(NO_LISTA * no) {
	if(no == NULL) {
		return;
	} else {
		printf("%s -> ", no->no_grafo->info);
		imprimeListaAux(no->prox);
	}
}

void imprimeLista(LISTA * lista) {
	imprimeListaAux(lista->ini);
}

int contemListaAux(NO_LISTA * no, char valor[TAM]) {
	if(no == NULL) {
		return 0;
	} else if(strcmp(no->no_grafo->info, valor) == 0) {
		return 1;
	} else {
		contemListaAux(no->prox, valor);
	}
}

int contemLista(LISTA * lista, char valor[TAM]) {
	return contemListaAux(lista->ini, valor);
}

int removerInicio(LISTA * lista) {
	if(lista->ini == NULL) {
		return 0;
	} else if(strcmp(lista->ini->no_grafo->info, lista->fim->no_grafo->info) == 0){
		NO_LISTA * tmp = lista->ini;
		lista->ini = lista->fim = NULL;
		free(tmp);
		return 1;
	} else {
		NO_LISTA * tmp = lista->ini;
		lista->ini = lista->ini->prox;
		lista->ini->ant = NULL;
		free(tmp);
		return 1;
	}
}

int removerFim(LISTA * lista) {
	if(lista->fim == NULL) {
		return 0;
	} else if(strcmp(lista->fim->no_grafo->info, lista->ini->no_grafo->info) == 0){
		NO_LISTA * tmp = lista->fim;
		lista->fim = lista->ini = NULL;
		free(tmp);
		return 1;
	} else {
		NO_LISTA * tmp = lista->fim;
		lista->fim = lista->fim->ant;
		lista->fim->prox = NULL;
		free(tmp);
		return 1;
	}
}

int removerListaAux(LISTA * lista, NO_LISTA * no, char valor[TAM], int delet) {
	if(no == NULL) {
		return 0;
	} else if(strcmp(no->no_grafo->info, valor) == 0) {
		if(strcmp(no->no_grafo->info, lista->ini->no_grafo->info) == 0) {
			removerInicio(lista);
		} else if(strcmp(no->no_grafo->info, lista->fim->no_grafo->info) == 0) {
			removerFim(lista);
		} else {
			NO_LISTA * ant = no->ant;
			NO_LISTA * prox = no->prox;
			ant->prox = prox;
			prox->ant = ant;
			no->prox = no->ant = NULL;
			if(delet == 1) {
                free(no->no_grafo->adjacentes);
                free(no->no_grafo);
			}
			free(no);
		}
	} else {
		return removerListaAux(lista, no->prox, valor, delet);
	}
}

int removerLista(LISTA * lista, char valor[TAM], int delet) {
	return removerListaAux(lista, lista->ini, valor, delet);
}

LISTA * copiaLista(LISTA * lista) {
    LISTA * copiado = aloca_lista();
    int i;
    for(i=0; i<tamLista(lista); i++) {
        inserir_lista_fim(copiado, get(lista, i)->no_grafo->info);
    }
    return copiado;
}

//GRAFO

void adicionarNo(GRAFO * grafo, char valor[TAM]) {
	inserir_lista_fim(grafo->nos, valor);
}

void adicionarAdjacencia(GRAFO * grafo, char n1[TAM], char n2[TAM]) {
	if(getIgualInfo(grafo->nos, n1) == NULL || getIgualInfo(grafo->nos, n2) == NULL) {
		return;
	} else {
		if(contemLista(getIgualInfo(grafo->nos, n1)->no_grafo->adjacentes, getIgualInfo(grafo->nos, n2)->no_grafo->info) == 0)
			inserir_lista_fim(getIgualInfo(grafo->nos, n1)->no_grafo->adjacentes, getIgualInfo(grafo->nos, n2)->no_grafo->info);
	}
}

void adicionarDuplaAdjacencia(GRAFO * grafo, char n1[TAM], char n2[TAM]) {
	if(getIgualInfo(grafo->nos, n1) == NULL || getIgualInfo(grafo->nos, n2) == NULL) {
		return;
	} else {
		if(contemLista(getIgualInfo(grafo->nos, n1)->no_grafo->adjacentes, getIgualInfo(grafo->nos, n2)->no_grafo->info) == 0)
			inserir_lista_fim(getIgualInfo(grafo->nos, n1)->no_grafo->adjacentes, getIgualInfo(grafo->nos, n2)->no_grafo->info);
		if(contemLista(getIgualInfo(grafo->nos, n2)->no_grafo->adjacentes, getIgualInfo(grafo->nos, n1)->no_grafo->info) == 0)
			inserir_lista_fim(getIgualInfo(grafo->nos, n2)->no_grafo->adjacentes, getIgualInfo(grafo->nos, n1)->no_grafo->info);
	}
}

void removerNo(GRAFO * grafo, char valor[TAM]) {
	int tam = tamLista(grafo->nos);
	int i;
	for(i=0; i<tam; i++) {
		removerLista(get(grafo->nos, i)->no_grafo->adjacentes, valor, 0);
	}
	removerLista(grafo->nos, valor, 1);
}

void removerAdjacencia(GRAFO * grafo, char n1[TAM], char n2[TAM]) {
	if(contemLista(getIgualInfo(grafo->nos, n1)->no_grafo->adjacentes, n2) == 1)
		removerLista(getIgualInfo(grafo->nos, n1)->no_grafo->adjacentes, n2, 0);
}

void removerDuplaAdjacencia(GRAFO * grafo, char n1[TAM], char n2[TAM]) {
	if(contemLista(getIgualInfo(grafo->nos, n1)->no_grafo->adjacentes, n2) == 1)
		removerLista(getIgualInfo(grafo->nos, n1)->no_grafo->adjacentes, n2, 0);
	if(contemLista(getIgualInfo(grafo->nos, n2)->no_grafo->adjacentes, n1) == 1)
		removerLista(getIgualInfo(grafo->nos, n2)->no_grafo->adjacentes, n1, 0);
}

void imprimirListaAdjacencia(GRAFO * grafo) {
    int i;
	for(i=0; i<tamLista(grafo->nos); i++) {
		printf("\nNO: %s ||| ADJACENTES: ", get(grafo->nos, i)->no_grafo->info);
		imprimeLista(get(grafo->nos, i)->no_grafo->adjacentes);
	}
}

int menorCaminhoAux(GRAFO * grafo, NO_LISTA * no, LISTA * percorridos, NO_LISTA * b, int tam) {
    imprimeLista(percorridos);
    printf("NO ATUAL: %s\n", no->no_grafo->info);
    if(strcmp(no->no_grafo->info, b->no_grafo->info) == 0) {
        return tam;
    } else if(contemLista(percorridos, no->no_grafo->info) == 1) {
        return INT_MAX;
    } else {
        inserir_lista_fim(percorridos, no->no_grafo->info);
        int menor = INT_MAX;
        int i;
        for(i=0; i<tamLista(no->no_grafo->adjacentes); i++) {
            int tmp = menorCaminhoAux(grafo, getIgualInfo(grafo->nos, get(no->no_grafo->adjacentes, i)->no_grafo->info), percorridos, b, tam + 1);
            if(tmp < menor)
                menor = tmp;
        }
        removerFim(percorridos);
        return menor;
    }
}

int menorCaminho(GRAFO * grafo, char a[TAM], char b[TAM]) {
    return menorCaminhoAux(grafo, getIgualInfo(grafo->nos, a), aloca_lista(), getIgualInfo(grafo->nos, b), 0);
}

int main(void) {

	GRAFO * grafo = aloca_grafo();
	char n1[TAM] = {'1'};
	char n2[TAM] = {'2'};
	char n3[TAM] = {'3'};
	char n4[TAM] = {'4'};
	char n5[TAM] = {'5'};
	char n6[TAM] = {'6'};
	char n7[TAM] = {'7'};
	adicionarNo(grafo, n1);
	adicionarNo(grafo, n2);
	adicionarNo(grafo, n3);
	adicionarNo(grafo, n4);
	adicionarNo(grafo, n5);
	adicionarNo(grafo, n6);
	adicionarNo(grafo, n7);
	adicionarAdjacencia(grafo, n1, n2);
	adicionarDuplaAdjacencia(grafo, n2, n3);
	adicionarAdjacencia(grafo, n3, n4);
	adicionarAdjacencia(grafo, n4, n5);
	adicionarDuplaAdjacencia(grafo, n5, n3);
	adicionarDuplaAdjacencia(grafo, n5, n6);
	adicionarAdjacencia(grafo, n6, n7);
	adicionarAdjacencia(grafo, n7, n1);
	int menor = menorCaminho(grafo, n1, n7);
	if(menor == INT_MAX) {
        printf("\nNAO EXISTE CAMINHO\n");
	} else {
	    printf("\nMENOR CAMINHO: %d\n", menor);
	}
	imprimirListaAdjacencia(grafo);
	return (0x0);
}
