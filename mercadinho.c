#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Trabalho final do semestre
Disciplina SCC0221 - Introducao a ciencia da computacao
Professor Rudinei Goularte - ICMC - USP
Realizado em grupo pelos alunos
Felipe Carneiro Machado - 14569373
Eduarda Tuboy Nardin - 13732495
Rafael Mansur - Rafael Brazolin Alves Mansur
*/

// Struct de cada produto do mercado
typedef struct produto {
  int quantidade;
  double preco;
  char nome[30];
} produto;

// Prototipos das funcoes
char *Entrada_palavra();
void Imprime_divisoria();
produto *Insere_produto(produto *estoque, int *nprodutos);
void Aumenta_estoque(produto *estoque);
void Modifica_preco(produto *estoque);
void Venda(double *saldo, produto *estoque);
void Consulta_estoque(int nprodutos, produto *estoque);
void Consulta_saldo(double saldo);
FILE *Ler_arquivo(int *nprodutos, double *saldo);
void Finalizar_dia(int nprodutos, double saldo, produto *estoque, FILE *database);
produto *Ler_estoque(int nprodutos, FILE *stream);
int Identifica_funcionalidade();
// Funcao para ler caracteres ate o espaco e retornando em uma string
char *Entrada_palavra() { // debugada
  int n_letras = 0;
  char c, *string = (char *)malloc(30 * sizeof(char));
  while (((c = getchar()) != ' ') && (c != '\n')) {
    string[n_letras] = c;
    n_letras++;
    if ((n_letras % 30) == 0)
      string = (char *)realloc(string, 30 + n_letras);
  }
  string[n_letras + 1] = '\0';
  return string;
}

void Imprime_divisoria() { // funcionamento pleno
  for (int i = 0; i < 50; i++)
    printf("-");
  printf("\n");
}
produto *Insere_produto(produto *estoque, int *nprodutos) {
  *nprodutos += 1;
  estoque = (produto *)realloc(estoque, (*nprodutos + 1) * sizeof(produto));
  char *nomeproduto = Entrada_palavra();
  strcpy(estoque[*(nprodutos)-1].nome, nomeproduto);
  // fvckscanf eh uma string temporária para pegar a quantidade e o preco
  char *fvckscanf = Entrada_palavra();
  int quantidade = atoi(fvckscanf);
  estoque[*(nprodutos)-1].quantidade = quantidade;
  fvckscanf = Entrada_palavra();
  double preco = atof(fvckscanf);
  estoque[*(nprodutos)-1].preco = preco;
  return estoque;
}

void Aumenta_estoque(produto *estoque) { // funcionamento pleno
  int codigo, quantidade_nova;
  char *fvckscanf = Entrada_palavra();
  codigo = atoi(fvckscanf);
  fvckscanf = Entrada_palavra();
  quantidade_nova = atoi(fvckscanf);
  estoque[codigo].quantidade += quantidade_nova;
}

void Modifica_preco(produto *estoque) { // funciona plenamente
  int codigo, preco;
  char *fvckscanf = Entrada_palavra();
  codigo = atoi(fvckscanf);
  fvckscanf = Entrada_palavra();
  preco = atof(fvckscanf);
  estoque[codigo].preco = preco;
}

void Venda(double *saldo, produto *estoque) { // funciona plenamente
  int codigo;
  double acum_saldo = 0.0;
  scanf("%d", &codigo);
  getchar(); // Para consumir o espaco
  while (codigo != (-1)) {
    estoque[codigo].quantidade--;
    acum_saldo += estoque[codigo].preco;
    printf("%s %lf\n", estoque[codigo].nome, estoque[codigo].preco);
    scanf("%d", &codigo);
    getchar();
  }
  *saldo += acum_saldo;
  printf("Total: %f\n", acum_saldo);
  Imprime_divisoria();
}

void Consulta_estoque(int nprodutos, produto *estoque) { // funciona plenamente
  for (int i = 0; i < nprodutos; i++) {
    printf("%d %s %d\n", i, estoque[i].nome, estoque[i].quantidade);
  }
  Imprime_divisoria();
}

void Consulta_saldo(double saldo) { // funciona plenamente
  printf("Saldo: %lf\n", saldo);
  Imprime_divisoria();
}

/* Funcao para abrir o arquivo e escrever nele se ele ja existir, senao, o
 * programa pergunta a quantidade de produtos e o saldo */
FILE *Ler_arquivo(int *nprodutos, double *saldo) {
  // aparentemente livre  de bugs
  FILE *database = fopen("database.bin", "r+b");
  bool arquivo_existe = true;
  if (database == NULL) {
    arquivo_existe = false;
    database = fopen("database.bin", "w+b");
  }
  if (arquivo_existe) {
    fread(nprodutos, sizeof(int), 1, database);
    fread(saldo, sizeof(double), 1, database);
  } else {
    char *fvckscanf = Entrada_palavra();
    *nprodutos = atoi(fvckscanf);
    fvckscanf = Entrada_palavra();
    *saldo = atof(fvckscanf);
    *nprodutos = 0; // cara n faz sentido ler o n de produtos pq ele aumenta
                    // sempre q insere produto ent fiz essa gambiarra
  }
  return database;
}
void Finalizar_dia(int nprodutos, double saldo, produto *estoque,
                   FILE *database) {
  // aparentemente livre de bugs
  database = freopen("database.bin", "w+b", database);
  fwrite(&nprodutos, sizeof(int), 1, database);
  fwrite(&saldo, sizeof(double), 1, database);
  fwrite(estoque, sizeof(produto), nprodutos, database);
  fclose(database);
  exit(0);
}

// Funcao que vai ler a struct produto
produto *Ler_estoque(int nprodutos, FILE *stream) {
  // sem bugs ate o momento
  produto *estoque = (produto *)malloc((nprodutos + 1) * sizeof(produto));
  fread(estoque, sizeof(produto), nprodutos, stream);
  return estoque;
}

// Funcao que identifica cada comando e retorna um numero correspondente
int Identifica_funcionalidade() {
  char c1 = getchar(), c2 = getchar();
  getchar();
  switch (c1) {
  case 'I':
    return 1;
  case 'A':
    return 2;
  case 'M':
    return 3;
  case 'V':
    return 4;
  case 'C':
    return (c2 == 'E') ? 5 : 6;
  case 'F':
    return 7;
  }
  return 0; // Para caso de erro
}

int main(void) {
  int n_produtos; // Numero de itens distintos no estoque
  double saldo;   // Saldo do mercado
  FILE *database = Ler_arquivo(&n_produtos, &saldo);
  produto *estoque = Ler_estoque(n_produtos, database);
  while (true) {
    int funcionalidade = Identifica_funcionalidade();
    switch (funcionalidade) {
    case 1:
      estoque = Insere_produto(estoque, &n_produtos); // IP <nome> <quantidade> <preco>
      break;
    case 2:
      Aumenta_estoque(estoque); // AE <codigo> <quantidade>
      break;
    case 3:
      Modifica_preco(estoque); // MP <codigo> <preco>
      break;
    case 4:
      Venda(&saldo, estoque); // VE <codigo> <codigo> <codigo> ... -1
      break;
    case 5:
      Consulta_estoque(n_produtos, estoque); // CE
      break;
    case 6:
      Consulta_saldo(saldo); // CS
      break;
    case 7:
      Finalizar_dia(n_produtos, saldo, estoque, database); // FD
      break;
    default:
      printf("Selecione um comando válido");
    }
  }
}