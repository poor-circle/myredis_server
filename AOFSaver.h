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
		static void aofencode(std::vector<string>& command, FILE* dest);
	public:
		static void aofwriter(std::vector<string>& command, FILE* fp);
		static void aofload() noexcept;
		static std::vector<string> aofdecode(FILE* fp);
		static FILE*& getFile(size_t dBIndex);
		static void saveToTempFile();
		static void moveTempFile();
	};


}