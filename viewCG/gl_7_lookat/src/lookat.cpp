#include <GL/glut.h>
//#include <GL/freeglut_ext.h> //callback da wheel do mouse.
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define SCREEN_X 1000
#define SCREEN_Y 700
#define PI 3.14159265
#define FILEPATH "/home/ana/Documentos/PIBIC-INPE/AlgoritmosFoF/viewCG/gl_7_lookat/particulas.txt"

int   polygonMode = 1;
float rx = 0, rz = 0;
float raio = 1;
float wireframe_x = 8;
float wireframe_y = 8;

float abertura = 100.0;
float znear  = 1;
float zfar   = 500;
float aspect = 1;

float *x, *y, *z, *color_r, *color_g, *color_b;
int *id, *igru, N;
float r, g, b;
bool done;

void LeDados(){
    int m;
    FILE  *fp;
    fp = fopen(FILEPATH,"rt");
    fscanf(fp, "%d", &N);

    //********* alocando memória************//
    id  = new int [N];
    igru = new int[N];
    x  = new float [N];  // posição x da partícula;
    y  = new float [N];  // posição y da partícula;
    z  = new float [N];  // posição z da partícula
    color_r = new float[N];
    color_g = new float[N];
    color_b = new float[N];

    printf("Numero de partículas: %d\n", N);
    for (int i = 0 ; i < N ; i++){
        fscanf(fp, "%d %f %f %f", &id[i], &x[i], &y[i],&z[i]);
        printf("%d %.2f %.2f %.2f\n", id[i], x[i], y[i], z[i]);
        igru[i] = 0;
    }
    fclose (fp);
}


void init()
{

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity( );
   gluPerspective(abertura, aspect, znear, zfar);
   glMatrixMode(GL_MODELVIEW);

   glClearColor(0, 0, 0, 1);

   //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

   glEnable(GL_DEPTH_TEST);
}

//imprime as matrizes da pilha do opengl
void printMatrix(GLenum pname)
{
   GLfloat m[16]; //usado para leitura das matrizes
   glGetFloatv (pname, m);
   int l, c;
   printf("\n\n Matriz = %d", pname);
   for(l=0; l<4; l++)
   {
      printf("\n");
      for(c=0; c<4; c++)
      {
         int pos = l * 4 + c;
          printf(" %.2f", m[pos] );
      }
   }
}

bool Friends(double rperc){

  float dist;
  int i = 0;
  int k = 0;
  int j, l;

  for (i = 0; i < N; i++){
    k++;
    r = rand()/(RAND_MAX + 1.);
    g = rand()/(RAND_MAX + 1.);
    b = rand()/(RAND_MAX + 1.);

    while (igru[i] != 0 ){
        i++; // já tem designação, então passa para o próximo!
    }

    igru[i] = k;

    color_r[i] = r;
    color_g[i] = g;
    color_b[i] = b;
    //printf("K: %d   ", k);

           // printf("%f %f %f\n",color_r[i], color_g[i], color_b[i] );

    for(j = i ; j < N ; j++){

      if(igru[j] == k){

        for (l = (i + 1) ; l < N ; l++){
          if (igru[l] == 0){
            dist = sqrt((x[j] - x[l])*(x[j] - x[l]) + (y[j] - y[l])*(y[j] - y[l]) + (z[j] - z[l])*(z[j] - z[l]) );
            printf("dist: %.2f rperc: %f\n", dist, rperc);
            if (dist <= rperc){
                igru[l] = k;

                color_r[l] = r;
                color_g[l] = g;
                color_b[l] = b;
            }
          }
        }
      }
   }
  }

  printf("Numero total de grupos: ");
  printf("%d\n", k);
  return
  true;

/********************escrevendo arquivo de saida ************************
  char str1[10],str2[10];

  FILE *fp;

  int num;
  char resultado;
  num = sprintf(str2, "%.2lf", rperc);

  strcpy(str1, "Grupos_RP");
  strcat(str1,str2);

  fp = fopen(str1,"w");

  fprintf(fp, "%d %d \n", N, k);

  for (i = 0 ; i < N ; i++)
  fprintf(fp,"%4d % 10d %4d % 10.6e % 10.6e % 10.6e % 10.6e % 10.6e % 10.6e \n", i,iden[i],igru[i],x[i], y[i],z[i],
  v1[i], v2[i],v3[i]);

  fclose (fp);*/

  /********* Calculando a quantidade de particulas por grupo ************/

  /*int nn, si, mult;
  int *Ngr;

  Ngr  = new int [k+1];
    for( nn = 1; nn <= k; nn++ ){
      mult = 0;
      for (si = 0; si < N; si++)
        if(igru[si] == nn) mult =  mult + 1;
            Ngr[nn] = mult;
    }
    int cont1 = 0;
    for (nn = 1 ; nn <= k ; nn++)
        if (Ngr[nn] > 1) cont1++;
    printf("Grupos com massa maior que 1: %d \n", cont1);

    delete Ngr;*/
}


////////////////////////////////////////////////////////////////////////////////////////
void display(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity( );

    gluPerspective(abertura, aspect, znear, zfar);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity( );

    gluLookAt(0, 0, 20,  //from. Posicao onde a camera esta posicionada
             0, 0, 0,  //to. Posicao absoluta onde a camera esta vendo
             0, 1, 0); //up. Vetor Up.

    glRotatef ((GLfloat) rx, 0.0f, 1.0f, 0.0f);
    glRotatef ((GLfloat) rz, 1.0f, 0.0f, 0.0f);

    //print FoF particles
    if(done){
        for(int i=0; i<N; i++){
            glColor3f (color_r[i], color_g[i], color_b[i]);
            glPushMatrix();
            glTranslatef(x[i], y[i], z[i]);
            glRotatef(60.0, 0, 0, 1);
            glutSolidSphere(raio, wireframe_x, wireframe_y);
            glPopMatrix ();
        }
    }

   /*glColor3f(1, 1, 1);
   glBegin(GL_LINE_LOOP);
   float ang, x, y;
   for(int i=0; i <= 30; i++){
        ang = 2*PI*i/30;
        x = cos(ang);
        y = sin(ang);
        glVertex2d(x/30, y/30);
   }
   glEnd();*/

    /*glColor3f (1, 1, 1);
    glPushMatrix ();
       glRotatef(60.0, 1, 0,0);
       glutSolidSphere(raio, wireframe_x, wireframe_y);
    glPopMatrix ();*/

   //todos os objetos estao definidos na origem do sistema global fixo
   //e sao transladados para a posicao destino.
   /*glColor3f(1, 1, 1);
   glBegin(GL_POLYGON);
      glVertex3f(-1, 0, 1);
      glVertex3f(1, 0, 1);
      glVertex3f(1, 0, -1);
      glVertex3f(-1, 0, -1);
   glEnd();

   //bule 1
   glPushMatrix();
      glColor3f(1, 0, 1);
      glTranslated(-0.5, 0.15, 0.5);
      glutWireTeapot(0.2);
   glPopMatrix();

   //bule 2
   glPushMatrix();
      glColor3f(0, 1, 0);
      glTranslated(0.5, 0.15, -0.5);
      glRotated(90, 0, 1, 0);
      glutWireTeapot(0.2);
   glPopMatrix();

   //bule 3
   glColor3f(0, 0, 1);
   glutSolidTeapot(1.2);*/

   glutSwapBuffers();
}


//faz a leitura da entrada do usuario
void keyboard(unsigned char key, int x, int y)
{
   //printf("%c", key);
   key = tolower(key);
   switch(key)
   {
      case 27:
	     exit(0);
	  break;

      case 'w': //wireframe
         if(polygonMode==1)
		 {
		    polygonMode=0;
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	     }
		 else
		 {
		    polygonMode=1;
		    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		 }
		break;
      case '+':
           abertura+=1; break;
      case '-':
           abertura-=1;

   }
}

void MotionFunc(int x, int y)
{
   float dx, dy;
   rx = x;
   rz = y;
}

void MouseFunc(int botao, int estado, int x, int y)
{
   //printf("\n%d %d %d %d", botao, estado, x, y);
}

////////////////////////////////////////////////////////////////////////////////////////
int main (int argc, char **argv){

    char* file;
    double raio;

    printf("Por favor entre com o raio de percolação!\n" );
    scanf("%lf", &raio);

    puts ("Iniciando...\n");
    // inicia o arquivo
    LeDados();
    done = Friends(raio);

    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    glutInitWindowSize (SCREEN_X, SCREEN_Y);
    glutInitWindowPosition (0, 10);
    /* create window */
    glutCreateWindow ("GluLookAt Demo");
    //init();
    printf("Digite W para mudar o modo de visualizacao: wireframe/fill");
    glutDisplayFunc(display);
    glutMotionFunc(MotionFunc);
    glutMouseFunc(MouseFunc);
    glutIdleFunc(display);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}
