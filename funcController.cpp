#include "stdafx.h"
#include "funcController.h"
#include "Func/funcManager.h"
#include "Func/pubsubFunc.h"
#include "AOFSaver.h"
#include "multiServer/SonServer.h"
#include "multiServer/FatherServer.h"
#include "code.h"
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
        //�ж��Ƿ��ڶ���̬
        else if (self.isSubscribed() && !isAllowWhenSubscribed(args[0]))
        {
            assert((fmt::print("�������ڶ���״̬�²��Ϸ�!\n"), 1));
            ret = code::illegal_command_when_subscribe;
            co_return;
        }
        //�������ڣ����иú���������������ظ��ͻ���
        else
        {
            auto reply=co_await funcRunner(iter->second, std::move(ctx));//���ж�Ӧ�ĺ���

            if (self.isBlocked()) //�������Ϊ����̬����ȴ���Ӧ�¼������������û��ṩ�Ļص����������ؽ��
            {
                reply=co_await self.wait();
            }

            //�������б�����ļ�����
            auto& que = self.wake_up_queue;
            while(!que.empty())
            {
                co_await BaseSession::wake_up(que.front().first, que.front().second);
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
    asio::awaitable<optional<string>> funcRunner(func::funcInfo foo,context&&ctx)
    {
        static auto clk = steady_clock();
        auto args = ctx.args;
        auto &session = ctx.session;
        std::optional<string> ret;
        auto server = FatherServer::getFatherServer();
        if ((foo.type !=func::funcType::connect&& foo.type != func::funcType::read)&&
            server != nullptr && server->getCas() != FatherServer::serverCas::no_father&&
            session.getIsInnerSession()==false)
        {
            ret = code::getErrorReply(code::endpoint_toString(server->getAddress()));//�ض������Ҫ��ͻ���ȥ������������
        }
        else
        {
            auto tp = clk.now();
            ret = foo.syncptr(std::move(ctx));//���ж�Ӧ�ĺ��� 
            auto duration = clk.now() - tp;//�����Լ���ִ��ʱ��
            if ((foo.type == func::funcType::write || foo.type == func::funcType::blocked) && code::isFuncSucceed(ret))
            {
                AOFSaver::aofwriter(args, AOFSaver::getFile(session.getDataBaseID()));
                auto server = SonServer::getSonServer();
                if (server!=nullptr&&server->getCas() != SonServer::serverCas::notConnect)
                    co_await server->send(args, duration_cast<seconds>(duration));
            }
        }
        co_return ret;
    }
}

