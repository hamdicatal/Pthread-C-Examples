/* Verilen fonksiyona göre Pi değerini paralel olarak hesaplayan Pthread uygulaması. Thread sayısı ve N değeri çalıştırma esnasında kullanıcı tarafından belirleniyor. */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int n, thread_count;
double pi = 0.0;

void * ParalelPiHesapla(void * thread)
{
  long my_rank = (long)thread;
  int local_n = n/thread_count;
  int local_i = (int)my_rank * local_n;
  int i;
  double local_pi = 0.0;
  int temp_i = 1, temp_n = n; // N thread_count'a esit bolunmuyorsa kullanilacak gecici degiskenler...

  for (i = local_i; i < local_i + local_n; i++) {
    local_pi += (double)(pow(-1, i) / pow(4, i)) * (double)( (2/(4*i+2)) + (1/(4*i+3)) + (2/(4*i+1)) );
  }

  // Eger n sayisi thread sayisine esit bolunmuyorsa;
  if(my_rank == thread_count-1) {
    while(temp_n % thread_count != 0) {
      temp_i = local_i + local_n;

      local_pi += (double)(pow(-1, temp_i) / pow(4, temp_i)) * (double)( (2/(4*temp_i+2)) + (1/(4*temp_i+3)) + (2/(4*temp_i+1)) );

      temp_i++;
      temp_n--;
    }
  }

  pthread_mutex_lock(&mutex);
  pi += local_pi;
  pthread_mutex_unlock(&mutex);
}

int main(int argc, char const *argv[]) {

  printf("Kac thread calisacak: " );
  scanf("%d", &thread_count);

  printf("n degerini girin: " );
  scanf("%d", &n);

  long thread;
  pthread_t * threads;
  threads = malloc(thread_count * sizeof(pthread_t));

  for (thread = 0; thread < thread_count; thread++) {
    pthread_create(&threads[thread], NULL, ParalelPiHesapla, (void*)thread);
  }

  printf("Hesaplanan Pi: %lf\n", pi);

  for (thread = 0; thread < thread_count; thread++) {
    pthread_join(threads[thread], NULL);
  }

  pthread_exit(NULL);
  return 0;
}
