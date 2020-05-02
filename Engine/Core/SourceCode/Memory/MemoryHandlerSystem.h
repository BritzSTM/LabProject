#pragma once


#include "CoreHelper.h"
#include "Common/EnumExtension.h"
#include "Common/Exception.h"


namespace Core
{

    DEFINE_EXT_ENUM
    (
        enum class, EMemHandlerType, ENUM_SIZE_DEFAULT,
        E_NAME(DefaultCppHandler)
        E_NAME(EmergencyMemoryHandler)
    );


    //메모리가 부족해서 확보되었을시 발생하는 예외
    class SecuredMemoryException final
        : public Exception
    {
    public:
        virtual ~SecuredMemoryException()
        {

        }

        //차후 location에 StackTracer를 적용한다.
        SecuredMemoryException()
            : Exception("SecuredMemoryException", nullptr, 
                EExceptionLevel::Enum::Critical, 0)
        {

        }
    };

    //forward declaration
    class CEmergencyMemory;


    struct SMemoryHandlerInfo
    {
        EMemHandlerType Type;
        
        //OnlyPointer Type
        union UHintType
        {
            void* VoidHint;
            CEmergencyMemory* CEmergencyMemoryHint;
        }Hint;

        static_assert(sizeof(UHintType) == 8, "UHintType did not consist solely of pointer.");
    };

    class CMemoryHandlerSystem final
    {
        DISALLOW_CREATE_INSTANCE_PERMANENTLY(CMemoryHandlerSystem);

    public:
        static bool InitSystem(const EMemHandlerType& type, const void* const hint);
        static void ReleaseSystem() noexcept;
        
        static SMemoryHandlerInfo GetHandlerType() noexcept
        {
            return s_typeInfo;
        }

    private:
        static void _InitDefaultCppHandler(const void* const hint);
        static void _InitEmergencyMemoryHandler(const void* const hint);

        static void _SetDefaultCppHandler() noexcept;
        static void _SetEmergencyMemoryHandler() noexcept;

        static void __cdecl _HandleEmergencyMemory();
    
    private:
        static SMemoryHandlerInfo s_typeInfo;
    };

}