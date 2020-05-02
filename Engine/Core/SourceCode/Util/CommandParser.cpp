#include "stdafx.h"
#include "CommandParser.h"
#include "Container/string.h"

#include <cassert>

namespace Core
{
	ICommandParser* CommandParser::Create(
		const ICommandParserSchema::ESchemaType& schemaType,
		const CommandParserSchemaSignature& signature)
	{
		THROWER(NotImplementedException, TEXTL("미구현"));

		switch (schemaType)
		{
		case ICommandParserSchema::ESchemaType::Enum::REGEX:
			return nullptr;

		default:
			THROWER(InaccessibleAreaException, 
				TEXTL(EF_MSG("진행불가능한 코드 영역에 진입했습니다.")));
		}
		

		return new CommandParserDummy();
	}



	void CArgumentRegexSchema::_CheckValidSchemaString(const char* const pszSchemaString) const
	{
		if (pszSchemaString == nullptr)
		{
			throw InvalidArgumentNullptrException(TEXTL("pszSchemaString is nullptr"));
		}
	}

	bool CArgumentRegexSchema::IsValid() const noexcept
	{
		return m_isValid;
	}

	void CArgumentRegexSchema::CompileSehema(const char* pszSchemaString)
	{
		_CheckValidSchemaString(pszSchemaString);

		try
		{
			m_regexSchema = std::regex(pszSchemaString);
			m_isValid = true;
		}
		catch(std::regex_error&)
		{
			m_isValid = false;
			std::throw_with_nested(CommandSchemaCompileException());
			//std::throw_with_nested(
			//	CommandSchemaCompileException(
			//		AppendsCharsToString(
			//			TEXTL("CArgumentRegexSchema compile failed : "), pszSchemaString))
			//);
		}
	}





	bool CCommandDocument::_CheckCommandWritingPossibility(const char* const pszArgCommand) const
	{
		_CheckCommandWritingPossibilityCommonPart(pszArgCommand);

		if (FindCommand(pszArgCommand) == true)
		{
			throw ArgumentCommandDocumentDuplicatedException(TEXTL("pszArgCommand is exist already"));
		}

		return true;
	}

	bool CCommandDocument::_CheckCommandDescWritingPossibility(
		const char* const pszArgCommand,
		const char* const pszDesc) const
	{
		_CheckCommandWritingPossibilityCommonPart(pszArgCommand);

		if (pszDesc == nullptr)
		{
			throw InvalidArgumentNullptrException(TEXTL("pszDesc is nullptr"));
		}

		if (FindCommand(pszArgCommand) == false)
		{
			_HandlingCommandNotFound(
				TEXTL("FindCommand failed in _CheckCommandDescWritingPossibility func : "),
				pszArgCommand);
		}

		return true;
	}


	

	

	void CCommandDocument::WriteCommand(const std::initializer_list<const char* const>& argCommands)
	{
		for (const auto& pszCommand : argCommands)
		{
			WriteCommand(pszCommand);
		}
	}

	void CCommandDocument::WriteCommand(const char* const pszArgCommand)
	{
		_CheckCommandWritingPossibility(pszArgCommand);
		m_CommandSet.insert(pszArgCommand);
	}
	
	void CCommandDocument::WriteCommandDesc(const char* const pszTargetArgCommand, const char* const pszDesc)
	{
		_CheckCommandDescWritingPossibility(pszTargetArgCommand, pszDesc);
		m_CommandMap.insert(std::make_pair(pszTargetArgCommand, pszDesc));
	}

	void CCommandDocument::WriteCommandAndDesc(const char* const pszArgCommand, const char* const pszDesc)
	{
		//중복되는 예외처리... How.
		WriteCommand(pszArgCommand);
		WriteCommandDesc(pszArgCommand, pszDesc);
	}

	void CCommandDocument::EraseCommand(const char* const pszCommand)
	{
		_EraseFromCommand(pszCommand, 
			[this, &pszCommand]()
		{
			m_CommandSet.erase(pszCommand);
			m_CommandMap.erase(pszCommand);
		});
	}

	void CCommandDocument::EraseCommandDesc(const char* const pszCommand)
	{
		_EraseFromCommand(pszCommand,
			[this, &pszCommand]()
		{
			m_CommandMap.erase(pszCommand);
		});
	}




}