/* Autumn Hale
 * CSCI 4100 Assignment 3
 * Source file for message queue implementation
 */

#include "msg_queue.h"

/* Initialize the message queue */
void mq_init(struct msg_queue * mq) {
  // Initialize in an out 
  mq->in = 0;
  mq->out = 0;

  // Initialize the lock and condition variables
  pthread_mutex_init(&mq->lock, NULL);
  pthread_cond_init(&mq->msg_removed, NULL);
  pthread_cond_init(&mq->msg_added, NULL);
}

/* Send a message to a message queue */
void mq_send(struct msg_queue * mq, struct mq_msg * message) {
  // Acquire the queue lock
  pthread_mutex_lock(&mq->lock);

  // When queue full, wait on the queue's msg_removed condition var
  // Note queue size is global variable
  if ((&mq->in - &mq->out) == QUEUE_SIZE) {
    pthread_cond_wait(&mq->msg_removed, &mq->lock);
  }

  // Copy message from the mq_msg struct to correct queue location
  mq->messages[mq->in - mq->out] = *message;

  mq->in++;

  // Signal the queue's msg_added condition var
  pthread_cond_signal(&mq->msg_added);

  // Release the queue lock
  pthread_mutex_unlock(&mq->lock);
}

/* Receive a message from a message queue */
void mq_receive(struct msg_queue *mq, struct mq_msg * message) {
   // Acquire the queue lock
  pthread_mutex_lock(&mq->lock);

  // While the queue is empty, wait on the queue's msg_added condition var
  if((&mq->in - &mq->out) == 0) {
    pthread_cond_wait(&mq->msg_added, &mq->lock);
  }

  // Copy message from correct queue location to mq_msg struct
  message = &mq->messages[&mq->in - &mq->out];

  mq->out++;

  // Signal the queue's msg_removed condition var
  pthread_cond_signal(&mq->msg_removed);

  // Release the queue lock
  pthread_mutex_unlock(&mq->lock);
}


