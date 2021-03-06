/**
1. ArgumentType은 Boost Tuple로 구현됨
2. 내부구현 항목은 _로 시작함
3. 내부구현 값의 경우 앞뒤로 _가 붙음
*/
#pragma once


#include "CoreHelper.h"

#include <boost/preprocessor/logical/not.hpp>
#include <boost/preprocessor/logical/and.hpp>
#include <boost/preprocessor/logical/or.hpp>
#include <boost/preprocessor/control/while.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/arithmetic/dec.hpp>

#include <boost/preprocessor/comparison/greater_equal.hpp>
#include <boost/preprocessor/comparison/less_equal.hpp>
#include <boost/preprocessor/comparison/equal.hpp>

#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/seq/elem.hpp>
#include <boost/preprocessor/seq/enum.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/seq/pop_front.hpp>
#include <boost/preprocessor/seq/push_back.hpp>

#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/tuple/size.hpp>

#include <boost/preprocessor/variadic/to_seq.hpp>
#include <boost/preprocessor/facilities/overload.hpp>
#include <boost/preprocessor/facilities/is_empty_variadic.hpp>
#include <boost/preprocessor/punctuation/comma.hpp>


/**Arg의 index 정의*/
#define ARG_TYPE_IDX 0
#define ARG_NAME_IDX 1
#define ARG_VALUE_IDX 2


//DArgument에서 값을 정의하지 않을 때 사용
#define DEFINE_ARG_NO

//ArgumentType을 정의
#define DEFINE_ARG(_TYPE_, _NAME_) (_TYPE_, _NAME_)

//Default값을 포함한 ArgumentType을 정의
#define DEFINE_ARG_DEFAULT(_TYPE_, _NAME_, _DEFAULT_) (_TYPE_, _NAME_, _DEFAULT_)

#define ARG_TYPE_DUMMY (0, 0)

/**ArgumentType*/
//기본형
#define EARG_TYPE 2

//기본값 포함
#define EARG_DEFAULT_TYPE 3


/**ArgumentType 크기*/

#define ARG_TYPE_SIZE 2
#define ARG_DEFAULT_TYPE_SIZE 3





/**Utils*/

//0이 반환된다
#define _ARG_IS_NOT_ARG_TYPE(...) 0

//아무것도 하지 않는다
#define _ARG_NOTHING(...)

//가변인자를 그대로 출력
#define _ARG_VA_ARGS_IDENTITY(...) __VA_ARGS__

//_ARG_TYPE_과 _EARG_TYPE_이 일치하면 1, 아니면 0
#define CHECK_ARG_TYPE(_ARG_TYPE_, _EARG_TYPE_) \
BOOST_PP_IF( \
    BOOST_PP_EQUAL( \
        BOOST_PP_TUPLE_SIZE(_ARG_TYPE_), _EARG_TYPE_), \
    1, 0 \
)

//_ARG_TYPE_이 ArgumentType들에 포함되면 1, 아니면 0
#define CHECK_ARG_TYPE_SET(_ARG_TYPE_) \
BOOST_PP_IF( \
    BOOST_PP_OR( \
        CHECK_ARG_TYPE(_ARG_TYPE_, EARG_TYPE), \
        CHECK_ARG_TYPE(_ARG_TYPE_, EARG_DEFAULT_TYPE)), \
    1, 0 \
)


//전달된 IDX가 [Start, End]에 포함되어야 함
#define __ARG_TYPE_GET_BASE(_ARG_TYPE_, _ARG_TYPE_IDX_, _START_IDX_, _END_IDX_, _ARG_SIZE_)\
BOOST_PP_IF( \
    BOOST_PP_AND( \
        BOOST_PP_GREATER_EQUAL(_ARG_TYPE_IDX_, _START_IDX_), \
        BOOST_PP_LESS_EQUAL(_ARG_TYPE_IDX_, _END_IDX_)), \
    BOOST_PP_TUPLE_ELEM, _ARG_IS_NOT_ARG_TYPE\
)(_ARG_SIZE_, _ARG_TYPE_IDX_, _ARG_TYPE_)

#define _ARG_TYPE_GET_IMPL(_ARG_TYPE_, _ARG_TYPE_IDX_) \
BOOST_PP_IF( \
    CHECK_ARG_TYPE(_ARG_TYPE_, EARG_TYPE), \
    __ARG_TYPE_GET_BASE(_ARG_TYPE_, _ARG_TYPE_IDX_, ARG_TYPE_IDX, ARG_NAME_IDX, ARG_TYPE_SIZE),\
    __ARG_TYPE_GET_BASE(_ARG_TYPE_, _ARG_TYPE_IDX_, ARG_TYPE_IDX, ARG_VALUE_IDX, ARG_DEFAULT_TYPE_SIZE) \
)

//idx를 이용해 값을 얻는다. idx로 값을 획득하지 못하면 0
#define ARG_TYPE_GET(_ARG_TYPE_, _ARG_TYPE_IDX_) \
BOOST_PP_IF( \
    CHECK_ARG_TYPE_SET(_ARG_TYPE_), \
    _ARG_TYPE_GET_IMPL, _ARG_IS_NOT_ARG_TYPE \
)(_ARG_TYPE_, _ARG_TYPE_IDX_)





/**Deco Util*/

//ARG_DECO_TYPE을 정의합니다. deco 데이터 중 ,은 불허. _DECO_TYPE_은 Deco함수를 의미함
#define DEFINE_ARG_DECO_TYPE(_DECO_TYPE_, _ARG_TYPE_IDX_, _FRONT_DECO_, _BACK_DECO_)\
(_DECO_TYPE_, _ARG_TYPE_IDX_, _FRONT_DECO_, _BACK_DECO_)

//DECO_TYPE들을 담은 Pack. (Tuple 구성)
#define DEFINE_ARG_DECO_PACK_TYPE(...) \
BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)


//_ARG_TYPE_IDX_, _IN_ARG_TYPE_IDX_ 값이 동일하면 _X_가 활성화 됩니다. 불일치하면 아무것도 없습니다
#define ___DECO_ENABLE(_ARG_TYPE_IDX_, _IN_ARG_TYPE_IDX_, _X_)\
BOOST_PP_IF(\
    BOOST_PP_EQUAL(_ARG_TYPE_IDX_, _IN_ARG_TYPE_IDX_), \
    _X_, DEFINE_ARG_NO \
)

//Type값, Deco가 붙을 곳의 idx, 활성화 기준 idx,....
#define ___DECO_BASE(_ARG_TYPE_, _ARG_TYPE_IDX_, _IN_ARG_TYPE_IDX_, _FRONT_DECO_, _BACK_DECO_)\
___DECO_ENABLE(_IN_ARG_TYPE_IDX_, _ARG_TYPE_IDX_, _FRONT_DECO_)\
_ARG_TYPE_GET_IMPL(_ARG_TYPE_, _IN_ARG_TYPE_IDX_)\
___DECO_ENABLE(_IN_ARG_TYPE_IDX_, _ARG_TYPE_IDX_, _BACK_DECO_)\

//Deco의 공백을 위한 것
#define ___DECO_BASE_FOR_SP(_ARG_TYPE_, _ARG_TYPE_IDX_, _IN_ARG_TYPE_IDX_, _FRONT_DECO_, _BACK_DECO_)\
___DECO_ENABLE(_IN_ARG_TYPE_IDX_, _ARG_TYPE_IDX_, _FRONT_DECO_) \
_ARG_TYPE_GET_IMPL(_ARG_TYPE_, _IN_ARG_TYPE_IDX_) \
___DECO_ENABLE(_IN_ARG_TYPE_IDX_, _ARG_TYPE_IDX_, _BACK_DECO_) \


#define __DECO_ARG_TYPE_IMPL(_ARG_TYPE_, _ARG_TYPE_IDX_, _FRONT_DECO_, _BACK_DECO_, _BASE_MACRO_)\
( \
_BASE_MACRO_(_ARG_TYPE_, _ARG_TYPE_IDX_, ARG_TYPE_IDX, _FRONT_DECO_, _BACK_DECO_), \
_BASE_MACRO_(_ARG_TYPE_, _ARG_TYPE_IDX_, ARG_NAME_IDX, _FRONT_DECO_, _BACK_DECO_) \
) \

#define __DECO_ARG_DEFAULT_TYPE_IMPL(_ARG_TYPE_, _ARG_TYPE_IDX_, _FRONT_DECO_, _BACK_DECO_, _BASE_MACRO_)\
( \
_BASE_MACRO_(_ARG_TYPE_, _ARG_TYPE_IDX_, ARG_TYPE_IDX, _FRONT_DECO_, _BACK_DECO_), \
_BASE_MACRO_(_ARG_TYPE_, _ARG_TYPE_IDX_, ARG_NAME_IDX, _FRONT_DECO_, _BACK_DECO_), \
_BASE_MACRO_(_ARG_TYPE_, _ARG_TYPE_IDX_, ARG_VALUE_IDX, _FRONT_DECO_, _BACK_DECO_) \
) \

//분리를 위한 impl. _BASE_MACRO_는 Deco를 추가할 때 공백삽입 함수를 구분하기 위한것
#define _DECO_ARG_TYPE_IMPL(_ARG_TYPE_, _ARG_TYPE_IDX_, _FRONT_DECO_, _BACK_DECO_, _BASE_MACRO_)\
BOOST_PP_IF(\
    CHECK_ARG_TYPE(_ARG_TYPE_, EARG_TYPE), \
    __DECO_ARG_TYPE_IMPL, __DECO_ARG_DEFAULT_TYPE_IMPL \
)(_ARG_TYPE_, _ARG_TYPE_IDX_, _FRONT_DECO_, _BACK_DECO_, _BASE_MACRO_)


//전달받은 IDX를 통해 값을 앞 뒤로 추가. 만약 ARG_TYPE이 아니라면 0
#define DECO_ARG_TYPE(_ARG_TYPE_, _ARG_TYPE_IDX_, _FRONT_DECO_, _BACK_DECO_)\
BOOST_PP_IF( \
    CHECK_ARG_TYPE_SET(_ARG_TYPE_), \
    _DECO_ARG_TYPE_IMPL, _ARG_IS_NOT_ARG_TYPE \
)(_ARG_TYPE_, _ARG_TYPE_IDX_, _FRONT_DECO_, _BACK_DECO_, ___DECO_BASE)

//전달받은 IDX를 통해 값을 공백을 두고 앞 뒤로 추가. 만약 ARG_TYPE이 아니라면 0
#define DECO_SP_ARG_TYPE(_ARG_TYPE_, _ARG_TYPE_IDX_, _FRONT_DECO_, _BACK_DECO_)\
BOOST_PP_IF( \
    CHECK_ARG_TYPE_SET(_ARG_TYPE_), \
    _DECO_ARG_TYPE_IMPL, _ARG_IS_NOT_ARG_TYPE \
)(_ARG_TYPE_, _ARG_TYPE_IDX_, _FRONT_DECO_, _BACK_DECO_, ___DECO_BASE_FOR_SP)

//DecoType을 _ARG_TYPE_에 적용합니다.
#define DECO_ARG_TYPE_FROM_DECO_TYPE(_DECO_TYPE_, _ARG_TYPE_) \
BOOST_PP_TUPLE_ELEM(0, _DECO_TYPE_)( \
     _ARG_TYPE_, \
    BOOST_PP_TUPLE_ELEM(1, _DECO_TYPE_), BOOST_PP_TUPLE_ELEM(2, _DECO_TYPE_), BOOST_PP_TUPLE_ELEM(3, _DECO_TYPE_) \
)

//DecoType에 들어있는 함수를 호출한다
#define _DECO_ARG_TYPES_IMPL(s, data, elem) \
( DECO_ARG_TYPE_FROM_DECO_TYPE(data, elem) )

//_DECO_TYPE_은 DECO_ARG_TYPE, DECO_SP_ARG_TYPE중 결정. _ARGS_들은 SEQ형식으로 반환됨
#define DECO_ARG_TYPES(_DECO_TYPE_, _ARGS_TYPE_)\
BOOST_PP_SEQ_FOR_EACH(_DECO_ARG_TYPES_IMPL, _DECO_TYPE_, _ARGS_TYPE_)


//DECO_ARG_TYPE_PACK 내부 구현요소
#define _DECO_ARG_TYPE_PACK_EMPTY_DECO_ \
DEFINE_ARG_DECO_TYPE(DECO_ARG_TYPE, ARG_NAME_IDX, DEFINE_ARG_NO, DEFINE_ARG_NO)

//0을 반환하는 함수
#define _DECO_ARG_TYPE_PACK_PRED(n, state) \
    BOOST_PP_TUPLE_ELEM(3, 2, state)    

//첫 연산 분배
#define _DECO_ARG_TYPE_PACK_OP(d, state) \
   _DECO_ARG_TYPE_PACK_OP_D( \
    d, BOOST_PP_TUPLE_ELEM(3, 0, state), BOOST_PP_TUPLE_ELEM(3, 1, state), BOOST_PP_TUPLE_ELEM(3, 2, state) )

#define _DECO_ARG_TYPE_PACK_OP_D(d, pack, arg, c) \
   ( BOOST_PP_SEQ_POP_FRONT(pack), \
    DECO_ARG_TYPE_FROM_DECO_TYPE(BOOST_PP_SEQ_HEAD(pack), arg ), \
    BOOST_PP_DEC(c) )

#define _DECO_ARG_TYPE_PACK_IMPL(_DECO_PACK_, _ARG_TYPE_) \
    BOOST_PP_TUPLE_ELEM( \
        3, \
        1, \
        BOOST_PP_WHILE( \
            _DECO_ARG_TYPE_PACK_PRED, \
            _DECO_ARG_TYPE_PACK_OP, \
            (BOOST_PP_SEQ_PUSH_BACK( \
                _DECO_PACK_, _DECO_ARG_TYPE_PACK_EMPTY_DECO_), _ARG_TYPE_,  BOOST_PP_SEQ_SIZE(_DECO_PACK_)) \
        ) \
    ) \


//DecoPack을 _ARG_TYPE_에 적용합니다.
#define DECO_ARG_TYPE_PACK(_DECO_PACK_, _ARG_TYPE_) \
BOOST_PP_IF( \
    CHECK_ARG_TYPE_SET(_ARG_TYPE_), \
    _DECO_ARG_TYPE_PACK_IMPL, _ARG_IS_NOT_ARG_TYPE \
)(_DECO_PACK_, _ARG_TYPE_)


// DecoType에 들어있는 함수를 호출한다
#define _DECO_ARG_TYPES_PACK_IMPL(s, _DECO_PACK, _ARG) \
( _DECO_ARG_TYPE_PACK_IMPL( _DECO_PACK, _ARG ) )

//_DECO_TYPE_은 DECO_ARG_TYPE, DECO_SP_ARG_TYPE중 결정. _ARGS_들은 SEQ형식으로 반환됨
#define DECO_ARG_TYPES_PACK(_DECO_PACK, _ARGS)\
BOOST_PP_SEQ_FOR_EACH(_DECO_ARG_TYPES_PACK_IMPL, _DECO_PACK, _ARGS)




/**Deploy Utils*/
//type name으로 배치합니다
#define DEPLOY_ARG_TYPE(_ARG_TYPE_)\
ARG_TYPE_GET(_ARG_TYPE_, ARG_TYPE_IDX) \
ARG_TYPE_GET(_ARG_TYPE_, ARG_NAME_IDX)

//type name = value로 배치합니다. 만약 DefaultType이 아니라면 우항 값은 배치되지 않습니다.
#define DEPLOY_ARG_TYPE_VALUE(_ARG_TYPE_) \
BOOST_PP_IF( \
    CHECK_ARG_TYPE(_ARG_TYPE_, EARG_DEFAULT_TYPE), \
    DEPLOY_ARG_TYPE(_ARG_TYPE_) = ARG_TYPE_GET(_ARG_TYPE_, ARG_VALUE_IDX), \
    DEPLOY_ARG_TYPE(_ARG_TYPE_) \
)

//type만 배치를 합니다
#define DEPLOY_ARG_TYPE_ONLY_TYPE(_ARG_TYPE_) \
ARG_TYPE_GET(_ARG_TYPE_, ARG_TYPE_IDX)

//name만 배치를 합니다.
#define DEPLOY_ARG_TYPE_ONLY_NAME(_ARG_TYPE_) \
ARG_TYPE_GET(_ARG_TYPE_, ARG_NAME_IDX)



#define EARG_SPLIT_CHARS 0
#define EARG_SPLIT_COMMA 1

//가변인자를 그대로 출력
#define _ARGS_CHARS_SPLITER_IMPL(_ETYPE_, ...) \
__VA_ARGS__

//지금은 ,밖에 없음
#define _ARGS_SPLITER_IMPL(_ETYPE_, ...) ,

/**
TYPE에 따라 spliter를 선택하여 반환 합니다. 
전달 타입이 EARG_SPLIT_CHARS가 아닐경우 가변인자들은 무시됩니다.
*/
#define ARGS_SPLITER(_ETYPE_, ...) \
BOOST_PP_IF(\
    BOOST_PP_EQUAL(EARG_SPLIT_CHARS, _ETYPE_), \
    _ARGS_CHARS_SPLITER_IMPL, _ARGS_SPLITER_IMPL \
)(_ETYPE_, __VA_ARGS__)


//SPLITER Type을 정의. type, is_empty, args_data
#define DEFINE_ARGS_SPLITER(_ETYPE_, ...) \
( _ETYPE_, BOOST_PP_NOT(BOOST_PP_IS_EMPTY(__VA_ARGS__)), (__VA_ARGS__) )

//타입 정보를 획득
#define ARG_SPLITER_GET_TYPE(_ARGS_SPLITER_TYPE_)\
BOOST_PP_TUPLE_ELEM(0, _ARGS_SPLITER_TYPE_)


//SEQ상태이다. 따라서 그대로 출력하도록 하게 한다.
#define _ARG_SPLITER_GET_VALUES_IMPL(_VALUES_) \
BOOST_PP_CAT(_ARG_VA_ARGS_IDENTITY, BOOST_PP_TUPLE_ELEM(2, _VALUES_))

//분할자 정보를 획득. 값이 없는경우는 아무것도 없다
#define ARG_SPLITER_GET_VALUES(_ARGS_SPLITER_TYPE_)\
BOOST_PP_IF(\
    BOOST_PP_TUPLE_ELEM(1, _ARGS_SPLITER_TYPE_), \
    _ARG_SPLITER_GET_VALUES_IMPL, _ARG_NOTHING \
)(_ARGS_SPLITER_TYPE_)

//TYPE으로 부터 split 문자열을 획득
#define ARGS_SPLITER_FROM_TYPE(_STYPE_) \
ARGS_SPLITER(\
    ARG_SPLITER_GET_TYPE(_STYPE_), \
    ARG_SPLITER_GET_VALUES(_STYPE_) \
)





//DECO( ARG )( DECO2(ARG) ) 로 섞어 ARG_TYPE으로 반환 합니다. 반환된 ARG_TYPE은 이름정보만 유효 
#define ARG_MIXER_FOR_INIT(_ARG_TYPE_, _DECO_TYPE1_, _DECO_TYPE2_) \
DEFINE_ARG( 0, \
    DEPLOY_ARG_TYPE_ONLY_NAME(DECO_ARG_TYPE_FROM_DECO_TYPE(_DECO_TYPE1_, _ARG_TYPE_)) \
    ( DEPLOY_ARG_TYPE_ONLY_NAME(DECO_ARG_TYPE_FROM_DECO_TYPE(_DECO_TYPE2_, _ARG_TYPE_)) )\
)

#define _ARGS_MIXER_FOR_INIT_IMPL(s, data, elem) \
( ARG_MIXER_FOR_INIT(elem, BOOST_PP_TUPLE_ELEM(2, 0, data), BOOST_PP_TUPLE_ELEM(2, 1, data)) )

#define ARGS_MIXER_FOR_INIT(_ARGS_TYPE_, _DECO_TYPE1_, _DECO_TYPE2_) \
BOOST_PP_SEQ_FOR_EACH(_ARGS_MIXER_FOR_INIT_IMPL, (_DECO_TYPE1_, _DECO_TYPE2_), _ARGS_TYPE_)





//내부 유틸함수
//, SEQ size, splitType
#define _DEFINE_ARGS_FOR_EACH_DATA(_DEPLOY_TYPE_, _ARGS_TYPE_, _STYPE_) \
( _DEPLOY_TYPE_, BOOST_PP_SEQ_SIZE(_ARGS_TYPE_), _STYPE_ )

//반복문 종료 카운트 획득
#define _ARGS_GET_END(_ARGS_FOR_EACH_DATA_) \
BOOST_PP_DEC(BOOST_PP_TUPLE_ELEM(1, _ARGS_FOR_EACH_DATA_))

//Split Simbol
#define _ARGS_GET_SIMBOL(_ARGS_FOR_EACH_DATA_) \
ARGS_SPLITER_FROM_TYPE( \
    BOOST_PP_TUPLE_ELEM(2, _ARGS_FOR_EACH_DATA_) \
)


//데이터에 들어온 데이터 심볼 분배
#define __DEPLOY_ARG_TYPES_FUNC_BASE_IMPL(r, data, i, elem) \
BOOST_PP_TUPLE_ELEM(0, data)(elem) \
BOOST_PP_IF( \
    BOOST_PP_NOT_EQUAL(i, _ARGS_GET_END(data)), \
    _ARGS_GET_SIMBOL, _ARG_NOTHING \
)(data)


#define _DEPLOY_ARG_TYPES_FUNC_2(_DEPLOY_TYPE_, _ARGS_TYPE_) \
BOOST_PP_SEQ_FOR_EACH_I( \
    __DEPLOY_ARG_TYPES_FUNC_BASE_IMPL, \
    _DEFINE_ARGS_FOR_EACH_DATA( \
        _DEPLOY_TYPE_, \
        _ARGS_TYPE_, \
        DEFINE_ARGS_SPLITER(EARG_SPLIT_COMMA)\
    ), \
    _ARGS_TYPE_ \
)

#define _DEPLOY_ARG_TYPES_FUNC_3(_DEPLOY_TYPE_, _ARGS_TYPE_, _STYPE_) \
BOOST_PP_SEQ_FOR_EACH_I( \
    __DEPLOY_ARG_TYPES_FUNC_BASE_IMPL, \
    _DEFINE_ARGS_FOR_EACH_DATA( \
        _DEPLOY_TYPE_, \
        _ARGS_TYPE_, \
        _STYPE_ \
    ), \
    _ARGS_TYPE_ \
)

//Type, SplitType, Deco, 그리고 DecoPackType을 이용해 미리 _ARGS_TYPE_들을 꾸민다
#define _DEPLOY_ARG_TYPES_FUNC_4(_DEPLOY_TYPE_, _ARGS_TYPE_, _STYPE_, _DECO_PACK_TYPE_) \
_DEPLOY_ARG_TYPES_FUNC_3( \
    _DEPLOY_TYPE_, \
    DECO_ARG_TYPES_PACK(_DECO_PACK_TYPE_, _ARGS_TYPE_), \
    _STYPE_ \
)


#if (SELECTED_COMPILER == MS_COMPILER) 
//Window일경우 Macro Overload는 Cat로 묶어 구현하도록 권장됨

/**
DeployFunc, ArgsType, SpliterType, DecoType을 전달받아 배치를 수행합니다.
앞의 두개의 인자는 필수 요소 입니다
*/
#define DEPLOY_ARG_TYPES(...) \
    BOOST_PP_CAT(\
        BOOST_PP_OVERLOAD(_DEPLOY_ARG_TYPES_FUNC_,__VA_ARGS__)(__VA_ARGS__), \
        BOOST_PP_EMPTY()) 

#else
    BOOST_PP_OVERLOAD(_DEPLOY_ARG_TYPES_FUNC_, __VA_ARGS__)(__VA_ARGS__)

#endif


//EMPTY가 아니먄 실행할 함수이름이 반환 _FRONT_PRIFIX_는 _EXEC 앞에 간격을 두고 붙음
#define ARG_EXEC_IF_NOT_EMPTY(_EXEC, _ARGS_) \
BOOST_PP_IF( \
    BOOST_PP_GREATER_EQUAL( \
        BOOST_PP_SEQ_SIZE(_ARGS_ (ARG_TYPE_DUMMY)), 2 \
    ), _EXEC, _ARG_NOTHING \
)


#define ARG_COMMA_IF_NOT_EMPTY(_DUUMMY_, _ARGS_) \
BOOST_PP_IF( \
    BOOST_PP_GREATER_EQUAL( \
        BOOST_PP_SEQ_SIZE(_ARGS_ (ARG_TYPE_DUMMY)), 2 \
    ), BOOST_PP_COMMA, BOOST_PP_EMPTY \
)()

#define _ARG_SAMI_PREFIX_() ;

#define ARG_SAMI_IF_NOT_EMPTY(_DUUMMY_, _ARGS_) \
BOOST_PP_IF( \
    BOOST_PP_GREATER_EQUAL( \
        BOOST_PP_SEQ_SIZE(_ARGS_ (ARG_TYPE_DUMMY)), 2 \
    ), _ARG_SAMI_PREFIX_, BOOST_PP_EMPTY \
)()



/**타입정의 추가도움 함수와 미리 만들어진 Preset*/

//ArgumentTypeSEQ를 정의
#define DEFINE_ARG_SEQ(_TYPE_, _NAME_) \
( DEFINE_ARG(_TYPE_, _NAME_) )

//Default값을 포함한 ArgumentTypeSEQ를 정의
#define DEFINE_ARG_DEFAULT_SEQ(_TYPE_, _NAME_, _DEFAULT_) \
( DEFINE_ARG_DEFAULT(_TYPE_, _NAME_, _DEFAULT_) )


//멤버 변수이름 Deco
#define DECO_PRESET_MEMBER_PREFIX \
DEFINE_ARG_DECO_TYPE(DECO_ARG_TYPE, ARG_NAME_IDX, m_, DEFINE_ARG_NO)

#define DECO_PRESET_CONST_MEMBER_PREFIX \
DEFINE_ARG_DECO_TYPE(DECO_SP_ARG_TYPE, ARG_TYPE_IDX, const, DEFINE_ARG_NO)

//선언을 위한 Spliter
#define SPLITER_PRESET_DECL_PREFIX \
DEFINE_ARGS_SPLITER(EARG_SPLIT_CHARS, ;)

//단순한 Comma분할
#define SPLITER_PRESET_COMMA_PREFIX \
DEFINE_ARGS_SPLITER(EARG_SPLIT_COMMA)

//미리정의된 Preset으로 정의된 항목 그대로 값을 포함해 배치합니다
#define DEPLOY_ARG_TYPES_PRESET_PURE(_ARGS_TYPE_) \
DEPLOY_ARG_TYPES(DEPLOY_ARG_TYPE_VALUE, _ARGS_TYPE_)

//미리정의된 Preset으로 Type정보만 배치합니다
#define DEPLOY_ARG_TYPES_PRESET_TYPES(_ARGS_TYPE_) \
DEPLOY_ARG_TYPES(DEPLOY_ARG_TYPE_ONLY_TYPE, _ARGS_TYPE_)

//미리정의된 Preset으로 이름정보만 배치합니다
#define DEPLOY_ARG_TYPES_PRESET_NAMES(_ARGS_TYPE_) \
DEPLOY_ARG_TYPES(DEPLOY_ARG_TYPE_ONLY_NAME, _ARGS_TYPE_)

//미리정의된 Preset으로 멤버변수로 배치를 시작합니다.
#define DEPLOY_ARG_TYPES_PRESET_MEMBER(_ARGS_TYPE_) \
DEPLOY_ARG_TYPES( \
    DEPLOY_ARG_TYPE, _ARGS_TYPE_, SPLITER_PRESET_DECL_PREFIX, \
    DEFINE_ARG_DECO_PACK_TYPE(DECO_PRESET_MEMBER_PREFIX) \
)

//미리정의된 Preset으로 상수형 멤버변수로 배치를 시작합니다.
#define DEPLOY_ARG_TYPES_PRESET_CONST_MEMBER(_ARGS_TYPE_) \
DEPLOY_ARG_TYPES( \
    DEPLOY_ARG_TYPE, _ARGS_TYPE_, SPLITER_PRESET_DECL_PREFIX, \
    DEFINE_ARG_DECO_PACK_TYPE(DECO_PRESET_CONST_MEMBER_PREFIX, DECO_PRESET_MEMBER_PREFIX) \
)

//미리정의된 Preset으로 생성자 초기화자로 배치
#define DEPLOY_ARG_TYPES_PRESET_INIT(_ARGS_TYPE_) \
DEPLOY_ARG_TYPES( \
    DEPLOY_ARG_TYPE_ONLY_NAME, \
    ARGS_MIXER_FOR_INIT(\
        _ARGS_TYPE_, DECO_PRESET_MEMBER_PREFIX, _DECO_ARG_TYPE_PACK_EMPTY_DECO_ ), \
    SPLITER_PRESET_COMMA_PREFIX \
)
