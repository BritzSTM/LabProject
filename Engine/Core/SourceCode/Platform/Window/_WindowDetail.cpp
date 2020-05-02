#include "stdafx.h"
#include "_WindowDetail.h"
#include "PlatformSystemMode.h"
#include "PlatformWorker.h"
#include "PlatformWorkerQueue.h"
#include "PlatformMsgPump.h"


namespace Platform
{

    namespace _internal
    {

        HINSTANCE _GetWindowsProgramInstance()
        {
            static HINSTANCE m_hinst = GetModuleHandle(nullptr);
            return m_hinst;
        }

        WNDCLASSEX _WindowsGetNativeWindowType() noexcept
        {
            /*
            WNDCLASSEX TYPE INFO
            UINT        cbSize;
            UINT        style;
            WNDPROC     lpfnWndProc;
            int         cbClsExtra;
            int         cbWndExtra;
            HINSTANCE   hInstance;
            HICON       hIcon;
            HCURSOR     hCursor;
            HBRUSH      hbrBackground;
            LPCWSTR     lpszMenuName;
            LPCWSTR     lpszClassName;
            HICON       hIconSm;
            */
            static WNDCLASSEX wcex{
                sizeof(WNDCLASSEX),
                (CS_HREDRAW | CS_VREDRAW | CS_OWNDC),
                _ProceedWindowMsgSource,
                0,
                0,
                _GetWindowsProgramInstance(),
                nullptr,
                nullptr,
                (HBRUSH)(COLOR_WINDOW + 1),
                nullptr,
                _WindowsWindowClassUniqueID,
                nullptr
            };
            
            return wcex;
        }

        
        RECT _WindowsGetNativeWIndowSize(const SWindowTypeInfo& info) noexcept
        {
            RECT rc {0, 0, static_cast<LONG>(info.Size.Width), static_cast<LONG>(info.Size.Height)};
            AdjustWindowRectEx(&rc, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_OVERLAPPEDWINDOW);
            return rc;
        }

        
        void _WindowsInitUniqueWindowType()
        {
            static std::once_flag regWindowFlag;
            std::call_once(regWindowFlag, []()
            {
                auto nativeType = _WindowsGetNativeWindowType();
                auto resAtom = RegisterClassEx(&nativeType);
                if (resAtom == 0)
                {
                    THROWER(Core::LogicErrorException, EF_TMSG("유니크 윈도우 타입 등록 실패"));
                }
            });
        }
        
        WindowContext _WindowsCreateWindowContext(const SWindowTypeInfo& info)
        {
            Core::CAsyncWait waiter;
            WindowContext context(0);

            CPlatformWorker worker(&waiter, [&info, &context]()
            {
                _WindowsInitUniqueWindowType();

                auto rc = _WindowsGetNativeWIndowSize(info);
                context =
                    CreateWindowEx(
                        WS_EX_OVERLAPPEDWINDOW,
                        _WindowsWindowClassUniqueID,
                        UTF8::ToUNICODE(info.TitleName).c_str(),
                        WS_OVERLAPPEDWINDOW,
                        info.Pos.x, info.Pos.y,
                        (rc.right - rc.left), (rc.bottom - rc.top),
                        nullptr, nullptr,
                        _GetWindowsProgramInstance(),
                        nullptr);
            });


            PLATFORM_EXECUTE_FUNC(waiter, worker);
            return context;
        }


        void _WindowsWindow::_CreateAndRegisterWindowContext()
        {
            m_context = _WindowsCreateWindowContext(m_windowInfo); //이미 윈도우 메시지가 호출중임;
            _GetWindowStorage().RegisterWindow(this);
        }

        void _WindowsWindow::_ReleaseWindowContext()
        {
            if (m_context != 0)
            {
                _GetWindowStorage().UnregisterWindow(this);
                
                //모든 리스너들에게 통지해야함

            }
        }

        _WindowsWindow::~_WindowsWindow()
        {
            _ReleaseWindowContext();
        }

        _WindowsWindow::_WindowsWindow(const SWindowTypeInfo& info)
            : m_windowInfo(info)
            , m_context(0)
        {
            _CreateAndRegisterWindowContext();
        }

        void _WindowsWindow::Show(const EWindowShowState& state)
        {
            m_windowInfo.State = state;

            ShowWindow(m_context, SW_SHOW);
        }

        void _WindowsWindow::Hide()
        {
            ShowWindow(m_context, SW_HIDE);
        }

    }

}