/**
@file Point.h
@brief 차원별 하나의 위치를 나타내기 위한 구조체 모임
@date 2018-10-22  오후 21:27:34
*/
#pragma once


namespace Math
{

    template<typename _VALUE_TYPE>
    struct SPoint1D
    {
        _VALUE_TYPE p;
    };

    template<typename _VALUE_TYPE>
    struct SPoint2D
    {
        _VALUE_TYPE x;
        _VALUE_TYPE y;
    };

    template<typename _VALUE_TYPE>
    struct SPoint3D
    {
        _VALUE_TYPE x;
        _VALUE_TYPE y;
        _VALUE_TYPE z;
    };

    template<typename _VALUE_TYPE>
    struct SPoint4D
    {
        _VALUE_TYPE x;
        _VALUE_TYPE y;
        _VALUE_TYPE z;
        _VALUE_TYPE w;
    };

}
