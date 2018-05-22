/* N boyutlu dizinin, ilk elemanının dizide kaç kez bulunduğunu paralel olarak hesaplayan Pthread uygulaması */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 10
#define NUM_THREADS 5

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int count = 1;
int thread_count;

char c[N] = {'a','b','a','a','e','a','f','a','b','h'};

void* KacTane(void* thread_id)
{
  long my_rank;
  my_rank = (long)thread_id;
  int local_n = N / thread_count;
  int my_first_i = my_rank * local_n;

  printf("Hello from thread %ld \n", my_rank);

  int local_count = 0;
  int i = 0;

  for(i = my_first_i; i < my_first_i + local_n; i++)
  {
      if(c[0] == c[i])
        local_count++;
  }

  printf("Local count from thread %ld -> %d \n", my_rank, local_count);

  pthread_mutex_lock(&mutex);
  count = count + local_count;
  pthread_mutex_unlock(&mutex);
}

int main(int argc, char const *argv[]) {

  printf("Kac thread calissin?: ");
  scanf("%d", &thread_count);

  pthread_t threads[thread_count];
  long thread;

  for(thread = 0; thread < thread_count; thread++)
  {
    pthread_create(&threads[thread], NULL, KacTane, (void*)thread);
  }

  for ( thread = 0; thread < thread_count; thread++) {
    pthread_join(threads[thread], NULL);
  }

  pthread_exit(NULL);

  printf("Ilk karakterin toplam tekrar sayisi: %d\n", count);

  return 0;
}
