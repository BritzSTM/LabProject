#include "stdafx.h"
#include "string.h"


namespace Core
{
	namespace Container
	{
		namespace String
		{
			
			string Unicode::ToUTF8(const wstring& str, uint32 codePage)
			{
				int32 len = WideCharToMultiByte(
					codePage, 0, 
					str.c_str(), -1, 
					nullptr, 0, nullptr, nullptr);


				std::vector<char> resStr(len);

				WideCharToMultiByte(
					codePage, 0, 
					str.data(), -1, 
					&resStr[0], len - 1, 
					nullptr, nullptr);

				return string(&resStr[0]);
			}


			wstring UTF8::ToUNICODE(const string& str, uint32 codePage)
			{
				int32 len = MultiByteToWideChar(
					codePage, 0,
					str.c_str(), -1,
					nullptr, 0);

				std::vector<wchar_t> resStr(len);

				MultiByteToWideChar(
					codePage, 0,
					str.c_str(), -1,
					&resStr[0], len - 1);

				return wstring(&resStr[0]);
			}
			
			std::vector<string> SplitStringToVector(const string srcStr, const string delimiter)
			{
				const size_t delimiterLength = delimiter.length();

				size_t delimiterFindPos = 0;
				size_t currStrStartOffset = 0;

				std::vector<string> resList;

				delimiterFindPos = srcStr.find(delimiter, currStrStartOffset);
				while (delimiterFindPos != string::npos)
				{
					const auto delimiterFindPosBias = delimiterFindPos + delimiterLength;

					resList.push_back(srcStr.substr(currStrStartOffset, delimiterFindPosBias - currStrStartOffset));

					currStrStartOffset = delimiterFindPosBias;
					delimiterFindPos = srcStr.find(delimiter, currStrStartOffset);
				}

				resList.push_back(srcStr.substr(currStrStartOffset));
				return std::move(resList);
			}
		}
	}
}