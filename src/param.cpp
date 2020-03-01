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

wchar_t *Param::getToken(const wchar_t **str, int isName)
{
	const wchar_t *p = *str;

	size_t len = 1;
	int quoted = (*p == L'"');

	if(quoted)
		len--;

	if(isName && ((*(p + quoted) == L'-') || (*(p + quoted) == L'/')))
		isoption = 1;

	while(1)
	{
		p++;

		if(!*p)
		{
			if(quoted)
			{
				*str = p;
				return NULL;
			}
			break;
		}

		if(quoted)
		{
			if(*p == L'"')
			{
				p++;
				if(*p != L'"')
					break;
			}
		}
		else
		{
			if(*p == L' ' || (isName && isoption && *p == L':'))
				break;
		}

		len++;
	}

	if(!len)
	{
		*str = p;
		return NULL;
	}

	wchar_t *buf = new wchar_t[len + 1];
	memset(buf, 0, sizeof(wchar_t) * (len + 1));

	p = *str;
	wchar_t *q = buf;

	if(quoted)
		p++;

	while(1)
	{
		if(!*p)
			break;

		if(quoted)
		{
			if(*p == L'"')
			{
				p++;
				if(*p != L'"')
					break;
			}
		}
		else
		{
			if(*p == L' ' || (isName && isoption && *p == L':'))
				break;
		}

		*q++ = *p++;
	}

	if(isoption && *buf == L'/')
		*buf = L'-';

	*str = p;
	return buf;
}


Param::Param(Param *root)
{
	name = name_l = value = value_l = NULL;
	isoption = 0;
	Root = Leaf = NULL;

	Root = root;
	if(Root)
		Root->Leaf = this;
}

Param::~Param()
{
	if(Root)
		Root->Leaf = Leaf;
	if(Leaf)
		Leaf->Root = Root;
	
	if(name)
		delete name;
	if(name_l)
		delete name_l;
	if(value)
		delete value;
	if(value_l)
		delete value_l;

	name = name_l = value = value_l = NULL;
}

const wchar_t *Param::Parse(const wchar_t *str)
{
	while(*str && *str == L' ')
		str++;

	if(!*str)
		return str;

	name = getToken(&str, 1);
	if(name == NULL)
		return str;

	size_t name_len = wcslen(name);
	name_l = new wchar_t[name_len + 1];
	memset(name_l, 0,  sizeof(wchar_t) * (name_len + 1));
	memcpy(name_l, name, sizeof(wchar_t) * name_len);
	CharLowerW(name_l);

	if(!*str)
		return str;

	if(isoption && *str == L':')
	{
		str++;

		value = getToken(&str, 0);
		if(value == NULL)
			return str;

		size_t value_len = wcslen(value);
		value_l = new wchar_t[value_len + 1];
		memset(value_l, 0,  sizeof(wchar_t) * (value_len + 1));
		memcpy(value_l, value, sizeof(wchar_t) * value_len);
		CharLowerW(value_l);
	}

	return str;
}

int Param::IsOption()
{
	return isoption;
}

int Param::NameIs(const wchar_t *candidate, int lc)
{
	if(!name)
		return 0;

	if(lc)
		return wcscmp(name_l, candidate) == 0;

	return wcscmp(name, candidate) == 0;
}

wchar_t *Param::GetName(int lc)
{
	if(lc)
		return name_l;

	return name;
}

wchar_t *Param::GetValue(int lc)
{
	if(lc)
		return value_l;

	return value;
}
