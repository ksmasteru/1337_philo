#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


pthread_mutex_t i_mutex;
void *routine(void *data)
{
    printf("new thread launchedd\n");
    pthread_mutex_lock(&i_mutex);
    int index = *(int *)data;
    pthread_mutex_unlock(&i_mutex);
    printf("pthread %lu has been launched with index %d",pthread_self(), index);
    return (NULL);
}
int main()
{
    
    pthread_t th[5];
    pthread_mutex_init(&i_mutex, NULL);
    int *index = (int *)malloc(sizeof(int));
    *index = 0;
    for (int i = 0; i < 5; i++)
    {
        pthread_mutex_lock(&i_mutex);
        *index = i;
        pthread_create(th + i, NULL, &routine, (void *)index);
        pthread_mutex_unlock(&i_mutex);
        usleep(100);
    }
    pthread_mutex_unlock(&i_mutex);
}