# Core Lib

## 의존 라이브러리, 도구
> 1. IdleReflectorTool 1.0 Ver
>    + Core/Reflection/External에 도구에서 제공하는 헤더파일이 위치


## Overview
+ 저수준의 기능집합을 위한 정적 라이브러리
+ 자체 system primitive data type을 제공하므로 이를 사용할 것
+ 반드시 simd, 운영체제 api 같은 플랫폼 종속 명령의 경우 이 라이브러리에서 반드시 래퍼 처리를 해서 제공할 것
+ 최대한 라이브러리 크기를 가볍게 유지할 것
+ c runtime 라이브러리를 다중 스레드로 적용할 것 
+ 경고 수준을 w3이상으로 적용할 것
