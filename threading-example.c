#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sched.h>

/* List of numbers to check for primality, up to 65535 */
int nums[] = {
    15147, 22002, 16071, 11039,
    37701, 46619, 51771, 27011,
    33041, 61017, 37023, 10671,
    38223, 45005, 12511, 63001,
    49613, 31471, 20191, 62619,
    17009, 31441, 27911, 52127,
    -1
};

/* Number to check for primality, set by main and prime checker threads */
volatile int test_for_prime = 0;

/* Use these prime factors to test for primality */
const int factors[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23,
    29, 31, 37, 41, 43, 47, 53, 59,
    61, 67, 71, 73, 79, 83, 89, 97,
    101, 103, 107, 109, 113, 127,
    131, 137, 139, 149, 151, 157,
    163, 167, 173, 179, 181, 191,
    193, 197, 199, 211, 223, 227,
    229, 233, 239, 241, 251
};

/* check one number for primality */
int is_prime(int x)
{
    int i = 0;
    if (x == 1) {
        return 0;
    }
    while(i < sizeof(factors)/sizeof(factors[0])) {
        if ((x % factors[i]) == 0) {
            return 0;
        }
        i++;
    }
    return 1;
}

pthread_mutex_t cond_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  cond_test_prime_ready = PTHREAD_COND_INITIALIZER;

void
test_for_primes()
{
    int i = 0;
    while(i < sizeof(nums)/sizeof(nums[0])) {
        pthread_mutex_lock(&cond_mutex);
        while(test_for_prime != 0) {
            pthread_mutex_unlock(&cond_mutex);
            sched_yield();
            pthread_mutex_lock(&cond_mutex);
        }
        test_for_prime = nums[i];
        pthread_cond_signal(&cond_test_prime_ready);
        pthread_mutex_unlock(&cond_mutex);
        i++;
    }
}

void *
thread_main(void *targ)
{
    pthread_mutex_lock(&cond_mutex);
    while (test_for_prime != -1) {
        if (test_for_prime == 0) {
            pthread_cond_wait(&cond_test_prime_ready, &cond_mutex);
            continue;
        }
        int x = test_for_prime;
        test_for_prime = 0;
        pthread_mutex_unlock(&cond_mutex);
        int prime = is_prime(x);
        if (prime) {
            printf("%d is prime\n", x);
        }
        else {
            printf("%d is not prime\n", x);
        }
        pthread_mutex_lock(&cond_mutex);
    }
    // signal the other primality checker threads so that
    // they too may exit
    pthread_cond_broadcast(&cond_test_prime_ready);
    pthread_mutex_unlock(&cond_mutex);
    return NULL;
}

pthread_t * threads;

void
start_threads(int nt)
{
    threads = (pthread_t *)calloc(nt, sizeof(pthread_t));
    int i;
    for(i = 0; i < nt; i++) {
        pthread_create(threads + i, NULL, thread_main, NULL);
    }
}

void
wait_threads(int nt)
{
    int i;
    for(i = 0; i < nt; i++) {
        pthread_join(*(threads + i), NULL);
    }
    free(threads);
    threads = (pthread_t *)NULL;
}

int
main(int argc, char **argv)
{
    int nt = 5;
    start_threads(nt);
    test_for_primes();
    wait_threads(nt);
    return 0;
}
