// -*- mode: c++ -*-
#ifndef GGENERATEVERSIONINFO_H
#define GGENERATEVERSIONINFO_H

/****************************************************************************
*** Copyright(C) 2015  Per Thomas Hille, pth@embc.no                     ***
*** This file is part of logmaster. logmaster is free software : you can ***
*** redistribute it and / or modify it under the terms of the Lesser GNU ***
*** General Public License (LGPL) V3 or later. See .cpp file for details ***
****************************************************************************/


#define G_STANDALONE

#include   "GGenerateVersionInfo.h"
#include   <utilities/GTokenizer.h>
#include   <utilities/GSystem.h>
#include   <utilities/GFileIOHandler.h>
#include   <utilities/GTime.h>
#include   <utilities/GString.h>

#include  <logging/LLogApi.h>

using namespace LOGMASTER;

#include   <cctype>
#include   <string>

#include <iostream>

using std::cout;
using std::endl;

using std::string;



#include <utilities/GDefinitions.h>
#include <string>

using std::string;

/** @brief   auto generation of classes that contains GIT version information 
 *  @details Runs git-info in the current directory, extracts
 *  this information, and puts it dedicated container classes. This is in order to make it
 *  possible to ask the application about relevant version information such as the git tag
 *  compilation options that was used to generated the code etc.
 *  Typically this information via the -version commandline tag.  */
class  GGenerateVersionInfo
{
public:
    virtual inline ~GGenerateVersionInfo();
    static inline string GitInfo();
    static inline string Branch();
    static inline string Version();
    static inline string CompileInfo(const string info_file);
    static inline string LinkInfo( const string info_file );
    static string OriginalFilename();
    static inline void GenerateClass( const string classname, const string exename, const string info_file, const string target_directory );

private:
    inline GGenerateVersionInfo();
    static inline string ReadCompileInfo( const unsigned int idx, const string fname);
};





GGenerateVersionInfo::GGenerateVersionInfo()
{

}


GGenerateVersionInfo::~GGenerateVersionInfo()
{

}


string
GGenerateVersionInfo::GitInfo()
{
#ifdef _WIN32
    return "git info is not avilable for Windows yet";
#else
    auto tmp =  g_system()->exec("git-info");
    auto tmp2 =  g_string()->Replace(tmp, "\"", " "); 
//    COUT << "!!!!!!! tmp2 =\n" << tmp2 << endl;
    return tmp2;

    ///return g_string()->Replace(tmp, "\"", "") + "blahhhhhh";
    //return tmp ;
#endif
}


string
GGenerateVersionInfo::Branch()
{
    string tmp;
#ifdef _WIN32
    tmp = g_system()->exec("git branch |  \"C:\\Program Files\\Git\\usr\\bin\\grep.exe\" \"*\" ");
//	tmp = "not implemented on windows";
#else
    tmp = g_system()->exec("git branch | grep \"*\"");

    //COUT << "branch !!!!!! = "  << tmp << std::endl;
#endif

    vector<string> tokens = g_tokenizer()->Tokenize(tmp, " ");

    if (tokens.size() == 2)
    {
        g_string()->Trim(tokens[1], '\n');
        //COUT << "branch !!!!!! = "  << tokens[1] << std::endl;
        return tokens[1];
    }
    else
    {
        return  "Error parsing branch !!";
    }
}


string
GGenerateVersionInfo::Version()
{
#ifndef _WIN32
    string tmp = g_system()->exec("git describe --tags");
#else
	  string tmp = g_system()->exec("git describe --tags");
	//string tmp  = "git describe --tags takes forever on windows, ignored !!";
#endif

	g_string()->Trim(tmp, '\n');
	return tmp;
}




string
GGenerateVersionInfo::CompileInfo(const string fname)
{
#ifndef  _WIN32
    return ReadCompileInfo(1, fname);
#else
	return "not implemented for windows";
#endif
}

string
GGenerateVersionInfo::LinkInfo(const string fname)
{
	#ifndef  _WIN32
    return ReadCompileInfo(0, fname);
#else
	return "not implemented for windows";
#endif
}





string
GGenerateVersionInfo::ReadCompileInfo(const unsigned int idx, const string fname)
{   
//    CERR << "reading file = " << fname << endl;
    vector<string> cont = g_file()->ReadAll(fname.c_str());

    string ret;

    if (cont.size() > idx)
    {
        ret = cont[idx];
    }
    else
    {

      //  printf("FATAL Error reading compil info from file: %s", fname.c_str());
        //   G_FATAL("Error reading compil info from file: %s", fname.c_str() );
        //  COUT << "Error reading compil info from file : " << fname << endl;
    }

    return ret;

}



void
GGenerateVersionInfo::GenerateClass(const string class_name, const string exename, const string info_file, const string target_directory)
{
    string filename_cpp;
    string filename_h;

    if (target_directory == ".")
    {
        filename_cpp = class_name + ".cpp";
        filename_h = class_name + ".h";
    }
    else
    {
        filename_cpp = target_directory + "/" + class_name + ".cpp";
        filename_h = target_directory + "/" + class_name + ".h";
    }


    string str = class_name;
    // string guard =std::transform( str.begin(), str.end(), str.begin(), ::toupper);
    std::transform(str.begin(), str.end(), str.begin(),
        [](unsigned char c) -> unsigned char { return std::toupper(c); });

#ifdef _WIN32
    FILE *fp = 0;
    fopen_s(&fp, filename_cpp.c_str(), "w");
#else
    FILE *fp = fopen(filename_cpp.c_str(), "w");
#endif

    if (fp == 0)
    {

#ifndef _WIN32
        G_FATAL("Could not open file: %s", filename_cpp.c_str());
#endif

		//      printf("Could not open file: %s", filename_cpp.c_str());

    }
    else
    {
        fprintf(fp, "// -*- mode: c++ -*-\n\n");
        fprintf(fp, "/******\t\tTHIS FILE IS AUTOGENERATED, DO NOT EDIT !!\t******/\n");
        fprintf(fp, "/******\t\tDate: %s\t\t*****/\n\n\n", GTime().TimeStamp().c_str());
        fprintf(fp, "\n\n");
        fprintf(fp, "#include <string>\n");
        fprintf(fp, "using std::string; \n\n");
        fprintf(fp, "#include <version-info/%s>\n\n", filename_h.c_str());
        fprintf(fp, "const string %s::fGitBranch = \"%s\";\n", class_name.c_str(), Branch().c_str());
        fprintf(fp, "const string %s::fGitTag = \"%s\";\n", class_name.c_str(), Version().c_str());
        fprintf(fp, "const string %s::fCompileFlags = \"%s\";\n", class_name.c_str(), CompileInfo(info_file).c_str());
        fprintf(fp, "const string %s::fLinkFlags = \"%s\";\n", class_name.c_str(), LinkInfo(info_file).c_str());
        fprintf(fp, "string %s::fOriginalExeName = \"%s\";\n", class_name.c_str(), exename.c_str());


#ifdef _WIN32
        fprintf(fp, "const string %s::fGitInfo = \"%s\";\n", class_name.c_str(), "Not yest implemented for Windows");
#else

        string tmp = GitInfo();
        g_string()->Trim(tmp, { ' ', '\n', '\t' });
        g_string()->ReplaceBadChar(tmp, '_', "#*@");
        tmp = g_string()->Replace(tmp, "\"", "");
        vector<string> tokens = g_tokenizer()->Tokenize(tmp, "\n");

        fprintf(fp, "const string %s::fGitInfo = \"", class_name.c_str() );
        for(size_t i=0; i < tokens.size(); i++ )
        {
            fprintf(fp,  "%s\\n\\\n", tokens[i].c_str() );
        }

        fprintf(fp, "\";\n\n\n");
        
        fclose(fp);
#endif

    }
}


#endif
