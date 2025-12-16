#ifndef SYNC_H
#define SYNC_H

#include <pthread.h>
#include <queue>
#include <iostream>

class ProducerConsumer {
public:
    ProducerConsumer(int size);
    ~ProducerConsumer();

    void produce(int item);
    int consume();

private:
    std::queue<int> buffer;
    int maxSize;

    pthread_mutex_t mutex;
    pthread_cond_t notFull;
    pthread_cond_t notEmpty;
};

#endif // SYNC_H
