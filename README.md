# smart_zone_server
Smart Zone 프로젝트는 라즈베리 파이 5와 카메라 모듈을 이용하여 실시간 모니터링 및 의미 있는 데이터를 모아 그래프로 보여 주는 프로젝트 입니다. 

Smart Zone Server는 Smart Zone Camera와 Smart Zone Client에 사용되는 데이터들을 저장하며 필요에 따라 데이터를 송신/수신합니다. 

### 필요한 패키지 설치

다음 명령어를 사용하여 `SQLite3` 패키지를 설치합니다:

```bash
sudo apt-get install libsqlite3-dev

# 실행
git clone https://github.com/VEDA-EAGLES/smart_zone_server
smart_zone_server/inc/ 경로로 이동합니다.
define.h 의 MY_IP를 "<본인 IP>"로 변경합니다.

smart_zone_server/ 경로의 smart_zone_server.sh 셀스크립트를 실행합니다.
