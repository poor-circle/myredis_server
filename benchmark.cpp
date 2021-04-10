
#include"stdafx.h"
#include"benchmark.h"
#include"ObjectVisitor/stringToObject.hpp"
#include"ObjectVisitor/type.hpp"
#include"ObjectVisitor/get.hpp"
#include"ObjectVisitor/object_encode.hpp"

//单元测试框架

//测试对象类型是否相等（对象，函数或lambda，待比较值，原始测试数据，提示信息）
#define TEST_OBJECT_EQAUL(elem,visitor,output,testdata,message)\
assert(visit([&](auto&& o)\
{\
    auto ___ans = visitor(o);\
    if (___ans == output)\
        return true;\
    fmt::print("error info:{}\n", message);\
    fmt::print("error at:{}\n", testdata);\
    fmt::print("compare:{} != {}\n\n", ___ans, output);\
    return false;\
}, elem))

//测试普通类型是否相等（待比较值1，函数或lambda，待比较值2，原始测试数据，提示信息）
#define TEST_EQAUL(elem,visitor,output,testdata,message)\
assert([&]()\
{\
    auto ___ans = visitor(elem);\
    if (___ans == output)\
        return true;\
    fmt::print("error info:{}\n", message);\
    fmt::print("error at:{}\n", testdata);\
    fmt::print("compare:{} != {}\n\n", ___ans, output);\
    return false;\
}())

namespace myredis
{
#include "namespace.h"
    void Test()
    {
        assert([]()
        {
            stringToObjectTest();
            //add more test here
            fmt::print("{}\n", "PASS TEST!");
            return 1;
        }());
    }
    using namespace error;
    void stringToObjectTest()
    {
        const vector<tuple<string, string>> data =
        {
            {"1","int64"},
            {"0","int64"},
            {"1.442311423","double"},
            {"1.4423114230","string"},
            {"0.0004423114251","double"},
            {"0.0004423114252","double"},
            {"0.0004423114253","double"},
            {"0.0004423114254","double"},
            {"0.0004423114255","double"},
            {"0.0004423114256","double"},
            {"0.0004423114257","double"},
            {"0.0004423114258","double"},
            {"0.0004423114259","double"},
            {"0.000004423114259111111","string"},
            {"10000000.000001","double"},
            {"10000000.000000000001","string"},
            {"0000010000000.1","string"},
            {"asdf dsf sadf ","string"},
            {"1.00000000","string"},
            {"1.1243342114234321142312431243124342312134413214234213","string"},
            {"9223372036854775807","int64"},
            {"-9223372036854775808","int64"},
            {"9223372036854775808","string"},
            {"-9223372036854775809","string"},
            {"-0","string"},
            {"00000000000","string"},
            {"00000123231","string"},
            {"0.0000001","string"},
            {"1e1000","string"},
            {"0x2348902438","string"},
            {"4218793257993dflksdfjdgldgldskgjlsdlkjslksjdlkjsflkjdsflksjdfg235238943438942980342093824dffdsgsgdfsdfsgsgdsdgdsgsd","string"},
            {boost::lexical_cast<string>("\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1"s),"string"},
        };
        for (auto &e:data)
        {
            string s = std::get<0>(e);
            auto g = s.size();
            auto object = stringToObject(std::move(s));

            //测试对象类型是否相等（对象，函数或lambda，待比较值，原始测试数据,错误提示）
            TEST_OBJECT_EQAUL(object,type,"string",e,"对象类型错误");
            TEST_OBJECT_EQAUL(object,object_encode, std::get<1>(e),e,"对象编码错误");

            auto ans = visit([](auto &&o) {return get(o); },object);

            //测试普通类型是否相等（待比较值1，函数或lambda，待比较值2，错误提示）

            TEST_EQAUL(ans,[](auto&& o) {return o.first; }, code::success,e, "操作不成功");
            TEST_EQAUL(ans,[](auto&& o) {return o.second;}, std::get<0>(e),e,"操作返回的字符串有误");

        }
    }
}

