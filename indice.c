#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct {
    char logradouro[72];
    char bairro[72];
    char cidade[72];
    char uf[72];
    char sigla[2];
    char cep[8];
    char lixo[2]; 
} Endereco;

typedef struct {
    char cep[8];
    long posicao; 
} IndiceEndereco;


int comparaIndice(const void *a, const void *b) {
    IndiceEndereco *i1 = (IndiceEndereco *)a;
    IndiceEndereco *i2 = (IndiceEndereco *)b;
    return strncmp(i1->cep, i2->cep, 8);
}

void gerarIndice() {
    FILE *cep = fopen("cep.dat", "rb");
    if (!cep) {
        printf("Erro: Arquivo cep.dat nao encontrado.\n");
        return;
    }


    fseek(cep, 0, SEEK_END);
    long total_registros = ftell(cep) / sizeof(Endereco);
    rewind(cep);

    IndiceEndereco *vetor_indice = malloc(total_registros * sizeof(IndiceEndereco));
    if (!vetor_indice) {
        printf("Erro: RAM insuficiente para gerar indice.\n");
        fclose(cep);
        return;
    }

    Endereco temp;
    long i = 0;
    
    while (fread(&temp, sizeof(Endereco), 1, cep) == 1) {
        strncpy(vetor_indice[i].cep, temp.cep, 8);
        vetor_indice[i].posicao = i * sizeof(Endereco); 
        i++;
    }
    fclose(cep);

    qsort(vetor_indice, total_registros, sizeof(IndiceEndereco), comparaIndice);

    FILE *ind = fopen("indiceOrdenado.dat", "wb");
    fwrite(vetor_indice, sizeof(IndiceEndereco), total_registros, ind);
    fclose(ind);
    free(vetor_indice);

}

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "USO: %s [CEP]\n", argv[0]);
        return 1;
    }

    gerarIndice();

    FILE *ind = fopen("indice.dat", "rb");
    FILE *cep = fopen("cep.dat", "rb");

    if (!ind || !cep) {
        perror("Erro ao abrir os arquivos para busca");
        return 1;
    }

    fseek(ind, 0, SEEK_END);
    long total_indices = ftell(ind) / sizeof(IndiceEndereco);
    
    long inicio = 0;
    long final = total_indices - 1;
    
    IndiceEndereco ie_temp;
    Endereco e;
    int encontrou = 0;


    while (inicio <= final) {
        long meio = (inicio + final) / 2;
        
        fseek(ind, meio * sizeof(IndiceEndereco), SEEK_SET);
        fread(&ie_temp, sizeof(IndiceEndereco), 1, ind);
        
        int cmp = strncmp(argv[1], ie_temp.cep, 8);

        if (cmp == 0) { 
            fseek(cep, ie_temp.posicao, SEEK_SET);
            fread(&e, sizeof(Endereco), 1, cep);
            
            printf("\n=== CEP ENCONTRADO ===\n");
            printf("%.72s\n%.72s\n%.72s\n%.72s\n%.2s\n%.8s\n", 
                   e.logradouro, e.bairro, e.cidade, e.uf, e.sigla, e.cep);
            
            encontrou = 1;
            break; 
        }
        else if (cmp > 0) { 
            inicio = meio + 1; 
        }
        else { 
            final = meio - 1;  
        }
    }

    if (!encontrou) {
        printf("\nCEP nao encontrado no banco de dados.\n");
    }



    fclose(ind);
    fclose(cep);
    return 0;
}
