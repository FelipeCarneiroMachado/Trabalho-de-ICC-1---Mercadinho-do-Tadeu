#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
ICC1 - professor Rudinei
*/

// Struct de cada produto do mercado
typedef struct produto {
  int quantidade;
  double preco;
  char nome[30];
} produto;

// Prototipos das funcoes

// Implementacao das funcoes para pegar strings
char *Entrada_palavra() { // debugada
  int n_letras = 0;
  char c, *string = (char*)malloc(30 * sizeof(char));
  while (((c = getchar()) != ' ') && (c != '\n')) {
    string[n_letras] = c;
    n_letras++;
    if ((n_letras % 30) == 0)
      string = (char*)realloc(string, 30 + n_letras);
  }
  string[n_letras + 1] = '\0';
  return string;
}
char *Entrada_linha() { //bugada, nao usar
  int n_letras = 0;
  char c, *string = (char*)malloc(30 * sizeof(char));
  while ((c = getchar()) != '\n') {
    string[n_letras] = c;
    n_letras++;
    if ((n_letras % 29) == 0)
      string = (char *)realloc(string, 30 + n_letras);
  }
  string[n_letras + 1] = '\0';
  getchar();
  return string;
}
void Imprime_divisoria(){ // funcionamento pleno
  for(int i = 0; i < 50; i++)
    printf("-");
  printf("\n");
}
void Insere_produto(produto *estoque,int *nprodutos){//bugged
  *nprodutos += 1;
  estoque = (produto*)realloc(estoque, *nprodutos + 1 * sizeof(produto));
  char *tempstr = Entrada_palavra();
  strcpy(estoque[*(nprodutos) - 1].nome, tempstr);
  scanf("%d %lf", &estoque[*nprodutos - 1].quantidade, &estoque[*nprodutos - 1].preco);
}

void Aumenta_estoque(produto *estoque){ //funcionamento pleno
  int codigo, quantidade_nova;
  char *fvckscanf = Entrada_palavra();
  codigo = atoi(fvckscanf);
  fvckscanf = Entrada_palavra();
  quantidade_nova = atoi(fvckscanf);
  estoque[codigo].quantidade += quantidade_nova;
}

void Modifica_preco(produto *estoque){//funciona plenamente
  int codigo, preco;
  char *fvckscanf = Entrada_palavra();
  codigo = atoi(fvckscanf);
  fvckscanf = Entrada_palavra();
  preco = atof(fvckscanf); 
  estoque[codigo].preco = preco;
}

void Venda(double* saldo, produto *estoque){ //funciona plenamente
  int codigo;
  double acum_saldo = 0.0;
  scanf("%d", &codigo);
  getchar();
  while(codigo!=(-1)){
  estoque[codigo].quantidade --;
  acum_saldo += estoque[codigo].preco;
  printf("%s %lf\n", estoque[codigo].nome, estoque[codigo].preco);
  scanf("%d", &codigo);
  getchar();
  }
  *saldo += acum_saldo;
  printf("Total: %f\n", acum_saldo);
  Imprime_divisoria();
}


void Consulta_estoque(int nprodutos, produto *estoque){ //funciona plenamente
  for(int i = 0; i < nprodutos; i++){
    printf("%d %s %d\n", i, estoque[i].nome, estoque[i].quantidade);
  }
  Imprime_divisoria();
}



void Consulta_saldo(double saldo){//funciona plenamente
  printf("Saldo: %lf\n", saldo);
  Imprime_divisoria();
}

/* Funcao para abrir o arquivo e escrever nele se ele ja existir, senao, o
 * programa pergunta a quantidade de produtos e o saldo */
FILE *Ler_arquivo(int *n_produtos, double *saldo) {
  //aparentemente livre  de bugs
  FILE *database = fopen("database.bin", "r+b");
  bool arquivo_existe = true;
  if (database == NULL) {
    arquivo_existe = false;
    database = fopen("database.bin", "w+b");
  }
  if (arquivo_existe) {
    fread(n_produtos, sizeof(int), 1, database);
    fread(saldo, sizeof(double), 1, database);
  }
  else 
  {
    scanf("%d %d ", n_produtos, saldo);
  }
  return database;
}
void Finalizar_dia(int nprodutos, double saldo, produto *estoque, FILE  *database){ 
  //aparentemente livre de bugs
  database = freopen("database.bin", "w+b", database);
  fwrite(&nprodutos, sizeof(int), 1, database);
  fwrite(&saldo, sizeof(double), 1, database);
  fwrite(estoque, sizeof(produto), nprodutos, database);
  fclose(database);
  exit(0);
}

// Funcao que vai ler a struct produto
produto *Ler_estoque(int nprodutos, FILE *stream) {
  //sem bugs ate o momento
  produto *estoque = (produto *)malloc((nprodutos + 1) * sizeof(produto));
  fread(estoque, sizeof(produto), nprodutos, stream);
  return estoque;
}

// Funcao que identifica cada comando e retorna um numero correspondente
int Identifica_funcionalidade() {
  char c1 = getchar(), c2 = getchar();
  getchar();
  switch (c1){
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
  double saldo;
  FILE *database = Ler_arquivo(&n_produtos, &saldo);
  produto *estoque = Ler_estoque(n_produtos, database);
  while(true){
    int funcionalidade = Identifica_funcionalidade();
    switch (funcionalidade) {
      case 1:
        Insere_produto(estoque, &n_produtos); //IP <nome> <quantidade> <preco>
        break;
      case 2:
        Aumenta_estoque(estoque); //AE <codigo> <quantidade>
        break;
      case 3:
        Modifica_preco(estoque); //MP <codigo> <preco>
        break;
      case 4:
        Venda(&saldo, estoque); // VE <codigo> <codigo> <codigo> ... -1
        break;
      case 5:
        Consulta_estoque(n_produtos, estoque);  // CE
        break;
      case 6:
        Consulta_saldo(saldo);  //CS
        break;
      case 7:
        Finalizar_dia(n_produtos, saldo, estoque, database); //FD
        break;
    }
  }
}
