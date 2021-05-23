#include "stdafx.h"
#include "funcController.h"
#include "Func/funcManager.h"
#include "Func/pubsubFunc.h"
#include "AOFSaver.h"
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
		//参数的第一个字符串是函数名，查找是否有这个函数
		boost::algorithm::to_lower(ctx.args[0]);
		auto iter = getfuncManager().find(args[0]);
		//函数不存在，返回错误信息
        if (iter == getfuncManager().end())
        {
            //assert((fmt::print("{}", getMessage(status::command_error), 1));
            ret=code::command_error;
            co_return;
        }
        //检验是否拥有权限
        else if (args[0] != "auth"sv && self.isLogined() == false)
        {
            assert((fmt::print("客户端无权限!\n"), 1));
            ret=code::auth_error;
            co_return;
        }
        //判断是否处于订阅态
        else if (self.isSubscribed() && !isAllowWhenSubscribed(args[0]))
        {
            assert((fmt::print("该命令在订阅状态下不合法!\n"), 1));
            ret = code::illegal_command_when_subscribe;
            co_return;
        }
        //函数存在，运行该函数，并将结果返回给客户端
        else
        {
            if (iter->second.type == func::funcType::write)
            {
                AOFSaver::aofwriter(ctx.args, AOFSaver::getFile(ctx.session.getDataBaseID()));
            }
            auto reply = iter->second.syncptr(std::move(ctx));//运行对应的函数


            if (self.isBlocked()) //如果设置为阻塞态，则等待对应事件发生，调用用户提供的回调函数并返回结果
            {
                reply=co_await self.wait();
            }

            //唤醒所有被激活的监视器
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
            //reply为空，函数运行时崩溃，返回错误信息
            else
            {
                assert((fmt::print("服务器内部错误\n", reply.value()), 1));
                ret = code::server_exception_error;
                co_return;
            }

        }
	}
}

