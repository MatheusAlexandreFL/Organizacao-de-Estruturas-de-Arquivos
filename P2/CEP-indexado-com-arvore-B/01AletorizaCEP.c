#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

int main() {
  Endereco e;
  int qt;
  FILE *f = fopen("cep.dat", "rb");
  if (!f) {
    fprintf(stderr, "Erro ao abrir o arquivo 'cep.dat'.\n");
    return 1;
  }
  qt = fread(&e, sizeof(Endereco), 1, f);

  FILE *cep1 = fopen("cep-1.dat", "wb");
  FILE *cep2 = fopen("cep-2.dat", "wb");
  srand(time(NULL));
  while (qt > 0) {
    float random_num = 1.0 * rand() / RAND_MAX;
    if (random_num < 0.8) {
      fwrite(&e, sizeof(Endereco), 1, cep1);
    }
    qt = fread(&e, sizeof(Endereco), 1, f);
  }
  srand(time(NULL) + 1);
  fseek(f, 0, SEEK_SET);
  qt = fread(&e, sizeof(Endereco), 1, f);
  while (qt > 0) {
    float random_num = 1.0 * rand() / RAND_MAX;
    if (random_num < 0.8) {
      fwrite(&e, sizeof(Endereco), 1, cep2);
    }
    qt = fread(&e, sizeof(Endereco), 1, f);
  }
  fclose(f);
  fclose(cep1);
  fclose(cep2);
  return 0;
}
