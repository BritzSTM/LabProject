/**
@file CompressBox.h
@brief 메모리상의 데이터를 압축하는 CompressBox클래스를 구현합니다.
@date 2018-10-12  오후 20:32:06
*/


#pragma once


#include "CoreHelper.h"
#include "Common/BaseTypes.h"
#include "Common/Exception.h"
#include <vector>


namespace Core
{

	/**CompressBox의 상태를 나타내는 열거형*/
	enum class ECompressBoxState : uint16
	{
		Invalid,          /**아무것도 유효한 것이 없다.*/
		Compressed,       /**존재하는 데이터가 압축되어 있는 상태*/
		Uncompressed      /**존재하는 데이터가 비압축되어 있는 상태*/
	};


	enum ECompressLevel : uint16
	{
		LEVEL0,
		LEVEL1,
		LEVEL2,
		LEVEL3,
		LEVEL4,
		LEVEL5,
		LEVEL6,
		LEVEL7,
		LEVEL8,
		LEVEL9
	};


	class CompressBoxInvalidStateException
		: public std::LogicErrorException
	{
	public:
		using logic_error::logic_error;
		using logic_error::what;
	};


	struct ICompressBox
	{
		/** 압축 레벨 상수 */
		static constexpr ECompressLevel kCompressLevel = ECompressLevel::LEVEL7;

		/** 예비영역 크기를 나타내는 상수 */
		static constexpr fp32 kReserveSizePower = 0.3f; 


		virtual ~ICompressBox() {}

		virtual void SetCompressLevel(const ECompressLevel level = kCompressLevel) noexcept = 0;
		
		/**0.1f ~ 10.0f 사이의 값이 아니면 예외가 발생합니다.*/
		virtual void SetReserveSizePower(const fp32 reservePower = kReserveSizePower) = 0;

		virtual const size_t GetBoxDataSize() const noexcept = 0;
		virtual const ECompressBoxState GetState() const noexcept = 0;
		virtual const ECompressLevel GetCompressLevel() const noexcept = 0;
		virtual const fp32 GetReserveSizePower() const noexcept = 0;
		virtual const size_t GetUncompressedDataSize() const noexcept = 0;
		
		virtual void CompressData(const byte* pSrc, const size_t srcSize) = 0;
		virtual void RecompressData() = 0;
		virtual void UncompressData() = 0;
		/**if box is invalid state, throw CompressBoxInvalidStateException.*/
		virtual void CopyDataToDest(byte* pDest, const size_t destSize) const = 0;
		virtual void Clear() noexcept = 0;
	};



	/*
	bytes를 간단하게 압축 및 해제 할 수 있는 상자 클래스
	압축된 데이터를 보관하고 방출할 수 있다.
	*/
	class CCompressBox final 
		: public ICompressBox
	{
	public:
		using DataCarrierType = std::vector<byte>;

	public:
		virtual ~CCompressBox();

		CCompressBox();
		CCompressBox(const CCompressBox& src);
		CCompressBox(const CCompressBox&& src) noexcept;
			
		virtual void SetCompressLevel(const ECompressLevel level) noexcept override;
		virtual void SetReserveSizePower(const fp32 reservePower) override;

		virtual const size_t GetBoxDataSize() const noexcept override;
		virtual const ECompressBoxState GetState() const noexcept override;
		virtual const ECompressLevel GetCompressLevel() const noexcept override;
		virtual const fp32 GetReserveSizePower() const noexcept override;
		virtual const size_t GetUncompressedDataSize() const noexcept override;

		void CompressData(const byte* pBytes, const size_t dataSize) override;
		void RecompressData() override;
		void UncompressData() override;

		virtual void CopyDataToDest(byte* pDest, const size_t destSize) const override;
		virtual void Clear() noexcept override;

	private:
		FLINE size_t _CalculateCarrierCapacity(const size_t& dataSize) const noexcept
		{
			//압축을 위한 배열공간. 작업을 위한 공간까지 필요함
			//dest size >= sourceLen + ( sourceLen + 12 ) * 0.1f 공식 메뉴얼 안정 크기
			return dataSize + static_cast<size_t>((dataSize + 12) * m_reserveSizePower);
		}

		/**전달된 dataSize의 크기만큼 Carrier를 설정함 그리고 최종 크기를 반환함*/
		size_t _SetupCarrier(const size_t& dataSize);
		/**전달된 Src의 유효성 확인. 비유효시 예외 발생*/
		void _CheckValidSrc(const byte* pSrc, const size_t& srcSize) const;
		/**전달된 목적지로 carrier의 Data 복제가 가능한지 확인. 아니라면 예외 발생*/
		void _CheckValidDest(const byte* pDest, const size_t& destSize) const;
		void _CheckRecompressPossibility() const;
		void _CheckUncompressPossibility() const;

		void _HandlingCompressResult(const int& resCode, const int& resSize) noexcept;
		void _HandlingUncompressResult(const int& resCode, const int& resSize) noexcept;

	private:
		size_t m_uncompressedDataSize;
		ECompressBoxState m_dataCarrierState;
		ECompressLevel m_compressLevel;
		fp32 m_reserveSizePower;
		DataCarrierType m_dataCarrier;
	};

	
}