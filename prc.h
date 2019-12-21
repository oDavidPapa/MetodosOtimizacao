#ifndef PRC_H_INCLUED
#define PRC_H_INCLUED

#define MAX_CULTURA 28
#define MAX_LOTE 20
#define MAX_PERIODO 24

//Variaveis Auxiliares
static int param;
static int PERIODO;


typedef struct tCultura
{
    int cultura;
    int familia;
    int inicioPlantio;
    int fimPlantio;
    int ciclo;
    int lucratividade;
} Cultura;

// Solucao do Problema:
typedef struct tSolucao
{
    double funcObj;
    int matSolucao[MAX_LOTE][MAX_PERIODO];
} Solucao;


//Variáveis e Vetores:
static Cultura vetCulturas[MAX_CULTURA];            // vetor Culturas
static float vetAreaLotes[MAX_LOTE];                // vetor das Areas dos Lotes
static int vetCicloCultura[MAX_CULTURA];            // vetor do Ciclo das Culturas Calculados
static int vetQtdVizinhos[MAX_LOTE];                // Quantidade de Vizinhos de Cada Lote
static int matLotesVizinhos[MAX_LOTE][MAX_LOTE];    // Os vizinhos de Cada Lote
static int vetIndiceCulturasOrdenadas[MAX_CULTURA]; // Vetor de Indices Ordenados por Lucr. / Ciclo
static double vetLucratividadeLote[MAX_LOTE];       // Vetor do Valor de Cada Lote


static int vetCicloFora[MAX_LOTE];                  // *Seria para contabilizar os ciclos fora em cada lote
static int vetCultAdjacente[MAX_LOTE];              // *Culturas plantadas 'erradas' em lotes Adjacentes
static int vetPousioLote[MAX_LOTE];                 // *Vetor de pousios por Lote
static int vetAdbVerdeLote[MAX_LOTE];               // *Vetor de Adubação Verde Por Lote;
static int vetEpocaFora[MAX_LOTE];                  // *Vetor de Época de Plantio Incorreta;
static int vetFamiliaAdjacente[MAX_LOTE];           // *Vetor de Mesma Familia Plantada Junta;


static int vetCulturaCabe[MAX_CULTURA];

static int pousio;
static int adubacaoVerde;
static int quantidadeLotes;

static int ef = 0;      // Epoca de Cultivo Fora;
static int cf = 0;      // Ciclo Fora de Época;
static int cfe = 0;     // Cultura Fora de Época ou Ciclo;
static int cc = 0;      // Mesma Cultura plantada em sequencia no mesmo lote;
static int pa = 0;      // Mesma Familia em mesmo periodo em lotes adjacentes
static int po = 0;      // Total de Pousios na Rotação
static int av = 0;      // Total de Adubação Verde por Rotação
static double lt = 0;   // Total lucratividade;

//Funções:
void criarLotesDiferentes();
void lerDadosLote(const char *arq);
void lerDadosCultura(const char *arq);
Solucao lerSolucao(const char *arq, int L, int P);
void calcularCicloCulturas();
void heuConsGulosaAleatoria(Solucao &solucao, int LRC);
void heuGrasp(Solucao &solucao, int LRC);
void calcularFO(Solucao &solucao);
void escreverSolucao(Solucao &solucao);
void imprimeDados();
void ordenarDados();
void clonarSolucao(Solucao &solucao, Solucao &clone);
void verificarSolucao(Solucao &solucao);
void escreverSolucaoArquivo(Solucao &solucao);
void criarSolucao(Solucao &solucao);
void heuBLRand(Solucao &solucao);

#endif
