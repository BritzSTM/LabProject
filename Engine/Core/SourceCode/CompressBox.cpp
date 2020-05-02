/**
@file CompressBox.h
@brief 메모리상의 데이터를 압축하는 CompressBox클래스를 구현합니다.
@date 2018-10-12  오후 20:32:06
*/


#include "stdafx.h"
#include "CompressBox.h"
#include "Container/string.h"

#include <algorithm>

#include <zlib/zlib.h>


namespace Core
{


	CCompressBox::~CCompressBox()
	{

	}

	CCompressBox::CCompressBox() 
		: m_uncompressedDataSize(0)
		, m_dataCarrierState(ECompressBoxState::Invalid)
		, m_compressLevel(kCompressLevel)
		, m_reserveSizePower(kReserveSizePower)
		, m_dataCarrier()
	{

	}

	CCompressBox::CCompressBox(const CCompressBox& src)
		: m_uncompressedDataSize(src.m_uncompressedDataSize)
		, m_dataCarrierState(src.m_dataCarrierState)
		, m_compressLevel(src.m_compressLevel)
		, m_reserveSizePower(src.m_reserveSizePower)
		, m_dataCarrier(src.m_dataCarrier)
	{

	}

	CCompressBox::CCompressBox(const CCompressBox&& src) noexcept
		: m_uncompressedDataSize(src.m_uncompressedDataSize)
		, m_dataCarrierState(src.m_dataCarrierState)
		, m_compressLevel(src.m_compressLevel)
		, m_reserveSizePower(src.m_reserveSizePower)
		, m_dataCarrier(std::move(src.m_dataCarrier))
	{

	}

	void CCompressBox::SetCompressLevel(const ECompressLevel level) noexcept
	{
		m_compressLevel = level;
	}
	
	void CCompressBox::SetReserveSizePower(const fp32 reservePower)
	{
		if (reservePower < 0.1f || reservePower > 10.0f)
		{
			throw std::LogicErrorException(TEXTL("reservePower value is out by boundary."));
		}
		else
		{
			m_reserveSizePower = reservePower;
		}
	}
	
	const size_t CCompressBox::GetBoxDataSize() const noexcept
	{
		return m_dataCarrier.size();
	}

	const ECompressBoxState CCompressBox::GetState() const noexcept
	{
		return m_dataCarrierState;
	}

	const ECompressLevel CCompressBox::GetCompressLevel() const noexcept
	{
		return m_compressLevel;
	}

	const fp32 CCompressBox::GetReserveSizePower() const noexcept
	{
		return m_reserveSizePower;
	}

	const size_t CCompressBox::GetUncompressedDataSize() const noexcept
	{
		return m_uncompressedDataSize;
	}

	size_t CCompressBox::_SetupCarrier(const size_t& dataSize)
	{
		const size_t requirableCapacity = _CalculateCarrierCapacity(dataSize);
		m_dataCarrier.resize(requirableCapacity);
		return requirableCapacity;
	}

	void CCompressBox::_CheckValidSrc(const byte* pSrc, const size_t& srcSize) const
	{
		if (pSrc == nullptr)
		{
			throw std::InvalidArgumentNullptrException("pSrc is nullptr");
		}

		if (srcSize < 1)
		{
			throw std::InvalidArgumentException("srcSize is less than 1");
		}
	}

	void CCompressBox::_CheckValidDest(const byte* pDest, const size_t& destSize) const
	{
		if (pDest == nullptr)
		{
			throw std::InvalidArgumentNullptrException(TEXTL("pDest argument is nullptr"));
		}

		if (destSize < m_dataCarrier.size())
		{
			throw std::InvalidArgumentException(TEXTL("destSize argument is less then CCompressBox data size"));
		}

		if (m_dataCarrierState == ECompressBoxState::Invalid)
		{
			throw CompressBoxInvalidStateException(TEXTL("CCompressBox state is invalid"));
		}
	}

	void CCompressBox::_CheckRecompressPossibility() const
	{
		if (GetState() != ECompressBoxState::Uncompressed)
		{
			throw std::LogicErrorException(TEXTL("CCompressBox state is not Uncompressed"));
		}
	}

	void CCompressBox::_CheckUncompressPossibility() const
	{
		if (GetState() != ECompressBoxState::Compressed)
		{
			throw std::LogicErrorException(TEXTL("CCompressBox state is not compressed"));
		}
	}

	void CCompressBox::_HandlingCompressResult(const int& resCode, const int& resSize) noexcept
	{
		if (resCode == Z_OK)
		{
			//Z_OK if success. resizing carrier size
			m_dataCarrierState = ECompressBoxState::Compressed;
			m_dataCarrier.resize(resSize);
		}
		else
		{
			// Z_MEM_ERROR if there was not enough memory, Z_BUF_ERROR
			// Z_STREAM_ERROR is compress level argument error.
			// but this class is limit compress level argument. so is not generate Z_STREAM_ERROR
			m_dataCarrierState = ECompressBoxState::Invalid;
		}
	}

	void CCompressBox::_HandlingUncompressResult(const int& resCode, const int& resSize) noexcept
	{
		if (resCode == Z_OK)
		{
			m_dataCarrierState = ECompressBoxState::Uncompressed;
			m_dataCarrier.resize(resSize);
		}
		else
		{
			/*
			uncompress returns Z_OK if success,
			Z_MEM_ERROR if there was not enough memory, Z_BUF_ERROR
			if there was not enough room in the output buffer, or Z_DATA_ERROR
			if the input data was corrupted or incomplete.
			In the case where there is not enough room,
			uncompress() will fill the output buffer with the uncompressed data up to that point.
			*/
			m_dataCarrierState = ECompressBoxState::Invalid;
		}
	}

	void CCompressBox::CompressData(const byte* pBytes, const size_t dataSize)
	{
		_CheckValidSrc(pBytes, dataSize);

		//원본 데이터 크기를 저장해 두고 Carrier의 준비 및 준비된 크기를 획득.
		//compress2 함수는 준비된 크기 compressedSize를 전달받아  
		//최종 변경된 크기를 기록함
		m_uncompressedDataSize = dataSize;
		uLong compressedSize = static_cast<uLong>(_SetupCarrier(dataSize));
		
		const int resCode = compress2(&m_dataCarrier[0], &compressedSize, 
			pBytes, static_cast<uLong>(dataSize), m_compressLevel);

		_HandlingCompressResult(resCode, compressedSize);
	}

	void CCompressBox::RecompressData()
	{
		_CheckRecompressPossibility();

		//보관 중인 데이터를 획득하고 이를 이용해 내부 Carrier에 압축결과를 저장 한다.
		DataCarrierType srcData(std::move(m_dataCarrier));
		CompressData(&srcData[0], srcData.size());
	}

	void CCompressBox::UncompressData()
	{
		_CheckUncompressPossibility();
		
		//보관 중인 데이터를 획득하고 내부 Carrier에 압축을 해제한다.
		DataCarrierType compressedSrcData(std::move(m_dataCarrier));
		uLong uncompressedSize = static_cast<uLong>(_SetupCarrier(m_uncompressedDataSize));

		int resCode = uncompress(&m_dataCarrier[0], &uncompressedSize,
			&compressedSrcData[0], static_cast<uLong>(compressedSrcData.size()));

		_HandlingUncompressResult(resCode, uncompressedSize);
	}

	void CCompressBox::CopyDataToDest(byte* pDest, const size_t destSize) const
	{
		_CheckValidDest(pDest, destSize);
		std::copy(std::begin(m_dataCarrier), std::end(m_dataCarrier), pDest);
	}

	void CCompressBox::Clear() noexcept
	{
		m_dataCarrier.clear();
		m_dataCarrier.resize(0);
	}
}
