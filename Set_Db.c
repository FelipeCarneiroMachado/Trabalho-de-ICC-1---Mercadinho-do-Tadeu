#include<stdio.h>
#include<stdlib.h>

typedef struct produto {
  int quantidade;
  float preco;
  char nome[30];
} produto;

void putint(FILE *db){
    int n;
    scanf("%d", &n);
    fwrite(&n, sizeof(int), 1, db);
    printf("oi");
}


void putfloat(FILE *db){
    float n;
    scanf("%f", &n);
    fwrite(&n, sizeof(float), 1, db);
    printf("oi");
}
void putstruct(FILE *db){
    produto a;
    scanf("%d %f %s" , &a.quantidade, &a.preco, a.nome);
    fwrite(&a, sizeof(produto), 1, db);
    printf("oi");
}
void end(FILE *db){
    fclose(db);
    printf("oi");
    exit(0);
}



int main(){
    int mode;
    FILE *db = fopen("database.bin", "w+b");
    while(1){
        scanf("%d", &mode);
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