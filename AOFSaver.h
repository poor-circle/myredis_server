#pragma once
#include "stdafx.h"
#include "stdc++.h"
#include "object.hpp"
#include "code.h"
#include "Func/funcManager.h"
#include "funcController.h"
namespace myredis
{
	class AOFSaver
	{
	private:
		
	public:
		static void aofwriter(std::vector<string>&& command, FILE* fp);
		static void aofload(FILE* fp, std::unique_ptr<BaseSession> self);
		static void aofencode(std::vector<string>& command, string& dest);
		static void aofdecode(FILE* fp, std::vector<string>& command);
	};


}