# Linux System Programming

리눅스 프로그래밍 전공 과목에서 진행한 개인 프로젝트입니다.  
C와 Linux 시스템 API를 활용해 **Unix Small Shell**과 **Directory Explorer**를 구현하며 프로세스 제어, 시그널 처리, 디렉터리 탐색, 파일 메타데이터 조회 과정을 학습했습니다.

---

## Projects

### 1. Unix Small Shell

사용자 명령을 입력받아 외부 프로그램을 실행하고 foreground/background 프로세스를 처리하는 간단한 Unix Shell입니다.

#### 주요 기능

* 사용자 입력을 token 단위로 분석
* `fork()`로 자식 프로세스 생성
* `execvp()`를 이용한 외부 명령 실행
* `waitpid()` 기반 foreground 프로세스 종료 대기
* background 프로세스 실행 및 PID 출력
* `SIGCHLD` handler와 `waitpid(..., WNOHANG)`을 이용한 종료 자식 프로세스 회수
* foreground/background 실행 방식에 따른 `SIGINT` 처리
* `cd`를 shell 내부 명령으로 처리

  * 홈 디렉터리 이동
  * 절대 경로 이동
  * 상대 경로 이동
  * `~` 기반 경로 처리
* 현재 작업 디렉터리를 반영한 shell prompt 구성

#### 핵심 학습 내용

`fork()` → `execvp()` → `waitpid()`로 이어지는 Unix 프로세스 실행 흐름과 부모·자식 프로세스의 역할을 직접 구현했습니다.  
또한 background 프로세스 종료 시 `SIGCHLD`를 처리해 자식 프로세스를 회수하고, foreground/background에 따라 `SIGINT` 동작을 구분했습니다.

---

### 2. Directory Explorer

현재 작업 디렉터리의 항목을 읽어 디렉터리와 일반 파일을 구분하고, 파일 시스템 메타데이터를 출력하는 CLI 프로그램입니다.

#### 주요 기능

* `getcwd()`를 이용한 현재 작업 경로 조회
* `opendir()` / `readdir()` 기반 디렉터리 엔트리 탐색
* `stat()` 기반 디렉터리/일반 파일 구분
* 파일 메타데이터 조회

  * 접근 권한
  * 소유자
  * 그룹
  * 링크 수
  * 파일 크기
  * 수정 시간
* permission bit를 판별해 `rwx` 형태의 권한 문자열 구성
* `qsort()` 기반 디렉터리·파일 이름순 정렬
* 일반 목록 / 상세 목록 출력 모드 제공
* 사용자 입력에 따라 `chdir()`로 디렉터리 이동

#### 핵심 학습 내용

Linux 파일 시스템에서 디렉터리 엔트리와 `stat` 구조체의 정보를 직접 조회하고, 파일 권한과 소유자 정보 등을 해석해 출력하는 과정을 구현했습니다.

---

## Tech Stack

* **Language**: C
* **Environment**: Linux
* **Process / Signal**

  * `fork()`
  * `execvp()`
  * `waitpid()`
  * `sigaction()`
  * `SIGCHLD`
  * `SIGINT`
* **File System / Directory**

  * `getcwd()`
  * `chdir()`
  * `opendir()`
  * `readdir()`
  * `stat()`
* **Other**

  * `qsort()`
  * POSIX API

---

## Build & Run

Linux 환경에서 GCC와 GNU Make가 필요합니다. Windows에서는 WSL의 Linux 터미널을 사용하세요.

Ubuntu/Debian에서 필요한 도구 설치:

```bash
sudo apt update
sudo apt install build-essential
```

저장소 다운로드(이 명령에는 Git이 필요합니다):

```bash
git clone https://github.com/GwakRY/linux-system-programming.git
cd linux-system-programming
```

Git 없이 GitHub의 **Code → Download ZIP**으로 내려받아 압축을 풀어도 됩니다. 아래 명령은 저장소 최상위 디렉터리에서 실행합니다.

### Unix Small Shell

```bash
make -C small-shell
./small-shell/smallsh
```

실행 파일은 `small-shell/smallsh`입니다. 실행 후 다음 명령을 한 줄씩 입력합니다.

```text
echo hello
pwd
cd small-shell
pwd
cd ..
sleep 1 &
exit
```

- `echo hello`는 `hello`를 출력하고, `pwd`는 현재 경로를 출력합니다.
- `cd` 이후 현재 경로와 프롬프트가 변경됩니다. 인자 없는 `cd`와 `cd ~`는 홈 디렉터리로 이동합니다.
- `sleep 1 &`는 `[Process id] <PID>`를 출력하고 입력을 계속 받습니다.
- `exit` 또는 입력 대기 중 `Ctrl+D`로 종료합니다.

이 프로그램은 학습용 Shell입니다. 파이프·리다이렉션·따옴표 해석·변수 확장과 완전한 작업 제어(job control)는 지원 대상으로 보장하지 않습니다.

### Directory Explorer

```bash
make -C directory-explorer
./directory-explorer/project1
```

실행 파일은 `directory-explorer/project1`입니다. 실행한 위치의 목록이 표시됩니다.

| 입력 | 동작 |
|---|---|
| 화면에 표시된 양의 디렉터리 번호 | 해당 디렉터리로 이동 |
| `-2` | 상세 목록 표시 켜기/끄기 |
| `-1` 또는 `Ctrl+D` | 종료 |

예시 출력(번호·경로·파일 목록은 실행 환경에 따라 달라집니다):

```text
/path/to/linux-system-programming$ 
[1] .
[2] ..
[3] directory-explorer
[4] small-shell
[X] README.md
>>Enter directory number(cancel: -1, -l option : -2):
```

디렉터리는 번호, 일반 파일 등은 `[X]`로 표시됩니다. `-2`를 입력하면 권한·링크 수·소유자·그룹·크기·수정 시간을 함께 표시합니다. 목록은 디렉터리/파일 구분별로 이름순 정렬하며, 각각 최대 1,024개 항목을 표시합니다.

### 빌드 결과 정리

```bash
make -C small-shell clean
make -C directory-explorer clean
```

### 실행 검증

2026-10-04 포트폴리오 정리 과정에서 **Ubuntu 24.04.3 LTS / GCC 13.3.0 / GNU Make 4.3 / Python 3** 환경으로 두 프로그램을 빌드하고 아래 동작을 확인했습니다.

- Shell: 연속 명령, 상대·절대·홈 경로 이동, 세미콜론 명령, 백그라운드 실행, 잘못된 명령·경로 이후 복구, 정상 종료
- Explorer: 일반·상세 목록, 디렉터리 이동, 잘못된 번호·문자 입력 이후 복구, EOF 종료, 긴 파일명 표시

같은 검증은 저장소 최상위에서 실행할 수 있습니다. Python은 검증 스크립트에만 필요합니다.

```bash
make -C small-shell
make -C directory-explorer
python3 tests/smoke.py
```

[실행 검증 스크립트](tests/smoke.py)의 8개 테스트가 통과했습니다. 모든 시그널 상황·파일시스템 오류·대규모 디렉터리를 검증한 것은 아닙니다.

이번 검증에서 발견한 명령 토큰 중복 소비, 초기화되지 않은 시그널 집합, `cd`의 중첩 입력 루프, 경로 문자열의 할당 범위 초과, 디렉터리 번호·EOF 처리 및 디렉터리 핸들 정리는 **프로젝트 당시 구현과 구분되는 후속 수정**입니다.

---

## What I Learned

* Linux에서 프로세스가 생성되고 외부 프로그램이 실행되는 흐름
* foreground/background 프로세스의 차이
* signal을 이용한 자식 프로세스 종료 처리
* shell 내부 명령과 외부 명령의 차이
* Linux 디렉터리 탐색 API의 동작 방식
* `stat` 구조체를 이용한 파일 메타데이터 해석
* 파일 permission bit와 사용자·그룹 정보 처리

---
