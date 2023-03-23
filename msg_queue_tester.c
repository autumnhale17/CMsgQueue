/* Autumn Hale
 * CSCI 4100 Assignment 3
 * This program tests a message queue by creating multiple sending and 
 * receiving threads
 */

#include <stdio.h>
#include <stdlib.h>
#include "msg_queue.h"

/* Structure to hold thread arguments */
struct thread_args {
  int t_id;
  int num_msgs;
  struct msg_queue * mq;
};

/* Function prototypes */
void thread_args_init(struct thread_args *args, int t_id, int num_msgs,
		      struct msg_queue * mq);
void * send_msgs(void * args);
void * receive_msgs(void * args);

/* Main function */
int main(int argc, char *argv[]) {
  
  pthread_t send0;
  pthread_t send1;
  pthread_t send2;

  pthread_t rec0;
  pthread_t rec1;
  pthread_t rec2;
  pthread_t rec3;

  struct msg_queue mes;
  mq_init(&mes);
  
  struct thread_args t1;
  struct thread_args t2;
  struct thread_args t3;
  
  struct thread_args t4;
  struct thread_args t5;
  struct thread_args t6;
  struct thread_args t7;
  
  thread_args_init(&t1, 0, 8, &mes);
  thread_args_init(&t2, 1, 8, &mes);
  thread_args_init(&t3, 2, 8, &mes);
  
  thread_args_init(&t4, 0, 6, &mes);
  thread_args_init(&t5, 1, 6, &mes);
  thread_args_init(&t6, 2, 6, &mes);
  thread_args_init(&t7, 3, 6, &mes);

  pthread_create(&send0, NULL, send_msgs, &t1);
  pthread_create(&send1, NULL, send_msgs, &t2);
  pthread_create(&send2, NULL, send_msgs, &t3);
  
  pthread_create(&rec0, NULL, receive_msgs, &t4);
  pthread_create(&rec1, NULL, receive_msgs, &t5);
  pthread_create(&rec2, NULL, receive_msgs, &t6);
  pthread_create(&rec3, NULL, receive_msgs, &t7);

  pthread_join(send0, NULL);
  pthread_join(send1, NULL);
  pthread_join(send2, NULL);

  pthread_join(rec0, NULL);
  pthread_join(rec1, NULL);
  pthread_join(rec2, NULL);
  pthread_join(rec3, NULL);

  
}

/* Initialize thread_args structure */
void thread_args_init(struct thread_args *args, int t_id, int num_msgs, struct msg_queue * mq) {
  args->t_id = t_id;
  args->num_msgs = num_msgs;
  args->mq = mq;
}

/* Function for sending thread to execute */
void * send_msgs(void * args) {
  // Cast the void pointer args to pointer to a thread_args struct
  struct thread_args * real_args = (struct thread_args *) args;

  // Declare a mq_msg struct
  struct mq_msg mes;

  // Send num messages specified in thread_args (args) struct
  for(int i = 0; i < real_args->num_msgs; i++) {
    // Initialize the mq_msg structure w appropiate fields
    mes.t_id = real_args->t_id;
    mes.m_id = i + 2;

    // Send the message to the queue in the thread_args struct
    mq_send(real_args->mq, &mes);
    
  }
  return NULL;
  }

/* Function for receiving thread to execute */
void * receive_msgs(void * args) {
  // Cast the void pointer args to pointer to a thread_args struct
  struct thread_args * real_args = (struct thread_args *) args;

  // Declare a mq_msg struct
  struct mq_msg mes;

  // Receive the num messages specified in thread_args (args) struct
  for (int i = 0; i < real_args->num_msgs; i++) {
    mq_receive(real_args->mq, &mes);
    printf("[sender: %d , msg id: %d , receiver: %d ]", mes.t_id, mes.m_id, real_args->t_id);
  }
  return NULL;
  }
