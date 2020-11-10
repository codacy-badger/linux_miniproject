#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>

pthread_t tid[5];
pthread_mutex_t lock;

struct mesg_buffer {
        long mesg_type;
        char mesg_text[100];
} message;

void* doSomeThing(void *arg)
{

        key_t key;
        int msgid;
        int i=0;

        // ftok to generate unique key 
        key = ftok("progfile", 65);

        // msgget creates a message queue 
        // and returns identifier 
        msgid = msgget(key, 0666 | IPC_CREAT);

        pthread_mutex_lock(&lock);
/***********************************************************************/


        // msgrcv to receive message 
        msgrcv(msgid, &message, sizeof(message), 1, 0);

        // display the message 
        printf("Notification Received is : %s \n",message.mesg_text);

/***********************************************************************/
        pthread_mutex_unlock(&lock);
        return NULL;
}
int main(void)
{
    int i = 0;
    int err;

    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }

    while(i < 5)
    {
        err = pthread_create(&(tid[i]), NULL, &doSomeThing, NULL);
        if (err != 0)
            printf("\ncan't create thread :[%s]", strerror(err));
        i++;
    }

    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    pthread_join(tid[2], NULL);
    pthread_join(tid[3], NULL);
    //pthread_join(tid[4], NULL);
    pthread_mutex_destroy(&lock);

    return 0;
}
