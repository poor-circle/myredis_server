
#include"stdafx.h"
#include"benchmark.h"
#include"../object.hpp"
#include"ObjectVisitor/keyVisitor/type.hpp"
#include"ObjectVisitor/StringVisitor/get.h"
#include"ObjectVisitor/keyVisitor/object_encode.hpp"

//��Ԫ���Կ��

//���Զ��������Ƿ���ȣ����󣬺�����lambda�����Ƚ�ֵ��ԭʼ�������ݣ���ʾ��Ϣ��
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

//������ͨ�����Ƿ���ȣ����Ƚ�ֵ1��������lambda�����Ƚ�ֵ2��ԭʼ�������ݣ���ʾ��Ϣ��
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
    void stringToObjectTest();//����set�����ܷ���������

#include "namespace.h"
    void Test()
    {
        assert([]()
        {
            stringToObjectTest();
            //add more test here
            fmt::print
            (
                "*{0:*^{2}}*\n"
                "*{0: ^{2}}*\n"
                "*{1: ^{2}}*\n"
                "*{0: ^{2}}*\n"
                "*{0:*^{2}}*\n\n",
                "", "myredis pass test!", 75
            );
            return 1;
        }());
    }
    using namespace code;
    void stringToObjectTest()
    {
        //�������ݼ���
        const vector<tuple<string, string>> data =
        {
            {"1","int64"},
            {"0","int64"},
            {"1.442311423","double"},
            {"1.4423114230","embstr"},
            {"0.0004423114251","double"},
            {"0.0004423114252","double"},
            {"0.0004423114253","double"},
            {"0.0004423114254","double"},
            {"0.0004423114255","double"},
            {"0.0004423114256","double"},
            {"0.0004423114257","double"},
            {"0.0004423114258","double"},
            {"0.0004423114259","double"},
            {"0.000004423114259111111","raw"},
            {"10000000.000001","double"},
            {"10000000.000000000001","embstr"},
            {"0000010000000.1","embstr"},
            {"asdf dsf sadf ","embstr"},
            {"1.00000000","embstr"},
            {"1.1243342114234321142312431243124342312134413214234213","raw"},
            {"9223372036854775807","int64"},
            {"-9223372036854775808","int64"},
            {"9223372036854775808","embstr"},
            {"-9223372036854775809","embstr"},
            {"-0","embstr"},
            {"00000000000","embstr"},
            {"00000123231","embstr"},
            {"0.0000001","embstr"},
            {"1e1000","embstr"},
            {"0x2348902438","embstr"},
            {"4218793257993dflksdfjdgldgldskgjlsdlkjslksjdlkjsflkjdsflksjdfg235238943438942980342093824dffdsgsgdfsdfsgsgdsdgdsgsd","raw"},
            {boost::lexical_cast<string>("\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1"s),"raw"},//
        };
        for (auto &e:data)
        {
            string s = std::get<0>(e);
            auto g = s.size();
            auto object = stringToObject(std::move(s));

            //���Զ��������Ƿ���ȣ����󣬺�����lambda�����Ƚ�ֵ��ԭʼ��������,������ʾ��
            TEST_OBJECT_EQAUL(object, visitor::type,"string",e,"�������ʹ���");
            TEST_OBJECT_EQAUL(object, visitor::object_encode, std::get<1>(e),e,"����������");

            auto ans = visit([](auto &&o) {return visitor::get(o); },object);

            //������ͨ�����Ƿ���ȣ����Ƚ�ֵ1��������lambda�����Ƚ�ֵ2��������ʾ��

            TEST_EQAUL(ans,[](auto&& o) {return o.first; }, code::success,e, "�������ɹ�");
            TEST_EQAUL(ans,[](auto&& o) {return o.second;}, std::get<0>(e),e,"�������ص��ַ�������");

        }
    }
}

