//NVD command line parsing library
//Copyright © 2019, Nikolay Dudkin

//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
//GNU Lesser General Public License for more details.
//You should have received a copy of the GNU Lesser General Public License
//along with this program.If not, see<https://www.gnu.org/licenses/>.

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
