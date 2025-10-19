#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "Sigmoid.h"
#include "ReLU.h"
#include "LeakyReLU.h"


#define PadroesValidacao 24
#define PadroesTreinamento 24 
#define Sucesso 0.004		 // 0.0004
#define NumeroCiclos 100000 // Exibir o progresso do treinamento a cada NumeroCiclos ciclos

//Sigmoide
#define TaxaAprendizado 0.3 //0.3 converge super rápido e com uma boa precisão (sigmoide na oculta).
#define Momentum 0.9 // Dificulta a convergencia da rede em minimos locais, fazendo com que convirja apenas quando realmente se tratar de um valor realmente significante.
#define MaximoPesoInicial 0.5


//ReLUish
//#define TaxaAprendizado 0.05  // ou até 0.01
//#define Momentum 0.8
//#define MaximoPesoInicial 1.0  // ou ignora, se usar He init


//Saidas da rede neural
//Angulo de rocacao (0 - 45 graus)

//Direcao de rotacao
//   Direita              Reto            Esquerda
//0.125 - 0.375      0.375 - 0.625      0.625 - 0.875
//    0,25				  0,5                0,75
#define OUT_DR_DIREITA    0.25    
#define OUT_DR_ESQUERDA   0.5   
#define OUT_DR_FRENTE     0.75

//Direcao de movimento
//	  Frente		    Re
//   0.1 - 0.5      0.5 - 0.9
#define OUT_DM_FRENTE     0.3      
#define OUT_DM_RE         0.7

//Angulo de rotacao
//   Lateral             Diagonal        Frontal
//0.125 - 0.375      0.375 - 0.625      0.625 - 0.875
//    0,25				  0,5               0,75
#define OUT_AR_LATERAL     0.25    //5
#define OUT_AR_DIAGONAL    0.5     //15
#define OUT_AR_FRONTAL     0.75    //45

#define ALCANCE_MAX_SENSOR 5000

// Camada de entrada
#define NodosEntrada 8

// Camada oculta
#define NodosOcultos 18

// Camada de saída
#define NodosSaida 3


class NeuralNetwork {
public:
    int i, j, p, q, r;
    int IntervaloTreinamentosPrintTela;
    int IndiceRandom[PadroesTreinamento];
    long CiclosDeTreinamento;
    float Rando;
    float Error;
    float AcumulaPeso;

    int esquerda = 0;
    int diagonal_esquerda_lateral = 0;
    int diagonal_esquerda_frontal = 0;
    int frente_esquerda = 0;
    int direita = 0;
    int diagonal_direita_lateral = 0;
    int diagonal_direita_frontal = 0;
    int frente_direita = 0;

    // Camada oculta
    float Oculto[NodosOcultos];
    float PesosCamadaOculta[NodosEntrada + 1][NodosOcultos];
    float OcultoDelta[NodosOcultos];
    float AlteracaoPesosOcultos[NodosEntrada + 1][NodosOcultos];
    ActivationFunction* activationFunctionCamadasOcultas;

    // Camada de saída
    float Saida[NodosSaida];
    float SaidaDelta[NodosSaida];
    float PesosSaida[NodosOcultos + 1][NodosSaida];
    float AlterarPesosSaida[NodosOcultos + 1][NodosSaida];
    ActivationFunction* activationFunctionCamadaSaida;

    float ValoresSensores[1][NodosEntrada] = {{0, 0, 0, 0, 0, 0, 0, 0}};


    // Direita(0.53)  Esquerda(0.59)  Frente(0.65)    ATRAS(0.71)   PARADO(0.77)
    const float Input[PadroesTreinamento][NodosEntrada] = {
    //ESQUERDA 							  FRETENE								  DIREITA
    // {0, 		1, 		2, 		3, 		4, 		5, 		6, 		7}                   {0,      0,       0,       0,       0,       0,       0,       0,},
    //Obstáculo apenas á frente
        {0,      0,       0,     900,     800,      0,       0,       0},
        {0,      0,       0,     750,       0,      0,       0,       0},
        {0,      0,       0,       0,     950,      0,       0,       0},
        {0,      0,       0,     600,     800,      0,       0,       0},
    //obstáculo a direit
        {0,      0,       0,       0,       0,     0,       0,       100},
        {0,      0,       0,       0,       0,     0,       0,       400},
        {0,      0,       0,       0,       0,     0,    1000,       300},
        {0,      0,       0,       0,       0,     0,       0,       450},
    //obstáculo a direita (diagonal
        {0,      0,       0,       0,        0,     800,     700,       0},
        {0,      0,       0,       0,        0,     600,     800,       0},
        {0,      0,       0,       0,        0,     650,     710,       0},
        {0,      0,       0,    1100,     1050,     800,     700,       0},
    //obstaculo a esquerd
        {150,    0,       0,      0,       0,       0,       0,       0},
        {370,    0,       0,      0,       0,       0,       0,       0},
        {450,   800,      0,      0,       0,       0,       0,       0},
        {400,    0,       0,      0,       0,       0,       0,       0},
    //obstaculo a esquerda (diagonal
        {0,      690,     830,       0,       0,       0,       0,       0},
        {0,      770,     620,       0,       0,       0,       0,       0},
        {0,      700,     590,       0,       0,       0,       0,       0},
        {0,      690,     840,    1090,    1300,       0,       0,       0},
    //obstaculo muito perto a frent
        {0,      0,       0,       150,       148,       0,       0,       0},
        {0,      0,       0,       100,       110,       0,       0,       0},
        {800,    900,     870,     180,       175,       0,       0,       0},
        {0,      0,       0,       120,       125,     890,     850,     800}
    };
    float InputNormalizado[PadroesTreinamento][NodosEntrada];

    const float Objetivo[PadroesTreinamento][NodosSaida] = {
    //   DR,  AR,  DM
    //Obstáculo apenas á frente
        {OUT_DR_ESQUERDA, OUT_AR_FRONTAL, OUT_DM_FRENTE}, 
        {OUT_DR_ESQUERDA, OUT_AR_FRONTAL, OUT_DM_FRENTE},
        {OUT_DR_DIREITA,  OUT_AR_FRONTAL, OUT_DM_FRENTE},
        {OUT_DR_DIREITA,  OUT_AR_FRONTAL, OUT_DM_FRENTE},
    //obstáculo a direita
        {OUT_DR_ESQUERDA, OUT_AR_LATERAL, OUT_DM_FRENTE}, 
        {OUT_DR_ESQUERDA, OUT_AR_LATERAL, OUT_DM_FRENTE},
        {OUT_DR_ESQUERDA, OUT_AR_LATERAL, OUT_DM_FRENTE},
        {OUT_DR_ESQUERDA, OUT_AR_LATERAL, OUT_DM_FRENTE},
    //obstáculo a direita (diagonal)
        {OUT_DR_ESQUERDA, OUT_AR_DIAGONAL, OUT_DM_FRENTE}, 
        {OUT_DR_ESQUERDA, OUT_AR_DIAGONAL, OUT_DM_FRENTE},
        {OUT_DR_ESQUERDA, OUT_AR_DIAGONAL, OUT_DM_FRENTE},
        {OUT_DR_ESQUERDA, OUT_AR_DIAGONAL, OUT_DM_FRENTE},
    //obstaculo a esquerda
        {OUT_DR_DIREITA, OUT_AR_LATERAL, OUT_DM_FRENTE}, 
        {OUT_DR_DIREITA, OUT_AR_LATERAL, OUT_DM_FRENTE},
        {OUT_DR_DIREITA, OUT_AR_LATERAL, OUT_DM_FRENTE},
        {OUT_DR_DIREITA, OUT_AR_LATERAL, OUT_DM_FRENTE},
    //obstaculo a esquerda (diagonal)
        {OUT_DR_DIREITA, OUT_AR_DIAGONAL, OUT_DM_FRENTE}, 
        {OUT_DR_DIREITA, OUT_AR_DIAGONAL, OUT_DM_FRENTE},
        {OUT_DR_DIREITA, OUT_AR_DIAGONAL, OUT_DM_FRENTE},
        {OUT_DR_DIREITA, OUT_AR_DIAGONAL, OUT_DM_FRENTE},
    //obstaculo muito perto a frente
        {OUT_DR_FRENTE, OUT_AR_FRONTAL, OUT_DM_RE}, 
        {OUT_DR_FRENTE, OUT_AR_FRONTAL, OUT_DM_RE},
        {OUT_DR_FRENTE,  OUT_AR_FRONTAL, OUT_DM_RE},
        {OUT_DR_FRENTE,  OUT_AR_FRONTAL, OUT_DM_RE}
    };
    
    //Dados de validação
    const float InputValidacao[PadroesValidacao][NodosEntrada] = {
        //Obstáculo apenas á frente
        {0,      0,       0,     900,     800,      0,       0,       0},
        {0,      0,       0,     750,       0,      0,       0,       0},
        {0,      0,       0,       0,     950,      0,       0,       0},
        {0,      0,       0,     600,     800,      0,       0,       0},
    //obstáculo a direit
        {0,      0,       0,       0,       0,     0,       0,       100},
        {0,      0,       0,       0,       0,     0,       0,       400},
        {0,      0,       0,       0,       0,     0,    1000,       300},
        {0,      0,       0,       0,       0,     0,       0,       450},
    //obstáculo a direita (diagonal
        {0,      0,       0,       0,        0,     800,     700,       0},
        {0,      0,       0,       0,        0,     600,     800,       0},
        {0,      0,       0,       0,        0,     650,     710,       0},
        {0,      0,       0,    1100,     1050,     800,     700,       0},
    //obstaculo a esquerd
        {150,    0,       0,      0,       0,       0,       0,       0},
        {370,    0,       0,      0,       0,       0,       0,       0},
        {450,   800,      0,      0,       0,       0,       0,       0},
        {400,    0,       0,      0,       0,       0,       0,       0},
    //obstaculo a esquerda (diagonal
        {0,      690,     830,       0,       0,       0,       0,       0},
        {0,      770,     620,       0,       0,       0,       0,       0},
        {0,      700,     590,       0,       0,       0,       0,       0},
        {0,      690,     840,    1090,    1300,       0,       0,       0},
    //obstaculo muito perto a frent
        {0,      0,       0,       150,       148,       0,       0,       0},
        {0,      0,       0,       100,       110,       0,       0,       0},
        {800,    900,     870,     180,       175,       0,       0,       0},
        {0,      0,       0,       120,       125,     890,     850,     800}
    
    };
    float InputValidacaoNormalizado[PadroesValidacao][NodosEntrada];
    
    const float ObjetivoValidacao[PadroesValidacao][NodosSaida] = {
        //Obstáculo apenas á frente
        {OUT_DR_ESQUERDA, OUT_AR_FRONTAL, OUT_DM_FRENTE}, 
        {OUT_DR_ESQUERDA, OUT_AR_FRONTAL, OUT_DM_FRENTE},
        {OUT_DR_DIREITA,  OUT_AR_FRONTAL, OUT_DM_FRENTE},
        {OUT_DR_DIREITA,  OUT_AR_FRONTAL, OUT_DM_FRENTE},
    //obstáculo a direita
        {OUT_DR_ESQUERDA, OUT_AR_LATERAL, OUT_DM_FRENTE}, 
        {OUT_DR_ESQUERDA, OUT_AR_LATERAL, OUT_DM_FRENTE},
        {OUT_DR_ESQUERDA, OUT_AR_LATERAL, OUT_DM_FRENTE},
        {OUT_DR_ESQUERDA, OUT_AR_LATERAL, OUT_DM_FRENTE},
    //obstáculo a direita (diagonal)
        {OUT_DR_ESQUERDA, OUT_AR_DIAGONAL, OUT_DM_FRENTE}, 
        {OUT_DR_ESQUERDA, OUT_AR_DIAGONAL, OUT_DM_FRENTE},
        {OUT_DR_ESQUERDA, OUT_AR_DIAGONAL, OUT_DM_FRENTE},
        {OUT_DR_ESQUERDA, OUT_AR_DIAGONAL, OUT_DM_FRENTE},
    //obstaculo a esquerda
        {OUT_DR_DIREITA, OUT_AR_LATERAL, OUT_DM_FRENTE}, 
        {OUT_DR_DIREITA, OUT_AR_LATERAL, OUT_DM_FRENTE},
        {OUT_DR_DIREITA, OUT_AR_LATERAL, OUT_DM_FRENTE},
        {OUT_DR_DIREITA, OUT_AR_LATERAL, OUT_DM_FRENTE},
    //obstaculo a esquerda (diagonal)
        {OUT_DR_DIREITA, OUT_AR_DIAGONAL, OUT_DM_FRENTE}, 
        {OUT_DR_DIREITA, OUT_AR_DIAGONAL, OUT_DM_FRENTE},
        {OUT_DR_DIREITA, OUT_AR_DIAGONAL, OUT_DM_FRENTE},
        {OUT_DR_DIREITA, OUT_AR_DIAGONAL, OUT_DM_FRENTE},
    //obstaculo muito perto a frente
        {OUT_DR_FRENTE, OUT_AR_FRONTAL, OUT_DM_RE}, 
        {OUT_DR_FRENTE, OUT_AR_FRONTAL, OUT_DM_RE},
        {OUT_DR_FRENTE,  OUT_AR_FRONTAL, OUT_DM_RE},
        {OUT_DR_FRENTE,  OUT_AR_FRONTAL, OUT_DM_RE}
    };
    
    //--

public:
    NeuralNetwork();
    void treinarRedeNeural();
    void inicializacaoPesos();
    int treinoInicialRede();
    void PrintarValores();
    void testarValor();
    void loop(int sensor0, int sensor1, int sensor2, int sensor3, int sensor4, int sensor5, int sensor6, int sensor7);
    void validarRedeNeural();
    void treinarValidar();
    void normalizarEntradas();
    void setupCamadas() ;
};

#endif // NEURALNETWORK_H
