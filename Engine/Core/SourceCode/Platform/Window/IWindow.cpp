#include "stdafx.h"
#include "IWindow.h"
#include "Thread/Mutex.h"


namespace Platform
{

    CWindowStorage IWindow::s_windowStorage;

    void CWindowStorage::_CheckValidWindow(const IWindow* const pWindow) const
    {

        if (pWindow == nullptr)
        {
            THROWER(InvalidWindowException, EF_TMSG("pWindow가 nullptr 입니다."));
        }

        if (pWindow->GetWindowContext() == 0)
        {
            THROWER(InvalidWindowException, EF_TMSG("Window의 Context가 비유효 합니다."));
        }

    }

    void CWindowStorage::RegisterWindow(const IWindow* const pWindow)
    {

        _CheckValidWindow(pWindow);

        const auto resIter = m_windowMap.find(pWindow->GetWindowContext());
        if (resIter == std::cend(m_windowMap))
        {
            m_windowMap.insert(
                std::make_pair(
                    pWindow->GetWindowContext(), const_cast<IWindow*>(pWindow)));
        }
        else
        {
            THROWER(AlreadyRegisterException, EF_TMSG("이미 등록되어 있는 윈도우 입니다."));
        }

    }

    void CWindowStorage::UnregisterWindow(const IWindow* const pWindow)
    {

        _CheckValidWindow(pWindow);
        std::lock_guard lock(m_syncObj);
        m_windowMap.unsafe_erase(pWindow->GetWindowContext());

    }

    IWindow* CWindowStorage::FindWindowFromContext(const WindowContext context) const
    {

        const auto resIter = m_windowMap.find(context);

        if (resIter == std::cend(m_windowMap))
        {
            return nullptr;
        }
        else
        {
            return const_cast<IWindow*>(resIter->second);
        }

    }

}