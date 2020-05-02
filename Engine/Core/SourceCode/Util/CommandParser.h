#pragma once

//user
#include "Container/string.h"
#include "Common/EnumExtension.h"
#include "Common/Exception.h"
#include "Locker.h"

//std
#include <initializer_list>
#include <regex>
#include <unordered_map>
#include <set>

//boost
#include <boost/lexical_cast.hpp>
#include <boost/stacktrace.hpp>

namespace Core
{

	/**Schema 구조에 대해 정의하는 문자열 타입 입니다. */
	using CommandParserSchemaSignature = string;
	

	DEFINE_EXCEPTION_TYPE
	(
		Core, LogicErrorException, CommandDocumentNotFoundCommandException, NO_DEFINE_DETAILS
	);

	DEFINE_EXCEPTION_TYPE
	(
		Core, LogicErrorException, CommandDocumentNotFoundCommandDescException, NO_DEFINE_DETAILS
	);

	DEFINE_EXCEPTION_TYPE
	(
		Core, LogicErrorException, CommandDocumentDuplicatedCommandException, NO_DEFINE_DETAILS
	);

	DEFINE_EXCEPTION_TYPE
	(
		Core, LogicErrorException, CommandDocumentLockedException, NO_DEFINE_DETAILS
	);


	/*
	질의한 Command의 보유 유무 및 내용물 반환
	제공되는 반복자는 key_value 값들을 순회함
	*/
	class CCommandDocument final
	{
	public:
		using key_type = SHashFromString;
		using value_type = string;
		using size_type = std::size_t;
		using set_type = std::set<key_type>;
		using map_type = std::unordered_map<key_type, value_type>;
		
		using iterator = set_type::iterator;
		using const_iterator = set_type::const_iterator;
		using reverse_iterator = set_type::reverse_iterator;
		using const_reverse_iterator = set_type::const_reverse_iterator;
	private:
		DEFINE_EXT_ENUM
		(
			enum class, EExceptionCode, ENUM_SIZE_DEFAULT,
			E_NAME(INVALID_ARGUMENT)
			E_NAME(INVALID_NULLPTR_ARGUMENT)
			E_NAME(NOT_FOUND_COMMAND)
			E_NAME(NOT_FOUND_COMMAND_DESC)
			E_NAME(DUPLICATED_COMMAND)
			E_NAME(LOCKED)
		);

	public:
		~CCommandDocument();

		CCommandDocument();
		CCommandDocument(const CCommandDocument&) = default;

		ILINE void Lock() noexcept;
		ILINE void Unlock() noexcept;
		ILINE bool IsLocked() const noexcept;

		ILINE const size_type GetCommandCount() const noexcept;
		bool FindCommand(const char* const pszCommand) const;
		const value_type GetCommandDesc(const char* const pszCommand) const;

		void WriteCommand(const std::initializer_list<const char* const>& argCommands);
		void WriteCommand(const char* const pszArgCommand);
		void WriteCommandDesc(const char* const pszTargetArgCommand, const char* const pszDesc);
		void WriteCommandAndDesc(const char* const pszArgCommand, const char* const pszDesc);

		void EraseCommand(const char* const pszCommand);
		void EraseCommandDesc(const char* const pszCommand);
		
		//Key_value iterators
		ILINE iterator begin() noexcept;
		ILINE const_iterator begin() const noexcept;
		ILINE const_iterator cbegin() const noexcept;

		ILINE iterator end() noexcept;
		ILINE const_iterator end() const noexcept;
		ILINE const_iterator cend() const noexcept;

		ILINE reverse_iterator rbegin() noexcept;
		ILINE const_reverse_iterator rbegin() const noexcept;
		ILINE const_reverse_iterator crbegin() const noexcept;

		ILINE reverse_iterator rend() noexcept;
		ILINE const_reverse_iterator rend() const noexcept;
		ILINE const_reverse_iterator crend() const noexcept;

	private:
		ILINE void _CheckValidCommandArgument(const char* const pszCommand) const;
		ILINE void _CheckValidCommandDescArgument(const char* const pszCommandDesc) const;

		void _HandleException(const EExceptionCode exceptionCode, const char* const pszMessage) const;

		FLINE void _HandlingCommandNotFound(const char* const pszComment, const char* const pszCommand) const;

		
		FLINE bool _CheckCommandWritingPossibilityCommonPart(const char* const pszArgCommand) const;
		bool _CheckCommandWritingPossibility(const char* const pszArgCommand) const;
		bool _CheckCommandDescWritingPossibility(const char* const pszArgCommand, const char* const pszDesc) const;
		
		template <typename _PROCESSING_FUNC>
		FLINE void _EraseFromCommand(const char* const pszCommand, const _PROCESSING_FUNC processingFunc);

	private:
		set_type m_CommandSet;
		map_type m_CommandMap;
		CSimpleAtomicLocker m_Locker;
	};


	static_assert(
		std::is_polymorphic<CCommandDocument>::value == false, 
		TEXTL("CCommandDocument has polymorphic"));

	void CCommandDocument::_HandleException(const EExceptionCode exceptionCode, const char* const pszMessage) const
	{
		StackWalker sw; sw.ShowCallstack();

		using ErrCode = EExceptionCode::Enum;
		
		switch (exceptionCode)
		{
		case ErrCode::INVALID_ARGUMENT:
			THROWER(InvalidArgumentException, pszMessage);
			break;

		case ErrCode::INVALID_NULLPTR_ARGUMENT:
			THROWER(InvalidArgumentNullptrException, pszMessage);
			break;

		case ErrCode::NOT_FOUND_COMMAND:
			THROWER(CommandDocumentNotFoundCommandException, pszMessage);
			break;

		case ErrCode::NOT_FOUND_COMMAND_DESC:
			THROWER(CommandDocumentNotFoundCommandDescException, pszMessage);
			break;

		case ErrCode::DUPLICATED_COMMAND:
			THROWER(CommandDocumentDuplicatedCommandException, pszMessage);
			break;
		
		case ErrCode::LOCKED:
			THROWER(CommandDocumentLockedException, pszMessage);
			break;

		default:
			break;
		}

		THROWER(InaccessibleAreaException, TEXTL("진행 불가능한 코드 영역에 진입했습니다."));
	}

	CCommandDocument::~CCommandDocument() 
	{
	
	}

	CCommandDocument::CCommandDocument()
		: m_CommandSet()
		, m_CommandMap()
		, m_Locker()
	{

	}

	void CCommandDocument::Lock() noexcept
	{
		m_Locker = true;
	}

	void CCommandDocument::Unlock() noexcept
	{
		m_Locker = false;
	}

	bool CCommandDocument::IsLocked() const noexcept
	{
		return m_Locker;
	}

	void CCommandDocument::_CheckValidCommandArgument(const char* const pszCommand) const
	{
		using ErrCode = EExceptionCode::Enum;

		if (pszCommand == nullptr)
		{
			//_HandleException(ErrCode::INVALID_NULLPTR_ARGUMENT, TEXTL(EF_MSG("pszCommand is nullptr")));
			THROWER(InvalidArgumentNullptrException, TEXTL("pszCommand is nullptr"));
		}
	}

	void CCommandDocument::_CheckValidCommandDescArgument(const char* const pszCommandDesc) const
	{
		if (pszCommandDesc == nullptr)
		{
			THROWER(InvalidArgumentNullptrException, TEXTL(EF_MSG("pszCommandDesc is nullptr")));
		}
	}

	const CCommandDocument::size_type
	CCommandDocument::GetCommandCount() const noexcept
	{
		return m_CommandSet.size();
	}

	bool CCommandDocument::FindCommand(const char* const pszCommand) const
	{
		_CheckValidCommandArgument(pszCommand);

		const auto cEndIter = std::cend(m_CommandSet);

		if (m_CommandSet.find(pszCommand) != cEndIter)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	const CCommandDocument::value_type
	CCommandDocument::GetCommandDesc(const char* const pszCommand) const
	{
		if (FindCommand(pszCommand) == true)
		{
			const auto cEndIter = std::cend(m_CommandMap);
			const auto cFindIter = m_CommandMap.find(pszCommand);

			if (cFindIter != cEndIter)
			{
				return (*cFindIter).second;
			}

		}
		else
		{
			_HandlingCommandNotFound(TEXTL("GetCommandDesc failed : "), pszCommand);
		}

		return EmptyString;
	}



	bool CCommandDocument::_CheckCommandWritingPossibilityCommonPart(const char* const pszArgCommand) const
	{
		if (IsLocked() == true)
		{
			THROWER(ArgumentCommandDocumentLockedException, TEXTL("CCommandDocument is locked"));
		}

		if (pszArgCommand == nullptr)
		{
			THROWER(InvalidArgumentNullptrException, TEXTL("CCommandDocument pszArgCommand is nullptr"));
		}

		return true;
	}


	




	CCommandDocument::iterator
	CCommandDocument::begin() noexcept
	{
		return m_CommandSet.begin();
	}

	CCommandDocument::const_iterator
	CCommandDocument::begin() const noexcept
	{
		return m_CommandSet.begin();
	}

	CCommandDocument::const_iterator
	CCommandDocument::cbegin() const noexcept
	{
		return m_CommandSet.cbegin();
	}

	CCommandDocument::iterator
	CCommandDocument::end() noexcept
	{
		return m_CommandSet.end();
	}

	CCommandDocument::const_iterator
	CCommandDocument::end() const noexcept
	{
		return m_CommandSet.end();
	}

	CCommandDocument::const_iterator
	CCommandDocument::cend() const noexcept
	{
		return m_CommandSet.cend();
	}

	CCommandDocument::reverse_iterator
	CCommandDocument::rbegin() noexcept
	{

		return m_CommandSet.rbegin();
	}

	CCommandDocument::const_reverse_iterator
	CCommandDocument::rbegin() const noexcept
	{
		return m_CommandSet.rbegin();
	}

	CCommandDocument::const_reverse_iterator
	CCommandDocument::crbegin() const noexcept
	{
		return m_CommandSet.crbegin();
	}

	CCommandDocument::reverse_iterator
	CCommandDocument::rend() noexcept
	{
		return m_CommandSet.rend();
	}

	CCommandDocument::const_reverse_iterator
	CCommandDocument::rend() const noexcept
	{
		return m_CommandSet.rend();
	}

	CCommandDocument::const_reverse_iterator
	CCommandDocument::crend() const noexcept
	{
		return m_CommandSet.crend();
	}

	void CCommandDocument::_HandlingCommandNotFound(
		const char* const pszComment,
		const char* const pszCommand) const
	{
		THROWER(ArgumentCommandDocumentNotFoundException, 
			AppendsCharsToString(pszComment, pszCommand, EF_MSG("Not Found Command")).c_str());
	}

	template <typename _PROCESSING_FUNC>
	void CCommandDocument::_EraseFromCommand(const char* const pszCommand, const _PROCESSING_FUNC processingFunc)
	{
		_CheckCommandWritingPossibility(pszCommand);

		if (FindCommand(pszCommand) == true)
		{
			processingFunc();
		}
		else
		{
			_HandlingCommandNotFound(TEXTL("EraseCommand failed : "), pszCommand);
		}
	}
	
	//*전달되는 Command를 파싱하는 Schema interface*/
	class ICommandParserSchema
	{
		DISALLOW_CREATE_INSTANCE(ICommandParserSchema);

	public:
		DEFINE_EXT_ENUM
		(
			enum class, ESchemaType, ENUM_SIZE_DEFAULT,
			E_NAME(REGEX)
		);

	public:
		virtual ~ICommandParserSchema() {}

		virtual bool IsValid() const noexcept = 0;
		virtual void CompileSehema(const char* const pszSchemaString) = 0;
		virtual CCommandDocument GetCommandDocument() = 0;
	};



	class ICommandParser
	{
		DISALLOW_CREATE_INSTANCE(ICommandParser);

	public:
		virtual ~ICommandParser() {}


	};

	class CommandParserDummy
		: public ICommandParser
	{

	};

	/**CommandParser생성을 담당하는 StaticClass*/
	class CommandParser
	{
		DISALLOW_CREATE_INSTANCE_PERMANENTLY(CommandParser);

	public:


	public:
		static ICommandParser* Create(
			const ICommandParserSchema::ESchemaType& schemaType,
			const CommandParserSchemaSignature& signature)
		{
			schemaType.ToString();
		}

	private:

	};


	////////////////////////////////
	class CCommandSchemaFactory;

	/**CommandSchemaFactory의 컴파일이 실패했을 때 발생하는 예외*/
	DEFINE_EXCEPTION_TYPE
	(
		Core, LogicErrorException, CommandSchemaCompileException, NO_DEFINE_DETAILS
	);



	class CArgumentRegexSchema
		: public ICommandParserSchema
	{
	public:
		virtual bool IsValid() const noexcept;
		virtual void CompileSehema(const char* pszSchemaString);

	private:
		//문자열 유효성 체크. 컴파일 유무는 확인 안함
		void _CheckValidSchemaString(const char* const pszSchemaString) const;

	private:
		std::regex m_regexSchema;
		bool m_isValid;
	};






	//Argument를 원하는 데이터 타입으로 변환하기 위한 클래스
	class IMarshalling
	{
	public:
		void SetValue(const string s);
		
	protected:
		const char* GetValue() const;
	};



	class CIntMashlling : IMarshalling
	{
		static int GetValue(IMarshalling* p)
		{
		}
	};


	
}