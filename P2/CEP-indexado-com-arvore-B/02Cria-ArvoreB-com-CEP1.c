#include "ArvoreB.h"
#include <stdio.h>

typedef struct _Endereco Endereco;

struct _Endereco {
  char logradouro[72];
  char bairro[72];
  char cidade[72];
  char uf[72];
  char sigla[2];
  char cep[8];
  char lixo[2];
};

int main(int argc, char **argv) {
  ArvoreB *a = ArvoreB_Abre("arvore.dat");
  Endereco e;
  FILE *f = fopen("cep-1.dat", "rb");
  char chave[TAM_CHAVE + 1];
  long posicao = 0;
  int qtd;
  qtd = fread(&e, sizeof(Endereco), 1, f);
  while (qtd > 0) {
    ArvoreB_Insere(a, e.cep, posicao);
    qtd = fread(&e, sizeof(Endereco), 1, f);
    posicao++;
  }

  // ArvoreB_PrintDebug(a);

  ArvoreB_Fecha(a);
  fclose(f);
  return 0;
}
