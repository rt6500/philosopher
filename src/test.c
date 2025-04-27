#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h> // sleep

typedef enum e_opcode
{
    INIT,
    DESTROY,
    LOCK,
    UNLOCK,
    CREATE,
    JOIN,
    DETACH
} t_opcode;

#include <stdio.h>

static int handle_mutex_error(int status, t_opcode opcode)
{
    if (status == 0)
        return 0;
    printf("Raw error status: %d\n", status);
    if (status == EINVAL)
    {
        if (opcode == INIT)
            printf("Error\nInvalid attribute or mutex pointer\n");
        else
            printf("Error\nInvalid mutex (not initialized?)\n");
    }
    else if (status == EDEADLK)
        printf("Error\nA deadlock would occur\n");
    else if (status == EPERM)
        printf("Error\nThe current thread does not hold a lock on mutex\n");
    else if (status == ENOMEM)
        printf("Error\nNot enough memory to initialize mutex\n");
    else if (status == EBUSY)
        printf("Error\nMutex is still locked (cannot destroy it)\n");
    else
        printf("Error\nUnknown error (%d)\n", status);
    return (1);
}

static int handle_thread_error(int status, t_opcode opcode)
{
    if (status == 0)
        return 0;
    if (status == EINVAL)
    {
        if (opcode == CREATE)
            printf("Error\nInvalid argments.\n");
        else
            printf("Error\nThread is not a joinable thread.\n");
    }
    else if (status == ESRCH)
    {
        if (opcode == DETACH || opcode == JOIN)
            printf("Error\nNo thread with the given ID could be found.\n");
    }
    else if (status == EDEADLK && opcode == JOIN)
        printf("Error\nA deadlock was detected or thread specifies the calling thread.\n");
    else if (status == EAGAIN && opcode == CREATE)
        printf("Error\nInsufficient resources to create another thread.\n");
    else if (status == EPERM && opcode == CREATE)
        printf("Error\nNo permission to set the scheduling policy and parameters.\n");
    else
        printf("Error\nUnknown error (%d)\n", status);
    return (1);
}

// --------- Testing functions -------------

void *dummy_thread(void *arg)
{
    (void)arg;
    sleep(1);
    return NULL;
}

int main(void)
{
    pthread_mutex_t mutex;
    pthread_t tid, tid2;
    int status;

    printf("==== Testing Mutex Errors ====\n");

    // 1. Unlock without locking
	// handle_mutex(&mutex, INIT);
    pthread_mutex_init(&mutex, NULL);
    status = pthread_mutex_unlock(&mutex);
    handle_mutex_error(status, UNLOCK);

    // 2. Double destroy
    // pthread_mutex_destroy(&mutex);
    // status = pthread_mutex_destroy(&mutex);
    // handle_mutex_error(status, DESTROY);

    // 3. Lock uninitialized mutex (Skip this dangerous test)
    // pthread_mutex_t invalid_mutex;
    // status = pthread_mutex_lock(&invalid_mutex); // Undefined behavior
    // handle_mutex_error(status, LOCK);

    printf("\n==== Testing Thread Errors ====\n");

    // 4. Deadlock by joining self (EDEADLK)
    status = pthread_join(pthread_self(), NULL);
    handle_thread_error(status, JOIN);

    // 5. Create thread, detach it, then try joining (EINVAL)
    pthread_create(&tid, NULL, dummy_thread, NULL);
    pthread_detach(tid);
    sleep(1); // ensure the thread finishes
    status = pthread_join(tid, NULL);
    handle_thread_error(status, JOIN);

    // 6. Create a thread, join it normally
    pthread_create(&tid2, NULL, dummy_thread, NULL);
    status = pthread_join(tid2, NULL);
    handle_thread_error(status, JOIN);

    printf("\n==== Test Completed ====\n");
    return 0;
}
