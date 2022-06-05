#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define BUFFSIZE 100
#define NAMESIZE 20

int StoHEX(char fi, char sc);
void error_handling(char *msg);
void * sned_message(void * arg);
void * recv_message(void * arg);

char message[BUFFSIZE];


int main(int argc, char **argv) {
    int sock;
    struct sockaddr_in serv_addr;
    pthread_t snd_thread, rcv_thread;
    void* thread_result;

    char id[100];

    if (argc < 2) {
        printf("you have to write ID\n");
        return 0;
    }
    strcpy(id,argv[1] );
    printf("id : %s\n",id);

    sock=socket(PF_INET, SOCK_STREAM, 0);
    //소켓 구멍 만듬

    if(sock==-1)
        printf("socket() error\n");

    memset(&serv_addr, 0 , sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    //tcp 버전
    serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    serv_addr.sin_port= htons(7989);

    if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr))==-1)
    {
            printf("connect error");
    }
    //unsigned char msg[100] = {0x01,2,3,4,5,6,1,2,3,4,2,1,2,3,0x0c};
    //15개의 패킷을 계속 보냄. 
    char chat[100];
    char msg[1000];
    


    /*쓰레드가 없으니 그냥 while(1)함*/
    while(1){

        printf("채팅 입력 : ");
        gets(chat);
        //여기에서 채팅을 받는다.

        sprintf(msg,"[%s]: %s\n",id,chat);
        //대화 입력받기

        printf("send: %s \n", msg);
        write(sock,msg,strlen(msg)+1);
        //3번째인자에서 msg를 보내고 NULL도 보내야 하니 +1함
        sleep(1);
        //1초에 한번씩 보낸다.
        
    }
    close(sock);
    // 소켓 닫기
    return 0;


}