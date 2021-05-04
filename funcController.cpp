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
        else if (args[0] != "auth"sv && self.isLogined() == false)
        {
            assert((fmt::print("�ͻ�����Ȩ��!\n"), 1));
            ret=code::auth_error;
            co_return;
        }
        //�������ڣ����иú���������������ظ��ͻ���
        else
        {
            optional<string> reply;
            //iter->second��һ��������ͨ��args[0]���Ҷ��ã�

            //�ж���ͬ����������Э��-�첽����,Ȼ��ݴ�ִ��ͬ��/�첽�ĵ��ã�


            if (!iter->second.isAsyncFunc())//ִ��ͬ������
            {
                reply = iter->second.syncptr(std::move(ctx));
            }
            else //ִ���첽����
            {
                //reply = co_await iter->second.asyncptr(std::move(ctx));
            }

            if (self.isBlocked()) //�������Ϊ����̬����ȴ���Ӧ�¼������������û��ṩ�Ļص����������ؽ��
            {
                reply=co_await self.wait();
            }

            //�������б�����ļ�����
            auto& que = self.wake_up_queue;
            while(!que.empty())
            {
                BaseSession::wake_up(que.front().first, que.front().second);
                que.pop();
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

