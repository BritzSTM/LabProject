#pragma once

#define __MACRO_CONNECT(_X_, _Y_) _X_##_Y_

/**코드 내부에서 사용되는 텍스트에 대한 일률적인 인코딩을 적용하기 위한 매크로*/
#define TEXTL(_X_) __MACRO_CONNECT(u8, _X_)
