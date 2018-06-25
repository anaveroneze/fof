/***************************************************************************
Programa que realiza o agrupamento de partículas de acordo com o raio de
percolação fornecido, usando o algoritmo Friends of Friends.
Ultima atualização:
Autor: Ana Luisa Veroneze Solórzano
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <stdbool.h>
#include "fofnovo.h"

#define original false
#define novo true

int  *igru, N;
float  *x, *y, *z, *v1, *v2, *v3;
float max_x, min_x, max_y, min_y, max_z, min_z;

//---------------------------------------------------------------------------
/********************* Cálculo do tempo em usec *****************/
//---------------------------------------------------------------------------
long getTime(){
  struct timeval time;
  gettimeofday(&time, (struct timezone *) NULL);
  return time.tv_sec*1000000 + time.tv_usec;
}

//---------------------------------------------------------------------------
/********************* Lendo o arquivo de dados de entrada *****************/
//---------------------------------------------------------------------------
bool LeDados(char *filename){
  float vx, vy, vz;
  FILE  *file;
  file = fopen(filename,"rt");

  //Número total de partículas
  fscanf (file, "%d", &N);

  //********* alocando memória************//
  igru  = malloc(sizeof(int)*N);  // índice do grupo
  x  = malloc(sizeof(int)*N);  // posição x da partícula;
  y  = malloc(sizeof(int)*N);  // posição y da partícula;
  z  = malloc(sizeof(int)*N);  // posição z da partícula
  v1 = malloc(sizeof(int)*N);  // coordenada x da velocidade
  v2 = malloc(sizeof(int)*N);  // coordenada y da velocidade
  v3 = malloc(sizeof(int)*N);  // coordenada z da velocidade

  int m;
  fscanf (file, "%d %f %f %f %f %f %f", &m, &x[0], &y[0], &z[0], &v1[0], &v2[0], &v3[0]);
  //printf("0 X: %.5f Y: %.5f Z: %.5f\n", x[0], y[0], z[0]);
  max_x = x[0];
  max_y = y[0];
  max_z = z[0];
  min_x = x[0];
  min_y = y[0];
  min_z = z[0];
  igru[0] = 0;

  for (int i = 1 ; i < N ; i++){
    fscanf (file, "%d %f %f %f %f %f %f", &m, &x[i], &y[i], &z[i], &v1[i], &v2[i], &v3[i]);
    //printf("%d X: %.5f Y: %.5f Z: %.5f\n", i, x[i], y[i], z[i]);
    igru[i] = 0;

    if (x[i] > max_x)
        max_x = x[i];
    if (x[i] < min_x)
        min_x = x[i];
    if (y[i] > max_y)
        max_y = y[i];
    if (y[i] < min_y)
        min_y = y[i];
    if (z[i] > max_z)
        max_z = z[i];
    if (z[i] < min_z)
        min_z = z[i];
  }

  printf("\nMaximos e Minimos:\n\t x_max : %f \t y_max: %f \t z_max: %f \n\t x_min : %f \t y_min: %f \t z_min: %f \n", max_x, max_y, max_z, min_x, min_y, min_z);
  fclose (file);
  return false;
}

//---------------------------------------------------------------------------
/************************* Executando o FoF *********************************/
//---------------------------------------------------------------------------
void Friends(float rperc){

  float dist;
  int i = 0;
  int k = 0;
  int j, l;

  for (i = 0; i < N; i++){
    //Identificador do grupo - Denomina um novo grupo
    if(igru[i] == 0){
      k++;
      igru[i] = k; //Encontra uma partícula sem grupo e começa um grupo para ela
      for (j = i ; j < N ; j++){ //Verifica os indices de igru a partir da partícula sem grupos
        if(igru[j] == k){ //Verifica se é um grupo ainda não definido
          for (l = (i + 1) ; l < N ; l++){ //Então percorre o resto do vetor de grupos
            if (igru[l] == 0){ //Se o grupo na posição l ainda não foi definido:
              //Calcula a distância entre a partícula atual(l) e a anterior(j)
              dist = (x[j] - x[l])*(x[j] - x[l]) + (y[j] - y[l])*(y[j] - y[l]) + (z[j] - z[l])*(z[j] - z[l]);
              if (dist <= (rperc*rperc)) //Se encontrou partículas amigas seta o grupo para k
                igru[l] = k;
            }
          }
        }
      }
    }
  }

/******************** escrevendo arquivo de saida ************************/

  char str1[10],str2[10], str3[10];

  FILE *file;
  time_t timestamp;
  char resultado;
  sprintf(str2, "%.2lf", rperc);
  sprintf(str3, "%ld", time(&timestamp));

  strcpy(str1, "Grupos_RP");
  strcat(str1, str2);
  strcat(str1, "_");
  strcat(str1, str3);

  file = fopen(str1,"w");

  fprintf(file, "%d %d \n", N, k);

  for (i = 0 ; i < N ; i++)
  fprintf(file,"%4d %4d % 10.6e % 10.6e % 10.6e % 10.6e % 10.6e % 10.6e \n", i,igru[i],x[i], y[i],z[i],
  v1[i], v2[i],v3[i]);

  fclose (file);

  printf("Numero total de grupos: ");
  printf("%d \n", k);

  /********* Calculando a quantidade de particulas por grupo ************/
  int nn, si, mult;
  int *Ngr;

  Ngr  = malloc(sizeof(int)*k+1);
    for ( nn = 1; nn <= k; nn++ ){ //Para todos os grupos
      mult = 0;
      for (si = 0; si < N; si++) //Para todas partículas
        if(igru[si] == nn) mult =  mult + 1; //Se a partícula pertence ao grupo analisado
          Ngr[nn] = mult; //Armazena número de partículas por grupo
    }

  int cont1 = 0;
  for (nn = 1 ; nn <= k ; nn++){
    if (Ngr[nn] > 1) cont1++;
  }

  printf("Grupos com massa maior que 1: %d \n", cont1);
  free(Ngr);
}

//---------------------------------------------------------------------------
/**************************** Limpa a memória ******************************/
//---------------------------------------------------------------------------
void LimpaMemoria(void){
  free(igru);
  free(x);
  free(y);
  free(z);
  free(v1);
  free(v2);
  free(v3);
}

//---------------------------------------------------------------------------
/***************************************************************************/
//---------------------------------------------------------------------------
int main(int argc, char **argv){
  float  local_v[100], rperc;
  char *Arg1;
  long start_fof, stop_fof, start_read, stop_read, start_novo, stop_novo;

  if(argc != 3 ){
    puts( "Por favor, entre com o nome do arquivo de dados e o raio de percolação!" );
    exit(1);
  }

  Arg1 = argv[1];
  rperc = atof(argv[2]);

  puts ("Iniciando...\n");

  start_read = getTime();
  LeDados(Arg1);
  stop_read = getTime();

#if original == true
  start_fof = getTime();
  Friends(rperc);
  stop_fof = getTime();
  printf("\nFoF original time: %ld\n", (long)(stop_fof-start_fof));
#endif

#if novo == true
  start_novo = getTime();
  divideEntrada();
  stop_novo = getTime();
  printf("\nFoF novo time: %ld\n", (long)(stop_novo-start_novo));
#endif

  LimpaMemoria();
  printf("Reading time: %ld\n", (long)(stop_read-start_read));
  return 0;
}
