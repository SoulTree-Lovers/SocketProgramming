# Lab 2-2 분석 결과 (20192883 강승민)

1. **Client 종류:**

   - `echo_client.c` (Lab 1-1)

   - `uecho_client.c` (Lab 2-1)

   - `uecho_con_client.c` (Lab 2-2)



2. **통신 방식의 차이:**

   - `echo_client.c`는 TCP 소켓을 사용하여 서버와의 연결을 설정하고, 데이터를 주고 받는다.

   - `uecho_client.c`는 UDP 소켓을 사용하여 서버와 통신한다.

   - `uecho_con_client.c`는 UDP 소켓을 사용하며, `connect()` 함수를 사용하여 소켓을 연결하고 있다. 하지만 UDP는 연결 지향형이 아닌 비연결형 프로토콜이므로, 실제로 연결이 수립되는 것이 아니라 주소를 목적지 주소로 설정하는 것뿐이다.



3. **데이터 송수신 방식의 차이:**

   - `echo_client.c`에서는 TCP 소켓이므로 `read()`와 `write()` 함수를 사용하여 데이터를 송수신한다.

   - `uecho_client.c`에서는 UDP 소켓이므로 `sendto()`와 `recvfrom()` 함수를 사용하여 데이터를 송수신한다.

   - `uecho_con_client.c`에서는 UDP 소켓을 사용하며, `connect()` 함수로 소켓을 연결하였기 때문에 연결 설정 후 `write()`와 `read()` 함수를 사용하여 데이터를 송수신한다. 하지만 이 방식은 UDP의 특성에 어긋나며, 실제로는 UDP에서 제공하는 비연결성과는 거리가 있다.