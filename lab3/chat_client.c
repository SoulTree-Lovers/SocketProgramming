#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define BUF_SIZE 100 // 버퍼 크기 정의
#define NAME_SIZE 20 // 이름 크기 정의

void * send_msg(void * arg); // 메시지 전송 함수 선언
void * recv_msg(void * arg); // 메시지 수신 함수 선언
void error_handling(char * msg); // 에러 처리 함수 선언

char name[NAME_SIZE] = "[DEFAULT]"; // 사용자 이름 초기화
char msg[BUF_SIZE]; // 메시지 버퍼

int main(int argc, char *argv[]) {

    int sock; // 소켓 디스크립터 변수
    struct sockaddr_in serv_addr; // 서버 주소 구조체
    pthread_t snd_thread, rcv_thread; // 전송 및 수신 스레드 식별자
    void * thread_return; // 스레드 반환값

    if (argc != 4) { // 명령행 인수가 4개가 아니면
        printf("Usage: %s <IP> <port> <name>\n", argv[0]); // 사용법 출력
        exit(1); // 종료
    }

    sprintf(name, "[%s]", argv[3]); // 사용자 이름 설정
    sock = socket(PF_INET, SOCK_STREAM, 0); // 소켓 생성
    
    memset(&serv_addr, 0, sizeof(serv_addr)); // 서버 주소 구조체 초기화
    serv_addr.sin_family = AF_INET; // 인터넷 주소 체계 설정
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]); // 서버 IP 설정
    serv_addr.sin_port = htons(atoi(argv[2])); // 서버 포트 설정

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) { // 서버에 연결 시도
        error_handling("connect() error"); // 에러 발생 시 처리
    }

    pthread_create(&snd_thread, NULL, send_msg, (void*)&sock); // 전송 스레드 생성
    pthread_create(&rcv_thread, NULL, recv_msg, (void*)&sock); // 수신 스레드 생성
    pthread_join(snd_thread, &thread_return); // 전송 스레드 종료 대기
    pthread_join(rcv_thread, &thread_return); // 수신 스레드 종료 대기

    close(sock); // 소켓 닫기
    return 0; // 종료
}

void * send_msg(void * arg) {
    int sock = *((int*) arg); // 소켓 디스크립터 가져오기
    char name_msg[NAME_SIZE + BUF_SIZE]; // 이름과 메시지를 담을 버퍼

    while (1) { // 무한 루프
        fgets(msg, BUF_SIZE, stdin); // 표준 입력에서 메시지 읽기
        if (!strcmp(msg, "q\n") || !strcmp(msg, "Q\n")) { // 종료 메시지 확인
            close(sock); // 소켓 닫기
            exit(0); // 프로그램 종료
        }
        sprintf(name_msg, "%s %s", name, msg); // 이름과 메시지를 결합하여 문자열 생성
        write(sock, name_msg, strlen(name_msg)); // 소켓을 통해 서버로 메시지 전송
    }

    return NULL; // 스레드 종료
}

void * recv_msg(void * arg) {
    int sock = *((int*) arg); // 소켓 디스크립터 가져오기
    char name_msg[NAME_SIZE + BUF_SIZE]; // 이름과 메시지를 담을 버퍼
    int str_len; // 문자열 길이 변수

    while (1) { // 무한 루프
        str_len = read(sock, name_msg, NAME_SIZE + BUF_SIZE - 1); // 서버로부터 메시지 수신
        if (str_len == -1) { // 에러 발생 시
            return (void*)-1; // -1 반환
        }
        name_msg[str_len] = 0; // 문자열 끝에 NULL 문자 추가
        fputs(name_msg, stdout); // 표준 출력에 메시지 출력
    }

    return NULL; // 스레드 종료
}


void error_handling(char * msg) {
    fputs(msg, stderr); // 에러 메시지 출력
    fputc('\n', stderr); // 개행 문자 출력
    exit(1); // 프로그램 종료
}
