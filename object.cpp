#include"stdafx.h"
#include"object.hpp"
namespace myredis
{
#include"namespace.h"
	hash_map<string, object>& getObjectMap()
	{
		static hash_map<string, object> map;
		return map;
	}

	object stringToObject(string&& str)
	{
		static string temp;
		static string upper = boost::lexical_cast<string>(LLONG_MAX),
			lower = boost::lexical_cast<string>(LLONG_MIN);
		if (str.size() > 20)//it must be string
			return make_unique<string>(std::move(str));
		int64_t integer;
		double db;

		if (boost::conversion::detail::try_lexical_convert(str, integer))
		{
			temp.clear();
			fmt::format_to(back_inserter(temp), FMT_COMPILE("{}"), integer);
			if (temp == str) return integer;
		}
		else if (boost::conversion::detail::try_lexical_convert(str, db))
		{
			temp.clear();
			fmt::format_to(back_inserter(temp), FMT_COMPILE("{}"), db);
			if (temp == str) return db;
		}
		return make_unique<string>(std::move(str));
	}
}