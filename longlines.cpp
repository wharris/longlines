/*
 * longlines - list long lines in a text file.
 * Copyright (C) 2001-2009  Will Harris
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <iostream>
#include <string>
#include <list>
#include <fstream>
using namespace std;

#include <stdlib.h>

const string SYNTAX =
    "Usage: %{command} -n -w <width> -t <tab size> [files]";

const string HELP = SYNTAX + "\n"
	"-n : show line numbers\n"
	"-w : set threshole\n"
	"-t : set width for tab expansion\n";
	

const int DEFAULT_MAX = 78;
const int DEFAULT_TAB_WIDTH = 4;

int max_line = DEFAULT_MAX;
int tab_width = DEFAULT_TAB_WIDTH;
bool show_line_numbers = false;

string replacetag(const string, const string, const string);

list<string> filenames;

int main(int argc, char** argv)
{
	string arg;

	arg = *argv;

	string syntax = replacetag(SYNTAX, "command", arg);
	string help = replacetag(HELP, "command", arg);

	argv++;
	argc--;

	while (argc)
	{
		arg = *argv;

    	argv++;
		argc--;

        if (arg == "-w")	// width
		{

			if (!argc)
			{
				cerr << syntax << "\n";
				return 2;
			}

			max_line = atoi(*argv);
			argv++;
			argc--;
		}

		else if (arg == "-t")	// tab width
		{
			if (!argc)
			{
				cerr << syntax << "\n";
				return 2;
			}

			tab_width = atoi(*argv);
			argv++;
			argc--;
		}

		else if (arg == "-n")	// show line numbers
		{
			show_line_numbers = true;
		}

		else if (arg == "-h")	// help
		{
			cout << help << "\n";
			return 0;
		}

		else if (arg[0] == '-')	// unknown switch
		{
			cerr << syntax << "\n";
			return 2;
		}

		else
			filenames.insert(filenames.end(), arg);
	}

	if (!filenames.size() )
	{
		cerr << syntax << "\n";
		return 2;
	}

	list<string>::iterator i = filenames.begin();

	string buf = "";
	int count = 0;

	while (i != filenames.end() )
	{
		string filename = *i;
		int lineno = 0;

		char chr = '\0';
		int col = 0;

		ifstream file;
		file.open(filename.c_str() );

		while (!file.eof() )
		{
			file.read(&chr,1);

			if (chr == '\t')
			{
				buf += " ";
				col++;
				while (col % tab_width)
				{
					buf += " ";
					col++;
				}
			}

			else if (chr == '\n')
			{
				lineno++;
				if (col > max_line)
				{
					cout << filename << ":";

					if (show_line_numbers)
						cout << lineno << ":";
				
					cout << buf << "\n";

					count++;
				}

				col = 0;
				buf = "";

			}

			else
			{
				buf += chr;
				col++;
			}
		}

		i++;
	}

	return count == 0;
}


string replacetag(const string src, const string tag, const string value)
{
	int strpos = src.find("%{" + tag + "}");

	return src.substr(0, strpos) + value
		+ src.substr(strpos + tag.length() + 3, src.length() - strpos - 2);
}