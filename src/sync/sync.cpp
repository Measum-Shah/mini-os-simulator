#include "sync.h"

ProducerConsumer::ProducerConsumer(int size)
{
    maxSize = size;
    pthread_mutex_init(&mutex, nullptr);
    pthread_cond_init(&notFull, nullptr);
    pthread_cond_init(&notEmpty, nullptr);
}

ProducerConsumer::~ProducerConsumer()
{
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&notFull);
    pthread_cond_destroy(&notEmpty);
}

void ProducerConsumer::produce(int item)
{
    pthread_mutex_lock(&mutex);
    while(buffer.size() >= maxSize)
        pthread_cond_wait(&notFull, &mutex);

    buffer.push(item);
    std::cout << "Produced: " << item << "\n";

    pthread_cond_signal(&notEmpty);
    pthread_mutex_unlock(&mutex);
}

int ProducerConsumer::consume()
{
    pthread_mutex_lock(&mutex);
    while(buffer.empty())
        pthread_cond_wait(&notEmpty, &mutex);

    int item = buffer.front();
    buffer.pop();
    std::cout << "Consumed: " << item << "\n";

    pthread_cond_signal(&notFull);
    pthread_mutex_unlock(&mutex);
    return item;
}