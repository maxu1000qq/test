#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#define  BUF_SIZE 1024
struct my_msg_st
{
    long int type;
    char text[BUF_SIZE];
};

int msgid;
int count=0;
int running=1;
//你好
void *mysend(void *arg){
    while(running){
     char str[100];
     sprintf(str,"this is %d",count++);
     int len=strlen(str);
     struct my_msg_st data;
     data.type=1;
     memcpy(data.text,str,len);
     if(msgsnd(msgid,&data,len,0)==-1){
 	printf("msgsnd faile");
        exit(EXIT_FAILURE);      
     }
     sleep(2); 
    }
    printf("mysend\r\n");
    return (void *)0;
}
void *myrecv(void *arg){
    while(running){
      struct my_msg_st data;
      if(msgrcv(msgid,&data,BUF_SIZE,1,0)==-1){
    	printf("msgrcv failed.\r\n");
	exit(EXIT_FAILURE);
      }
      printf("myrecv %s\r\n",data.text);
    }
    return (void *)0;
}
int main(){
    msgid=msgget((key_t)1234,IPC_CREAT); 
    if(msgid==-1){
      printf("msgid is -1");
      exit(EXIT_FAILURE);
    }
    pthread_t t1,t2;
    int ret=pthread_create(&t1,NULL,mysend,NULL);
    if(ret!=0){perror("error");return 1;} 

    ret=pthread_create(&t2,NULL,myrecv,NULL);
    if(ret!=0){perror("error");return 1;} 

    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    printf("hello world\r\n");
    return 0;
}
