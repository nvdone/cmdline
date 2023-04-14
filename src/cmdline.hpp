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

#ifndef CMDLINE_HPP
#define CMDLINE_HPP

#include <windows.h>
#include <wctype.h>

class Param
{
	private:

		wchar_t *name;
		wchar_t *name_l;
		wchar_t *value;
		wchar_t *value_l;

		int isoption;

		wchar_t *getToken(const wchar_t **str, int isName);

	public:

		Param *Root;
		Param *Leaf;

		Param(Param *root);
		~Param();

		const wchar_t *Parse(const wchar_t *str);

		int IsOption();
		int NameIs(const wchar_t *candidate, int lc);

		wchar_t *GetName(int lc);
		wchar_t *GetValue(int lc);
};

class CmdLine
{
	private:

		Param *root;
		Param *leaf;

		int nParams;
		int nOptions;
		int nCommands;

	public:

		CmdLine(const wchar_t *cmdline);
		~CmdLine();

		int HasParam(const wchar_t *name, int lc);

		Param *GetParam(int n);
		Param *GetOption(int n);
		wchar_t *GetOptionValue(const wchar_t *name, int lc, int lcv);
		Param *GetCommand(int n);

		int CountParams();
		int CountOptions();
		int CountCommands();
};

#endif