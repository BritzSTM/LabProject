#include "stdafx.h"
#include "PlatformMsgPump.h"
#include "PlatformBroadcastSystem.h"
#include "Window/IWindow.h"
#include "Thread/Task.h"


namespace Platform
{

    namespace _internal
    {

        void _WindowsPumpMessage() noexcept
        {

            MSG msg;
            while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }

        }


        void _ProceedWindowsKeyboardMsg(_WindowsMsgPack& msgPack, bool& isOutput)
        {

            SKeyboardEvents event;

            switch (msgPack.Message)
            {
            default:
                return;
                break;
            }

            isOutput = true;
            SBroadcastingStation::pKeyboard->Notify(event);
            SBroadcastingStation::pKeyboard->NotifiyImmediately(event);

        }

        void _ProceedWindowsMouseMsg(_WindowsMsgPack& msgPack, bool& isOutput)
        {

            using Enum = EMouseEventType::Enum;

            //{GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)}
            SMouseEvents event{
                Enum::None,
                0,
                static_cast<uint32>(msgPack.Lparam) };

            /*
            //case WM_MOUSEHOVER:   위에 올라와 있을때
            //case WM_MOUSELEAVE:   떠났을때
            //case WM_CAPTURECHANGED: 포커싱 변경
            //case WM_MOUSEACTIVATE: 활성화 시켰을때
            case WM_MOUSEHWHEEL는 활성화 되있는것 H가 없는것은 포커싱 상태
            */
            switch (msgPack.Message)
            {

            case WM_LBUTTONDOWN:
                event.Type = Enum::LeftButtonDown;
                break;

            case WM_LBUTTONUP:
                event.Type = Enum::LeftButtonUp;
                break;

            case WM_LBUTTONDBLCLK:
                event.Type = Enum::LeftButtonDoubleClick;
                break;

            case WM_RBUTTONDOWN:
                event.Type = Enum::RightButtonDown;
                break;

            case WM_RBUTTONUP:
                event.Type = Enum::RightButtonUp;
                break;

            case WM_RBUTTONDBLCLK:
                event.Type = Enum::RightButtonDoubleClick;
                break;

            case WM_MBUTTONDOWN:
                event.Type = Enum::WhellButtonDown;
                break;

            case WM_MBUTTONUP:
                event.Type = Enum::WhellButtonUp;
                break;

            case WM_MBUTTONDBLCLK:
                event.Type = Enum::WhellButtonDoubleClick;
                break;

            case WM_MOUSEWHEEL:
            {
                event.Type = Enum::WhellRotate;
                event.WhellDelta = GET_WHEEL_DELTA_WPARAM(msgPack.Wparam);
            }
            break;

            case WM_MOUSEMOVE:
                event.Type = Enum::Movement;
                break;

            default:
                isOutput = false;
                return;
            }

            isOutput = true;
            SBroadcastingStation::pMouse->Notify(event);
            SBroadcastingStation::pMouse->NotifiyImmediately(event);

        }

        //내부에서만 사용되는 클래스
        class _IWindowWindowProxy final
            : public IWindow
        {
            DISALLOW_CREATE_INSTANCE_PERMANENTLY(_IWindowWindowProxy);

        public:
            virtual WindowContext GetWindowContext() const noexcept {}
            virtual SWindowTypeInfo GetWindowTypeInfo() const noexcept {}

            virtual void Show(const EWindowShowState& state) {}
            virtual void Hide() {}

            static IWindow* GetIWindowFromWindowContext(const WindowContext context)
            {
                return _GetWindowStorage().FindWindowFromContext(context);
            }
        };

        void _ProceedWindowsWindowMsg(_WindowsMsgPack& msgPack, bool& isOutput)
        {
            SWindowEvents event;
            switch (msgPack.Message)
            {
            case WM_ACTIVATEAPP:
            case WM_CANCELMODE:
            case WM_CHILDACTIVATE:
            case WM_CLOSE:
            case WM_COMPACTING:
            case WM_CREATE:
            case WM_DESTROY:
            case WM_DPICHANGED:
            case WM_ENABLE:
            case WM_ENTERSIZEMOVE:
            case WM_EXITSIZEMOVE:
            case WM_GETICON:
            case WM_GETMINMAXINFO:
            case WM_INPUTLANGCHANGE:
            case WM_INPUTLANGCHANGEREQUEST:
            case WM_MOVE:
            case WM_MOVING:
            case WM_NCACTIVATE:
            case WM_NCCALCSIZE:
            case WM_NCCREATE:
            case WM_NCDESTROY:
            case WM_NULL:
            case WM_QUERYDRAGICON:
            case WM_QUERYOPEN:
            case WM_QUIT:
            case WM_SHOWWINDOW:
            case WM_SIZE:
            case WM_SIZING:
            case WM_STYLECHANGED:
            case WM_STYLECHANGING:
            case WM_THEMECHANGED:
            case WM_USERCHANGED:
            case WM_WINDOWPOSCHANGED:
            case WM_WINDOWPOSCHANGING:
                //Paint
            case WM_DISPLAYCHANGE:
            case WM_ERASEBKGND:
            case WM_NCPAINT:
            case WM_PAINT:
            case WM_PRINT:
            case WM_PRINTCLIENT:
            case WM_SETREDRAW:
            case WM_SYNCPAINT:
                return;
                break;
            }

            isOutput = true;
            //SBroadcastingStation::pWindow->Notify(event, foundWindow);
            //SBroadcastingStation::pWindow->NotifiyImmediately(event, foundWindow);

        }
        
        Core::CTask* _SpwanMsgTask(_WindowsMsgPack* const pPack, bool* const pResPack)
        {

            using namespace Core;

            CTask* pRootTask;
            pRootTask = new(CTask::allocate_root()) CEmptyTask;
            pRootTask->set_ref_count(4);


            auto& keyboardTask = *new(pRootTask->allocate_child())
                CLambdaSyncTask([&pPack, &pResPack]()
            {
                _ProceedWindowsKeyboardMsg(
                    pPack[EPlatformMsgType::Keyboard], pResPack[EPlatformMsgType::Keyboard]);
            });


            auto& mouseTask = *new(pRootTask->allocate_child())
                CLambdaSyncTask([&pPack, &pResPack]()
            {
                _ProceedWindowsMouseMsg(
                    pPack[EPlatformMsgType::Mouse], pResPack[EPlatformMsgType::Mouse]);
            });


            auto& windowTask = *new(pRootTask->allocate_child())
                CLambdaSyncTask([&pPack, &pResPack]()
            {
                _ProceedWindowsWindowMsg(
                    pPack[EPlatformMsgType::Window], pResPack[EPlatformMsgType::Window]);
            });
            

            pRootTask->spawn(keyboardTask);
            pRootTask->spawn(mouseTask);
            pRootTask->spawn(windowTask);
            return pRootTask;

        }

        LRESULT WINDOWS_API _ProceedWindowMsgSource(
            HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
        {

            using namespace Core;
            //파괴. 나가기 명령만 여기서 처리한다. 나머지는 Task로 분할한다.

            Platform::IWindow* foundWindow = _IWindowWindowProxy::GetIWindowFromWindowContext(WindowContext(hWnd));
            if (foundWindow == nullptr)
            {
                DefWindowProc(hWnd, message, wParam, lParam);
            }


            bool resOutput[4] = { false, false, false, false }; //한번에 int32로 형변환하여 확인하기 위해 4바이트 준수
            _WindowsMsgPack resMsgPack[3] = 
            {
                {hWnd, message, wParam, lParam},
                {hWnd, message, wParam, lParam},
                {hWnd, message, wParam, lParam}
            };            

            CTask* pRootTask = _SpwanMsgTask(resMsgPack, resOutput);

            //Task Join and destroy
            pRootTask->wait_for_all();
            pRootTask->destroy(*pRootTask);


            //전체 값이 0이면 처리되지 않음
            if (*(reinterpret_cast<int32*>(resOutput)) == 0)
            {
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
            else
            {
                return 0;
            }

        }

    }
       
}