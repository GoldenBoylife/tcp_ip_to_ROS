#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define CLNT_MAX 10 //최대 인원수 
//#define SOCK_STREAM 1  //이미 라이브러리에 있어서 선언됨. 
//TCP프로토콜 방식 할거야.  SOCK_DGRAM 2는 UDP프로토콜, RAW 3은 가공하지 않은 소켓

#define BUFFSIZE 200

/*전역 변수*/
int g_clnt_socks[CLNT_MAX];
int g_clnt_count =0;

void * clnt_connection(void * arg) {
    int clnt_sock = (int)arg;
    int str_len =0;

    char msg[BUFFSIZE];
    int i;

    while(1) {
        str_len = read(clnt_sock,msg,sizeof(msg));
        //read 성공하면 받아들인 숫자가 나옴, 애러면 -1 나옴.
        //read로 client의 msg를 받은 것임.

        /*-1나오면 세션이 끊어진것임.*/
        if(str_len ==-1) {
            printf("clnt[%d] close\n",clnt_sock);
            break;
        }
        printf("%s\n",msg);
        //read 성공 시 출력됨. 

    }
    close(clnt_sock);
    pthread_exit(0);    //메모리 반환
    return NULL;
}

/*입력에 따라서 포트 번호 바꾸도록 하고 싶어서 argv씀*/
int main(int argc, char ** argv) {
    int serv_sock;
    int clnt_sock;
    
    pthread_t t_thread; 

    struct sockaddr_in clnt_addr;
    int clnt_addr_size;

    struct sockaddr_in serv_addr;

    /*빈 껍데기 소켓 만들기*/
    serv_sock = socket(PF_INET,SOCK_STREAM,0);
    //인수1: 소켓을 열거고, tcp ip 4버전과 6버전 중에서 4를 쓸거야. 
    //인수2: 소켓 타입 : SOCK_STREAM : TCP통신할거야.  
    //인수3: 0은 IPPROTO_HOPOPTS를 뜻함. 
    serv_addr.sin_family = AF_INET;    //ip버전 4씀.
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    //소켓 바인드시에 어떤 주소로 넣는지 설정함.  INADD_ANY는 현재의 내  pc아이피다.
    //장소에 따라서 매번 아이피가 달라지기 때문에 이렇게 넣는다.
    //htonl : host 방식을 네트워크 방식으로 변환하고 끝이long이니까 32비트다.
    //serv_addr.sin_port=htons(atoi(argv[1]);
    serv_addr.sin_port=htons(7989);
    //포트 bind
    //htons: host방식을 네트워크 방식으로  끝이 short니까 16비트다. 

    /*바인드 해주지만 애러가 나면 -1 반환한다.*/
    if( bind(serv_sock,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) ==-1){
        printf("bind error\n");
    }

    /*listen하게 되서 고객들의 대기열 갯수가 5개이다. 6명대기하게 되면 애러가 난다.*/
    if(listen(serv_sock,5)==-1){
        printf("listen error\n");
    }

    char buff[200]; //버퍼 최대 크기 200
    int recv_len=0 ; //얼마나 받아 왔는지 
    while(1) {
        clnt_addr_size = sizeof(clnt_addr);
        clnt_sock = accept(serv_sock,(struct sockaddr *) &clnt_addr, &clnt_addr_size);
        //listen이 되면 accept 시킨다. 고객 한명당 1개 생성되는 소켓이다. 
        //clnt_addr 여기 안에 나에게 접속한 고객 정보가 담긴다.(ip정보, 포트)
        //여기서 안 ip와 포트로 ip추적이나 ip차단 할 수도 있다.
        //g_clnt_socks[g_clnt_cont++] = clnt_sock;
        //접속한 고객의 숫자 증가

        /*고객 들어오면 thread 생성해서 callback같은 함수 실행*/
        pthread_create(&t_thread,NULL,clnt_connection,(void *) clnt_sock);
        //clnt_connection함수 써서 clnt_sock을 보낸다.
        

        


    }




}