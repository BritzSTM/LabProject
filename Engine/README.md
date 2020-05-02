# Engine
> SA 실행파일, 에디터, 바이너리 생성도구등 핵심라이브러리를 포함한 일체의 집합을 Engine라고 표현


## 규칙
> + 폴더이름은 항상 대문자로 시작
> + 관습적 이름 include, lib, bin등의 이름은 소문자 그대로 사용할 것
> + Install폴더는 컴파일 된 결과물이 위치될 수 있는 곳이며 결과물들은 git에서 관리되지 않음
> + 단 Install/Pre 폴더의 내용물은 유일하게 관리됨. 따라서 컴파일하기 어려운 도구나 소스를 미리 배치시킬 수 있음


## 구성요소
+ [Core](Core/README.md)
+ [ExternalTools](ExternalTools/README.md)