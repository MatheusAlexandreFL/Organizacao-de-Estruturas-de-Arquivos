#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int compara(const void *a, const void *b) {
    Endereco *e1 = (Endereco *)a;
    Endereco *e2 = (Endereco *)b;
    return strncmp(e1->cep, e2->cep, 8);
}

void intercala(char *nome1, char *nome2, char *nome_saida) {
    FILE *f1 = fopen(nome1, "rb");
    FILE *f2 = fopen(nome2, "rb");
    FILE *fs = fopen(nome_saida, "wb");
    
    if (!f1 || !f2 || !fs) return;
    
    Endereco e1, e2;
    int leu1 = fread(&e1, sizeof(Endereco), 1, f1);
    int leu2 = fread(&e2, sizeof(Endereco), 1, f2);
    
    while (leu1 && leu2) {
        if (strncmp(e1.cep, e2.cep, 8) <= 0) {
            fwrite(&e1, sizeof(Endereco), 1, fs);
            leu1 = fread(&e1, sizeof(Endereco), 1, f1);
        } else {
            fwrite(&e2, sizeof(Endereco), 1, fs);
            leu2 = fread(&e2, sizeof(Endereco), 1, f2);
        }
    }
    
    while (leu1) {
        fwrite(&e1, sizeof(Endereco), 1, fs);
        leu1 = fread(&e1, sizeof(Endereco), 1, f1);
    }
    
    while (leu2) {
        fwrite(&e2, sizeof(Endereco), 1, fs);
        leu2 = fread(&e2, sizeof(Endereco), 1, f2);
    }
    
    fclose(f1);
    fclose(f2);
    fclose(fs);
}

int main(int argc, char** argv) {
    if (argc != 2) return 1;
    
    int k = atoi(argv[1]);
    if (k <= 0) return 1;

    FILE *f = fopen("cep_a.dat", "rb");
    if (!f) return 1;
    
    fseek(f, 0, SEEK_END);
    long total = ftell(f) / sizeof(Endereco);
    rewind(f);
    
    long qtb = total / k;
    long sobra = total % k;
    
    char **nomes = malloc(k * sizeof(char*));
    
    for (int i = 0; i < k; i++) {
        nomes[i] = malloc(30);
        sprintf(nomes[i], "b_%d.dat", i);
        
        long qt = (i < sobra) ? qtb + 1 : qtb;
        Endereco *buf = malloc(qt * sizeof(Endereco));
        
        fread(buf, sizeof(Endereco), qt, f);
        qsort(buf, qt, sizeof(Endereco), compara);
        
        FILE *fb = fopen(nomes[i], "wb");
        fwrite(buf, sizeof(Endereco), qt, fb);
        fclose(fb);
        free(buf);
    }
    fclose(f);
    
    int ativos = k;
    int rodada = 0;
    
    while (ativos > 1) {
        int proximo_ativos = (ativos + 1) / 2;
        char **novos_nomes = malloc(proximo_ativos * sizeof(char*));
        
        for (int i = 0, j = 0; i < ativos; i += 2, j++) {
            novos_nomes[j] = malloc(30);
            
            if (i + 1 < ativos) {
                sprintf(novos_nomes[j], "m_%d_%d.dat", rodada, j);
                intercala(nomes[i], nomes[i+1], novos_nomes[j]);
                remove(nomes[i]);
                remove(nomes[i+1]);
            } else {
                strcpy(novos_nomes[j], nomes[i]);
            }
        }
        
        for (int i = 0; i < ativos; i++) free(nomes[i]);
        free(nomes);
        
        nomes = novos_nomes;
        ativos = proximo_ativos;
        rodada++;
    }
    
    remove("cep_b.dat");
    rename(nomes[0], "cep_b.dat");
    
    free(nomes[0]);
    free(nomes);
    
    return 0;
}
