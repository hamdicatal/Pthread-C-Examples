/* Dizide kaç tane asal sayı olduğunu paralel olarak Pthread uygulaması */

#include <stdlib.h>
#include <pthread.h>

#define N 6
#define NUM_THREADS 3

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int dizi[N] = {3,4,5,6,2,9};
int count = 0;

void* Asal(void* thread)
{
  long my_rank = (long) thread;
  int local_n = N / NUM_THREADS;
  int my_first_i = (int)my_rank * local_n;
  int i, j, num, temp = 0, local_count = 0;
  printf("Hello from thread %ld\n", my_rank);
  for (i = my_first_i; i < my_first_i + local_n; i++) {
    num = dizi[i];
    printf("%ld. thread %d sayisina bakiyor.\n", my_rank, num);
    if(num > 1 && num != 2) {
      for (j = 2; j < num; j++) {
        if(num % j == 0) {
          temp++;
        }
      }
    }

    if(num == 2)
      local_count++;
    else if(temp == 0)
      local_count++;

    temp = 0;
  }

  pthread_mutex_lock(&mutex);
  count += local_count;
  pthread_mutex_unlock(&mutex);
}

int main(int argc, char const *argv[]) {
  pthread_t threads[NUM_THREADS];
  long i, thread;

  for (thread = 0; thread < NUM_THREADS; thread++) {
    pthread_create(&threads[thread], NULL, Asal, (void*)thread);
  }

  for (i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[thread], NULL);
  }

  printf("\nDizide toplam %d adet asal sayi mevcut.\n", count);

  pthread_exit(NULL);
  return 0;
}
