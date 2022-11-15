// Rafael Bonfim Zacco

/*
Sua tarefa será construir uma hash table usando como estrutura base uma árvore heap que use como índice o resultado de uma função de hash disponível na internet.  

Esta hash table deve ser capaz de armazenar senhas (strings) com no mínimo 10 caracteres e no máximo 50 caracteres. 

No seu trabalho, como comentário, deve constar a referência para a função de hash escolhida para a criação do índice da árvore heap.  

Você deverá criar, no mínimo, uma função de inclusão, exclusão, e busca. A função de inclusão deve retornar falso, sempre que o elemento a ser inserido já exista na tabela.  
*/

/*
Referência:
https://www.digitalocean.com/community/tutorials/hash-table-in-c-plus-plus#choose-a-hash-function
*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define TAM 50
#define TAM_S 52

int funcaoHash(char chave[]){ //adaptado removendo ponteiro, modificando tipos e incrementando for
  
  int tam_string = strlen(chave);
  unsigned int valor = 0;

  for (int i=0 ; i<tam_string ; i++){
    valor += chave[i] * (i*i);
  }

  return valor % TAM;

}

struct heap_max {

  struct no *vetor;
  int tamanho;
  int ultimo;

};

struct no {

  int indice;
  char senha[TAM_S];

};

int pai(int indice) {
  return (indice - 1) / 2; 
}

int filho_esquerda(int indice) {
  return indice * 2 + 1; 
}

int filho_direita(int indice) {
  return indice * 2 + 2;
}

bool verificaVazio(struct heap_max *heap) {

  if (heap->ultimo == -1) {
    return true;
  }
    
  else {
    return false;
  }
  
}

bool verificaIndice(struct heap_max *heap,int indice) {

  if ( (indice >= 0) && (indice <= heap->tamanho) ) {
    return true;
  }

  else {
    return false;
  }
  
}

bool verificaFolha(struct heap_max *heap, int indice) {

  if ( (indice > pai(heap->ultimo)) && (indice <= heap->ultimo) ) {
    return true;
  }

  else {
    return false;
  }
  
}

int verificaMaiorIndice(struct heap_max *heap, int pai, int direita,int esquerda) {

  if (heap->vetor[pai].indice > heap->vetor[esquerda].indice) {
    if (verificaIndice(heap, direita) == true) {
      if (heap->vetor[pai].indice < heap->vetor[direita].indice) {
        return direita;
      }
    }

    return pai;

  } 
    
  else {
    if (verificaIndice(heap, direita) == true) {
      if (heap->vetor[esquerda].indice < heap->vetor[direita].indice) {
        return direita;
      }
    }

    return esquerda;
    
  }
  
}

void heapify_max(struct heap_max *heap, int indice) {

  if (verificaFolha(heap, indice) || !verificaIndice(heap, indice)) {
    return;
  }

  int maior_indice = verificaMaiorIndice(heap, indice, filho_esquerda(indice), filho_direita(indice));

  if (maior_indice != indice) {
    struct no aux = heap->vetor[indice];
    heap->vetor[indice] = heap->vetor[maior_indice];
    heap->vetor[maior_indice] = aux;

    heapify_max(heap, maior_indice);

  }
  
}

bool buscar(struct heap_max *heap, char senha []) {

  int indice = funcaoHash(senha);
  
  for (int i=0 ; i <= heap->ultimo ; i++) {
    if (heap->vetor[i].indice == indice) {
      printf("\n\tO valor digitado se encontra no índice %d!\n",i);
      return true;
    }
  }

  return false;

}

struct heap_max *inserir(struct heap_max *heap, char senha[]) {

  int indice = funcaoHash(senha);

  if(buscar(heap,senha) == false){
    if (heap->ultimo >= (heap->tamanho)) {
      printf("O heap esta cheio!\n");
      return heap;
    }
  
    heap->ultimo += 1;
    struct no novo;
    novo.indice = indice;
    strcat(novo.senha,senha);
    heap->vetor[heap->ultimo] = novo;
  
    int i = heap->ultimo;
    while ( (heap->vetor[pai(i)].indice < heap->vetor[i].indice) && (i > 0) ) {
      struct no aux = heap->vetor[i];
      heap->vetor[i] = heap->vetor[pai(i)];
      heap->vetor[pai(i)] = aux;
      i = pai(i);
    }
    return heap;
  }
  return heap;  
  
}

bool remover(struct heap_max *heap) {

  if (verificaVazio(heap) == true) {
    return false;  
  }

  else {
    heap->vetor[0] = heap->vetor[heap->ultimo];
    heap->vetor[heap->ultimo].indice = 0;
    memset(heap->vetor[heap->ultimo].senha, 0, TAM_S);
    heap->ultimo -= 1;

    heapify_max(heap, 0);
  }

  return true;
  
}

void imprimir(struct heap_max *heap){

  for(int i=0 ; i<TAM ; i++){
    if((int)strlen(heap->vetor[i].senha)>0){
      printf("Senha[%d] = %s",i,heap->vetor[i].senha);
    }
      
    else{
       printf("Senha[%d] = %s\n",i,heap->vetor[i].senha);
    }
  }

}

void verificaSenha(char senha[]){
  
  fgets(senha,TAM_S,stdin);
  if(strlen(senha)<50){
    printf("\nPressione ENTER para confirmar");
    while ((getchar()) != '\n');
  }
  setbuf(stdin,NULL);
  
  while( (strlen(senha)>50) || (strlen(senha)<10) ){
    printf("\n\tO valor digitado deve ter entre 10 e 50 caracteres!\n");

    if(strlen(senha)>50){     
      printf("\nPressione ENTER para confirmar");
      while ((getchar()) != '\n');
      setbuf(stdin,NULL);
      
      if(strlen(senha)<50){
        printf("\nPressione ENTER para confirmar");
      }
    }  

    printf("\n\tDigite a senha: ");
    fgets(senha,TAM_S,stdin);
    
    if(strlen(senha)<50){
     printf("\nPressione ENTER para confirmar");
    }
    while ((getchar()) != '\n');
  }
   
}

int main(){

  int opcao, retorno, tabela[TAM];
  char senha[TAM_S];
  struct heap_max *heap;
  heap = (struct heap_max *)malloc(sizeof(struct heap_max));
  heap->vetor = (struct no *)calloc(TAM, sizeof(struct no));
  heap->tamanho = TAM;
  heap->ultimo = -1;

  do{
    printf("HASH TABLE - MENU DE CONTROLE\n");
    printf("\n\t0 - Sair\n\t1 - Inserir\n\t2 - Excluir\n\t3 - Buscar\n\t4 - Imprimir table\n\n>");
    scanf("%d", &opcao);
    getchar();
    switch(opcao){
    case 0:
      printf("\nMenu encerrado.....\n");
      break;
    case 1:
      printf("\n\tDigite a senha: ");
      verificaSenha(senha);
      heap = inserir(heap, senha);
      printf("\n\tValor inserido com sucesso!\n\n");
      printf("----------------------------------------\n\n");
      break;
    case 2:
      if (remover(heap))
        printf("\n\tA raiz da hash table foi excluída!\n\n");
      else
        printf("\n\tO heap está vazio!\n\n");
      printf("----------------------------------------\n\n");
      break;
    case 3:
      verificaSenha(senha);
      if(buscar(heap,senha) == false){
        printf("\n\tNão foi possível encontrar o valor digitado!\n");
      }
      printf("\n----------------------------------------\n\n");
      break;
    case 4:
      printf("\n----------------IMPRESSÃO---------------\n\n");
      imprimir(heap);
      printf("\n----------------------------------------\n\n");
      break;
    default:
      printf("\nOpção inválida!\n");
    }
  }while(opcao != 0);

  return 0;
}