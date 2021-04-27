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
        else if (args[0] != "auth" && self.isLogined() == false)
        {
            assert((fmt::print("客户端无权限!\n"), 1));
            ret=code::auth_error;
            co_return;
        }
        //函数存在，运行该函数，并将结果返回给客户端
        else
        {
            //iter->second是一个函数（通过args[0]查找而得）
            
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

