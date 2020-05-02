/**
@file BaseType_Integers.h
@brief Platform 이성을 높이기 위해 기본 원자타입에 대해 정의 합니다.
@date 2018-10-11  오후 18:45:02
*/


#pragma once


namespace Core
{
	namespace BaseTypes
	{


		/*

		   정수 원자 타입에 대한 정적 크기 확인

		*/

		static_assert(sizeof(char) == 1, "char type size isn't 1byte!");
		static_assert(sizeof(short) == 2, "short type size isn't 2byte!");
		static_assert(sizeof(int) == 4, "int type size isn't 4byte!");
		static_assert(sizeof(long) == 4, "long type size isn't 4byte!");
		static_assert(sizeof(long long) == 8, "long long type size isn't 8byte!");



		/*

		   정수형 타입 정의

		*/

		using uint8 = unsigned __int8;
		using int8 = __int8;

		using uint16 = unsigned __int16;
		using int16 = __int16;

		using uint32 = unsigned __int32;
		using int32 = __int32;

		using uint64 = unsigned __int64;
		using int64  = __int64;

		using ulong     = unsigned long;
		using ulonglong = unsigned long long;

		//추가된 정수 원자타입 크기가 유도한 대로인지 확인
		static_assert(sizeof(uint8)  == 1, "uint8 type size isn't 1byte!");
		static_assert(sizeof(int8)   == 1, "int8 type size isn't 1byte!");
		static_assert(sizeof(uint16) == 2, "uint16 type size isn't 2byte!");
		static_assert(sizeof(int16)  == 2, "int16 type size isn't 2byte!");
		static_assert(sizeof(uint32) == 4, "uint32 type size isn't 4byte!");
		static_assert(sizeof(int32)  == 4, "int32 type size isn't 4byte!");
		static_assert(sizeof(uint64) == 8, "uint64 type size isn't 8byte!");
		static_assert(sizeof(int64)  == 8, "int64 type size isn't 8byte!");

		static_assert(sizeof(ulong)     == 4, "ulong type size isn't 4byte!");
		static_assert(sizeof(ulonglong) == 8, "ulonglong type size isn't 8byte!");


        template<typename _TYPE_>
        struct PtrTypeTraitsNullptrBase
        {
            static constexpr _TYPE_ NullptrAddrValue = 0x0;
        };

        template<typename _TYPE_>
        struct PtrTypeTraits;

		template<>
		struct PtrTypeTraits<uint64> 
            : public PtrTypeTraitsNullptrBase<uint64>
		{
			static constexpr uint64 ErrorAddress = 0xFFFF'FFFF'FFFF'FFFF;
		};

		template<>
		struct PtrTypeTraits<int64>
            : public PtrTypeTraitsNullptrBase<int64>
		{
			static constexpr int64 ErrorAddress = 0x7FFF'FFFF'FFFF'FFFF;
		};

		template<>
		struct PtrTypeTraits<uint32>
            : public PtrTypeTraitsNullptrBase<uint32>
		{
			static constexpr uint32 ErrorAddress = 0xFFFF'FFFF;
		};

		template<>
		struct PtrTypeTraits<int32>
            : public PtrTypeTraitsNullptrBase<int32>
		{
			static constexpr int32 ErrorAddress = 0x7FFF'FFFF;
		};

		//PtrValue Type 정의. 운영체제 bit수에 따라 크기가 변화하므로 이름에 _t는 붙지 않는다. 
#if defined(WIN64) || defined(_WIN64)
		using uintptr = uint64;
		using intptr = int64;

#else
		//차후 운영체제 비트에 따른 처리
		COMPILE_ERROR("Ptr type is not defined");

#endif
	}
}
