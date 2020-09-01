#include <exception>

namespace fd
{
    /*

    */
    class CUncaughtExceptionMarker
    {
    public:
        //using size_type = decltype(std::uncaught_exceptions)
        using size_type = int;

    public:
        ~CUncaughtExceptionMarker() = default;

        CUncaughtExceptionMarker();
        CUncaughtExceptionMarker(const CUncaughtExceptionMarker&);
        CUncaughtExceptionMarker(CUncaughtExceptionMarker && noexcept);

        // 마커 생성시점 이후 새로운 예외 발생 여부
        bool isNewException() const noexcept;

        // 마커 생성시점에서 예외 수
        size_type getExceptionCount() const noexcept;

        CUncaughtExceptionMarker& operator=(CUncaughtExceptionMarker&);
        CUncaughtExceptionMarker& operator=(CUncaughtExceptionMarker&&) noexcept;

    private:
        const size_type m_c;
    };
}