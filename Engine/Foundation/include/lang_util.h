#pragma once

#include <gsl/gsl_assert>


#define __FD_DISALLOW_CREATE_INSTANCE_RAW(CLASS_NAME)\
    CLASS_NAME() = default; \
    CLASS_NAME(const CLASS_NAME&) = default; \
    CLASS_NAME& operator=(const CLASS_NAME&) = default

/** 해당 매크로를 정의한 클래스는 인스턴스 생성을 비허용 합니다. */
#define FD_DISALLOW_CREATE_INSTANCE(CLASS_NAME) \
    protected: \
        __FD_DISALLOW_CREATE_INSTANCE_RAW(CLASS_NAME)

/** 해당 매크로를 정의한 클래스는 인스턴스 생성을 자식까지 비허용 합니다. */
#define FD_DISALLOW_CREATE_INSTANCE_PERMANENTLY(CLASS_NAME) \
    private: \
        __FD_DISALLOW_CREATE_INSTANCE_RAW(CLASS_NAME)

/** 해당 매크로를 정의한 클래스는 복제 생성자를 비허용 합니다. */
#define FD_DISALLOW_COPY_COTR(CLASS_NAME) \
    private: \
        CLASS_NAME(const CLASS_NAME&) = delete

/** 해당 매크로를 정의한 클래스는 복제 생성자, 대입연산자를 비허용 합니다. */
#define FD_DISALLOW_COPY_COTR_AND_ASSIGN(CLASS_NAME) \
    FD_DISALLOW_COPY_COTR(CLASS_NAME); \
    CLASS_NAME& operator=(const CLASS_NAME&) = delete


/*
    Helper section
*/

/** 순수 인터페이스 속성을 부여. 부여된 객체는 인스턴스 생성을 비허용. */
#define FD_PURE_INTERFACE_PROP(CLASS_NAME) \
    FD_DISALLOW_CREATE_INSTANCE(CLASS_NAME); \
    public: \
        virtual ~CLASS_NAME()
