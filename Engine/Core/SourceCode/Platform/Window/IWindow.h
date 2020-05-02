#pragma once


#include "../CoreHelper.h"
#include "../Common/Exception.h"
#include "WindowBaseTypes.h"
#include "../Thread/CriticalSection.h"
#include "../Container/ConcurrencyUnorderedMap.h"


namespace Platform
{

    class CWindowStorage;
    
    //Window의 Storage 등록은 IWindow 구현자에게 위임된다.
    struct IWindow
    {
        DISALLOW_CREATE_INSTANCE(IWindow);
   
    public:
        virtual ~IWindow() {}
        
        virtual WindowContext GetWindowContext() const noexcept = 0;
        virtual SWindowTypeInfo GetWindowTypeInfo() const noexcept = 0;

        virtual void Show(const EWindowShowState& state) = 0;
        virtual void Hide() = 0;
        
    protected:
        static ILINE CWindowStorage& _GetWindowStorage() noexcept
        {
            return s_windowStorage;
        }

    private:
        static CWindowStorage s_windowStorage;

    };


    DEFINE_EXCEPTION_TYPE(Core, LogicErrorException, AlreadyRegisterException, NO_DEFINE_DETAILS);
    DEFINE_EXCEPTION_TYPE(Core, LogicErrorException, InvalidWindowException, NO_DEFINE_DETAILS);

    class CWindowStorage final
    {
        DISALLOW_COPY_COTR_AND_ASSIGN(CWindowStorage);

    public:
        using ContainerType =
            Core::Container::concurrent_unordered_map<WindowContext, IWindow*>;

    public:
        ~CWindowStorage() {}
        CWindowStorage() {}

        void RegisterWindow(const IWindow* const pWindow);
        void UnregisterWindow(const IWindow* const pWindow);

        [[nodiscard]]IWindow* FindWindowFromContext(const WindowContext context) const;

        [[nodiscard]]ILINE auto ToCopy() const
        {
            ContainerType copied(m_windowMap);
            return std::move(copied);
        }

        void _CheckValidWindow(const IWindow* const pWindow) const;

    private:
        ContainerType m_windowMap;
        Core::CriticalSection m_syncObj;

    };

}