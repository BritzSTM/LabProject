#pragma once


//std
#include <array>
#include <unordered_map>
#include <exception>

//boost
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/seq/cat.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/transform.hpp>
#include <boost/preprocessor/seq/enum.hpp>


#define E_NAME(_NAME_) ((_NAME_))
#define E_NAME_V(_NAME_, _VALUE_) ((_NAME_)(=_VALUE_))
#define ENUM_SIZE_DEFAULT
#define ENUM_SIZE_SET(_TYPE_) : _TYPE_

#define __ENUM_STRINGIZE(s, data, elem) BOOST_PP_STRINGIZE(elem)

#define __ENUM_NAME_GET(s, data, elem) (BOOST_PP_SEQ_ELEM(0, elem))
#define __ENUM_FULL_NAME_GET(s, data, elem) (BOOST_PP_CAT(data, BOOST_PP_SEQ_ELEM(0, elem)))
#define __ENUM_ELEMENT_GET(s, data, elem) (BOOST_PP_SEQ_CAT(elem))

#define __ENUM_NAME_LIST(_SEQ_) BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_FOR_EACH(__ENUM_NAME_GET, 0, _SEQ_))
#define __ENUM_ElEMENT_LIST(_SEQ_) BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_FOR_EACH(__ENUM_ELEMENT_GET, 0, _SEQ_))
#define __ENUM_FULL_ELEMENT_LIST(_ENUM_NAME_, _SEQ_) BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_FOR_EACH(__ENUM_FULL_NAME_GET, _ENUM_NAME_##::, _SEQ_))
#define __ENUM_NAME_LITERAL_LIST(_SEQ_) BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_TRANSFORM(__ENUM_STRINGIZE, 0, BOOST_PP_SEQ_FOR_EACH(__ENUM_NAME_GET, 0, _SEQ_)))


#define __EXT_ENUM_EXCEPTION_MESSAGE_FORMAT " " __FILE__ "(" STRINGIZE(__LINE__) ")"
#define __EXT_ENUM_EXCEPTION(_ENUM_NAME_, _TEXT_MESSAGE_) \
throw std::runtime_error(BOOST_PP_CAT(BOOST_PP_CAT(BOOST_PP_STRINGIZE(_ENUM_NAME_), _TEXT_MESSAGE_), __EXT_ENUM_EXCEPTION_MESSAGE_FORMAT))


#define DEFINE_EXT_ENUM(_ENUM_TYPE_, _ENUM_NAME_, _ENUM_SIZE_, _SEQ_)\
struct _ENUM_NAME_ final {                                                           \
private:                                                                                                                   \
    static constexpr size_t _sg_kEnumCount = BOOST_PP_SEQ_SIZE(_SEQ_);                                                     \
    static constexpr std::array<const char* const, _sg_kEnumCount> _sg_arrRawEnumString = { __ENUM_NAME_LITERAL_LIST(_SEQ_) }; \
public:                                                                                                                    \
    using EncodeType = char;                                                                                               \
    _ENUM_TYPE_ Enum _ENUM_SIZE_ {__ENUM_ElEMENT_LIST(_SEQ_)};                                                             \
    using MapType = std::unordered_map<Enum, const char*>;                                                                 \
    static constexpr std::array<_ENUM_NAME_::Enum, _sg_kEnumCount> EnumIter = {__ENUM_FULL_ELEMENT_LIST(Enum, _SEQ_)};     \
    constexpr _ENUM_NAME_() : m_realEnumValue(BOOST_PP_CAT(Enum::,BOOST_PP_SEQ_ELEM(0, BOOST_PP_SEQ_ELEM(0, _SEQ_)))) { }  \
    constexpr _ENUM_NAME_(const Enum value) noexcept : m_realEnumValue(const_cast<Enum>(value)) { }                        \
                                                                                                                           \
    constexpr operator Enum() const noexcept { return m_realEnumValue; }                                                   \
                                                                                                                           \
	const char* const ToString() const {                                                                                   \
		const auto& enumMap = _GetEnumMap();                                                                               \
		const auto resIter = enumMap.find(m_realEnumValue);                                                                \
		if (resIter != std::cend(enumMap)) { return resIter->second;}                                                      \
        __EXT_ENUM_EXCEPTION(_ENUM_NAME_, "ExtEnum ToString() Mapping Error");                                             \
    }                                                                                                                      \
    static _ENUM_NAME_ FromString(const char* pszStr){                                                                     \
        const auto& enumMap = _GetEnumMap();                                                                               \
        const auto mapCendIter = std::cbegin(enumMap);                                                                     \
        const auto resIter =                                                                                               \
                std::find_if(std::cbegin(enumMap), mapCendIter,                                                            \
			[&pszStr](const auto& pair){return (std::strcmp(pair.second, pszStr) == 0);});                                 \
                                                                                                                           \
        if (resIter != mapCendIter){ return resIter->first; }                                                              \
        __EXT_ENUM_EXCEPTION(_ENUM_NAME_,"ExtEnum FromString() Mapping Error");                                            \
    }                                                                                                                      \
private:                                                                                                                   \
    static const MapType& _GetEnumMap() noexcept{                                                                          \
	    static MapType kMap;                                                                                               \
	    int count = 0;                                                                                                     \
                                                                                                                           \
	    for (const auto& item : EnumIter){                                                                                 \
		    kMap[item] = _sg_arrRawEnumString[count++];                                                                    \
	    }                                                                                                                  \
	    return kMap;                                                                                                       \
    }                                                                                                                      \
    Enum m_realEnumValue;                                                                                                  \
 }