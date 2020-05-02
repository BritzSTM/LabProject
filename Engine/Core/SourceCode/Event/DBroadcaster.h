#pragma once


#include "DArgument.h"
#include "Thread/BlockedRange.h"

#include <tuple>


//ListType 정의
#define _ARG_LISTENER_LIST_(_LIST_TYPE_) DEFINE_ARG_SEQ(_LIST_TYPE_* const, pListenerList)

//기본값을 포함한 상수형 참조 타입으로 배치
#define _DEPLOY_DBROADCASTER_CTOR(_ARGS_TYPE_) \
DEPLOY_ARG_TYPES( \
    DEPLOY_ARG_TYPE_VALUE, _ARGS_TYPE_, \
    SPLITER_PRESET_COMMA_PREFIX, \
    DEFINE_ARG_DECO_PACK_TYPE(DEFINE_ARG_DECO_TYPE(DECO_SP_ARG_TYPE, ARG_TYPE_IDX, const, &)) \
)

//리스너 Arguments로 배치 m_a, m_b....
#define _DEPLOY_DBROADCASTER_NOTIFY(_ARGS_TYPE_) \
DEPLOY_ARG_TYPES( \
    DEPLOY_ARG_TYPE_ONLY_NAME, _ARGS_TYPE_, \
    SPLITER_PRESET_COMMA_PREFIX, DEFINE_ARG_DECO_PACK_TYPE(DECO_PRESET_MEMBER_PREFIX) \
)

/**
ParallelFor를 위한 Broadcaster 정의(tbb) DEFINE_ARG를 통해 매개변수들을 정의할 것
_ARGS_TYPE_은 SEQ를 의미
*/
#define DEFINE_BROADCASTER(_CLASS_NAME_, _EVENT_FUNC_NAME_, _LIST_TYPE_, _ARGS_TYPE_)   \
class _CLASS_NAME_ final{                                                               \
public:                                                                                 \
using DataTupleType = std::tuple<                                                       \
    DEPLOY_ARG_TYPES_PRESET_TYPES(_ARG_LISTENER_LIST_(_LIST_TYPE_) _ARGS_TYPE_ )>;      \
using ListenerType = typename _LIST_TYPE_::ListenerType;                                \
public:                                                                                 \
    ~_CLASS_NAME_() {}                                                                  \
    _CLASS_NAME_(                                                                       \
        DEPLOY_ARG_TYPES_PRESET_PURE(_ARG_LISTENER_LIST_(_LIST_TYPE_))                  \
        ARG_COMMA_IF_NOT_EMPTY(0, _ARGS_TYPE_)                                          \
        ARG_EXEC_IF_NOT_EMPTY(_DEPLOY_DBROADCASTER_CTOR, _ARGS_TYPE_)(_ARGS_TYPE_) )    \
    : DEPLOY_ARG_TYPES_PRESET_INIT(_ARG_LISTENER_LIST_(_LIST_TYPE_) _ARGS_TYPE_) {}     \
                                                                                        \
    void operator()(const Core::blocked_range<size_t>& range) const{                    \
        auto iter = (*m_pListenerList)[range.begin()];                                  \
        for (auto i = range.begin(); i != range.end(); ++i, ++iter){                    \
            (*iter).pListener->_EVENT_FUNC_NAME_(                                       \
                ARG_EXEC_IF_NOT_EMPTY(_DEPLOY_DBROADCASTER_NOTIFY, _ARGS_TYPE_)(_ARGS_TYPE_)); \
        }                                                                               \
    }                                                                                   \
private:                                                                                \
    DEPLOY_ARG_TYPES_PRESET_MEMBER(_ARG_LISTENER_LIST_(_LIST_TYPE_));                   \
    ARG_EXEC_IF_NOT_EMPTY(                                                              \
        DEPLOY_ARG_TYPES_PRESET_CONST_MEMBER, _ARGS_TYPE_)(_ARGS_TYPE_)                 \
    ARG_SAMI_IF_NOT_EMPTY(0, _ARGS_TYPE_)                                               \
}
