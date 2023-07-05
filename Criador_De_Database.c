#include<stdio.h>
#include<stdlib.h>
//Esse codigo permite criar arquivos no formato convencionado para o trabalho, a fim de realizar testes
typedef struct produto {
  int quantidade;
  float preco;
  char nome[30];
} produto;

void putint(FILE *db){
    int n;
    scanf("%d", &n);
    fwrite(&n, sizeof(int), 1, db);
}


void putfloat(FILE *db){
    float n;
    scanf("%f", &n);
    fwrite(&n, sizeof(float), 1, db);
}
void putstruct(FILE *db){
    produto a;
    scanf("%d %f %s" , &a.quantidade, &a.preco, a.nome);
    fwrite(&a, sizeof(produto), 1, db);
}
void end(FILE *db){
    fclose(db);
    exit(0);
}
int main(){
    int mode;
    FILE *db = fopen("database.bin", "w+b");
    while(1){
        scanf("%d", &mode);  //Para utilizar, basta escolher algo para inserir (1 - int/ 2 -float/ 3 - produto/ 4 - finaliza a execucao
        switch(mode){
            case 1:
                putint(db);
                break;
            case 2:
                putfloat(db);
                break;
            case 3:
                putstruct(db);
                break;
            case 4:
                end(db);
                break;
        }
    }
}
