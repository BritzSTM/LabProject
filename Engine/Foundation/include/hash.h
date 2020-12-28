/**
    @file hash.h
    @brief hash 구현한 파일
    @remarks
    http://www.chrissavoie.com/articles/research/hash2 해쉬알고리즘 성능표
*/
#pragma once

#include "base_types.h"

#include <string_view>
#include <iosfwd>


//경고 비활성화 명령어의 플랫폼 종속문제
//#pragma warning(push)
//#pragma warning(disable : 4307)
namespace fd
{
    namespace hash
    {
        using Hash32 = uint32;
        using Hash64 = uint64;
        using Hash = Hash64;

        namespace _internal_hash
        {
            template<typename _HashTy>
            constexpr _HashTy Encode65599Hash(std::string_view s) noexcept
            {
                _HashTy hashValue = 0;
                for (size_t i = 0; i < s.size(); ++i)
                {
                    hashValue = 65599 * hashValue + s[i];
                }

                return hashValue ^ (hashValue >> 16);
            }
        }

        constexpr Hash32 EncodeHash32(std::string_view s) noexcept
        {
            return _internal_hash::Encode65599Hash<Hash32>(s);
        }

        constexpr Hash64 EncodeHash64(std::string_view s) noexcept
        {
            return _internal_hash::Encode65599Hash<Hash64>(s);
        }

        constexpr Hash EncodeHash(std::string_view s) noexcept
        {
            return _internal_hash::Encode65599Hash<Hash>(s);
        }
    }
}
//#pragma warning(pop)
