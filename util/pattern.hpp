#pragma once
#include "../stdc++.h"
#include "string.hpp"
namespace myredis
{
	class Pattern {
	private :
		string patternStr;
		std::regex regex;

	public :
		Pattern(string&& str, std::regex&& rx):patternStr(std::move(str)),regex(std::move(rx)) {}
		const string& getPatternStr() const{
			return patternStr;
		}
		const std::regex& getRegex() const{
			return regex;
		}
		void setPatternStr(string str) {
			patternStr = str;
		}
		void setRegex(std::regex rx)
		{
			regex = rx;
		}
		friend bool operator==(const Pattern& A,const Pattern& B) {
			return B.patternStr == A.patternStr;
		}
	};

}
namespace std
{
	template<>
	struct hash<myredis::Pattern>
	{
		std::size_t operator()(myredis::Pattern const& p) const
		{
			return boost::container::hash_value(p.getPatternStr());
		}
	};
	//添加标准库对于myredis::string的哈希支持
}