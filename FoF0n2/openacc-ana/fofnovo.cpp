#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

extern int  *igru, *iden, N;
extern float  *x, *y, *z, *v1, *v2, *v3;
extern float max_x, min_x, max_y, min_y, max_z, min_z;

typedef struct particulas{
  int id;
  int size;
  int grupo;
  float x, y, z;
  struct particulas *prox;
} particulas;

particulas* totalQuadros[8];

//Função para adicionar uma particula em um quadro
void addParticula (particulas **first, int i, float x, float y, float z) {

    if((*first)->size >= 100){
    //  *first = realloc(*first, 100);
    }

    particulas *nova = new particulas[sizeof(particulas)];
    (*first)->size = (*first)->size + 1;
    nova->id = i;
    nova->x = x;
    nova->y = y;
    nova->z = z;
    nova->grupo = 0;
    nova->size = (*first)->size;
    nova->prox = NULL;
    if(*first != NULL){
      nova->prox = *first;
      *first = nova;
    }
}
//Função para liberar espaço de memória alocado para as partículas de um quadro
void liberaParticulas(particulas **first){

  while(*first != NULL){
    particulas *t = *first;
    *first = t->prox;
    free(t);
  }
}

void quadrosFriends(particulas **first, float rperc){

  float dist;
  int i = 0;
  int k = 0;
  int j, l;

  particulas *igru = new particulas[sizeof(particulas)*(*first)->size];
  particulas *j_value = new particulas[sizeof(particulas)];
  particulas *l_value = new particulas[sizeof(particulas)];
  igru = *first;
  int max_particulas = igru->size;

  //Para todas as partículas daquele grupo aplica o fof:
  for (i = 0; i < max_particulas; i++){
    //Identificador do grupo - Denomina um novo grupo
    if(igru->grupo == 0){
      k++;
      igru->grupo = k; //Encontra uma partícula sem grupo e define um grupo para ela
      printf("\nNovo grupo: %d\n", igru->grupo);
      j_value = igru;
      printf("Particula nº: %d\n", igru->id);
      for (j = i ; j < igru->size ; j++){ //Verifica os indices de igru a partir da partícula sem grupos
        if(igru->grupo == k){ //Verifica se é um grupo ainda não definido
          l_value = igru;
          igru = igru->prox;
          for (l = (i + 1) ; l < igru->size ; l++){ //Então percorre o resto do vetor de grupos
            if (igru->grupo == 0){ //Se o grupo na posição l ainda não foi definido:
              //Calcula a distância entre a partícula atual(l) e a anterior(j)
              dist = (j_value->x - igru->x)*(j_value->x - igru->x) + (j_value->y - igru->y)*(j_value->y - igru->y) + (j_value->z - igru->z)*(j_value->z - igru->z);
              if (dist <= rperc*rperc) //Se encontrou partículas amigas seta o grupo para k
                igru->grupo = k;
            }
            igru = igru->prox;
          }
          //printf("\nIgru pegou l_value: %d", igru->id);
          igru = l_value;
        }
        igru = igru->prox;
      }
      igru = j_value;
    }
    igru = igru->prox;
  }

  particulas *out = new particulas[sizeof(particulas)*(*first)->size];
  out = *first;
  /***************** print dos grupos **********************************/
  for (i = 0 ; i < max_particulas; i++){
    printf("%d %d %d %.4f %.4f %.4f\n", i, out->id, out->grupo, out->x, out->y, out->z);
    out = out->prox;
  }

  printf("Numero total de grupos: ");
    printf("%d \n", k);
/********************escrevendo arquivo de saida ************************

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
  fprintf(file,"%4d % 10d %4d % 10.6e % 10.6e % 10.6e % 10.6e % 10.6e % 10.6e \n", i,iden[i],igru[i],x[i], y[i],z[i],
  v1[i], v2[i],v3[i]);

  fclose (file);

  printf("Numero total de grupos: ");
  printf("%d \n", k);

  /********* Calculando a quantidade de particulas por grupo ************
  int nn, si, mult;
  int *Ngr = (int*)malloc(sizeof(int)*(k+1));
    for ( nn = 1; nn <= k; nn++ ){
      mult = 0;
      for (si = 0; si < N; si++)
        if(igru[si] == nn) mult =  mult + 1;
          Ngr[nn] = mult;
    }

  int cont1 = 0;
  for (nn = 1 ; nn <= k ; nn++)
    if (Ngr[nn] > 1) cont1++;

  printf("Grupos com massa maior que 1: %d \n", cont1);
  free(Ngr);*/
}

void divideEntrada(){

  printf("\nMaximos e Minimos:\n\tx_max : %f\ty_max: %f \tz_max: %f \n\tx_min : %f \ty_min: %f \tz_min: %f \n", max_x, max_y, max_z, min_x, min_y, min_z);

  // Descobre valores médios entre os máximos e mínimos das posições das particulas
  float meioX = min_x + ((max_x - min_x) / 2);
  float meioY = min_y + ((max_y - min_y) / 2);
  float meioZ = min_z + ((max_z - min_z) / 2);

  printf("\nValores médios entre os max e min das posições:");
  printf("\n\tx: %f \ty: %f \tz: %f\n", meioX, meioY, meioZ);

  int quadro;

  //Trabalhando com 8 quadros
  for(int k = 0; k<8; k++){
    totalQuadros[k] = new particulas[sizeof(particulas)*100];
    totalQuadros[k]->size = 0;
    totalQuadros[k]->grupo = 0;
  }

  //Agrupa as particulas
  for(int i = 0; i < N; i++) {
      quadro = 0;
      if (x[i] > meioX)
          quadro += 1;
      if (y[i] > meioY)
          quadro += 2;
      if (z[i] > meioZ)
          quadro += 4;
      addParticula(&totalQuadros[quadro], i, x[i], y[i], z[i]);
      printf("Quadro: %d  Particula: %d X: %.5f Y: %.5f Z: %.5f\n", quadro, totalQuadros[quadro]->id, totalQuadros[quadro]->x, totalQuadros[quadro]->y, totalQuadros[quadro]->z);
  }

  for (int c = 0; c < 8; c++) {
      printf("\nQuadro %d", c);
      printf("\n\tCorpos neste quadro: %d", totalQuadros[c]->size);
  }

  //Testar pra agrupar o quadro 7:
  quadrosFriends(&totalQuadros[7], 0.1);
  //Arrumar aqui:
  //for(int k = 0; k<8; k++){
  //  liberaParticulas(&totalQuadros[0]);
  //}
}
