# Linux System Programming

리눅스 프로그래밍 전공 과목에서 진행한 개인 프로젝트입니다.  
C와 Linux 시스템 API를 활용해 **Unix Small Shell**과 **Directory Explorer**를 구현하며 프로세스 제어, 시그널 처리, 디렉터리 탐색, 파일 메타데이터 조회 과정을 학습했습니다.

\---

## Projects

### 1\. Unix Small Shell

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
  * `\~` 기반 경로 처리
* 현재 작업 디렉터리를 반영한 shell prompt 구성

#### 핵심 학습 내용

`fork()` → `execvp()` → `waitpid()`로 이어지는 Unix 프로세스 실행 흐름과 부모·자식 프로세스의 역할을 직접 구현했습니다.  
또한 background 프로세스 종료 시 `SIGCHLD`를 처리해 자식 프로세스를 회수하고, foreground/background에 따라 `SIGINT` 동작을 구분했습니다.

\---

### 2\. Directory Explorer

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

\---

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

\---

## Suggested Repository Structure

```text
linux-system-programming/
├── small-shell/
│   ├── main.c
│   ├── smallsh.c
│   ├── smallsh.h
│   └── Makefile
├── directory-explorer/
│   ├── main.c
│   ├── dir\_explorer.c
│   ├── dir\_explorer.h
│   └── Makefile
└── README.md
```

\---

## Build \& Run

각 프로젝트 디렉터리의 `Makefile`을 사용하는 경우:

```bash
make
```

생성된 실행 파일을 실행합니다.

```bash
./<executable>
```

실행 파일명은 각 프로젝트의 `Makefile` 설정에 따라 확인하면 됩니다.

\---

## What I Learned

* Linux에서 프로세스가 생성되고 외부 프로그램이 실행되는 흐름
* foreground/background 프로세스의 차이
* signal을 이용한 자식 프로세스 종료 처리
* shell 내부 명령과 외부 명령의 차이
* Linux 디렉터리 탐색 API의 동작 방식
* `stat` 구조체를 이용한 파일 메타데이터 해석
* 파일 permission bit와 사용자·그룹 정보 처리

\---

