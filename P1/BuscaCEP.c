    #include <stdio.h>
#include <string.h>

typedef struct _Endereco Endereco;

struct _Endereco
{
	char logradouro[72];
	char bairro[72];
	char cidade[72];  
	char uf[72];
	char sigla[2];
	char cep[8];
	char lixo[2]; 
};

int main(int argc, char**argv)
{
	FILE *f;
	Endereco e;
	int qt;
	int c;

	if(argc != 2)
	{
		fprintf(stderr, "USO: %s [CEP]", argv[0]);
		return 1;
	}

	c = 0;
	printf("Tamanho da Estrutura: %ld\n\n", sizeof(Endereco));
	f = fopen("cep_ordenado.dat","rb");
	if (!f) {
        perror("Erro ao abrir arquivo");
        return 1;
    }
	fseek(f, 0, SEEK_END);
	long tamanhoBytes = ftell(f);
    long totalRegistros = tamanhoBytes / sizeof(Endereco);
	qt = fread(&e,sizeof(Endereco),1,f);
	int inicio = 0;
    int fim = totalRegistros - 1;

    while (inicio <= fim) {
        c++;
        int meio = (inicio + fim) / 2;

        
        fseek(f, meio * sizeof(Endereco), SEEK_SET);
        fread(&e, sizeof(Endereco), 1, f);
		// argv[1] < e.cep  => strcmp(argv[1],e.cep) < 0
		// argv[1] > e.cep  => strcmp(argv[1],e.cep) > 0
		// argv[1] == e.cep  => strcmp(argv[1],e.cep) == 0
		int comparacao = strncmp(argv[1], e.cep, 8);
		if (comparacao == 0) {
            printf("Encontrado em %d tentativas!\n", c);
            printf("%.72s\n%.72s\n%.72s\n%.72s\n%.2s\n%.8s\n", 
                    e.logradouro, e.bairro, e.cidade, e.uf, e.sigla, e.cep);
            fclose(f);
            return 0;
        } else if (comparacao < 0) {
            fim = meio - 1;
        } else {
            inicio = meio + 1; 
        }
    }

    printf("CEP não encontrado. Total de leituras: %d\n", c);
    fclose(f);
    return 0;
}

