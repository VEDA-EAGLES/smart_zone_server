# smart_zone_server

## 의존성 설치

이 프로젝트는 몇 가지 외부 라이브러리에 의존합니다. 아래 명령어들을 사용하여 필요한 패키지를 설치해주세요.

### 필요한 패키지 설치

다음 명령어를 사용하여 `gstreamer`, `SQLite3`, 및 기타 필수 패키지를 설치합니다:

```bash
# gstreamer 관련 패키지 설치
sudo apt-get install --no-install-recommends -y \
    libgstrtspserver-1.0-dev \
    gstreamer1.0-plugins-ugly \
    gstreamer1.0-plugins-base \
    gstreamer1.0-plugins-bad \
    gstreamer1.0-plugins-good \
    gstreamer1.0-x \
    gstreamer1.0-libav \
    gstreamer1.0-tools \
    libglib2.0-dev \
    libgstreamer1.0-0 \
    gstreamer1.0-alsa \
    # SQLite3 헤더 파일 사용을 위한 패키지 설치
    libsqlite3-dev

# 실행
git clone https://github.com/VEDA-EAGLES/smart_zone_server
cd smart_zone_server
./smart_zone_server.sh
