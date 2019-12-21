#include <iostream>
#include <stdio.h>
#include <memory.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#include "prc.h"

#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

// PESOS PARA PENALIZAÇÃO DA FO

int TEMPMAX = 300;

int OM1 = 100;
int OM2 = 1000;
int OM3 = 50;
int OM4 = 1000;
int OM5 = 1000;

clock_t h;
double tempo;

int main(int argc, char *argv[])
{
    srand(time(NULL));

    //Variaveis:
    // Horizonte de Tempo:
    param = 2;
    PERIODO = param * 12;

    char aux[50];
    Solucao solucao;
    Solucao clone;
    Solucao solucaoExterna;

    //Ler dos dados do Problema:

    strcpy(aux, "lotes20.txt");
    lerDadosLote(aux);

    strcpy(aux, "culturas.txt");
    lerDadosCultura(aux);

    // Pré Operacoes:
    calcularCicloCulturas();
    ordenarDados();

    // ## ----------------------------------- ## //

    int tLRC = 40; // Percentual escolhido;

    heuGrasp(solucao, tLRC);
}

// Ler dados do LOTE:

void lerDadosLote(const char *arq)
{
    FILE *f = fopen(arq, "r");
    fscanf(f, "%d ", &quantidadeLotes);

    for (int i = 0; i < quantidadeLotes; i++)
        fscanf(f, "%f ", &vetAreaLotes[i]);

    for (int i = 0; i < quantidadeLotes; i++)
    {
        fscanf(f, "%d", &vetQtdVizinhos[i]);
        for (int j = 0; j < vetQtdVizinhos[i]; j++)
        {
            fscanf(f, "%d ", &matLotesVizinhos[i][j]);
        }
    }

    fclose(f);
}
// Ler dados das CULTURAS:

void lerDadosCultura(const char *arq)
{
    FILE *f = fopen(arq, "r");
    fscanf(f, "%d %d \n", &pousio, &adubacaoVerde);
    for (int i = 0; i < MAX_CULTURA; i++)
    {
        fscanf(f, "%d %d %d %d %d %d", &vetCulturas[i].cultura, &vetCulturas[i].familia, &vetCulturas[i].inicioPlantio,
               &vetCulturas[i].fimPlantio, &vetCulturas[i].ciclo, &vetCulturas[i].lucratividade);
    }

    fclose(f);
}

// Lendo uma solucão qualquer
Solucao lerSolucao(const char *arq, int L, int P)
{
    Solucao s;
    FILE *f = fopen(arq, "r");
    fscanf(f, "%f", &s.funcObj);
    for (int i = 0; i < L; i++)
    {
        for (int j = 0; j < P; j++)
        {
            fscanf(f, "%d", &s.matSolucao[i][j]);
        }
    }
    fclose(f);

    return s;
}

// Imprimir Dados LOTES E CULTURAS:

void imprimeDados()
{
    printf("%d \n", quantidadeLotes);

    for (int i = 0; i < quantidadeLotes; i++)
        printf("%.2f ", vetAreaLotes[i]);
    printf("\n");

    printf("%d  %d", pousio, adubacaoVerde);
    printf("\n");
    for (int i = 0; i < MAX_CULTURA; i++)
    {
        printf("%d %d %d %d %d %d \n", i, vetCulturas[i].cultura, vetCulturas[i].familia, vetCulturas[i].inicioPlantio,
               vetCulturas[i].fimPlantio, vetCulturas[i].ciclo, vetCulturas[i].lucratividade);
    }

    for (int i = 0; i < quantidadeLotes; i++)
    {
        printf("%d\n", vetQtdVizinhos[i]);
        for (int j = 0; j < vetQtdVizinhos[i]; j++)
        {

            printf("%d  ", matLotesVizinhos[i][j]);
        }
        printf("\n");
    }
}

// Calcular Ciclo das Culturas:

void calcularCicloCulturas()
{
    for (int i = 0; i < MAX_CULTURA; i++)
        vetCicloCultura[i] = ceil(vetCulturas[i].ciclo / 30.0);
}

// Clonar uma solucao:

void clonarSolucao(Solucao &solucao, Solucao &clone)
{
    memcpy(&clone, &solucao, sizeof(solucao));
}

// Solução Gulosa Aleatoria, com LRC;
void heuConsGulosaAleatoria(Solucao &solucao, int LRC)
{

    // Definindo quantos elementos irao ser selecionados das melhores CULTURAS;
    int tamanho = 0;
    int perc = ceil((LRC / 100.0) * MAX_CULTURA);

    tamanho = MAX(3, perc);

    int periodo, cultura, ciclo, opcao;
    int adbVerde;

    memset(&solucao, 0, sizeof(solucao));

    for (int i = 0; i < quantidadeLotes; i++)
    {
        for (int j = 0; j < PERIODO; j++)
        {
            if (solucao.matSolucao[i][j] == 0)
            {
                solucao.matSolucao[i][j] = 28;
            }
        }
    }
    // Adubação VERDE:
    for (int i = 0; i < quantidadeLotes; i++)
    {
        // Seleciona entre as 3 adubações verde possíveis.

        adbVerde = MAX_CULTURA - ((rand() % 3) + 1);

        switch (adbVerde)
        {

        case 25:
            // Neste caso não foi necessário realizar um tratamento para 12 ou 24 meses
            periodo = PERIODO - ((rand() % 4) + 7);

            solucao.matSolucao[i][periodo] = 25;
            solucao.matSolucao[i][periodo + 1] = 25;
            solucao.matSolucao[i][periodo + 2] = 25;
            solucao.matSolucao[i][periodo + 3] = 25;
            solucao.matSolucao[i][periodo + 4] = 25;
            break;
        case 26:
            // Neste caso em particular precisei fazer a divisão entre 12 e 24 MESES.
            if (PERIODO == 12)
            {
                periodo = PERIODO - ((rand() % 4) + 1);

                switch (periodo)
                {
                case 8:
                    solucao.matSolucao[i][periodo] = 26;
                    solucao.matSolucao[i][periodo + 1] = 26;
                    solucao.matSolucao[i][periodo + 2] = 26;
                    break;

                case 9:
                    solucao.matSolucao[i][periodo] = 26;
                    solucao.matSolucao[i][periodo + 1] = 26;
                    solucao.matSolucao[i][periodo + 2] = 26;
                    break;

                case 10:
                    solucao.matSolucao[i][periodo] = 26;
                    solucao.matSolucao[i][periodo + 1] = 26;
                    solucao.matSolucao[i][periodo - periodo] = 26;
                    break;

                case 11:
                    solucao.matSolucao[i][periodo] = 26;
                    solucao.matSolucao[i][periodo - periodo] = 26;
                    solucao.matSolucao[i][(periodo + 1) - periodo] = 26;
                    break;

                default:
                    break;
                }
            }

            if (PERIODO == 24)
            {
                opcao = rand() % 2;
                //int opcao = 0;
                if (opcao == 1)
                {
                    periodo = 12 - ((rand() % 4) + 1);

                    solucao.matSolucao[i][periodo] = 26;
                    solucao.matSolucao[i][periodo + 1] = 26;
                    solucao.matSolucao[i][periodo + 2] = 26;
                    break;
                }
                else
                {
                    periodo = PERIODO - ((rand() % 4) + 1);
                    switch (periodo)
                    {
                    case 20:
                        solucao.matSolucao[i][periodo] = 26;
                        solucao.matSolucao[i][periodo + 1] = 26;
                        solucao.matSolucao[i][periodo + 2] = 26;
                        break;

                    case 21:
                        solucao.matSolucao[i][periodo] = 26;
                        solucao.matSolucao[i][periodo + 1] = 26;
                        solucao.matSolucao[i][periodo + 2] = 26;
                        break;

                    case 22:
                        solucao.matSolucao[i][periodo] = 26;
                        solucao.matSolucao[i][periodo + 1] = 26;
                        solucao.matSolucao[i][periodo - periodo] = 26;
                        break;

                    case 23:
                        solucao.matSolucao[i][periodo] = 26;
                        solucao.matSolucao[i][periodo - periodo] = 26;
                        solucao.matSolucao[i][(periodo + 1) - periodo] = 26;
                        break;

                    default:
                        break;
                    }
                }

            case 27:
                // Aqui novamente o método de colocar a adubação verde é diferente se considerarmos 24 ou 12 meses
                if (PERIODO == 12)
                {
                    periodo = PERIODO - ((rand() % 5) + 1);

                    switch (periodo)
                    {
                    case 8:
                        solucao.matSolucao[i][periodo] = 27;
                        solucao.matSolucao[i][periodo + 1] = 27;
                        solucao.matSolucao[i][periodo + 2] = 27;
                        solucao.matSolucao[i][periodo + 3] = 27;
                        break;

                    case 9:
                        solucao.matSolucao[i][periodo] = 27;
                        solucao.matSolucao[i][periodo + 1] = 27;
                        solucao.matSolucao[i][periodo + 2] = 27;
                        solucao.matSolucao[i][0] = 27;
                        break;

                    case 10:
                        solucao.matSolucao[i][periodo] = 27;
                        solucao.matSolucao[i][periodo + 1] = 27;
                        solucao.matSolucao[i][0] = 27;
                        solucao.matSolucao[i][1] = 27;
                        break;

                    case 11:
                        solucao.matSolucao[i][periodo] = 27;
                        solucao.matSolucao[i][0] = 27;
                        solucao.matSolucao[i][1] = 27;
                        solucao.matSolucao[i][2] = 27;
                        break;

                    case 7:
                        solucao.matSolucao[i][0] = 27;
                        solucao.matSolucao[i][1] = 27;
                        solucao.matSolucao[i][2] = 27;
                        solucao.matSolucao[i][3] = 27;
                        break;

                    default:
                        break;
                    }
                }
                if (PERIODO == 24)
                {
                    int opcao;
                    opcao = rand() % 2;

                    if (opcao == 0)
                    {
                        periodo = 12 - ((rand() % 5) + 1);

                        solucao.matSolucao[i][periodo] = 27;
                        solucao.matSolucao[i][periodo + 1] = 27;
                        solucao.matSolucao[i][periodo + 2] = 27;
                        solucao.matSolucao[i][periodo + 3] = 27;
                    }
                    else
                    {

                        if (PERIODO == 24)
                        {
                            periodo = PERIODO - ((rand() % 5));

                            switch (periodo)
                            {
                            case 20:
                                solucao.matSolucao[i][periodo] = 27;
                                solucao.matSolucao[i][periodo + 1] = 27;
                                solucao.matSolucao[i][periodo + 2] = 27;
                                solucao.matSolucao[i][periodo + 3] = 27;
                                break;

                            case 21:
                                solucao.matSolucao[i][periodo] = 27;
                                solucao.matSolucao[i][periodo + 1] = 27;
                                solucao.matSolucao[i][periodo + 2] = 27;
                                solucao.matSolucao[i][periodo - periodo] = 27;
                                break;

                            case 22:
                                solucao.matSolucao[i][periodo] = 27;
                                solucao.matSolucao[i][periodo + 1] = 27;
                                solucao.matSolucao[i][periodo - periodo] = 27;
                                solucao.matSolucao[i][(periodo - periodo) + 1] = 27;
                                break;

                            case 23:
                                solucao.matSolucao[i][periodo] = 27;
                                solucao.matSolucao[i][0] = 27;
                                solucao.matSolucao[i][1] = 27;
                                solucao.matSolucao[i][2] = 27;
                                break;

                            case 24:
                                periodo = 0;
                                solucao.matSolucao[i][periodo] = 27;
                                solucao.matSolucao[i][periodo + 1] = 27;
                                solucao.matSolucao[i][periodo + 2] = 27;
                                solucao.matSolucao[i][periodo + 3] = 27;
                                break;

                            default:
                                break;
                            }
                        }
                    }
                }

            default:
                break;
            }
        }
    }

    int mes = 0;
    int index = 0, inicio, fim;

    // Colocando as culturas, respeitando o seu ciclo e seu período de Plantio;

    for (int i = 0; i < quantidadeLotes; i++)
    {
        mes = 0;
        while (mes < PERIODO)
        {
            if (solucao.matSolucao[i][mes] == 28)
            {
                //Influencia do LRC na escolha das Culturas:
                // Escolhendo aleatoriamente dentre as opcoes disponiveis, de acordo com o LRC passado;
                index = rand() % tamanho;

                cultura = vetCulturas[vetIndiceCulturasOrdenadas[index]].cultura;
                ciclo = vetCicloCultura[vetIndiceCulturasOrdenadas[index]];
                inicio = vetCulturas[vetIndiceCulturasOrdenadas[index]].inicioPlantio - 1;
                fim = vetCulturas[vetIndiceCulturasOrdenadas[index]].fimPlantio - 1;

                // Uma vez selecionada a cultura, ele verifica se a mesma está apta a ser plantada no MES vigente, respeitando seu inicio e fim de cultivo.
                if (mes >= inicio && mes <= fim)
                {
                    // Enquanto não terminar o ciclo, ele coloca a cultura selecionada
                    // Porém, somente nos espaços onde existe primeiramente "Pousio";
                    // Sendo assim, pode ocorrer uma quebra de ciclo, caso haja alguma adubação verde no caminho

                    int nCabe = 0;
                    int cicloAux = ciclo;
                    int mesAux = mes;

                    // Verificando se existe espaço para plantar a Cultura respeitando o Ciclo;
                    // Utilizando uma variável "nCabe", toda vez que um ciclo é quebrado, é incrementado um valor a esta variável!

                    while (ciclo != 0)
                    {
                        if (solucao.matSolucao[i][mes] == 28)
                        {
                            ciclo--;
                            mes++;
                        }
                        else
                        {
                            ciclo--;
                            mes++;
                            nCabe++;
                        }
                    }
                    // Se nCabe não sofreu nenhum incremento, significa que aquela cultura certamente caberá naquele espaço, respeitando o seu ciclo de maneira correta!

                    if (nCabe == 0)
                    {
                        while (cicloAux != 0)
                        {
                            solucao.matSolucao[i][mesAux] = cultura;
                            cicloAux--;
                            mesAux++;
                        }
                    }
                }
                else
                {
                    mes++;
                }
            }
            else
            {
                mes++;
            }
        }
    }

    verificarSolucao(solucao);
    calcularFO(solucao);
}

// Escrever a solução no Terminal (TELA):

void escreverSolucao(Solucao &solucao)
{
    printf("FO: %.2f\n", solucao.funcObj);
    for (int i = 0; i < quantidadeLotes; i++)
    {
        for (int j = 0; j < PERIODO; j++)
        {
            printf(" [%d] ", solucao.matSolucao[i][j]);
        }
        printf("\n");
    }
    printf("\n ---- \n");
}

// Escrever a Solucao em Arquivo Externo: ("solucaoDetalhe.txt")

void escreverSolucaoArquivo(Solucao &solucao)
{
    FILE *f = fopen("solucaoDetalhe.txt", "w");

    // Funcao Objetiva
    fprintf(f, "%.2f\n", solucao.funcObj);

    // Lotes x Periodos com Culturas da rotação
    for (int i = 0; i < quantidadeLotes; i++)
    {
        for (int j = 0; j < PERIODO; j++)
        {
            fprintf(f, "%d  ", solucao.matSolucao[i][j]);
        }
        fprintf(f, "\n");
    }

    fprintf(f, "\n");

    // Quantidade Total de Pousios da Rotação:
    fprintf(f, "%d \n", po);

    // Quantidade de Pousios por Lote:
    for (int i = 0; i < quantidadeLotes; i++)
    {
        fprintf(f, "%d  ", vetPousioLote[i]);
    }

    fprintf(f, "\n");

    // Quantidade TOTAL de Familia com Conflitos em Lotes Adjacente:
    fprintf(f, "%d \n", pa);

    // Quantidade de Familia com Conflitos POR LOTE:
    for (int i = 0; i < quantidadeLotes; i++)
    {
        fprintf(f, "%d  ", vetCultAdjacente[i]);
    }
    fprintf(f, "\n");

    // Lucratividade Total da Rotação:
    fprintf(f, "%.2f \n", lt);

    // Lucratividade Por Lote:
    for (int i = 0; i < quantidadeLotes; i++)
    {
        fprintf(f, "%.2f  ", vetLucratividadeLote[i]);
    }

    fclose(f);
}

// Ordenacao dos Dados: (Lucratividade / Ciclo) ;

void ordenarDados()
{
    int flag, auxiliar;

    for (int i = 0; i < MAX_CULTURA; i++)
        vetIndiceCulturasOrdenadas[i] = i;

    flag = 1;

    while (flag)
    {
        flag = 0;
        for (int i = 0; i < MAX_CULTURA - 1; i++)
        {
            if ((double)(vetCulturas[vetIndiceCulturasOrdenadas[i]].lucratividade / vetCicloCultura[vetIndiceCulturasOrdenadas[i]]) < (double)(vetCulturas[vetIndiceCulturasOrdenadas[i + 1]].lucratividade / vetCicloCultura[vetIndiceCulturasOrdenadas[i + 1]]))
            {
                flag = 1;
                auxiliar = vetIndiceCulturasOrdenadas[i];
                vetIndiceCulturasOrdenadas[i] = vetIndiceCulturasOrdenadas[i + 1];
                vetIndiceCulturasOrdenadas[i + 1] = auxiliar;
            }
        }
    }

    // for (int i = 0; i < MAX_CULTURA; i++);

    // printf("%d , ", vetCulturas[vetIndiceCulturasOrdenadas[i]]);
}

// Verificando a solucao gerada:

void verificarSolucao(Solucao &solucao)
{

    memset(&vetEpocaFora, 0, sizeof(vetEpocaFora));
    memset(&vetAdbVerdeLote, 0, sizeof(vetAdbVerdeLote));
    memset(&vetCicloFora, 0, sizeof(vetCicloFora));
    memset(&vetCultAdjacente, 0, sizeof(vetCultAdjacente));
    memset(&vetPousioLote, 0, sizeof(vetPousioLote));
    memset(&vetFamiliaAdjacente, 0, sizeof(vetFamiliaAdjacente));
    memset(&vetLucratividadeLote, 0, sizeof(vetLucratividadeLote));

    // Verificando os Lotes adjacentes:

    for (int i = 0; i < quantidadeLotes; i++)
    {
        int qtdVizinhos = vetQtdVizinhos[i];

        for (int q = 0; q < qtdVizinhos; q++)
        {
            int vizinho = matLotesVizinhos[i][q] - 1;

            for (int j = 0; j < PERIODO; j++)
            {
                if (vetCulturas[solucao.matSolucao[i][j] - 1].familia == vetCulturas[solucao.matSolucao[vizinho][j] - 1].familia && solucao.matSolucao[i][j] != 28)
                {
                    // Neste printf ele detalha os Lotes com familia em conflito, a cultura e o periodo;
                    // Bastando Tirar o comentário do Printf;

                    //printf("Conflito no Lote %d com o Lote %d, nas Culturas: %d / %d, no Periodo: %d\n", i, vizinho, solucao.matSolucao[i][j], solucao.matSolucao[vizinho][j], j);

                    vetCultAdjacente[i] += 1;
                }
                else
                {
                    // Neste printf ele lista as adjacências que estão 'OK', sem conflito:

                    //printf("NAO ha conflito: Lote i: %d - Lote v: %d, %d / %d\n", i, vizinho, solucao.matSolucao[i][j], solucao.matSolucao[vizinho][j]);
                }
            }
        }
    }

    // Verificando Lotes Adjacentes por Lote:
    for (int i = 0; i < quantidadeLotes; i++)
    {
        if (vetCultAdjacente[i] != 0)
        {
            pa += vetCultAdjacente[i];
        }
    }

    // Verificando os Pousios por Lote:

    for (int i = 0; i < quantidadeLotes; i++)
    {
        for (int j = 0; j < PERIODO; j++)
        {
            if (solucao.matSolucao[i][j] == 28)
            {
                vetPousioLote[i] = 1;
                break;
            }
        }
    }

    // Verificar a Adubacao Verde por Lote:

    for (int i = 0; i < quantidadeLotes; i++)
    {
        for (int j = 0; j < PERIODO; j++)
        {
            if (solucao.matSolucao[i][j] == 26 || solucao.matSolucao[i][j] == 27 || solucao.matSolucao[i][j] == 25)
            {
                vetAdbVerdeLote[i] = 1;
                break;
            }
        }
    }

    // Verificando Cultura Fora de Época:

    int cultura = 0, culturaAux = 0;
    int inicio = 0;
    int fim = 0;
    int ciclo = 0, cicloAux = 0;
    int mes = 0, mesAux = 0;
    int aux = 0;

    for (int i = 0; i < quantidadeLotes; i++)
    {
        mes = 0;
        mesAux = 0;
        cicloAux = 0;

        while (mes < PERIODO)
        {
            mesAux = mes;
            culturaAux = solucao.matSolucao[i][mes];
            ciclo = vetCicloCultura[culturaAux - 1];
            inicio = vetCulturas[culturaAux - 1].inicioPlantio - 1;
            fim = vetCulturas[culturaAux - 1].fimPlantio - 1;
            cicloAux = 0;

            if (inicio > fim)
            {
                if (mesAux >= 0 && mesAux <= fim || mesAux >= inicio && mesAux <= PERIODO - 1)
                {
                }
                else
                {
                    vetEpocaFora[i] += 1;
                }
            }
            else
            {
                if (mesAux < inicio || mesAux > fim)
                {
                    if (solucao.matSolucao[i][PERIODO - 1] != solucao.matSolucao[i][mesAux])
                    {
                        vetEpocaFora[i] += 1;
                    }
                }
                else
                {
                }
            }

            while (culturaAux == solucao.matSolucao[i][mesAux])
            {
                cicloAux++;

                int cont = 0;

                if ((mesAux) == PERIODO - 1)
                {

                    while (culturaAux == solucao.matSolucao[i][cont])
                    {
                        cont++;
                        cicloAux++;
                    }
                }

                if ((mesAux) == 0)
                {
                    cont = PERIODO - 1;

                    while (culturaAux == solucao.matSolucao[i][cont])
                    {
                        cont--;
                        cicloAux++;
                    }
                }

                mesAux++;

                // Aqui que eu mexo para resolver a parada do ciclo circular
            }
            mes = mesAux;

            if (culturaAux != 28)
            {
                if (cicloAux != ciclo)
                {
                    vetCicloFora[i] += 1;
                }
            }
        }
    }

    // Verificando Culturas no Mesmo lote plantadas com familias adjacentes

    int mesC = 0;
    int cont = 0;
    for (int i = 0; i < quantidadeLotes; i++)
    {
        while (mesC < PERIODO)
        {
            if (solucao.matSolucao[i][mesC] == 28)
            {
                mesC++;
            }
            else
            {
                int clt = solucao.matSolucao[i][mesC] - 1;
                while (solucao.matSolucao[i][mesC + cont] == (clt + 1))
                {
                    cont++;
                }
                if (vetCulturas[solucao.matSolucao[i][mesC + cont]].familia == vetCulturas[clt].familia)
                {
                    vetFamiliaAdjacente[i] += 1;
                }
                mesC += cont;
                cont = 0;
            }
        }
    }
}

// Calculo da FO
void calcularFO(Solucao &solucao)
{

    solucao.funcObj = 0.0;

    for (int i = 0; i < quantidadeLotes; i++)
    {
        for (int j = 0; j < PERIODO; j++)
        {
            if (solucao.matSolucao[i][j] != 28 && solucao.matSolucao[i][j] != 25 &&
                solucao.matSolucao[i][j] != 26 && solucao.matSolucao[i][j] != 27)
            {
                int cultura = solucao.matSolucao[i][j];
                int ciclo = vetCicloCultura[solucao.matSolucao[i][j] - 1];
                int completo = 0;
                int cont = 0;
                int cicloAux = ciclo;
                int aux = 0;
                while (ciclo != 0)
                {
                    if (solucao.matSolucao[i][j + cont] == cultura)
                    {
                        completo++;
                        ciclo--;
                        cont++;
                    }
                    else if ((j + cont) % PERIODO == 0)
                    {
                        int aux = 0;
                        while (ciclo != 0)
                        {
                            if (solucao.matSolucao[i][aux] == cultura)
                            {
                                completo++;
                                ciclo--;
                                cont++;
                                aux++;
                            }
                            else
                            {
                                ciclo--;
                                cont++;
                            }
                        }
                    }
                    else
                    {
                        ciclo--;
                        cont++;
                    }

                    // Só soma na lucratividade SE o ciclo da plantação estiver COMPLETO

                    if (completo == cicloAux)
                    {
                        // Guardando a Lucratividade por Lote:
                        vetLucratividadeLote[i] += vetCulturas[cultura - 1].lucratividade * vetAreaLotes[i];

                        // Somando a Lucratividade:
                        lt += vetCulturas[cultura - 1].lucratividade * vetAreaLotes[i];

                        // Somando a Funcao Objetivo:
                        solucao.funcObj += vetCulturas[cultura - 1].lucratividade * vetAreaLotes[i];
                    }
                }
            }
        }
    }

    // Fora de Épica:
    for (int i = 0; i < PERIODO; i++)
    {
        if (vetEpocaFora[i] != 0)
        {
            ef++;
        }
    }

    // Ciclo Fora:
    for (int i = 0; i < quantidadeLotes; i++)
    {
        if (vetCicloFora[i] != 0)
        {
            cf++;
        }
    }

    cfe = cf + ef; // Cultura Fora do Ciclo ou Epoca Fora de Plantio;

    int pousios = 0;

    // Pousio:

    for (int i = 0; i < quantidadeLotes; i++)
    {
        if (vetPousioLote[i] == 0)
        {
            po++;
        }
    }

    // Adubação Verde:
    for (int i = 0; i < quantidadeLotes; i++)
    {
        if (vetAdbVerdeLote[i] == 0)
        {
            av++;
        }
    }

    // Familia seguida no mesmo LOTE:
    for (int i = 0; i < quantidadeLotes; i++)
    {
        if (vetFamiliaAdjacente[i] != 0)
        {
            cc++;
        }
    }

    // printf("%d\n", cc);

    // Aplicando as Penalizações na FO:
    solucao.funcObj -= OM1 * MAX(0, cfe); // Cultura Fora de Época ou Ciclo;
    solucao.funcObj -= OM2 * MAX(0, cc);  // Mesma Cultura/Familia plantada em sequencia no mesmo lote;
    solucao.funcObj -= OM3 * MAX(0, pa);  // Mesma Familia em mesmo periodo em lotes adjacentes
    solucao.funcObj -= OM4 * MAX(0, av);  // Total de Adubação Verde por Rotação
    solucao.funcObj -= OM5 * MAX(0, po);  // Total de Falta de pousios por lote;

    // Resetando as variáveis;
    cfe = 0;
    cc = 0;
    pa = 0;
    av = 0;
    po = 0;
    cf = 0;
    ef = 0;
    lt = 0;
}

// Busca Local Randômica
void heuBLRand(Solucao &solucao)
{
    tempo = 0;
    Solucao melhorSolucao;

    clonarSolucao(solucao, melhorSolucao);
    verificarSolucao(solucao);
    calcularFO(solucao);
    int lote;
    int mes;
    int cultura;
    double foVelha = solucao.funcObj;
    double melhorFO = solucao.funcObj;

    for (int r = 0; (r < 2 * quantidadeLotes); r++)
    {
        // Escolhi um lote aleatoriamente
        lote = rand() % quantidadeLotes;

        // Escolhendo uma Cultura aleatoriamente, tendo que ser diferente de uma adubação verde;
        do
        {
            mes = rand() % (PERIODO);
            cultura = solucao.matSolucao[lote][mes];

        } while (cultura == 25 || cultura == 26 || cultura == 27);

        int mesInicio = mes;
        int mesFim = mes;
        int ciclo = 0;
        int vetAux[MAX_CULTURA];

        memset(&vetAux, 0, sizeof(vetAux));

        // Uma vez selecionado o Lote a Cultura:
        // Ele busca as informações exatas de onde começa e onde termina essa cultura escolhida na solução
        // ou seja, o perído do lote onde a cultura foi plantada: de tal mes (inicio) a tal mes (fim):
        while (solucao.matSolucao[lote][mesInicio - 1] == cultura)
        {
            mesInicio--;
        }

        if (mesInicio < 0)
        {
            mesInicio = 0;
        }

        while (solucao.matSolucao[lote][mesFim] == cultura)
        {
            mesFim++;
        }
        mesFim--;

        ciclo = (mesFim - mesInicio) + 1;

        int cont = 0;

        // Encontrando a Cultura MENOR ou IGUAL ao que vou ter de espaço;
        // Exceto Adb. Verde e Pousio;
        // E claro, exceto a própria cultura que está no lote no momento:

        for (int i = 0; i < MAX_CULTURA; i++)
        {
            if (vetCicloCultura[i] <= ciclo &&
                vetCulturas[i].cultura != 28 && vetCulturas[i].cultura != 27 &&
                vetCulturas[i].cultura != 26 && vetCulturas[i].cultura != 25 &&
                vetCulturas[i].cultura != cultura)
            {
                vetAux[cont] = i;
                cont++;
            }
        }

        // Verificando se posso plantar determinada cultura no mês correto;

        int inicioPlantio = vetCulturas[cultura - 1].inicioPlantio - 1;
        int fimPlantio = vetCulturas[cultura - 1].fimPlantio - 1;

        for (int i = 0; i < cont; i++)
        {
            int cicloAux, inicioAux, fimAux, cultAux;

            cultAux = vetCulturas[vetAux[i]].cultura;
            cicloAux = vetCicloCultura[vetAux[i]];
            inicioAux = vetCulturas[vetAux[i]].inicioPlantio - 1;
            fimAux = vetCulturas[vetAux[i]].fimPlantio - 1;

            // Selecionando uma cultura já com o ciclo correto, e que cabe no intervalo do mês selecionado!
            if ((inicioAux == 0 && mes <= fimAux || inicioAux >= mes && mes <= fimAux) && (inicioAux + cicloAux) <= mesFim)
            {
                int aux1 = mesInicio, aux2 = mesFim;
                int aux3 = mesInicio, aux4 = cicloAux;

                int tci = cicloAux; // ciclo da Cultura;

                // Apagando a cultura antiga da solução:
                while (aux1 <= aux2)
                {
                    solucao.matSolucao[lote][aux1] = 0;
                    aux1++;
                }

                // Plantando a cultura nova selecionada anteriormente;
                // Já respeitando período e ciclo compatível;
                while (tci > 0)
                {
                    solucao.matSolucao[lote][aux3] = cultAux;
                    aux3++;
                    tci--;
                }

                // Caso sobre períodos vazios depois que a nova cultura foi plantada, preenchemos com pousio:

                for (int i = 0; i < quantidadeLotes; i++)
                {
                    for (int j = 0; j < PERIODO; j++)
                    {
                        if (solucao.matSolucao[i][j] == 0)
                        {
                            solucao.matSolucao[i][j] = 28;
                        }
                    }
                }
            }
        }

        // A partir das alterações realizadas vamos verificar se houve uma melhora:

        verificarSolucao(solucao);
        calcularFO(solucao);

        if (melhorFO < solucao.funcObj)
        {
            // Se houve melhora, então:
            melhorFO = solucao.funcObj;            // Passa a ser a melhor FO conhecida;
            clonarSolucao(solucao, melhorSolucao); // passa a ser a melhor Solução conhecida;
        }
        else
        {                                          // Se não houve melhora, então:
            clonarSolucao(melhorSolucao, solucao); // voltamos a solução anterior;
            solucao.funcObj = foVelha;             // solucao passa a ter a fo antiga novamente;
        }
    }
}

// Grasp
void heuGrasp(Solucao &solucao, int LRC)
{

    solucao.funcObj = -INT32_MAX;

    Solucao solucaoAlternativa;
    Solucao melhorSolucao;

    // Melhor Solucao Conhecida ate o momento:
    melhorSolucao.funcObj = solucao.funcObj;

    double melhorFO = 0;
    double primeiraFO = solucao.funcObj;

    double tempo = 0;
    clock_t tempoInicio, tempoFim;
    double tempoMelhor = 0;

    tempoInicio = clock();

    while (tempo <= TEMPMAX)
    {
        heuConsGulosaAleatoria(solucaoAlternativa, LRC);
        heuBLRand(solucaoAlternativa);

        //Verificando a solucao, pois é aqui que contabilizamos as penalidades,
        //Em seguida o calculo da FO;

        if (solucaoAlternativa.funcObj > melhorSolucao.funcObj)
        {
            melhorFO = solucaoAlternativa.funcObj;
            clonarSolucao(solucaoAlternativa, melhorSolucao);

            printf("Melhor Conhecido (FO): %.2f\n", solucaoAlternativa.funcObj);

            tempoMelhor = ((double)(tempoFim - tempoInicio)) / CLOCKS_PER_SEC;

            printf("Melhor Conhecido (TP): %.2f\n", tempoMelhor);
            printf("\n");
        }
        tempoFim = clock();
        tempo = ((double)(tempoFim - tempoInicio)) / CLOCKS_PER_SEC;
    }

    // ################################## ESCREVENDO EM TELA ####################################

    printf("\n");

    printf("## ----------------- Informacoes da Heuristica GRASP ----------------- ##\n\n");
    printf("LRC: ................................................................. %d%%\n", LRC);
    printf("TEMPO LIMITE: ........................................................ %ds (Min: %d)\n\n", TEMPMAX, (TEMPMAX / 60));

    printf("  ---------------------------- PENALIZACOES ----------------------------\n\n");
    printf("Peso Cultura Fora de Epoca / Fora de Ciclo (CFE): .................... %d\n", OM1);
    printf("Peso Culturas Plantadas Adjacentes no Mesmo Lote (CC): ............... %d\n", OM2);
    printf("Peso Culturas Plantadas Adjacentes no Lote Vizinho (PA): ............. %d\n", OM3);
    printf("Peso Adubacao Verde (AV): ............................................ %d\n", OM4);
    printf("Peso Pousio (PO): .................................................... %d\n\n", OM5);

    printf(" ----------------------------- RESULTADOS --------------------------- \n\n");
    printf("Funcao Objetivo da Solucao Inicial: .................................. %.2f\n", primeiraFO);
    printf("Funcao Objetivo (FO) da Melhor Solucao: .............................. %.2f\n", melhorSolucao.funcObj);
    printf("Tempo Total: ......................................................... %.3f\n", tempo);
    printf("Tempo da Melhor Solucao Encontrada: .................................. %.3f\n\n", tempoMelhor);

    printf("------------------------------- SOLUCAO ------------------------------- \n\n");

    printf("FO: %.2f\n", melhorSolucao.funcObj);
    for (int i = 0; i < quantidadeLotes; i++)
    {
        for (int j = 0; j < PERIODO; j++)
        {
            printf(" [%d] ", melhorSolucao.matSolucao[i][j]);
        }
        printf("\n");
        printf("QUalquer coisa");
    }

    // ################################## ESCREVENDO NO ARQUIVO ####################################

    printf("\n ---- \n");

    FILE *f = fopen("c6r3.txt", "w");

    // Funcao Objetivo
    fprintf(f, "%.2f\n", melhorSolucao.funcObj);

    // Lotes x Periodos com Culturas da rotação
    for (int i = 0; i < quantidadeLotes; i++)
    {
        for (int j = 0; j < PERIODO; j++)
        {
            fprintf(f, "%d  ", melhorSolucao.matSolucao[i][j]);
        }
        fprintf(f, "\n");
    }

    fprintf(f, "\n");

    // Quantidade Total de Infrações com o Pousio:
    fprintf(f, "POUSIO: \n\n");
    for (int i = 0; i < quantidadeLotes; i++)
    {
        if (vetPousioLote[i] == 0)
        {
            fprintf(f, "Lote %d sem Pousio.\n", i + 1);
        }
        else
        {
            fprintf(f, "Lote %d com Pousio.\n", i + 1);
        }
    }

    fprintf(f, "\n");

    // Quantidade Total de Infrações com Adjacencia:

    fprintf(f, "CULTURAS ADJACENTES COM OUTROS LOTES: \n\n");
    for (int i = 0; i < quantidadeLotes; i++)
    {
        if (vetCultAdjacente[i] != 0)
        {
            fprintf(f, "Lote %d com %d conflitos.\n", i + 1, vetCultAdjacente[i]);
        }
    }

    fprintf(f, "\n");

    // Ciclo Fora:
    fprintf(f, "CULTURAS FORA DE CICLO: \n\n");
    for (int i = 0; i < quantidadeLotes; i++)
    {
        if (vetCicloFora[i] != 0)
        {
            fprintf(f, "Lote %d com %d cultura(s) fora de ciclo.\n", i + 1, vetCicloFora[i]);
        }
        else
        {
            fprintf(f, "Lote %d com %d cultura(s) fora de ciclo.\n", i + 1, vetCicloFora[i]);
        }
    }

    fprintf(f, "\n");

    fprintf(f, "ADUBACAO VERDE: \n\n");
    for (int i = 0; i < quantidadeLotes; i++)
    {
        if (vetAdbVerdeLote[i] != 0)
        {
            fprintf(f, "Lote %d com adubacao Verde.\n", i + 1);
        }
        else
        {
            fprintf(f, "Lote %d sem adubacao Verde.\n", i + 1);
        }
    }

    fprintf(f, "\n");

    fprintf(f, "CULTURA FORA DE EPOCA: \n\n");
    for (int i = 0; i < quantidadeLotes; i++)
    {
        if (vetEpocaFora[i] != 0)
        {
            fprintf(f, "Lote %d com %d Culturas Fora de Epoca.\n", i + 1, vetEpocaFora[i]);
        }
        else
        {
            fprintf(f, "Lote %d sem Culturas Fora de Epoca.\n", i + 1, vetEpocaFora[i]);
        }
    }

    fprintf(f, "\n");

    // NAO TÁ FUNCIONANDO: (ERA PRA ESTAR! rs);

    fprintf(f, "CULTURA COM FAMILIA ADJACENTE NO MESMO LOTE: \n\n");
    for (int i = 0; i < quantidadeLotes; i++)
    {
        if (vetFamiliaAdjacente[i] <= 0)
        {
            fprintf(f, "Lote %d com Conflitos com Vizinhos.\n", i + 1);
        }
        else
        {
            fprintf(f, "Lote %d sem Conflitos com Vizinhos.\n", i + 1, vetFamiliaAdjacente[i]);
        }
    }

    fprintf(f, "\n");
    fprintf(f, "LUCRATIVIDADE POR LOTE: \n\n");
    for (int i = 0; i < quantidadeLotes; i++)
    {
        fprintf(f, "Lote %d: R$ %.2f de Lucro.\n", i + 1, vetLucratividadeLote[i]);
    }

    fprintf(f, "\n\n");

    fprintf(f, "########################### INFORMACOES GRASP ############################# \n\n");
    fprintf(f, "LRC: ................................................................. %d%%\n", LRC);
    fprintf(f, "TEMPO LIMITE: ........................................................ %ds (Min: %d)\n\n", TEMPMAX, (TEMPMAX / 60));

    fprintf(f, "  ---------------------------- PENALIZACOES ----------------------------\n\n");
    fprintf(f, "Peso Cultura Fora de Epoca / Fora de Ciclo (CFE): .................... %d\n", OM1);
    fprintf(f, "Peso Culturas Plantadas Adjacentes no Mesmo Lote (CC): ............... %d\n", OM2);
    fprintf(f, "Peso Culturas Plantadas Adjacentes no Lote Vizinho (PA): ............. %d\n", OM3);
    fprintf(f, "Peso Adubacao Verde (AV): ............................................ %d\n", OM4);
    fprintf(f, "Peso Pousio (PO): .................................................... %d\n\n", OM5);

    fprintf(f, " ----------------------------- RESULTADOS --------------------------- \n\n");
    fprintf(f, "Funcao Objetivo da Solucao Inicial: .................................. %.2f\n", primeiraFO);
    fprintf(f, "Funcao Objetivo (FO) da Melhor Solucao: .............................. %.2f\n", melhorSolucao.funcObj);
    fprintf(f, "Tempo Total: ......................................................... %.3f\n", tempo);
    fprintf(f, "Tempo da Melhor Solucao Encontrada: .................................. %.3f\n\n", tempoMelhor);

    fclose(f);
}
