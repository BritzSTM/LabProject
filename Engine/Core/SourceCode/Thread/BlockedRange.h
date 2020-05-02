#pragma once


#include "../Common/Exception.h"

#include <tbb/blocked_range.h>


namespace Core
{

    //impl
    
    using tbb::blocked_range;


    DEFINE_EXCEPTION_TYPE(Core, LogicErrorException, GrainSizeException, NO_DEFINE_DETAILS);

}