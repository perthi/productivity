// -*- mode: c++ -*-

/*****************************************************************************
***          Author: Per Thomas Hille <pth@embc.no>                       ****
******************************************************************************/

/*****************************************************************************
******************************************************************************
*** This file is part of logmaster.                                        ***
*** Copyright (C) 2018 Per Thomas Hille <pth@embc.no> http:///www.embc.no  ***
*** all rights reserved                                                    ***
***                                                                        ***
*** logmaster is free software: you can redistribute it and/or modify      ***
*** it under the terms of the Lesser GNU General Public License (LGPL)     ***
*** as published by the Free Software Foundation, either version 3 of the  ***
*** License, or (at your option) any later version.                        ***
***                                                                        ***
*** logmaster is distributed in the hope that it will be useful,           ***
*** but WITHOUT ANY WARRANTY; without even the implied warranty of         ***
*** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          ***
*** GNU General Public License for more details.                           ***
***                                                                        ***
*** You should have received a copy of the Lesser GNU General Public       ***
*** License along with logmaster in the README file in the source code     ***
*** directory. If not, see <http://www.gnu.org/licenses/>.                 ***
******************************************************************************
******************************************************************************/


#include <utilities/GTokenizer.h>
#include <utilities/GUtilities.h>
#include <utilities/GString.h>



GTokenizer * g_tokenizer()
{
    static GTokenizer *instance = new GTokenizer();
    return  instance;
}



/** Tokenizing of C style command line (argc argv etc...) It does basically the same as the regular tokinezer, exept that
 *   aguments enclosed in quotes are considered a single argument (for instance if you have an argument containing spaces). 
 *   In addition  space is assumed to be the separator.
 *   @param line The input command line string to tokenize
 *   @return a vector of command line tokens. */
vector<string>
GTokenizer::TokenizeCommandline(string line)
{  
    //    vector<string> tmptokens;

    char quote = '"';
    int n = g_string()->CountOccurences(line, quote, true);
    if (n % 2 != 0)
    {
        throw(std::runtime_error("Undterminated string (closing quote was not found)"));
    }

    const vector<string> tokens = Tokenize(line, " ", fgkIGNORE_CASE);
    vector<string> tmptokens;
    string tmp;
    bool qstartfound = false;
    bool qendfound = false;

    for (uint16_t i = 0; i < tokens.size(); i++)
    {
        long long int length = tokens[i].size();

        if (tokens[i].c_str()[0] == '\"')
        {
            qstartfound = true;
        }
        if (tokens[i].c_str()[length - 1] == '\"')
        {
            qendfound = true;
        }

        if (qstartfound == false && qendfound == false)
        {
            if (tokens[i] != "")
            {
                tmptokens.push_back(tokens[i]);
            }
        }

        if (qstartfound == true)
        {
            tmp += tokens[i] + " ";
        }

        if (qstartfound == true && qendfound == true)
        {
            string substring = tmp.substr(1, tmp.size() - 3);
            tmptokens.push_back(substring);
            qstartfound = false;
            qendfound = false;
            tmp.erase();
        }
    }
    
    return tmptokens;
}





/** Strips the file path an leaves only the filename. We want to allow both "\\" and "/" as separator.
 *  We first tokenize bu "\\" then by "/"
 *  @param[in]      fin    The input filename
 *  @param[in,out]  dir    The resulting directory, without the filename is stored in this variable
 *  @param[in,out]  fout   The resulting filename, without the directory is stored in this variable 
 *  @param[in]      keep_trailing_slahs Wether or not to keep the trailing slash on the directory path after the path has been stripped */
void
GTokenizer::StripPath(const string fin, string &dir, string & fout, const bool keep_trailing_slahs)
{
    vector<string> separators = { "\\", "/" };
    auto tokens = Tokenize(fin, separators, DISCARD_EMPTY, KEEP_SEPARATOR);
    dir.clear();
    fout.clear();

    size_t n = fin.size();

    
    if (fin[n - 1] == '/' || fin[n - 1] == '\\')
    {
        dir = fin;
        fout = ""; 
        return;
    }
    else if (tokens.size() == 1)
    {
        fout = tokens[0];
        dir = "";
        return;
    }


    for (uint16_t i = 0; i < tokens.size() - 1; i++)
    {
        dir += tokens[i];
    }
    
    if (tokens.size() > 1)
    {
        fout = tokens[tokens.size() - 1];
    }


    if (keep_trailing_slahs == false)
    {
        tokens = Tokenize(dir, separators,  DISCARD_EMPTY, DISCAR_SEPARATOR);

        if (tokens.size() == 1)
        {
            dir = tokens[0];
        }
    }

}


/**@{
 *  Tokenizing an input string/vector of strings using  arbitrary separator/vector of separators
 *  @param[in] source The input string(s) to tokenize
 *  @param[in] sep    The separator(s) to use when tokenizing the string, "sep" can be either a single separator or an array (vector) of separators       
 *  @param[in] keep_empty Wether or not to keep empty tokens (tokens with zero length) after tokenizing the string.
 *  For example, concider the string  "a\\tb\t \\t\\t\\t" if this string is tokenized using tab ("\\t") as the separator, then  <br>
 *  1) If the keep_empty flag is false (default) then we will get 3 tokens, namely  "a", "b", " ", whereas the 3 empty strings will be discarded.<br>
 *  2) If the keep_empty flag is true then we will get 5 tokens, "a", "b", " "   and 3 empty strings ("", "", "") <br>
 *  @param  keep_sep  Wether or not to keep the separator in the tokens after the string has been tokenized.  
 *  As an example, consider the string dir1/dir2/dir3/filename. The <br>
 *  1) If the keep_sep flag is false (default) and the separator is "/", the the genrated tokens will be  {"dir1", "dir2", "dir3", "filename"}. <br>
 *  1) If the keep_sep flag is true   and the separator is "/", the the genrated tokens will be  {"dir1/", "dir2/", "dir3/", "filename"}. <br>
 *  @return a vector of tokens <br><br>
 *  It is possible to us an array (vector) of tokens. Concider for instance the string   a/b\\c/d . <br>
 *  Tokenizing this string using the vector {"/", "\\"}  will yield the result {"a", "b", "c", "d"} */
vector<string>
GTokenizer::Tokenize(const vector<string> &source, const string sep, const bool keep_empty, const bool keep_sep )
{
    vector<string> tout;
 
    for (uint16_t i = 0; i < source.size(); i++)
    {
        vector<string> tmp = Tokenize(source[i], sep, keep_empty, keep_sep );
        for (uint16_t j = 0; j < tmp.size(); j++)
        {
            tout.push_back(tmp[j]);
        }
    }
   
    return tout;
}



vector<string>
GTokenizer::Tokenize(const string source, const string sep, const bool keep_empty, const bool keep_sep )
{
    //	std::lock_guard<std::mutex> guard(tokenizer_mutex);
    //	tokenizer_mutex.lock();
	
    std::vector<std::string> results;
    
    size_t prev = 0;
    size_t next = 0;

    if (sep == "")
    {
	//	static char tmp[2];
        char tmp[2];
        for (uint16_t i = 0; i < source.size(); i++)
        {
            SPRINTF(tmp, 2, "%c", source[i]);
            results.push_back( tmp );
                        
        }

        return results;
    }
    
    while ((next = source.find(sep, prev)) != std::string::npos)
    {
        /// We need to check against empty separators, othervise we get stucked in the while loop
        if (sep == "")
        {
            continue;
        }
        
        if  ( keep_empty || (next - prev != 0))
        {
            string sub = source.substr(prev, next - prev);
            if (!(g_utilities()->IsEmpty( source.substr(prev, next - prev) ) && keep_empty == false))
            {
                if (keep_sep == false)
                {
                    results.push_back(source.substr(prev, next - prev));
                }
                else
                {
                    results.push_back(source.substr(prev, next - prev) + sep);
                }
            }
        }
        prev = next + sep.size();
    }

    if (prev < source.size())
    {
        if (keep_sep == false)
        {
            results.push_back(source.substr(prev) );
        }
        else
        {
            results.push_back(source.substr(prev));
        }
    }

    //	tokenizer_mutex.unlock();

    return results;
}


vector<string>   
GTokenizer::Tokenize(const string source, const vector<string> sep,  bool keep_empty, bool keep_sep)
{
    //	tokenizer_mutex.lock();
  
    if (sep.size() == 0)
    {
        return Tokenize(source, "\t",  keep_empty, keep_sep );
    }

    vector<string> tokens_out;
    tokens_out = Tokenize(source, sep[0], keep_empty, keep_sep);

    for (uint16_t i = 1; i < sep.size(); i++)
    {
        tokens_out = Tokenize(tokens_out, sep[i], keep_empty, keep_sep );
    }
    //	tokenizer_mutex.unlock();
    return tokens_out;

}
/**@}*/


vector<string>
GTokenizer::Tokenize(const int argc, const char** argv)
{
    //	tokenizer_mutex.lock();
    vector<string> tmp;
 
    for (int i = 0; i < argc; i++)
    {
        tmp.push_back(string(argv[i]));
    }
    //	tokenizer_mutex.unlock();
    return tmp;
}











