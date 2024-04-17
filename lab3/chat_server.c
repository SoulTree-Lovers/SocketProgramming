#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define BUF_SIZE 100
#define MAX_CLNT 256

void * handle_clnt(void * arg); // 각 클라이언트 연결을 처리하는 함수
void send_msg(char * msg, int len); // 모든 클라이언트에게 메시지를 보내는 함수
void error_handling(char * msg); // 에러를 처리하는 함수

int clnt_cnt = 0; // 연결된 클라이언트 수
int clnt_socks[MAX_CLNT]; // 클라이언트 소켓 디스크립터를 저장하는 배열
pthread_mutex_t mutx; // 스레드 간 동기화를 위한 뮤텍스

int main(int argc, char *argv[]) {

    int serv_sock, clnt_sock; // 서버와 클라이언트 소켓 디스크립터
    struct sockaddr_in serv_adr, clnt_adr; // 서버와 클라이언트 주소 구조체
    int clnt_adr_sz; // 클라이언트 주소 구조체의 크기
    pthread_t t_id; // 스레드 ID 변수

    if (argc != 2) { // 포트 번호가 명령줄 인수로 제공되었는지 확인
        printf("Usage: %s <port>\n", argv[0]); // 사용법 메시지 출력
        exit(1); // 에러로 종료
    }

    pthread_mutex_init(&mutx, NULL); // 뮤텍스 초기화

    serv_sock = socket(PF_INET, SOCK_STREAM, 0); // TCP 소켓 생성
    // 서버 주소 구조체 설정
    memset(&serv_adr, 0, sizeof(serv_adr)); 
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY); // 모든 IP 주소에서 연결 수락
    serv_adr.sin_port = htons(atoi(argv[1])); // 명령줄 인수에서 포트 번호 설정

    if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1) { // 소켓을 서버 주소에 바인딩
        error_handling("bind() error"); // 바인딩 실패 시 에러 처리
    }

    if (listen(serv_sock, 5) == -1) { // 연결 요청 대기
        error_handling("listen() error"); // 대기 실패 시 에러 처리
    }

    while (1) { // 메인 서버 루프
        clnt_adr_sz = sizeof(clnt_adr); // 클라이언트 주소 구조체 크기 획득
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz); // 연결 요청 수락
        pthread_mutex_lock(&mutx); // 뮤텍스 잠금
        clnt_socks[clnt_cnt++] = clnt_sock; // 클라이언트 소켓을 배열에 추가
        pthread_mutex_unlock(&mutx); // 뮤텍스 잠금 해제
        pthread_create(&t_id, NULL, handle_clnt, (void*)&clnt_sock); // 클라이언트 처리용 스레드 생성
        pthread_detach(t_id); // 스레드 분리
        printf("Connected client IP: %s \n", inet_ntoa(clnt_adr.sin_addr)); // 클라이언트 IP 주소 출력
    }

    close(serv_sock); // 서버 소켓 닫기
    return 0; // 메인에서 반환
}

void * handle_clnt(void * arg) {
    int clnt_sock = *((int*) arg); // 클라이언트 소켓 디스크립터 획득
    int str_len = 0, i; // 문자열 길이 및 반복문 카운터 변수
    char msg[BUF_SIZE]; // 메시지를 위한 버퍼

    while ((str_len = read(clnt_sock, msg, sizeof(msg))) != 0) { // 클라이언트로부터 메시지 수신
        send_msg(msg, str_len); // 모든 클라이언트에게 메시지 전송
    }

    pthread_mutex_lock(&mutx); // 뮤텍스 잠금
    for (i = 0; i < clnt_cnt; i++) { // 클라이언트 소켓 배열을 반복
        if (clnt_sock == clnt_socks[i]) { // 배열에서 클라이언트 소켓 찾기
            while (i++ < clnt_cnt - 1) { // 배열에서 나머지 요소들을 이동
                clnt_socks[i] = clnt_socks[i + 1]; 
            }
            break; // 반복문 종료
        }
    }
    clnt_cnt--; // 클라이언트 수 감소
    pthread_mutex_unlock(&mutx); // 뮤텍스 잠금 해제
    close(clnt_sock); // 클라이언트 소켓 닫기
    return NULL; // 스레드에서 반환
}

void send_msg(char * msg, int len) {
    int i; // 반복문 카운터
    pthread_mutex_lock(&mutx); // 뮤텍스 잠금
    for (i = 0; i < clnt_cnt; i++) { // 클라이언트 소켓을 반복
        write(clnt_socks[i], msg, len); // 메시지를 클라이언트 소켓에 쓰기
    }
    pthread_mutex_unlock(&mutx); // 뮤텍스 잠금 해제
}

void error_handling(char * msg) {
    fputs(msg, stderr); // 에러 메시지 출력
    fputc('\n', stderr); // 새 줄 문자 출력
    exit(1); // 에러로 종료
}
