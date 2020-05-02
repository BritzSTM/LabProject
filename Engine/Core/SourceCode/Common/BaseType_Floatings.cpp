/*


*/

#include "stdafx.h"

#include "BaseType_Floatings.h"
#include "Common\Exception.h"




namespace Core
{
	namespace BaseTypes
	{

		FP16Control::FP16RawToFP32Converter FP16Control::s_pFP16RawToFP32;
		FP16Control::FP32ToFP16RawConverter FP16Control::s_pFP32ToFP16Raw;
		FP16Control::FP16RawIsNaNStater     FP16Control::s_pFP16RawIsNaN;
		FP16Control::FP16RawAddOperator     FP16Control::s_pFP16RawAdd;
		FP16Control::FP16RawSubOperator     FP16Control::s_pFP16RawSub;
		FP16Control::FP16RawMulOperator     FP16Control::s_pFP16RawMul;
		FP16Control::FP16RawDivOperator     FP16Control::s_pFP16RawDiv;

		void API FP16Control::InitializeFP16Converter() noexcept
		{
			s_pFP16RawToFP32 = ConvertFP16RawToFP32_Softv2;
			s_pFP32ToFP16Raw = ConvertFP32ToFP16Raw_Softv2;
		}

		void API FP16Control::SetFP16Converter(
			const FP16RawToFP32Converter pFP16RawToFP32,
			const FP32ToFP16RawConverter pFP32ToFP16Raw)
		{
			//empty 함수 객체를 받으면 예외 발생
			if ((pFP16RawToFP32 == nullptr) || (pFP32ToFP16Raw == nullptr))
			{
				THROWER(InvalidArgumentNullptrException, EF_MSG("Converter arguments is nullptr"));
			}

			s_pFP16RawToFP32 = pFP16RawToFP32;
			s_pFP32ToFP16Raw = pFP32ToFP16Raw;
		}

		void API FP16Control::GetFP16Converter(
			FP16RawToFP32Converter* ppFP16RawToFP32,
			FP32ToFP16RawConverter* ppFP32ToFP16Raw)
		{
			//nullptr을 받으면 예외 발생
			if ((ppFP16RawToFP32 == nullptr) || (ppFP32ToFP16Raw == nullptr))
			{
				throw new InvalidArgumentNullptrException("Converter out arguments is nullptr");
			}

			*ppFP16RawToFP32 = s_pFP16RawToFP32;
			*ppFP32ToFP16Raw = s_pFP32ToFP16Raw;
		}





		void API FP16Control::InitializeFP16Stater() noexcept
		{
			s_pFP16RawIsNaN = IsNaN_FP16RawType_Soft;
		}

		void API FP16Control::SetFP16Stater(const FP16RawIsNaNStater pIsNan)
		{
			if (pIsNan == nullptr)
			{
				throw InvalidArgumentNullptrException("Stater arguments is nullptr");
			}

			s_pFP16RawIsNaN = pIsNan;
		}

		void API FP16Control::GetFP16Stater(FP16RawIsNaNStater *ppIsNan)
		{
			if (ppIsNan == nullptr)
			{
				throw InvalidArgumentNullptrException("Stater out arguments is nullptr");
			}

			*ppIsNan = s_pFP16RawIsNaN;
		}


	}
}

