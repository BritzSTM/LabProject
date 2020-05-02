#pragma once

#include "string.h"
#include "../Common/Exception.h"
#include "../Type/TypeTraits.h"


namespace Core
{
	namespace Container
	{
		namespace String
		{
			template<typename _Elem, size_t _FixSize>
			class CFixStringBase;

			template <typename _StringType>
			struct _is_string : std::false_type
			{

			};

			template <typename charT, typename traits, typename Alloc>
			struct _is_string<std::basic_string<charT, traits, Alloc> >
				: std::true_type
			{

			};

			template<typename _Elem, size_t _FixSize>
			struct _is_string<CFixStringBase<_Elem, _FixSize>> : std::true_type
			{

			};
			
			template<size_t N>
			struct _is_string<char[N] > : std::true_type
			{

			};
			

			/**
			고정크기 배열을 가진 문자열 클래스
			*/
			template<typename _Elem, size_t _FixSize>
			class CFixStringBase
			{
			public:
				using array_type = std::array<_Elem, _FixSize + 1>;

				//std
				using value_type = typename array_type::value_type;
				using traits_type = std::char_traits<value_type>;
				using reference = typename array_type::reference;
				using const_reference = typename array_type::const_reference;
				using pointer = typename array_type::pointer;
				using const_pointer = typename array_type::const_pointer;

				using iterator       = typename array_type::iterator;
				using const_iterator = typename array_type::const_iterator;
				using reverse_iterator       = typename array_type::reverse_iterator;
				using const_reverse_iterator = typename array_type::const_reverse_iterator;

				using difference_type = typename array_type::difference_type;
				using size_type = typename array_type::size_type;

				/*
				a random access iterator to char(convertible to const_iterator)
				a random access iterator to const char
				*/

				using self_type = typename CFixStringBase<_Elem, _FixSize>;
				friend CFixStringBase;

				static constexpr size_type npos = -1;
			public:
				//기본 생성자
				CFixStringBase()
					: m_stringLength(0)
				{
					_MakingNUL();
					//m_stringArray.fill(0);
				}

				//복제 생성자 ok
				template <typename _StringType>
				CFixStringBase(const _StringType& str)
					: CFixStringBase(str, 0)
				{
					static_assert(_is_string<_StringType>::value,
						"Constructor str argument is not string type");

				}
				
				//ok
				template <typename _StringType>
				CFixStringBase(const _StringType& str, size_type pos, size_type len = npos)
					: CFixStringBase()
				{
					static_assert(_is_string<_StringType>::value,
						"Constructor str argument is not string type");

					size_type selectedLen = (len < str.length()) ? (len) : (str.length());
					selectedLen = _GetBiasLen(selectedLen);
					m_stringLength = selectedLen;
					_MakingNUL();


					auto iter = std::cbegin(str);
					std::advance(iter, pos);

					//복사
					std::copy_n(iter, selectedLen, std::begin(*this));

				}


				CFixStringBase(const_pointer s)
				{
					size_type selectedLen = _GetBiasLen(traits_type::length(s));
					m_stringLength = selectedLen;
					_MakingNUL();

					//dsst, src, cout
					traits_type::copy(&m_stringArray[0], s, selectedLen);
				}

				CFixStringBase(const_pointer s, size_t n)
				{
					size_type rawLen = traits_type::length(s);

					size_type selectedLen =	(n < rawLen) ? (n) : (rawLen);

					selectedLen = _GetBiasLen(selectedLen);
					m_stringLength = selectedLen;
					_MakingNUL();

					//dsst, src, cout
					traits_type::copy(&m_stringArray[0], s, selectedLen);
				}

				CFixStringBase(size_t n, char c)
				{
					m_stringArray.fill(c);
					m_stringLength = n;
				}

				/**
				@see 보강필요
				*/
				template <class InputIterator>
				CFixStringBase(InputIterator first, InputIterator last)
					: CFixStringBase()
				{
					try
					{
 						while (first != last)
						{
							push_back(*(first++));
						}
					}
					catch (...)
					{

					}
					_MakingNUL();
				}

				/**
                @see 보강필요
                */
				CFixStringBase(std::initializer_list<value_type> il)
					: CFixStringBase()
				{
					try
					{
						for (auto& ch : il)
						{
							push_back(ch);
						}
					}
					catch (...)
					{

					}

					_MakingNUL();
				}

				/**
				@see 이동 생성관련해서는 작성하지 않음. 이유는 string class 접근불가
				*/
				/*
				CFixStringBase(string&& str) noexcept
				{

				}
				*/

				//Element access
				reference operator[](size_type pos)
				{
					return m_stringLength[pos];
				}

				const_reference operator[](size_type pos) const
				{
					return m_stringLength[pos];
				}

				reference at(size_type pos)
				{
					return m_stringLength[pos];
				}

				const_reference at(size_type pos) const
				{
					return m_stringLength[pos];
				}

				reference back()
				{
					return m_stringLength[m_stringLength];
				}

				const_reference back() const
				{
					return m_stringLength[m_stringLength];
				}
				//
				reference front()
				{
					return m_stringLength[0];
				}
				//
				const_reference front() const
				{
					return m_stringLength[0];
				}

				//Capacity
				/**문자열 길이를 반환함*/
				FLINE size_type size() const noexcept
				{
					return m_stringLength;
				}

				/**문자열 길이를 반환함*/
				FLINE size_type length() const noexcept
				{
					return m_stringLength;
				}

				/**FixString의 최대 크기를 반환 함*/
				FLINE constexpr size_type max_size() const noexcept
				{
					//'\0' 제외
					return m_stringArray.max_size() - 1;
				}

				/**FixString의 최대 크기를 반환 함*/
				FLINE constexpr size_type capacity() const noexcept
				{
					//'\0' 제외
					return max_size();
				}

				FLINE void clear() noexcept
				{
					//실제로 데이터들을 0으로 비울 필요 없이 인덱스만 0
					m_stringLength = 0;
				}

				FLINE bool empty() const noexcept
				{
					return (m_stringLength == 0);
				}





				//iterator
				iterator begin() noexcept
				{
					return iterator(&m_stringArray[0], 0);
				}

				const_iterator begin() const noexcept
				{
					return const_iterator(&m_stringArray[0], 0);
				}

				const_iterator cbegin() const noexcept
				{
					return begin();
				}


				iterator end() noexcept
				{
					return iterator(&m_stringArray[0], m_stringLength);
				}

				const_iterator end() const noexcept
				{
					return const_iterator(&m_stringArray[0], m_stringLength);
				}

				const_iterator cend() const noexcept
				{
					return end();
				}


				reverse_iterator rbegin() noexcept
				{

					return reverse_iterator(end());
				}

				const_reverse_iterator rbegin() const noexcept
				{
					return const_reverse_iterator(end());
				}

				const_reverse_iterator crbegin() const noexcept
				{
					return rbegin();
				}


				reverse_iterator rend() noexcept
				{
					return reverse_iterator(begin());
				}

				const_reverse_iterator rend() const noexcept
				{
					return const_reverse_iterator(begin());
				}

				const_reverse_iterator crend() const noexcept
				{
					return rend();
				}



				//String operations
				self_type substr(size_type pos = 0, size_type len = npos) const
				{
					return self_type(*this, pos, len);
				}


				//Modifiers
				template<typename _StringType>
				self_type& append(const _StringType& str)
				{
					append(str, 0);
					return *this;
				}

				template<typename _StringType>
				typename std::enable_if< _is_string<_StringType>::value, self_type&>::type
				append(const _StringType& str, size_type subpos, size_type sublen = npos)
				{
					insert(m_stringLength, str, subpos, sublen);
					return *this;
				}

				self_type& append(const_pointer s)
				{
					append(s, traits_type::length(s));
					return *this;
				}

				self_type& append(const_pointer s, size_type n)
				{
					string str(s, n);
					insert(length(), str);
					return *this;
				}


				self_type& append(size_type n, value_type c)
				{
					insert(length(), n, c);
					return *this;
				}

				template <class InputIterator,
					class = std::enable_if_t<std::_Is_iterator_v<InputIterator>>>
				self_type& append(InputIterator first, InputIterator last)
				{
					insert(end(), first, last);
					return *this;
				}

				self_type& append(std::initializer_list<value_type> il)
				{
					insert(end(), il.begin(), il.end());
					return *this;
				}


				void push_back(_Elem c)
				{
					if (max_size() == length())
					{
						//예외 발생 std 표준 a length_error exception
						throw std::length_error("Exceed CFixString size");
					}
					else
					{
						m_stringArray[m_stringLength++] = c;
						_MakingNUL();
					}
				}

				void pop_back()
				{
					if ((--m_stringLength) < 0)
					{
						m_stringLength = 0;
					}

					_MakingNUL();
				}



				template<typename _StringType>
				self_type& insert(size_type pos, const _StringType& str)
				{
					insert(pos, str, 0);
					return *this;
				}

				/*
				enable_if를 통한 타입제한
				template<typename _StringType>
				self_type& insert(
					size_type pos,
					const _StringType& str,
					size_type subpos,
					size_type sublen = npos)
				*/

				template<typename _StringType>
				typename std::enable_if< _is_string<_StringType>::value, self_type&>::type
					insert(
					size_type pos,
					const _StringType& str,
					size_type subpos,
					size_type sublen = npos)

				{
					//check string type
					static_assert(_is_string<_StringType>::value,
						"Insert function str argument is not string type");

					/*
					If pos is greater than the string length,
					or if subpos is greater than str's length, an out_of_range exception is thrown.
					*/
					if ((length() < pos) || (str.length() < subpos))
					{
						throw std::out_of_range("pos or subpos is invalid");
					}


					/*
					If the resulting string length would exceed the max_size,
					a length_error exception is thrown.
					*/
					_StringType subStr;
					if (0 < subpos)
					{
						subStr = str.substr(subpos, sublen); //실제 삽입될 문자열
					}
					else
					{
						subStr = str;
					}

					size_type count = subStr.length();        //삽입 되는 숫자
					size_type predictionLength = length() + count; //예측되는 마지막 문자열 길이

					//현재 길이 + 삽입될 숫자는 최대 크기를 넘으면 안됨
					if (max_size() < predictionLength)
					{
						throw std::length_error("the resulting string length would exceed the max_size");
					}


					//move
					//count + 1 is NUL
					std::memmove(
						&m_stringArray[pos + count], &m_stringArray[pos],
						(length() - pos) + 1);

					//copy
					std::copy_n(
						std::cbegin(subStr), count,
						&m_stringArray[pos]);

					//len fix
					m_stringLength = predictionLength;

					return *this;
				}
				
				self_type& insert(size_type pos, const_pointer s)
				{
					return insert(pos, s, traits_type::length(s));
				}

				self_type& insert(size_type pos, const_pointer s, size_type n)
				{
					auto posIter = begin();
					std::advance(posIter, pos);

					insert(posIter, s, s + n);
					return *this;
				}


				self_type& insert(size_type pos, size_type n, value_type c)
				{
					string s(n, c);
					insert(pos, s, 0);
					return *this;
				}

				iterator insert(const_iterator p, size_type n, value_type c)
				{
					string s(n, c);
					insert(p, std::begin(s), n);
					return begin();
				}

				iterator insert(const_iterator p, value_type c)
				{
					insert(p, 1, c);
					return begin();
				}


				template <class InputIterator, 
					class = std::enable_if_t<std::_Is_iterator_v<InputIterator>>>
				iterator insert(iterator p, InputIterator first, InputIterator last)
				{
					//p = pos인데 미리 정해져서 와야 될듯 함 검사는 어떻게 할 수가 없다.
					size_type count = std::distance(first, last); //삽입되는 숫자.

					/*
					If pos is greater than the string length,
					or if subpos is greater than str's length, an out_of_range exception is thrown.
					*/
					//음수가 되면 역전되어 있는 상태임 불허
					if (std::distance(p, end()) < 0)
					{
						throw std::out_of_range("p is invalid");
					}

					//반복자로 받았음. 체크 안함
					size_type predictionLength = length() + count; //예측되는 마지막 문자열 길이

					//현재 길이 + 삽입될 숫자는 최대 크기를 넘으면 안됨
					if (max_size() < predictionLength)
					{
						throw std::length_error("the resulting string length would exceed the max_size");
					}

					size_type pos = std::distance(begin(), p);
					auto destIter = p;
					std::advance(destIter, count);

					//move 이것은 메모리의 연속성이 보장되서 사용가능 한것
					//count + 1 is NUL
					std::memmove(
						&(*destIter), &(*p),
						(length() - pos) + 1);

					//copy
					std::copy_n(first, count, p);

					//len fix
					m_stringLength = predictionLength;

					return begin();
				}

				self_type& insert(const_iterator p, std::initializer_list<char> il)
				{
					insert(p, il.begin(), il.end());
					return *this;
				}
				
				//operator+=
				template<typename _StringType>
				self_type& operator+=(const _StringType& str)
				{
					return append(str);
				}

				self_type& operator+=(const_pointer s)
				{
					return append(s);
				}

				self_type& operator+=(value_type c)
				{
					push_back(c);
					return *this;
				}

				self_type& operator+=(std::initializer_list<value_type> il)
				{
					return append(il);
				}

				//erase
				self_type& erase(size_t pos = 0, size_t len = npos)
				{
					auto firstIter = iterator(&m_stringArray[0], pos);
					size_type selectedLen = (len < length()) ? (len) : (length());

					auto lastIter  = iterator(&m_stringArray[0], pos + selectedLen);

					erase(firstIter, lastIter);
					return *this;
				}
				
				iterator erase(const_iterator p)
				{
					return erase(p, end());
				}
				
				iterator erase(const_iterator first, const_iterator last)
				{
					//이동할 객체의 수
					size_type moveCount   = std::distance(last, cend()) + 1;
					size_type deleteCount = std::distance(first, last);

					std::memmove(
						const_cast<value_type*>(&(*first)),
						const_cast<value_type*>(&(*last)),
						moveCount);

					m_stringLength -= deleteCount;
					
					return begin();
				}

				void swap(self_type& str)
				{
					std::swap(m_stringArray, str.m_stringArray);
					
					auto temp = m_stringLength;
					m_stringLength = str.m_stringLength;
					str.m_stringLength = temp;
				}

				//assign. 기존값이 제거된다는 것을 상기하자.
				template<typename _StringType>
				self_type& assign(const _StringType& str)
				{
					return assign(str.c_str());
				}

				/*
				template<typename _StringType>
				typename std::enable_if<_is_string<_StringType>::value, self_type&>::type
				*/

				template<typename _StringType>
				typename std::enable_if<_is_string<_StringType>::value, self_type&>::type
					assign(const _StringType& str, size_type subpos, size_type sublen = npos)
				{
					auto sIter = std::begin(str);
					std::advance(sIter, subpos);

					auto eIter = std::end(str);
					size_type len = 0;

					if (sublen == npos)
					{
						len = std::distance(sIter, eIter);
					}
					else
					{
						len = sublen;
					}
					
					return assign(&(*sIter), len);
				}

				self_type& assign(const_pointer s)
				{
					return assign(s, traits_type::length(s));
				}

				self_type& assign(const_pointer s, size_type n)
				{
					if (max_size() < n)
					{
						throw std::length_error("the resulting string length would exceed the max_size");
					}

					m_stringLength = n;
					traits_type::copy(&m_stringArray[0], s, m_stringLength);
					_MakingNUL();

					return *this;
				}

				self_type& assign(size_type n, value_type c)
				{
					if (max_size() < n)
					{
						throw std::length_error("the resulting string length would exceed the max_size");
					}

					std::memset(&m_stringArray[0], c, n);
					m_stringLength = n;
					_MakingNUL();
					return *this;
				}

				template <class InputIterator, 
					class = std::enable_if_t<std::_Is_iterator_v<InputIterator>>>
					self_type& assign(InputIterator first, InputIterator last)
				{
					auto distance = std::distance(first, last);
					return assign(&(*first), distance);
				}

				self_type& assign(std::initializer_list<value_type> il)
				{
					return assign(&(*il.begin()), il.size());
				}


				/*
				replace
				@see 미구현
				*/
				template<typename _StringType>
				self_type& replace(size_type pos, size_type len, const _StringType& str)
				{
					if (length() < (pos + len))
					{
						throw std::out_of_range("pos is out of range");
					}

					;
					//char[] //string, fixstring
					iterator destIter = iterator(&m_stringArray[0], pos);
					auto srcIter = std::begin(str);
					size_type count = _StrLenHelper(str);

					if (len < count)
					{
						count = len;
					}

					traits_type::copy(
						&(*destIter),
						&(*srcIter),
						count);

					return *this;
				}
				/*

				string& replace(const_iterator i1, const_iterator i2, const string& str)
				{

				}
				string& replace(size_t pos, size_t len, const string& str,
					size_t subpos, size_t sublen = npos)
				{

				}
				string& replace(size_t pos, size_t len, const char* s)
				{

				}
				string& replace(const_iterator i1, const_iterator i2, const char* s)
				{

				}
				string& replace(size_t pos, size_t len, const char* s, size_t n)
				{

				}
				string& replace(const_iterator i1, const_iterator i2, const char* s, size_t n)
				{

				}
				string& replace(size_t pos, size_t len, size_t n, char c)
				{

				}
				string& replace(const_iterator i1, const_iterator i2, size_t n, char c)
				{

				}
				template <class InputIterator>
				string& replace(const_iterator i1, const_iterator i2,
					InputIterator first, InputIterator last)
				{

				}
				string& replace(const_iterator i1, const_iterator i2, std::initializer_list<char> il)
				{

				}
				*/

				const_pointer c_str() const noexcept
				{
					return &m_stringArray[0];
				}

				operator string() const
				{
					return string(c_str());
				}

			private:
				FLINE void _MakingNUL() noexcept
				{
					m_stringArray[m_stringLength] = '\0';
				}
				
				/**
				입력된 길이를 재단합니다.
				*/
				FLINE size_type _GetBiasLen(const size_type& len)
				{
					return (max_size() < len) ? (max_size()) : (len);
				}


				FLINE size_type _StrLenHelper(const self_type& str)
				{
					return str.length();
				}

				FLINE size_type _StrLenHelper(const string& str)
				{
					return str.length();
				}

				FLINE size_type _StrLenHelper(const_pointer pStr)
				{
					return std::strlen(pStr);
				}


			private:
				array_type  m_stringArray;
				size_type   m_stringLength;  //'\0'의 위치이자 문자열의 길이
			};

			template <size_t _Size >
			using FixString = CFixStringBase<char, _Size>;

			template <size_t _Size >
			using WFixString = CFixStringBase<wchar_t, _Size>;
		}
	}
}