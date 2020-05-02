#pragma once


#include "../CoreHelper.h"
#include "../Common/BaseTypes.h"
#include "../Common/EnumExtension.h"
#include "../Container/string.h"
#include "../Math/Point.h"
#include "../WrappedDependentBaseType.h"


namespace Platform
{

    struct SWindowSize
    {
        int16 Width;
        int16 Height;
    };

    struct SWindowPos :
        public Math::SPoint2D<int16>
    {

    };

    DEFINE_EXT_ENUM
    (
        enum class, EWindowShowState, ENUM_SIZE_DEFAULT,
        E_NAME_V(None, -1)
        E_NAME(Show)
        E_NAME(Hide)
    );
    
    struct SWindowTypeInfo
    {
        SWindowSize Size;
        SWindowPos Pos;
        EWindowShowState State;
        string TitleName;
    };

}