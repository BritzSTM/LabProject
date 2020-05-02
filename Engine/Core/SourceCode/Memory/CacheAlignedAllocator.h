#pragma once


#include <tbb/cache_aligned_allocator.h>


namespace Core
{
    template<typename _TYPE_>
    using CacheAlignedAllocator = tbb::cache_aligned_allocator<_TYPE_>;
}