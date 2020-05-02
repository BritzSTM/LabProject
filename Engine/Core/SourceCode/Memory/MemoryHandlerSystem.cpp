#include "stdafx.h"
#include "Common/StackTracer.h"
#include "MemoryHandlerSystem.h"
#include "EmergencyMemory.h"

#include <exception>
#include <memory>


namespace Core
{

    bool CMemoryHandlerSystem::InitSystem(const EMemHandlerType& type, const void* const hint)
    {
        using Enum = EMemHandlerType::Enum;

        switch (type)
        {
        case Enum::DefaultCppHandler:
        {
            _InitDefaultCppHandler(hint);
            _SetDefaultCppHandler();
        }
            break;

        case Enum::EmergencyMemoryHandler:
        {
            _InitEmergencyMemoryHandler(hint);
            _SetEmergencyMemoryHandler();
        }
            break;

        default:
            //예외발생
            throw std::invalid_argument("Unknown EMemHandlerType");
        }

        return true;
    }

    void CMemoryHandlerSystem::ReleaseSystem() noexcept
    {
        //기본 CppHandler 적용
        _InitDefaultCppHandler(nullptr);
        _SetDefaultCppHandler();
    }

    void CMemoryHandlerSystem::_SetDefaultCppHandler() noexcept
    {
        std::set_new_handler(nullptr);
    }

    void CMemoryHandlerSystem::_SetEmergencyMemoryHandler() noexcept
    {
        std::set_new_handler(_HandleEmergencyMemory);
    }

    void CMemoryHandlerSystem::_InitDefaultCppHandler(const void* const hint)
    {
        s_typeInfo.Type = EMemHandlerType::Enum::DefaultCppHandler;
        s_typeInfo.Hint.VoidHint = const_cast<void*>(hint);
    }

    void CMemoryHandlerSystem::_InitEmergencyMemoryHandler(const void* const hint)
    {
        if (hint == nullptr)
        {
            throw std::invalid_argument("EmergencyMemoryHandler hint is nullptr");
        }

        s_typeInfo.Type = EMemHandlerType::Enum::EmergencyMemoryHandler;
        s_typeInfo.Hint.CEmergencyMemoryHint = 
            static_cast<CEmergencyMemory*>(const_cast<void*>(hint));
    }

    void __cdecl CMemoryHandlerSystem::_HandleEmergencyMemory()
    {
        auto* pHint = s_typeInfo.Hint.CEmergencyMemoryHint;

        if (pHint->IsSecureAvailable())
        {
            pHint->SecureHeapMemory();
            throw SecuredMemoryException();
        }
        else
        {
            //확보 불가능 상태. 회복불가
            throw std::bad_alloc();
        }
    }


    SMemoryHandlerInfo CMemoryHandlerSystem::s_typeInfo;

}