#include "LedControl.h"
#include <TimerOne.h>

const int botao_O = 3, botao_N = 4, botao_S = 5, botao_L = 6, led = 8;

LedControl tabuleiro = LedControl(12, 11, 10, 1); // cria uma instância de uma Matriz
                                                 //pino 12 - DIN
                                                 //pino 11 - CLK
                                                 //pino 10 - CS

typedef struct cobra{
  int cabeca_linha;
  int cabeca_coluna;
  int tamanho;
  char posicao;
};


typedef struct comida{ //DEFINE A COMIDA
  int coluna;
  int linha;
};

cobra cobrinha;

comida comidinha;

int **matriz_tabuleiro;

int *vetor_linha, *vetor_coluna;
int comida_linha, comida_coluna;

void nasce_cobrinha(){
  int aux = random(4);
  int i;
  if(aux == 1){ // Oeste
    vetor_linha[0] = random(8);
    vetor_coluna[0] = random(5) + 1;
    cobrinha.posicao = 'O';
    cobrinha.tamanho = 3;
    vetor_linha[1] = vetor_linha[0];
    vetor_linha[2] = vetor_linha[0];
    vetor_coluna[1] = vetor_coluna[0] + 1;
    vetor_coluna[2] = vetor_coluna[1] + 1;
  }
  else if(aux == 2){ // Norte
    vetor_linha[0] = random(5)+ 2;
    vetor_coluna[0] = random(8);
    cobrinha.posicao = 'N';
    cobrinha.tamanho = 3;
    vetor_linha[1] = vetor_linha[0] - 1;
    vetor_coluna[1] = vetor_coluna[0];
    vetor_linha[2] = vetor_linha[1] - 1;
    vetor_coluna[2] = vetor_coluna[0];
  }
  else if(aux == 3){ // Leste
    vetor_coluna[0] = random(5)+2;
    vetor_linha[0] = random(8);
    cobrinha.posicao = 'L';
    cobrinha.tamanho = 3;
    vetor_linha[1] = vetor_linha[0] ;
    vetor_coluna[1] = vetor_coluna[0]-1;
    vetor_linha[2] = vetor_linha[0];
    vetor_coluna[2] = vetor_coluna[1]-1;
  }
  else{ // Sul
    vetor_linha[0] = random(5)+1;
    vetor_coluna[0] = random(8);
    cobrinha.posicao = 'S';
    cobrinha.tamanho = 3;
    vetor_linha[1] = vetor_linha[0]+1 ;
    vetor_coluna[1] = vetor_coluna[0];
    vetor_linha[2] = vetor_linha[1]+1;
    vetor_coluna[2] = vetor_coluna[1];
    
  }
}

void nasce_comida(){ //DEFINE A POSIÇÃO DA COMIDA
  comida_linha = random(8);
  comida_coluna = random(8);
  if( (comidinha.linha == vetor_linha) && (comida_coluna == vetor_coluna)){
    comida_linha = random(8);
    comida_coluna = random(8);
    }
  /*
  comidinha.coluna = random(8);
  comidinha.linha = random(8);
  while ((cobrinha.posicao == 'L' || cobrinha.posicao == 'O') && comidinha.linha == cobrinha.cabeca_linha){
      comidinha.linha = random(8);
  }
  while ((cobrinha.posicao == 'N' || cobrinha.posicao == 'S') && comidinha.coluna == cobrinha.cabeca_coluna){
      comidinha.coluna = random(8);
  }*/
}


void setup() {
  pinMode(led, OUTPUT);
  pinMode(botao_L, INPUT_PULLUP);
  pinMode(botao_O, INPUT_PULLUP);
  pinMode(botao_N, INPUT_PULLUP);
  pinMode(botao_S, INPUT_PULLUP);
  tabuleiro.shutdown(0, false); // habilita o display
  tabuleiro.setIntensity(0, 1); // define o brilho dos leds
 /*tabuleiro.clearDisplay(0);    // limpa o display*/
  randomSeed(analogRead(0));   //
  Serial.begin(9600);
  Timer1.initialize(7500000);
  Timer1.attachInterrupt(nasce_comida);
  matriz_tabuleiro = (int**)calloc(8,sizeof(int*));
  vetor_linha = (int*)calloc(3,sizeof(int));
  vetor_coluna = (int*)calloc(3,sizeof(int));
  int i, j;
  for(i=0; i<8; i++){
    matriz_tabuleiro[i] = (int*)calloc(8,sizeof(int));
  }
  /*for(i=0; i<8; i++){
    for(j=0; j<8; j++){
      matriz_tabuleiro[i][j] = 1;
      Serial.print(matriz_tabuleiro[i][j]);
      Serial.print(" ");
    }
    Serial.print("\n");
  }
  */
  //nasce_comida();
  nasce_cobrinha();
  }
  //tabuleiro.setLed(0, 2, 1, 1);
  
void imprime_matriz(){
  int i,k,m;
  for(k=0; k<8; k++){
    for(m=0; m<8; m++){
      matriz_tabuleiro[m][k] = 0;
    }
  }
  for(i=0; i<3; i++){
    matriz_tabuleiro[vetor_linha[i]][vetor_coluna[i]] = 1;
  }
  matriz_tabuleiro[comida_linha][comida_coluna] = 1;
  for(k=0; k<8; k++){
    for(m=0; m<8; m++){
      tabuleiro.setLed(0, m, k, matriz_tabuleiro[k][m]);
    }
  }
}

void andar(){
  int j=0, i, bot_O= digitalRead(botao_O),bot_L = digitalRead(botao_L),bot_S = digitalRead(botao_S),bot_N = digitalRead(botao_N);
  if (cobrinha.posicao == 'L'){
    for(j = cobrinha.tamanho - 1; j > 0; j--){
      vetor_coluna[j] = vetor_coluna[j-1];
      vetor_linha[j] = vetor_linha[j-1];
      }
    if(bot_N==LOW){
      cobrinha.posicao = 'N';
      vetor_linha[0] = vetor_linha[0] + 1;
    }
    else if(bot_S==LOW){
      cobrinha.posicao = 'S';
      vetor_linha[0] = vetor_linha[0] - 1;
    }
    else vetor_coluna[0] = vetor_coluna[0] + 1;
    imprime_matriz();
    delay(500);
  }
  
  if (cobrinha.posicao == 'O'){
    for(j = cobrinha.tamanho - 1; j > 0; j--){
      vetor_coluna[j] = vetor_coluna[j-1];
      vetor_linha[j] = vetor_linha[j-1];
      }
    if(bot_N==LOW){
      cobrinha.posicao = 'N';
      vetor_linha[0] = vetor_linha[0] + 1;
    }
    else if(bot_S==LOW){
      cobrinha.posicao = 'S';
      vetor_linha[0] = vetor_linha[0] - 1;
    }
    else vetor_coluna[0] = vetor_coluna[0] - 1;
    imprime_matriz();
    delay(500);
  }
  
  if (cobrinha.posicao == 'N'){
    for(j = cobrinha.tamanho - 1; j > 0; j--){
      vetor_coluna[j] = vetor_coluna[j-1];
      vetor_linha[j] = vetor_linha[j-1];
      }
    if(bot_L==LOW){
      cobrinha.posicao = 'L';
      vetor_coluna[0] = vetor_coluna[0] + 1;
    }
    else if(bot_O==LOW){
      cobrinha.posicao = 'O';
      vetor_coluna[0] = vetor_coluna[0] - 1;
    }
    else vetor_linha[0] = vetor_linha[0] + 1;
    imprime_matriz();
    delay(500);
  }
  
  if (cobrinha.posicao == 'S'){
    for(j = cobrinha.tamanho - 1; j > 0; j--){
      vetor_coluna[j] = vetor_coluna[j-1];
      vetor_linha[j] = vetor_linha[j-1];
      }
    if(bot_L==LOW){
      cobrinha.posicao = 'L';
      vetor_coluna[0] = vetor_coluna[0] + 1;
    }
    else if(bot_O==LOW){
      cobrinha.posicao = 'O';
      vetor_coluna[0] = vetor_coluna[0] - 1;
    }
    else vetor_linha[0] = vetor_linha[0] - 1;
    
    imprime_matriz();
    delay(500);
  }
}

void loop() {
  //direcao();
  //imprime_matriz();
  //delay(500);
 andar();
  /*nasce_comida(cobrinha);
  le_botao();*/
}




  
