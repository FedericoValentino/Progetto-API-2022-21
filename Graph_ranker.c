#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int return_value = 0;

struct Graph
{
  int cammino_minimo;
  int indice;
};
typedef struct Graph grafo;

struct Heap
{
  int max_size;
  int curr_size;
};
typedef struct Heap heap;

heap* crea_mucchio(int max)
{
  heap* mucchio = malloc(sizeof(heap));
  mucchio->max_size = max;
  mucchio->curr_size = 0;
  return mucchio;
}

void scambia( grafo *a, grafo *b ) {
  grafo t;
  t = *a;
  *a = *b;
  *b = t;
}

int right_child(grafo A[], int index, heap* mucchio)
{
  return (2*index) + 2;
}

int left_child(grafo A[], int index, heap* mucchio)
{
  return 2*index + 1;
}

int father(grafo A[], int index, heap* mucchio)
{
    return (index-1)/2;
}

void increase_Key(grafo A[], int index, int key, heap* mucchio)
{
  A[index].cammino_minimo = key;
  while(index != 0 && A[father(A, index, mucchio)].cammino_minimo < A[index].cammino_minimo)
  {
    scambia(&A[index], &A[father(A, index, mucchio)]);
    index = father(A, index, mucchio);
  }
}

void inserisci(grafo A[], int key, int indice, heap* mucchio)
{
  mucchio->curr_size++;
  A[mucchio->curr_size - 1].cammino_minimo = -1*INT_MAX;
  A[mucchio->curr_size - 1].indice = indice;
  increase_Key(A, mucchio->curr_size - 1, key , mucchio);
}

void max_heapify(grafo A[], int index, heap* mucchio)
{
  int sx = left_child(A, index, mucchio);
  int dx = right_child(A, index, mucchio);
  int largest = index;
  if(sx < mucchio->curr_size)
  {
    if(A[sx].cammino_minimo > A[largest].cammino_minimo)
    {
      largest = sx;
    }
  }
  if(dx < mucchio->curr_size)
  {
    if(A[dx].cammino_minimo > A[largest].cammino_minimo)
    {
      largest = dx;
    }
  }
  if(largest != index)
  {
    scambia(&A[index], &A[largest]);
    max_heapify(A, largest, mucchio);
  }
}

grafo maximum(grafo A[])
{
  return A[0];
}

grafo extract_max(grafo A[], heap* mucchio)
{
  grafo maxm = A[0];
  A[0] = A[mucchio->curr_size-1];
  mucchio->curr_size--;
  max_heapify(A, 0, mucchio);
  return maxm;
}

int minDistance(int dist[], int sptSet[], int nodi)
{
  int min = INT_MAX;
  int min_index = 0;
  for(int v = 0; v < nodi; v++)
  {
    if (sptSet[v] == 0 && dist[v] <= min)
    {
      min = dist[v];
      min_index = v;
    }
  }
  return min_index;
}


int Dijkstra(int **grafo, int source, int nodi)
{
  int costi[nodi];
  int sptSet[nodi];
  int somma_cammini = 0;
  for (int i = 0; i < nodi; i++)
  {
    costi[i] = INT_MAX;
    sptSet[i] = 0;
  }
  costi[source] = 0;
  for (int count = 0; count < nodi-1; count++)
  {
    int u = minDistance(costi, sptSet, nodi);
    sptSet[u] = 1;
    for (int v = 0; v < nodi; v++)
    {
      if (!sptSet[v] && grafo[u][v] && costi[u] != INT_MAX && costi[u] + grafo[u][v] < costi[v])
      {
          costi[v] = costi[u] + grafo[u][v];
      }
      if (costi[v] != INT_MAX && count == nodi-2)
      {
        somma_cammini = somma_cammini + costi[v];
      }
    }
  }
  return somma_cammini;
}

void AggiungiGrafo(grafo array[], int nodi, int indice, heap* mucchio, FILE* INPUT)
{
  int **matrice;
  int x = 0;
  int y = 0;
  int c;
  matrice = malloc(sizeof(int*) * nodi);
  matrice[y] = malloc(sizeof(int) * nodi);

  while(y<nodi)
  {
    c = getchar();
    int val = 0;

    while(c >= '0' && c <= '9')
    {
       val = val * 10 + c - '0';
       c = getchar();
    }

    matrice[y][x] = val;

    if(c == ',')
    {
      x++;
    }
    else if(c == '\n')
    {
      y++;
      x = 0;
      matrice[y] = malloc(sizeof(int) * nodi);
    }
  }

  int key = Dijkstra(matrice, 0, nodi);
  for(int i = 0; i < nodi; i++)
  {
    free(matrice[i]);
  }
  free(matrice);
  if (mucchio->curr_size < mucchio->max_size)
  {
    inserisci(array, key, indice-1, mucchio);
  }
  else
  {
    grafo max = maximum(array);
    if (key < max.cammino_minimo)
    {
      extract_max(array, mucchio);
      inserisci(array, key, indice-1, mucchio);
    }
  }
}

void print(int n)
{
  if(n / 10 != 0)
  {
    print(n/10);
  }
  putchar((n%10) + '0');
}

void stampaHeap(grafo A[], heap* mucchio)
{
  for(int i=0; i<mucchio->curr_size; i++) {
    //printf("%d",A[i].indice);
    print(A[i].indice);
    if (i != mucchio->curr_size-1)
    {
      printf(" ");
    }
  }
  printf("\n");
}

int main()
{
  FILE *INPUT;
  INPUT = stdin;
  int nodi;
  int topk;
  int numero_grafi = 0;
  char comando[20];

  return_value = fscanf(INPUT, "%d %d", &nodi, &topk);

  grafo *topkeks;
  topkeks = malloc(sizeof(grafo)*numero_grafi);
  heap *mucchio = crea_mucchio(topk);


  while(!feof(INPUT))
  {
    if(fgets(comando, 20, INPUT) == NULL)
    {
      printf("\n");
      break;
    }

    if(comando[0] == 'A')
    {
      numero_grafi++;
      topkeks = realloc(topkeks, sizeof(grafo) * numero_grafi);
      AggiungiGrafo(topkeks, nodi, numero_grafi, mucchio, INPUT);
    }

    if(comando[0] == 'T')
    {
      stampaHeap(topkeks, mucchio);
    }
  }
  free(mucchio);
  fclose(INPUT);
}