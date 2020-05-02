
#pragma once


#include "../CoreHelper.h"
#include "../Common/Exception.h"
#include "Common/OSType.h"
#include "IWindow.h"


namespace Platform
{

    namespace _internal
    {

#if (TARGET_OS == OS_WIN64)
        constexpr const wchar_t* _WindowsWindowClassUniqueID = L"PlatformSystemLayers";

        HINSTANCE _GetWindowsProgramInstance();

        //실제 작업영역의 크기를 반환해야함.
        RECT _WindowsGetNativeWIndowSize(const SWindowTypeInfo& info) noexcept;

        //단 한번만 유효한 실행
        void _WindowsInitUniqueWindowType();

        WindowContext _WindowsCreateWindowContext(const SWindowTypeInfo& info);


        class _WindowsWindow final
            : public IWindow
        {
            DISALLOW_COPY_COTR_AND_ASSIGN(_WindowsWindow);

        public:
            virtual ~_WindowsWindow();
            _WindowsWindow(const SWindowTypeInfo& info);

            virtual WindowContext GetWindowContext() const noexcept
            {
                return m_context;
            }

            virtual SWindowTypeInfo GetWindowTypeInfo() const noexcept
            {
                return m_windowInfo;
            }

            virtual void Show(const EWindowShowState& state);
            virtual void Hide();

        private:
            void _CreateAndRegisterWindowContext();
            void _ReleaseWindowContext();

        private:
            SWindowTypeInfo m_windowInfo;
            WindowContext m_context;

        };

#else
        COMPILE_ERROR(" WindowsType Not impl");

#endif

    }

}