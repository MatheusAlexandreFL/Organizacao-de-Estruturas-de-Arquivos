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
  FILE *f = fopen("cep-2.dat", "rb");
  FILE *s = fopen("cep-join.dat", "wb");
  Endereco e;
  long posicao;

  while (fread(&e, sizeof(Endereco), 1, f) > 0) {
    posicao = ArvoreB_Busca(a, e.cep); // Existe na árvore
    if (posicao != -1) {
      fwrite(&e, sizeof(Endereco), 1, s);
    }
  }
  ArvoreB_Fecha(a);
  fclose(f);
  fclose(s);
  return 0;
}
