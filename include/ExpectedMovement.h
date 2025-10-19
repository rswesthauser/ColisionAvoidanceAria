#ifndef EXPECTEDMOVEMENT_H
#define EXPECTEDMOVEMENT_H

/*
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
*/ 

class ExpectedMovement {
  public:
  float DirecaoRotacao;
  float DirecaoMovimento;
  float AnguloRotacao;

  ExpectedMovement(float _direcaoRotacao, float _direcaoMovimento, float _anguloRotacao);
};


#endif
