#include "stdafx.h"
#include "funcController.h"
#include "Func/funcManager.h"
namespace myredis::func
{
#include"namespace.i"
    asio::awaitable<void> funcControll(context&& ctx,string &ret)
	{
        auto&& args = ctx.args;
        auto& self = ctx.session;
        if (args.size() == 0)
        {
            ret = "";
            co_return;
        }
		//�����ĵ�һ���ַ����Ǻ������������Ƿ����������
		boost::algorithm::to_lower(ctx.args[0]);
		auto iter = getfuncManager().find(args[0]);
		//���������ڣ����ش�����Ϣ
        if (iter == getfuncManager().end())
        {
            //assert((fmt::print("{}", getMessage(status::command_error), 1));
            ret=code::command_error;
            co_return;
        }
        //�����Ƿ�ӵ��Ȩ��
        else if (args[0] != "auth" && self.isLogined() == false)
        {
            assert((fmt::print("�ͻ�����Ȩ��!\n"), 1));
            ret=code::auth_error;
            co_return;
        }
        //�������ڣ����иú���������������ظ��ͻ���
        else
        {
            //iter->second��һ��������ͨ��args[0]���Ҷ��ã�
            
            auto reply = iter->second.func(std::move(ctx));

            if (self.isBlocked())
            {
                auto new_args=co_await self.block();
                string temp;
                funcControll(context(std::move(new_args),self), temp);
                ret = std::move(temp);
                co_return;
            }


            if (reply.has_value())
            {
                assert((fmt::print("reply:{}\n", reply.value()), 1));
                ret = std::move(reply.value());
                co_return;
                
            }
            //replyΪ�գ���������ʱ���������ش�����Ϣ
            else
            {
                assert((fmt::print("�������ڲ�����\n", reply.value()), 1));
                ret = code::server_exception_error;
                co_return;
            }

        }
	}
}

