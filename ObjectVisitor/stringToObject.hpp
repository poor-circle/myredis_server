#pragma once
#include "../stdafx.h"

namespace myredis
{
    //get函数：只适用于字符串对象，不能用于其他类型的对象
    static object stringToObject(string&& str)
    {
        static string temp;
        static string upper = boost::lexical_cast<string>(LLONG_MAX),
            lower = boost::lexical_cast<string>(LLONG_MIN);
        if (str.size() > 20)//it must be string
            return std::make_unique<string>(std::move(str));
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

        return std::make_unique<string>(std::move(str));
    }
}
