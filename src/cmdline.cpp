//NVD command line parsing library
//Copyright © 2019, Nikolay Dudkin

//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files (the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.

#include "cmdline.hpp"

CmdLine::CmdLine(const wchar_t *cmdline)
{
	nParams = nOptions = nCommands = 0;
	root = leaf = NULL;

	while(*cmdline)
	{
		Param *param = new Param(leaf);
		cmdline = param->Parse(cmdline);

		if(!param->GetName(0))
		{
			delete param;
		}
		else
		{
			if(!root)
				root = param;

			leaf = param;

			nParams++;
			if(param->IsOption())
				nOptions++;
			else
				nCommands++;
		}
	}
}

CmdLine::~CmdLine()
{
	while(root)
	{
		leaf = root->Leaf;
		delete root;
		root = leaf;
	}
}

int CmdLine::HasParam(const wchar_t *name, int lc)
{
	Param *l = root;

	while(l)
	{
		if(l->NameIs(name, lc))
			return 1;
		l = l->Leaf;
	}

	return 0;
}

Param *CmdLine::GetParam(int n)
{
	if(n > nParams - 1)
		return NULL;

	Param *l = root;

	while(l)
	{
		if(!n)
			return l;

		l = l->Leaf;

		n--;
	}
	
	return NULL;
}

Param *CmdLine::GetOption(int n)
{
	if(n > nOptions - 1)
		return NULL;

	Param *l = root;

	while(l)
	{
		if(l->IsOption())
		{
			if(!n)
				return l;
			n--;
		}

		l = l->Leaf;
	}
	
	return NULL;
}

wchar_t *CmdLine::GetOptionValue(const wchar_t *name, int lc, int lcv)
{
	Param *l = root;

	while(l)
	{
		if(l->NameIs(name, lc))
			return l->GetValue(lcv);

		l = l->Leaf;
	}

	return NULL;
}

Param *CmdLine::GetCommand(int n)
{
	if(n > nCommands - 1)
		return NULL;

	Param *l = root;

	while(l)
	{
		if(!l->IsOption())
		{
			if(!n)
				return l;
			n--;
		}

		l = l->Leaf;
	}
	
	return NULL;
}

int CmdLine::CountParams()
{
	return nParams;
}

int CmdLine::CountOptions()
{
	return nOptions;
}

int CmdLine::CountCommands()
{
	return nCommands;
}
