#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "Sigmoid.h"
#include "ReLU.h"
#include "LeakyReLU.h"
#include "ExpectedMovement.h"


#define PadroesValidacao 56
#define PadroesTreinamento 56 
#define Sucesso 0.1		 // 0.0004
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

//Angulo de rocacao (0 - 45 graus)
//Angulo de rotacao
//Sem Rotacao(0)     Lateral(5)             Diagonal(15)        Frontal(34)
// 0,1 - 0,3         0,3 - 0,5              0,5 - 0,7           0,7 - 0,9
//    0.2               0,4                    0,6                 0,8
#define OUT_AR_SEM_ROTACAO  0.2    //0
#define OUT_AR_LATERAL      0.4    //5
#define OUT_AR_DIAGONAL     0.6    //15
#define OUT_AR_FRONTAL      0.8    //45

#define ALCANCE_MAX_SENSOR 5000

// Camada de entrada
#define NodosEntrada 8

// Camada oculta
#define NodosOcultos 18 //18

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
    //Obstáculo apenas á frente (perto) (<=1000)
        {5000,      5000,       5000,     900,     800,      5000,       5000,       5000},
        {5000,      5000,       5000,     750,    5000,      5000,       5000,       5000},
        {5000,      5000,       5000,    5000,     950,      5000,       5000,       5000},
        {5000,      5000,       5000,     600,     800,      5000,       5000,       5000},
    //Obstáculo apenas á frente (medio) +250 25%
        {5000,      5000,       5000,    1150,    1050,     5000,       5000,       5000},
        {5000,      5000,       5000,    1000,    5000,     5000,       5000,       5000},
        {5000,      5000,       5000,    5000,    1200,     5000,       5000,       5000},
        {5000,      5000,       5000,     850,    1050,     5000,       5000,       5000},

    //obstáculo a direita (perto) (<= 500)
        {5000,      5000,       5000,       5000,       5000,     5000,    5000,       100},
        {5000,      5000,       5000,       5000,       5000,     5000,    5000,       400},
        {5000,      5000,       5000,       5000,       5000,     5000,    1000,       300},
        {5000,      5000,       5000,       5000,       5000,     5000,    5000,       450},
    //obstáculo a direita (medio) +125 25%
        {5000,      5000,       5000,       5000,       5000,     5000,    5000,       225},
        {5000,      5000,       5000,       5000,       5000,     5000,    5000,       525},
        {5000,      5000,       5000,       5000,       5000,     5000,    1225,       425},
        {5000,      5000,       5000,       5000,       5000,     5000,    5000,       575},

    //obstáculo a direita (diagonal) (<=750)
        {5000,      5000,       5000,    5000,     5000,     800,     700,       5000},
        {5000,      5000,       5000,    5000,     5000,     600,     800,       5000},
        {5000,      5000,       5000,    5000,     5000,     650,     710,       5000},
        {5000,      5000,       5000,    1100,     1050,     800,     700,       5000},
    //obstáculo a direita (diagonal) + 187.5 25%
        {5000,      5000,       5000,    5000,     5000,     987.5,   887.5,       5000},
        {5000,      5000,       5000,    5000,     5000,     787.5,   987.5,       5000},
        {5000,      5000,       5000,    5000,     5000,     837.5,   897.5,       5000},
        {5000,      5000,       5000,  1287.5,   1237.5,     987.5,   887.5,       5000},


    //obstaculo a esquerda (perto) (perto) (<= 500)
        {150,   5000,     5000,      5000,       5000,       5000,       5000,       5000},
        {370,   5000,     5000,      5000,       5000,       5000,       5000,       5000},
        {450,    800,     5000,      5000,       5000,       5000,       5000,       5000},
        {400,   5000,     5000,      5000,       5000,       5000,       5000,       5000},
    //obstaculo a esquerda (medio) +125 25%
        {275,    0,       5000,      5000,       5000,       5000,       5000,       5000},
        {575,    0,       5000,      5000,       5000,       5000,       5000,       5000},
        {625,  925,       5000,      5000,       5000,       5000,       5000,       5000},
        {525,    0,       5000,      5000,       5000,       5000,       5000,       5000},


    //obstaculo a esquerda (diagonal) (perto) (<=750)
        {5000,      690,     830,    5000,    5000,       5000,       5000,       5000},
        {5000,      770,     620,    5000,    5000,       5000,       5000,       5000},
        {5000,      700,     590,    5000,    5000,       5000,       5000,       5000},
        {5000,      690,     840,    1090,    1300,       5000,       5000,       5000},
    //obstaculo a esquerda (diagonal) (medio) + 187.5 25%
        {0,    877.5,   1017.5,   5000,    5000,       5000,       5000,      5000},
        {0,    957.5,    807.5,   5000,    5000,       5000,       5000,      5000},
        {0,    887.5,    777.5,   5000,    5000,       5000,       5000,      5000},
        {0,    877.5,    727.5, 1277.5,  1487.5,       5000,       5000,      5000},
 
    //obstaculo muito perto a frente (peto) (<=200)
        {5000,  5000,     5000,     150,       148,    5000,    5000,    5000}, 
        {5000,  5000,     5000,     100,       110,    5000,    5000,    5000}, 
        {800,    900,      870,     175,       180,    5000,    5000,    5000}, 
        {5000,  5000,     5000,     125,       120,     890,     850,     800}, 
    //obstaculo muito perto a frente (medio) +50 25%
        {5000,  5000,    5000,       200,       198,    5000,    5000,    5000},
        {5000,  5000,    5000,       150,       160,    5000,    5000,    5000},
        {850,    950,     920,       225,       230,    5000,    5000,    5000},
        {5000,  5000,    5000,       175,       170,     940,     900,     850},

    //sem bostaculos proximos (200+ 500+ 750+)  
    //  {500,    750,     750,     200,       200,     750,     750,     500}, o obstaculo precisa estar abaixo disso para tomar uma ação
        {600,    800,     800,     300,       300,     800,     800,     800},
        {1000,   1002,    1004,    1006,      1010,    1001,    1020,    1100},
        {2000,   1802,    1904,    2306,      1912,    2221,    2329,    1999},
        {3002,   3010,    3134,    3020,      3113,    3029,    3789,    3321},
        {4003,   4022,    4204,    4506,      4811,    4201,    3990,    3970},
        {800,    1000,    1000,     500,       500,    1000,    1000,    1000},
        {1003,   2022,    2204,    1306,      1411,    2301,    2290,    1270},
        {5000,   5000,    5000,    5000,      5000,    5000,    5000,    5000}
    };
    float InputNormalizado[PadroesTreinamento][NodosEntrada];

    const float Objetivo[PadroesTreinamento][NodosSaida] = {
    //   DR,  AR,  DM
    //Obstáculo apenas á frente
        {OUT_DR_ESQUERDA, OUT_AR_FRONTAL, OUT_DM_FRENTE}, 
        {OUT_DR_ESQUERDA, OUT_AR_FRONTAL, OUT_DM_FRENTE},
        {OUT_DR_DIREITA,  OUT_AR_FRONTAL, OUT_DM_FRENTE},
        {OUT_DR_DIREITA,  OUT_AR_FRONTAL, OUT_DM_FRENTE},
        {OUT_DR_ESQUERDA, OUT_AR_FRONTAL, OUT_DM_FRENTE}, 
        {OUT_DR_ESQUERDA, OUT_AR_FRONTAL, OUT_DM_FRENTE},
        {OUT_DR_DIREITA,  OUT_AR_FRONTAL, OUT_DM_FRENTE},
        {OUT_DR_DIREITA,  OUT_AR_FRONTAL, OUT_DM_FRENTE},
    //obstáculo a direita
        {OUT_DR_ESQUERDA, OUT_AR_LATERAL, OUT_DM_FRENTE}, 
        {OUT_DR_ESQUERDA, OUT_AR_LATERAL, OUT_DM_FRENTE},
        {OUT_DR_ESQUERDA, OUT_AR_LATERAL, OUT_DM_FRENTE},
        {OUT_DR_ESQUERDA, OUT_AR_LATERAL, OUT_DM_FRENTE},
        {OUT_DR_ESQUERDA, OUT_AR_LATERAL, OUT_DM_FRENTE}, 
        {OUT_DR_ESQUERDA, OUT_AR_LATERAL, OUT_DM_FRENTE},
        {OUT_DR_ESQUERDA, OUT_AR_LATERAL, OUT_DM_FRENTE},
        {OUT_DR_ESQUERDA, OUT_AR_LATERAL, OUT_DM_FRENTE},
    //obstáculo a direita (diagonal)
        {OUT_DR_ESQUERDA, OUT_AR_DIAGONAL, OUT_DM_FRENTE}, 
        {OUT_DR_ESQUERDA, OUT_AR_DIAGONAL, OUT_DM_FRENTE},
        {OUT_DR_ESQUERDA, OUT_AR_DIAGONAL, OUT_DM_FRENTE},
        {OUT_DR_ESQUERDA, OUT_AR_DIAGONAL, OUT_DM_FRENTE},
        {OUT_DR_ESQUERDA, OUT_AR_DIAGONAL, OUT_DM_FRENTE}, 
        {OUT_DR_ESQUERDA, OUT_AR_DIAGONAL, OUT_DM_FRENTE},
        {OUT_DR_ESQUERDA, OUT_AR_DIAGONAL, OUT_DM_FRENTE},
        {OUT_DR_ESQUERDA, OUT_AR_DIAGONAL, OUT_DM_FRENTE},
    //obstaculo a esquerda
        {OUT_DR_DIREITA, OUT_AR_LATERAL, OUT_DM_FRENTE}, 
        {OUT_DR_DIREITA, OUT_AR_LATERAL, OUT_DM_FRENTE},
        {OUT_DR_DIREITA, OUT_AR_LATERAL, OUT_DM_FRENTE},
        {OUT_DR_DIREITA, OUT_AR_LATERAL, OUT_DM_FRENTE},
        {OUT_DR_DIREITA, OUT_AR_LATERAL, OUT_DM_FRENTE}, 
        {OUT_DR_DIREITA, OUT_AR_LATERAL, OUT_DM_FRENTE},
        {OUT_DR_DIREITA, OUT_AR_LATERAL, OUT_DM_FRENTE},
        {OUT_DR_DIREITA, OUT_AR_LATERAL, OUT_DM_FRENTE},
    //obstaculo a esquerda (diagonal)
        {OUT_DR_DIREITA, OUT_AR_DIAGONAL, OUT_DM_FRENTE}, 
        {OUT_DR_DIREITA, OUT_AR_DIAGONAL, OUT_DM_FRENTE},
        {OUT_DR_DIREITA, OUT_AR_DIAGONAL, OUT_DM_FRENTE},
        {OUT_DR_DIREITA, OUT_AR_DIAGONAL, OUT_DM_FRENTE},
        {OUT_DR_DIREITA, OUT_AR_DIAGONAL, OUT_DM_FRENTE}, 
        {OUT_DR_DIREITA, OUT_AR_DIAGONAL, OUT_DM_FRENTE},
        {OUT_DR_DIREITA, OUT_AR_DIAGONAL, OUT_DM_FRENTE},
        {OUT_DR_DIREITA, OUT_AR_DIAGONAL, OUT_DM_FRENTE},
    //obstaculo muito perto a frente
        {OUT_DR_ESQUERDA, OUT_AR_DIAGONAL, OUT_DM_RE}, 
        {OUT_DR_DIREITA,  OUT_AR_DIAGONAL, OUT_DM_RE},
        {OUT_DR_DIREITA,  OUT_AR_DIAGONAL, OUT_DM_RE},
        {OUT_DR_ESQUERDA, OUT_AR_DIAGONAL, OUT_DM_RE},

        {OUT_DR_ESQUERDA, OUT_AR_DIAGONAL, OUT_DM_RE}, 
        {OUT_DR_DIREITA,  OUT_AR_DIAGONAL, OUT_DM_RE},
        {OUT_DR_DIREITA,  OUT_AR_DIAGONAL, OUT_DM_RE},
        {OUT_DR_ESQUERDA, OUT_AR_DIAGONAL, OUT_DM_RE},
    //sem bostaculos proximos 
        {OUT_DR_FRENTE,  OUT_AR_SEM_ROTACAO, OUT_DM_FRENTE}, 
        {OUT_DR_FRENTE,  OUT_AR_SEM_ROTACAO, OUT_DM_FRENTE},
        {OUT_DR_FRENTE,  OUT_AR_SEM_ROTACAO, OUT_DM_FRENTE},
        {OUT_DR_FRENTE,  OUT_AR_SEM_ROTACAO, OUT_DM_FRENTE},
        {OUT_DR_FRENTE,  OUT_AR_SEM_ROTACAO, OUT_DM_FRENTE}, 
        {OUT_DR_FRENTE,  OUT_AR_SEM_ROTACAO, OUT_DM_FRENTE},
        {OUT_DR_FRENTE,  OUT_AR_SEM_ROTACAO, OUT_DM_FRENTE},
        {OUT_DR_FRENTE,  OUT_AR_SEM_ROTACAO, OUT_DM_FRENTE}
    };
    
    //Dados de validação
    const float InputValidacao[PadroesValidacao][NodosEntrada] = {
    //Obstáculo apenas á frente (perto) (<=1000)
        {5000,      5000,       5000,     900,     800,      5000,       5000,       5000},
        {5000,      5000,       5000,     750,    5000,      5000,       5000,       5000},
        {5000,      5000,       5000,    5000,     950,      5000,       5000,       5000},
        {5000,      5000,       5000,     600,     800,      5000,       5000,       5000},
    //Obstáculo apenas á frente (medio) +250 25%
        {5000,      5000,       5000,    1150,    1050,     5000,       5000,       5000},
        {5000,      5000,       5000,    1000,    5000,     5000,       5000,       5000},
        {5000,      5000,       5000,    5000,    1200,     5000,       5000,       5000},
        {5000,      5000,       5000,     850,    1050,     5000,       5000,       5000},

    //obstáculo a direita (perto) (<= 500)
        {5000,      5000,       5000,       5000,       5000,     5000,    5000,       100},
        {5000,      5000,       5000,       5000,       5000,     5000,    5000,       400},
        {5000,      5000,       5000,       5000,       5000,     5000,    1000,       300},
        {5000,      5000,       5000,       5000,       5000,     5000,    5000,       450},
    //obstáculo a direita (medio) +125 25%
        {5000,      5000,       5000,       5000,       5000,     5000,    5000,       225},
        {5000,      5000,       5000,       5000,       5000,     5000,    5000,       525},
        {5000,      5000,       5000,       5000,       5000,     5000,    1225,       425},
        {5000,      5000,       5000,       5000,       5000,     5000,    5000,       575},

    //obstáculo a direita (diagonal) (<=750)
        {5000,      5000,       5000,    5000,     5000,     800,     700,       5000},
        {5000,      5000,       5000,    5000,     5000,     600,     800,       5000},
        {5000,      5000,       5000,    5000,     5000,     650,     710,       5000},
        {5000,      5000,       5000,    1100,     1050,     800,     700,       5000},
    //obstáculo a direita (diagonal) + 187.5 25%
        {5000,      5000,       5000,    5000,     5000,     987.5,   887.5,       5000},
        {5000,      5000,       5000,    5000,     5000,     787.5,   987.5,       5000},
        {5000,      5000,       5000,    5000,     5000,     837.5,   897.5,       5000},
        {5000,      5000,       5000,  1287.5,   1237.5,     987.5,   887.5,       5000},


    //obstaculo a esquerda (perto) (perto) (<= 500)
        {150,   5000,     5000,      5000,       5000,       5000,       5000,       5000},
        {370,   5000,     5000,      5000,       5000,       5000,       5000,       5000},
        {450,    800,     5000,      5000,       5000,       5000,       5000,       5000},
        {400,   5000,     5000,      5000,       5000,       5000,       5000,       5000},
    //obstaculo a esquerda (medio) +125 25%
        {275,    0,       5000,      5000,       5000,       5000,       5000,       5000},
        {575,    0,       5000,      5000,       5000,       5000,       5000,       5000},
        {625,  925,       5000,      5000,       5000,       5000,       5000,       5000},
        {525,    0,       5000,      5000,       5000,       5000,       5000,       5000},


    //obstaculo a esquerda (diagonal) (perto) (<=750)
        {5000,      690,     830,    5000,    5000,       5000,       5000,       5000},
        {5000,      770,     620,    5000,    5000,       5000,       5000,       5000},
        {5000,      700,     590,    5000,    5000,       5000,       5000,       5000},
        {5000,      690,     840,    1090,    1300,       5000,       5000,       5000},
    //obstaculo a esquerda (diagonal) (medio) + 187.5 25%
        {0,    877.5,   1017.5,   5000,    5000,       5000,       5000,      5000},
        {0,    957.5,    807.5,   5000,    5000,       5000,       5000,      5000},
        {0,    887.5,    777.5,   5000,    5000,       5000,       5000,      5000},
        {0,    877.5,    727.5, 1277.5,  1487.5,       5000,       5000,      5000},
 
    //obstaculo muito perto a frente (peto) (<=200)
        {5000,  5000,     5000,     150,       148,    5000,    5000,    5000}, 
        {5000,  5000,     5000,     100,       110,    5000,    5000,    5000}, 
        {800,    900,      870,     175,       180,    5000,    5000,    5000}, 
        {5000,  5000,     5000,     125,       120,     890,     850,     800}, 
    //obstaculo muito perto a frente (medio) +50 25%
        {5000,  5000,    5000,       200,       198,    5000,    5000,    5000},
        {5000,  5000,    5000,       150,       160,    5000,    5000,    5000},
        {850,    950,     920,       225,       230,    5000,    5000,    5000},
        {5000,  5000,    5000,       175,       170,     940,     900,     850},

    //sem bostaculos proximos (200+ 500+ 750+)  
    //  {500,    750,     750,     200,       200,     750,     750,     500}, o obstaculo precisa estar abaixo disso para tomar uma ação
        {600,    800,     800,     300,       300,     800,     800,     800},
        {1000,   1002,    1004,    1006,      1010,    1001,    1020,    1100},
        {2000,   1802,    1904,    2306,      1912,    2221,    2329,    1999},
        {3002,   3010,    3134,    3020,      3113,    3029,    3789,    3321},
        {4003,   4022,    4204,    4506,      4811,    4201,    3990,    3970},
        {800,    1000,    1000,     500,       500,    1000,    1000,    1000},
        {1003,   2022,    2204,    1306,      1411,    2301,    2290,    1270},
        {5000,   5000,    5000,    5000,      5000,    5000,    5000,    5000}
    };
    float InputValidacaoNormalizado[PadroesValidacao][NodosEntrada];
    
    const float ObjetivoValidacao[PadroesValidacao][NodosSaida] = {
    //Obstáculo apenas á frente
        {OUT_DR_ESQUERDA, OUT_AR_FRONTAL, OUT_DM_FRENTE}, 
        {OUT_DR_ESQUERDA, OUT_AR_FRONTAL, OUT_DM_FRENTE},
        {OUT_DR_DIREITA,  OUT_AR_FRONTAL, OUT_DM_FRENTE},
        {OUT_DR_DIREITA,  OUT_AR_FRONTAL, OUT_DM_FRENTE},
        {OUT_DR_ESQUERDA, OUT_AR_FRONTAL, OUT_DM_FRENTE}, 
        {OUT_DR_ESQUERDA, OUT_AR_FRONTAL, OUT_DM_FRENTE},
        {OUT_DR_DIREITA,  OUT_AR_FRONTAL, OUT_DM_FRENTE},
        {OUT_DR_DIREITA,  OUT_AR_FRONTAL, OUT_DM_FRENTE},
    //obstáculo a direita
        {OUT_DR_ESQUERDA, OUT_AR_LATERAL, OUT_DM_FRENTE}, 
        {OUT_DR_ESQUERDA, OUT_AR_LATERAL, OUT_DM_FRENTE},
        {OUT_DR_ESQUERDA, OUT_AR_LATERAL, OUT_DM_FRENTE},
        {OUT_DR_ESQUERDA, OUT_AR_LATERAL, OUT_DM_FRENTE},
        {OUT_DR_ESQUERDA, OUT_AR_LATERAL, OUT_DM_FRENTE}, 
        {OUT_DR_ESQUERDA, OUT_AR_LATERAL, OUT_DM_FRENTE},
        {OUT_DR_ESQUERDA, OUT_AR_LATERAL, OUT_DM_FRENTE},
        {OUT_DR_ESQUERDA, OUT_AR_LATERAL, OUT_DM_FRENTE},
    //obstáculo a direita (diagonal)
        {OUT_DR_ESQUERDA, OUT_AR_DIAGONAL, OUT_DM_FRENTE}, 
        {OUT_DR_ESQUERDA, OUT_AR_DIAGONAL, OUT_DM_FRENTE},
        {OUT_DR_ESQUERDA, OUT_AR_DIAGONAL, OUT_DM_FRENTE},
        {OUT_DR_ESQUERDA, OUT_AR_DIAGONAL, OUT_DM_FRENTE},
        {OUT_DR_ESQUERDA, OUT_AR_DIAGONAL, OUT_DM_FRENTE}, 
        {OUT_DR_ESQUERDA, OUT_AR_DIAGONAL, OUT_DM_FRENTE},
        {OUT_DR_ESQUERDA, OUT_AR_DIAGONAL, OUT_DM_FRENTE},
        {OUT_DR_ESQUERDA, OUT_AR_DIAGONAL, OUT_DM_FRENTE},
    //obstaculo a esquerda
        {OUT_DR_DIREITA, OUT_AR_LATERAL, OUT_DM_FRENTE}, 
        {OUT_DR_DIREITA, OUT_AR_LATERAL, OUT_DM_FRENTE},
        {OUT_DR_DIREITA, OUT_AR_LATERAL, OUT_DM_FRENTE},
        {OUT_DR_DIREITA, OUT_AR_LATERAL, OUT_DM_FRENTE},
        {OUT_DR_DIREITA, OUT_AR_LATERAL, OUT_DM_FRENTE}, 
        {OUT_DR_DIREITA, OUT_AR_LATERAL, OUT_DM_FRENTE},
        {OUT_DR_DIREITA, OUT_AR_LATERAL, OUT_DM_FRENTE},
        {OUT_DR_DIREITA, OUT_AR_LATERAL, OUT_DM_FRENTE},
    //obstaculo a esquerda (diagonal)
        {OUT_DR_DIREITA, OUT_AR_DIAGONAL, OUT_DM_FRENTE}, 
        {OUT_DR_DIREITA, OUT_AR_DIAGONAL, OUT_DM_FRENTE},
        {OUT_DR_DIREITA, OUT_AR_DIAGONAL, OUT_DM_FRENTE},
        {OUT_DR_DIREITA, OUT_AR_DIAGONAL, OUT_DM_FRENTE},
        {OUT_DR_DIREITA, OUT_AR_DIAGONAL, OUT_DM_FRENTE}, 
        {OUT_DR_DIREITA, OUT_AR_DIAGONAL, OUT_DM_FRENTE},
        {OUT_DR_DIREITA, OUT_AR_DIAGONAL, OUT_DM_FRENTE},
        {OUT_DR_DIREITA, OUT_AR_DIAGONAL, OUT_DM_FRENTE},
    //obstaculo muito perto a frente
        {OUT_DR_ESQUERDA, OUT_AR_DIAGONAL, OUT_DM_RE}, 
        {OUT_DR_DIREITA,  OUT_AR_DIAGONAL, OUT_DM_RE},
        {OUT_DR_DIREITA,  OUT_AR_DIAGONAL, OUT_DM_RE},
        {OUT_DR_ESQUERDA, OUT_AR_DIAGONAL, OUT_DM_RE},

        {OUT_DR_ESQUERDA, OUT_AR_DIAGONAL, OUT_DM_RE}, 
        {OUT_DR_DIREITA,  OUT_AR_DIAGONAL, OUT_DM_RE},
        {OUT_DR_DIREITA,  OUT_AR_DIAGONAL, OUT_DM_RE},
        {OUT_DR_ESQUERDA, OUT_AR_DIAGONAL, OUT_DM_RE},
    //sem bostaculos proximos 
        {OUT_DR_FRENTE,  OUT_AR_SEM_ROTACAO, OUT_DM_FRENTE}, 
        {OUT_DR_FRENTE,  OUT_AR_SEM_ROTACAO, OUT_DM_FRENTE},
        {OUT_DR_FRENTE,  OUT_AR_SEM_ROTACAO, OUT_DM_FRENTE},
        {OUT_DR_FRENTE,  OUT_AR_SEM_ROTACAO, OUT_DM_FRENTE},
        {OUT_DR_FRENTE,  OUT_AR_SEM_ROTACAO, OUT_DM_FRENTE}, 
        {OUT_DR_FRENTE,  OUT_AR_SEM_ROTACAO, OUT_DM_FRENTE},
        {OUT_DR_FRENTE,  OUT_AR_SEM_ROTACAO, OUT_DM_FRENTE},
        {OUT_DR_FRENTE,  OUT_AR_SEM_ROTACAO, OUT_DM_FRENTE}
    };
    
    //--

public:
    NeuralNetwork();
    void treinarRedeNeural();
    void inicializacaoPesos();
    int treinoInicialRede();
    void PrintarValores();
    ExpectedMovement testarValor();
    ExpectedMovement definirAcao(int sensor0, int sensor1, int sensor2, int sensor3, int sensor4, int sensor5, int sensor6, int sensor7);
    void validarRedeNeural();
    void treinarValidar();
    void normalizarEntradas();
    void setupCamadas() ;
};

#endif // NEURALNETWORK_H
