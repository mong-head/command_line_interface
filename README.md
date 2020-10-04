# command_line_interface (2019 1학기)
리눅스 기본적인 명령어 구현하기 (최대한 세세하게)

## 함수 설명

### main함수

```
While(1)
{
Argv, argc를 만들었다. 
구현시에 명령어 개수를 먼저 세고, argc에 넣는다. 그 후 argc수 만큼 동적할당을 받고, argv에 그 내용을 채운다.
동적할당 한 것은 매 반복문마다 명령어가 다르기 때문에 전에 한 명령어가 남아있을 수도 있어서 매번 새롭게 저장하려고 썼다.


argv[0]의 내용에 명령어가 있으므로 그것과 각각의 명령어를 비교해 실행시키는 if else if else문을 만들었다.
 ** 구현한 명령어 : Help, cpu, pro, date, pwd ,exit, Mv, Cat , Exe, Cd, Rm
 ** 예시 : 
  if (명령어 == Help) help 함수
  else (명령어 == cpu) cpu 함수
  ...
  

동적할당했던 argv를 해제했다.
}
```

### 명령어 관련 함수

1. void print_help();
    - 만든 명령어들의 설명 적음
2. void print_date();
    - 현재 날짜 출력함. 윈도우 함수 사용함(GetLocalTime함수)
3. char* change_dir(int argc, char** argv, char tmpDir[]);
    - 명령어와 전에 cd하기 전의 디렉토리 경로를 받아서 현재 디렉토리 경로를 전달한다.
    - tmpDir와 반환값인 char*은 cd –를 구현하기 위해 사용하는 것이다.
    - 경로 . ~ .. 를 처리했다. 여기서 ~의 Home directory는 처음 cli실행시 나왔던 경로이다. .\＇경로’ \＇경로’ 등도 구현했다.
    - 만약 그러한 경로가 없다면 없다는 에러메세지도 출력하게 했다.
4. Void print_processes();
    - 윈도우 함수를 사용(EnumProcesses)해서 출력함
5. void print_cpuLoad();
    - 윈도우 함수 사용(GetSystemTimes)했고, 0.6초 지나고 출력되게 하였으며, 0.6초간 평균을 cpu load로 출력함
6. DWORD subtract_times(FILETIME prev, FILETIME now);
    - cpuload함수 관련하여 전의 시간과 현재의 시간을 빼준다.
 7. void print_ls(char* path);
    - _finddata_t구조체를 사용해서 파일 이름, 속성, 날짜 크기에 대한 정보를 얻었다.
    - _findfirst, _findnext함수를 통해 경로 내부 모든 파일들을 찾았다.
8. void cat(int argc, char** argv);
    - 옵션이(-n)있는 경우와 없는 경우를 나누었다.
    - 파일 출력을 사용해서 읽었다.
9. void my_mv(int argc, char** argv);
    - 먼저 경로가 처음부터 있는게 아니라 .\경로 ~\경로 \경로 이런식으로 되어있는 경우 경로를 바꾸기 위해서 full_directory라는 내가 만든 경로 바꿔주는 함수를 사용했다.
    - ls때 사용했던 _finddata_t를 사용해서 디렉토리 인지 파일인지 구분하여 다르게 적용시켰다.
    - 디렉토리인 경우 rename이라는 함수를 이용했다.
    - 파일인 경우 파일 입출력을 사용했으며, argv[2]의 경로대로 파일을 만들어 파일1의 내용을 복사시킨 후 파일 1을 삭제하는 방법을 사용했다.
10. void my_rm(int argc, char** argv);
    - 파일이나 디렉토리 삭제를 위해서 _rmdir, remove함수를 이용햇고, 그 반환값을 이용하여 만약 그런 파일이나 디렉토리가 없으면 없다고 출력하기 위해 flag라는 변수를 만들었다.
11. void execute(int argc, char** argv);
    - system함수를 사용했다.
12. char* full_directory(char dir[]);
    - 'route' ./'route' /'route' ~/'route' 이런식으로 되어있는 경로 고치기 위해 만든 함수이다.
    - 만약 . 이라고만 한다면 현재 경로를 구해서 그 경로로 반환
    - .\ 경우 . 없애고 현재 경로에 받아온 경로를 붙인다
    - ~\ 경우도 ~ 없애고 home 경로에 받아온 경로를 붙인다
    - 나머지 경우(그냥 이름만 있는 경우)  현재 경로에 \붙이고 받아온 경로를 붙인다.

## test description

![image](https://user-images.githubusercontent.com/52481037/93217943-9c779000-f7a4-11ea-8c50-b8be862c5bfc.png)

Help, cpu, pro, date, pwd ,exit


![image](https://user-images.githubusercontent.com/52481037/93217956-9f728080-f7a4-11ea-9ece-e20d6e7736dc.png)

Mv: 디렉토리 이름 변경

![image](https://user-images.githubusercontent.com/52481037/93217965-a13c4400-f7a4-11ea-8e53-5320cbd0a79c.png)

![image](https://user-images.githubusercontent.com/52481037/93217969-a39e9e00-f7a4-11ea-8d45-70f886f48051.png)

  - Cat –n, 1개, 2개이상 파일 내용 출력

  - Exe

  - Mv:파일 이름 및 디렉토리 변경 

  - Cd –

  - Rm : 파일, 디렉토리


 - 오류입력시 처리

![image](https://user-images.githubusercontent.com/52481037/93217979-a600f800-f7a4-11ea-84a0-c42ebc468604.png)


## self evaluation

- 굉장히 시간이 많이 걸렸다. Cd와 cat같은 경우 최대한 디테일을 살리려고 노력했다. 
이번 과제를 하면서 파일 입출력에 대해 많이 공부했고, c언어에서 동적할당(malloc, free)도 공부 할 수 있었다. 
또한, 리눅스 공부한지 오래돼어 까먹고 있었는데, 다시한번 리눅스 명령어에 대해 공부할 수 있는 기회가 되었다.

