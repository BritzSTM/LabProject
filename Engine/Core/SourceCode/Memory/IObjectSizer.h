#pragma once


#include "CoreHelper.h"
#include "Common/BaseTypes.h"


namespace Core
{

    /**
    실제 객체가 최종적으로 사용중인
    메모리 크기 반환을 지원하는
    인터페이스 입니다.
    */
    class IObjectSizer
    {
        DISALLOW_CREATE_INSTANCE(IObjectSizer);

    public:
        virtual ~IObjectSizer() {}
        virtual size_t GetObjectSize() const noexcept = 0;

    };

    /**
    인터페이스를 사용하지 않고
    GetObjectSize 메소드를 지원하는 객체를 위한 템플릿 함수
    */
    template <typename _OBJECT_TYPE_>
    FLINE size_t GetObjectSize(const _OBJECT_TYPE_& target) noexcept
    {
        return target.GetObjectSize();
    }

}