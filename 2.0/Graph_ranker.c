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
  return (2*index) +1;
}

int left_child(grafo A[], int index, heap* mucchio)
{
  return 2*index;
}

int father(grafo A[], int index, heap* mucchio)
{
    return index/2;
}

void increase_Key(grafo A[], int index, int key, heap* mucchio)
{
  A[index].cammino_minimo = key;
  while(index > 1 && A[father(A, index, mucchio)].cammino_minimo < A[index].cammino_minimo)
  {
    scambia(&A[index], &A[father(A, index, mucchio)]);
    index = father(A, index, mucchio);
  }
}

void inserisci(grafo A[], int key, int indice, heap* mucchio)
{
  mucchio->curr_size++;
  A[mucchio->curr_size].cammino_minimo = -1*INT_MAX;
  A[mucchio->curr_size].indice = indice;
  increase_Key(A, mucchio->curr_size, key , mucchio);
}

void max_heapify(grafo A[], int index, heap* mucchio)
{
  int sx = left_child(A, index, mucchio);
  int dx = right_child(A, index, mucchio);
  int largest = index;
  if(sx <= mucchio->curr_size && sx > 0)
  {
    if(A[sx].cammino_minimo > A[largest].cammino_minimo)
    {
      largest = sx;
    }
  }
  if(dx <= mucchio->curr_size && dx > 0)
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
  return A[1];
}

grafo extract_max(grafo A[], heap* mucchio)
{
  grafo maxm = A[1];
  A[1] = A[mucchio->curr_size];
  mucchio->curr_size--;
  max_heapify(A, 1, mucchio);
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
  matrice = malloc(sizeof(int*) * nodi);
  for(int i = 0; i < nodi; i++)
  {
    matrice[i] = malloc(sizeof(int) * nodi);
    for (int j = 0; j < nodi; j++)
    {
      return_value = fscanf(INPUT, "%d,", &matrice[i][j]);
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

void stampaHeap(grafo A[], heap* mucchio)
{
  for(int i=1; i<=mucchio->curr_size; i++) {
    printf("%d",A[i].indice);
    if (i != mucchio->curr_size)
    {
      printf(" ");
    }
  }
  printf("\n");
}

int main()
{
  FILE *INPUT;
  INPUT = fopen("input.txt", "r");
  int nodi;
  int topk;
  int numero_grafi = 0;
  char comando[20];

  return_value = fscanf(INPUT, "%d %d", &nodi, &topk);

  grafo topkeks[topk];
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