# VcpkgPipe
## VS2019로 구성된 이유
1. 현재 wxWidgets 라이브러는 vcpkg와 호환성 문제가 있음. 따라서 Vcpkg, CMake, Ninja로 빌드불가 상태
2. 정확한 원인은 wxWidgets의 결과물 출력과 config파일의 비표준으로 인한 것. 이미 해당 vcpkg issue가 생성되어 있음
3. 따라서 해결될 때 까지 VS로 프로젝트를 구성함
4. ! 반드시 빌드되기 위해서는 WXUSINGDLL=1 전처리기에 정의 할 것
5. 이 프로젝트 빌드 구성은 임시인 것을 명심

## 배포구성
> vpp 실행파일이 있는 root path를 기준으로 상대적 위치로 배치
> 실제 프로젝트 Root path $(RealProjectRootPath) 속성 매크로 추가
1. 실행파일 /vpp.exe
2. 설정파일 /configs
3. GUI 구성파일 /gui
4. 설명파일 /README.md
5. 라이센스 /LICENSE
