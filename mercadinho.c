#include <stbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
ICC1 - professor Rudinei

*/

// Struct de cada produto do mercado
typedef struct produto {
  int quantidade;
  float preco;
  char nome[30];
} produto;

// Prototipos das funcoes

// Implementacao das funcoes para pegar strings
char *Entrada_palavra() {
  int n_letras = 0;
  char c, string = (char)malloc(10 * sizeof(char));
  while ((c = getchar()) != '_') {
    string[n_letras] = c;
    n_letras++;
    if ((n_letras % 9) == 0)
      string = (char)realloc(string, 10 + n_letras);
  }
  return string;
}
char *Entrada_linha() {
  int n_letras = 0;
  char c, string = (char)malloc(10 * sizeof(char));
  while ((c = getchar()) != '\n') {
    string[n_letras] = c;
    n_letras++;
    if ((n_letras % 9) == 0)
      string = (char *)realloc(string, 10 + n_letras);
  }
  return string;
}
void Imprime_divisoria(){
  for(int i = 0; i < 50; i++)
    printf("-");
  printf("\n");
}
void Insere_produto(produto *estoque,int *nprodutos){
  *nprodutos++;
  estoque = (produto*)realloc(estoque, *nprodutos * sizeof(produto));
  estoque[*nprodutos - 1].nome = Entrada_palavra();
  scanf("%d %f", &estoque[*nprodutos - 1].quantidade, &estoque[*nprodutos - 1].preco);
}

void Aumenta_estoque(produto *estoque){
  int codigo, quantidade_nova;
  scanf("%d %d", &codigo, &quantidade_nova);
  estoque[codigo].quantidade = quantidade_nova;
}

void Modifica_preco(produto *estoque){
  int codigo, preco;
  scanf("%d %f", &codigo, &preco);  
  estoque[].preco = preco;
}

void Venda(float* saldo, produto *estoque){
  int codigo;
  float acum_saldo = 0.0;
  scanf("%d", &codigo);
  while(codigo!=(-1)){
  estoque[codigo].quantidade --;
  acum_saldo += estoque[codigo].preco;
  printf("%s %f", estoque[codigo].nome, estoque[codigo].preco);
  
  Imprime_divisoria();
  }
}


void Consulta_estoque(int nprodutos, produto *estoque){
  for(int i = 0; i < nprodutos - 1; i++){
    printf("%d %s %d", i, estoque[i].nome, estoque[i].quantidade);
  }
  Imprime_divisoria();
}



void Consulta_saldo(float saldo){
  printf("Saldo: %f\n", saldo);
  Imprime_divisoria();
}

/* Funcao para abrir o arquivo e escrever nele se ele ja existir, senao, o
 * programa pergunta a quantidade de produtos e o saldo */
FILE *Ler_arquivo(int *n_produtos, float *saldo) {
  FILE *database = fopen("database", "r+b");
  bool arquivo_existe = true;
  if (database == NULL) {
    arquivo_existe = false;
    *database = fopen("database", "w+b");
  }
  if (arquivo_existe) {
    fread(n_produtos, sizeof(int), 1, database);
    fread(saldo, sizeof(float), 1, database);
  }
  else 
  {
    scanf("%d %d ", n_produtos, saldo);
  }
  return database;
}

// Funcao que vai ler a struct produto
produto *Ler_estoque(int nprodutos, FILE *stream) {
  produto *estoque = (produto *)malloc(nprodutos * sizeof(produto));
  fread(estoque, sizeof(produto), nprodutos, stream);
}

// Funcao que identifica cada comando e retorna um numero correspondente
int Identifica_funcionalidade() {
  char c1, c2;
  scanf("%c%c ", &c1, &c2);
  switch c1{
    case 'I':return 1;
    case 'A':return 2;
    case 'M':return 3;
    case 'V':return 4;
    case 'C':return (c2 == 'E') ? 5 : 6;
    case 'F':return 7;
  }
}

int main(void) {
  int n_produtos;
  float saldo;
  FILE *database = Ler_arquivo(&n_produtos, &saldo);
  produto *estoque = Ler_estoque(n_produtos, database);

  int funcionalidade = Identifica_funcionalidade();
  switch (funcionalidade) {
  case 1:
    Insere_produto(estoque, n_produtos);
  case 2:
    Aumenta_estoque(estoque);
  case 3:
    Modifica_preco(estoque);
  case 4:
    Venda(&saldo, estoque);
  case 5:
    Consulta_estoque();
  case 6:
    Consulta_saldo();
  case 7:
    Finalizar_dia();
  }
  return 0;
}

