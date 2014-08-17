
CC Tool이란?

CC(Create Component) Tool이란, 컴포넌트 코드 템플릿을 기반으로 자동으로 완성된 코드를 생성해주는 툴입니다.
아래의 간단한 문법을 통해 템플릿에 코드가 삽입되고, 그것을 컴포넌트 코드로 생성해주는 툴입니다.

* 주의 *
- 이 프로그램은 코드의 결함을 체크하지는 않습니다. 따라서, 템플릿과 결합된 코드가 완전하지 않을 수 있습니다.


* 참고 *
- 컴파일을 위한 툴은 통합되지 않고, 별도로 제공할 예정입니다. 


[사용방법]

(1) 디폴트(basic) 템플릿을 기반으로 testcomponent.tag 라는 파일내용을 기반으로 매핑시킨 cpp와 h파일을 생성합니다.
 
$ cc --create testcomponent

- input : testcomponent.tag
- output : testcomponent.cpp, testcomponent.h


[Template Tag]

- $component_name : 만들 컴포넌트 이름. (component_name을 기반으로 class와 파일이 생성)
- $date : 날짜
- $author : 제작한 사람
- $copyright : copyright 문구 입력
- $short_desc : 컴포넌트에 대한 짧은 설명.
- $include_path : include 디렉토리의 상대경로

