# Processamento de CSV da Covid

Arquivos:

  - 01ProcessamentoCSVCovid.c
  - CSVParser.c
  - CSVParser.h
O programa lê owid-covid-data.csv e calcula o total de casos e mortes na América do Sul.
```
cd P2\Processamento-de-CSV-Covid
gcc 01ProcessamentoCSVCovid.c CSVParser.c -o 01ProcessamentoCSVCovid.exe
.\01ProcessamentoCSVCovid.exe
```
Arquivo necessário nesta pasta:
 - owid-covid-data.csv
# CEP indexado com Árvore B

Esta parte usa cep.dat para criar dois subconjuntos de dados, indexar o primeiro em uma Árvore B e verificar quais CEPs do segundo subconjunto também existem no primeiro.

Arquivos:
 - 01AleatorizaCep.c: cria cep-1.dat e cep-2.dat a partir de cep.dat.
 - 02CriaArvoreBcomCep1.c: cria/atualiza arvore.dat, indexando os CEPs de cep-1.dat.
 - 03JoinArvoreBcomCep.c: lê cep-2.dat, busca cada CEP em arvore.dat e grava os encontrados em cep-join.dat.
 - ArvoreB.c e ArvoreB.h: implementação da Árvore B usada pelos programas principais.
 - ArvoreB-Busca.c: consulta manualmente um CEP específico em arvore.dat.

### Fluxo principal
```
cd P2\CEP-indexado-com-arvore-B

# Cria cep-1.dat e cep-2.dat
gcc 01AleatorizaCep.c -o 01AleatorizaCep.exe
.\01AleatorizaCep.exe

# Cria a Árvore B usando cep-1.dat
gcc 02CriaArvoreBcomCep1.c ArvoreB.c -o 02CriaArvoreBcomCep1.exe
.\02Cria-ArvoreB-com-Cep1.exe

# Gera cep-join.dat com os CEPs de cep-2.dat encontrados na Árvore B
gcc 03JoinArvoreBcomCep.c ArvoreB.c -o 03JoinArvoreBcomCep.exe
.\03Join-ArvoreB-com-Cep.exe
```

Arquivos necessários nesta pasta:
  - cep.dat, antes de executar 01AleatorizaCep.c
  - cep-1.dat e cep-2.dat, gerados por 01AleatorizaCep.c
  - arvore.dat, gerado por 02CriaArvoreBcomCep1.c

### BuscaCEP: testar um CEP específico

O arquivo ArvoreB-Busca.c pode ser compilado para consultar um CEP diretamente em arvore.dat.
```
cd P2\CEP-indexado-com-arvore-B
gcc ArvoreB-Busca.c -o buscaCEP.exe
.\buscaCEP.exe 69907060
```
Antes de testar a busca, arvore.dat precisa existir. Para isso, execute primeiro:
```
.\02CriaArvoreBcomCep1.exe
```
O retorno do buscaCEP.exe indica a posição do CEP dentro de cep-1.dat:
  - -1: CEP não encontrado.
  - 0: CEP encontrado na primeira posição.
  - Qualquer número positivo, como 310295: CEP encontrado nessa posição do arquivo.
Exemplos:
```
.\buscaCEP.exe 69907060
.\buscaCEP.exe 23013480
.\buscaCEP.exe 12280102
.\buscaCEP.exe 00000000
```

