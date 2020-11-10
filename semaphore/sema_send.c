// C program to demonstrate working of Semaphores 
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>

sem_t mutex;

struct mesg_buffer {
        long mesg_type;
        char mesg_text[100];
} message;

void* thread(void* arg)
{
        key_t key;
        int msgid;

        // ftok to generate unique key 
        key = ftok("progfile", 65);

        // msgget creates a message queue 
        // and returns identifier 
        msgid = msgget(key, 0666 | IPC_CREAT);
        message.mesg_type = 1;

        //wait 
        sem_wait(&mutex);
/*********************************************************************/
        // msgrcv to receive message 
        msgrcv(msgid, &message, sizeof(message), 1, 0);

        // display the message 
        printf("Notification is : %s \n",message.mesg_text);

/**********************************************************************/
        sem_post(&mutex);
}
int main()
{
        sem_init(&mutex, 0, 1);
        pthread_t t1,t2,t3,t4,t5;
        pthread_create(&t1,NULL,thread,NULL);
        pthread_create(&t2,NULL,thread,NULL);
        pthread_create(&t3,NULL,thread,NULL);
        pthread_create(&t4,NULL,thread,NULL);
        pthread_create(&t5,NULL,thread,NULL);
        pthread_join(t1,NULL);
        pthread_join(t2,NULL);
        pthread_join(t3,NULL);
        pthread_join(t4,NULL);
        pthread_join(t5,NULL);
        sem_destroy(&mutex);
        return 0;
}

