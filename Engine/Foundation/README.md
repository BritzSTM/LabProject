# Foundation
> + 정적 라이브러리
> + MultiPlatform을 위한 LowLevel 추상화 함수 구현

## 규칙
> + 미리 컴파일된 헤더는 pch.h 이름을 가진다
> + 모든 구현사항은 fd namespace에 종속
> + 모든 매크로 함수는 FD_* prefix 사용
> + fd namespace는 트리 레벨0
> + 상위레벨은 하위레벨에 접근불가. 하위레벨은 상위레벨에 접근가능. 형제레벨 접근가능
