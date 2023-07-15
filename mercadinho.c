#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Trabalho de final do semestre
Disciplina [SCC0221] - Introducao a ciencia da computacao
Professor Rudinei Goularte - ICMC - USP

Programa que realiza funcoes de um sistema de manutencao de estoque de um
mercado, podendo: inserir novos produtos, aumentar o estoque, modificar o preco
de um produto, realizar vendas, consultar o estoque dos produtos, cnsultar o
saldo do caixa e armanezar as informacoes em um arquivo para proximas consultas.

Realizado em grupo pelos alunos:
Felipe Carneiro Machado - nº 14569373
Eduarda Tuboy Nardin - nº 13732495
Rafael Brazolin Alves Mansur - nº 14604020
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
void Finalizar_dia(int nprodutos, double saldo, produto *estoque,
                   FILE *database);
produto *Ler_estoque(int nprodutos, FILE *stream);
int Identifica_funcionalidade();

/* Funcao para ler caracteres ate o espaco ou quebra de linha e retornando em
 * uma string */
char *Entrada_palavra() {
  int n_letras = 0;
  char c, *string = (char *)malloc(30 * sizeof(char));
  while (((c = getchar()) != ' ') && (c != '\n') && c != '\r') {
    string[n_letras] = c;
    n_letras++;
    if ((n_letras % 30) == 0)
      string = (char *)realloc(string, 30 + n_letras);
  }
  string[n_letras + 1] = '\0';
  return string;
}

void Imprime_divisoria() {
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

void Aumenta_estoque(produto *estoque) {
  int codigo, quantidade_nova;
  char *fvckscanf = Entrada_palavra();
  codigo = atoi(fvckscanf);
  fvckscanf = Entrada_palavra();
  quantidade_nova = atoi(fvckscanf);
  estoque[codigo].quantidade += quantidade_nova;
}

void Modifica_preco(produto *estoque) {
  int codigo;
  double preco;
  char *fvckscanf = Entrada_palavra();
  codigo = atoi(fvckscanf);
  fvckscanf = Entrada_palavra();
  preco = atof(fvckscanf);
  estoque[codigo].preco = preco;
}

void Venda(double *saldo, produto *estoque) {
  int codigo = 0;
  double acum_saldo = 0.0;
  while (codigo != (-1)) {
    scanf("%d", &codigo);
    getchar();
    // printf("codigo:%d", codigo);
    if (estoque[codigo].quantidade == 0 || codigo == -1) {
      continue;
    }
    estoque[codigo].quantidade--;
    acum_saldo += estoque[codigo].preco;
    printf("%s %.02lf\n", estoque[codigo].nome, estoque[codigo].preco);
  }
  *saldo += acum_saldo;
  printf("Total: %.02f\n", acum_saldo);
  Imprime_divisoria();
}

void Consulta_estoque(int nprodutos, produto *estoque) {
  for (int i = 0; i < nprodutos; i++) {
    printf("%d %s %d\n", i, estoque[i].nome, estoque[i].quantidade);
  }
  Imprime_divisoria();
}

void Consulta_saldo(double saldo) {
  printf("Saldo: %.02lf\n", saldo);
  Imprime_divisoria();
}

/* Funcao para abrir o arquivo e escrever nele se ele ja existir, senao, o
 * programa pergunta a quantidade de produtos e o saldo */
FILE *Ler_arquivo(int *nprodutos, double *saldo) {
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
  database = freopen("database.bin", "w+b", database);
  fwrite(&nprodutos, sizeof(int), 1, database);
  fwrite(&saldo, sizeof(double), 1, database);
  fwrite(estoque, sizeof(produto), nprodutos, database);
  fclose(database);
  exit(0);
}

// Funcao que vai ler a struct produto
produto *Ler_estoque(int nprodutos, FILE *stream) {
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
      estoque = Insere_produto(estoque,
                               &n_produtos); // IP <nome> <quantidade> <preco>
      break;
    case 2:
      Aumenta_estoque(estoque);
      break;
    case 3:
      Modifica_preco(estoque);
      break;
    case 4:
      Venda(&saldo, estoque);
      break;
    case 5:
      Consulta_estoque(n_produtos, estoque);
      break;
    case 6:
      Consulta_saldo(saldo);
      break;
    case 7:
      Finalizar_dia(n_produtos, saldo, estoque, database);
      break;
    default:
      printf("ler dnv\n");
      // funcionalidade = Identifica_funcionalidade();
    }
  }
}
