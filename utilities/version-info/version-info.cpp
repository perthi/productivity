
// -*- mode: c++ -*-

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


#include <iostream>

using std::cout;
using std::endl;



#include <utilities/GUtilities.h>

#include <utilities/GTokenizer.h>
#include <utilities/GString.h>
#include <utilities/GRegexp.h>
#include <utilities/GSystem.h>
#include <utilities/GTime.h>
#include <utilities/GNumbers.h>
#include <logging/LLogApi.h>
#include <cmdline/GLogApplication.h>
#include <exception/GException.h>
#include <utilities/GFileIOHandler.h>
#include <utilities/GText.h>
#include <utilities/GString.h>

#include <string>
#include <vector>
#include <time.h>

#ifdef _WIN32
#include <Windows.h>
#endif

#include "GGenerateVersionInfo.h"
//#define G GGenerateVersionInfo
#define G_STANDALONE


using std::stringstream;
using std::string;
using std::vector;

/// void    parseSvnInfo(const string svn_info, int &rev, int &lc_rev, string &repo_name, string &version, string &branch);
string  generateVersionString(const vector<int> num);
// void generateSvnFile(const string directory, const string classname, const string repo, const string version,
  //                      const string branch, vector<string> svn_tokens, string &outdir);
void generateVersionFile(const string directory, const string fname, const int rev, const string version, const string branch, const string                                     configuration, const string platform, string &outdir);

void generate_rc_file(  string directory, const string rc_filename, const string company, const string desc, const string filename, const string                             copyright, const string prod_name);
void autoClause(FILE *fp);
void helpMenu();
bool hasArgument(int argc, const char **args, const string argument);
void scanParameter(int argc, const char **args, string name, string &par);
bool is_valid_tag(const string tag);
bool is_valid_argument(int argc, const char **argv, string &invalid_arg);
void print_tags();
vector<string> g_valid_tags;

//bool GCmdScan::fDoIgnoreStrayArguments = false;


int main( int argc, const char **argv )
{
    string configuration;
    string platform;

#ifdef _WIN64
    platform = "x64 (64 bit)";
#else
    platform = "x86 (32 bit)";
#endif

#ifdef _DEBUG
    configuration = "Debug";
#else
    configuration = "Release";
#endif

    time_t  the_time = time(NULL);

#ifdef _WIN32
#pragma warning(suppress: 4996) 
    struct tm *a_time = localtime(&the_time);
#else
    struct tm *a_time = localtime(&the_time);
#endif






    string year = g_string()->ToString<int>(a_time->tm_year + 1900);
    string rc_filename = "unknown_resource_file.rc";
    string compileflags_file = "unknown_compileflags_file";
    string company = "Flekkefjord Elektro, " + year;
    string dllname = "unknown_dll";
    string exename = "unknowne_exefile";
    string productname = "unknown_product";
    string description = "no_description";
    string copyright = "unknown_copyright";
    string compileinfo = "not_set";

    try
    {
        scanParameter( argc, argv,  "-rc_filename", rc_filename);
        scanParameter( argc, argv,  "-productname", productname);
        scanParameter( argc, argv,  "-description", description);
        scanParameter( argc, argv,  "-copyright", copyright);
        scanParameter( argc, argv,  "-compileflags_file", compileflags_file );
        scanParameter( argc, argv,  "-exename",  exename );
        scanParameter( argc, argv,  "-compileinfo", compileinfo );
	}
    catch (std::exception &e)
    {
        cerr << e.what() << endl;
        exit(-1);
    }

    g_valid_tags = vector<string>{ "-rc_filename", "-company", "-dllname" , "-productname", "-description", "-copyright",  "-info", "-help", "-repo-name", "-branch", "-version",  "-rev", "-lc-rev",  "-generate-files", "-compileflags_file", "-exename", "-compileinfo" };
    string invalid;

    try
    {
        string path = "";
        if ( argc  < 2  )
        {
            path = ".";
        }
        else
        {
            path = "\"" + string(argv[1]);
        }
        const string  classname = "SvnData";
        int rev = -1;
//        int lc_rev = -1;
        string repo_name = "unknown_repo_name";
        string version =  GGenerateVersionInfo::Version();
		string branch =  GGenerateVersionInfo::Branch();

        if ( g_string()->Contains(path, "@"))
        {
            path += "@";
        }
        if (hasArgument(argc, argv, "-help"))
        {
            helpMenu();
        }

        bool generate_files = true;

        if (hasArgument( argc, argv, "-repo-name"))
        {
            cout << repo_name << endl;
            generate_files = false;
        }
        if (hasArgument( argc, argv, "-branch"))
        {
            cout << branch << endl;
            generate_files = false;
        }

        if (hasArgument( argc, argv, "-version"))
        {
            cout << version << endl;
            generate_files = false;
        }

		if (hasArgument( argc, argv, "-rev"))
        {
            cout << rev;
            generate_files = false;
        }

        if (hasArgument( argc, argv, "-generate-files") || generate_files == true)
        {
            string outdir;
            #ifdef _WIN32
            generateVersionFile("include", "Version.h", rev, version, branch, configuration, platform, outdir);
			generate_rc_file(outdir, rc_filename, company, description, dllname, copyright, productname);
            #endif
            GGenerateVersionInfo::GenerateClass("GVersion", exename, compileflags_file, ".");
		}
    }

    catch (std::exception &e)
    {
        CERR << "Exception caught: " << e.what() << endl;
    }
    catch (...)
    {
        CERR << "Unknown exception caught" << endl;
    }


    return 0;
}






/*
void
generate_rc_file(string outdir, const string rc_filename, const string company, const string desc, const string filename, const string copyright, const string prod_name)
{
    FILE *fp = 0;
    
    #ifdef _WIN32
    fopen_s(&fp, rc_filename.c_str(), "w"); /// @todo check on return value 
    #else
    fp = fopen(rc_filename.c_str(), "w");
    #endif

    if (fp == 0)
    {
        throw(std::runtime_error ( "could not open file" + rc_filename ) );
    }

    fprintf(fp, "\n\n//DO NOT EDIT !!. KNS_TS auto generated resource file. Any changes will be overwritten during next compilation\n\n");

    if (outdir == "")
    {
        fprintf(fp, "#include \"resource.h\"\n");
        fprintf(fp, "#include \"Version.h\"\n");
    }
    else
    {
        fprintf(fp, "#include <%s\\resource.h>\n", outdir.c_str());
        fprintf(fp, "#include <%s\\Version.h>\n", outdir.c_str());
    }

    fprintf(fp, "#define  APSTUDIO_READONLY_SYMBOLS\n");
    fprintf(fp, "#include \"winres.h\"\n");
    fprintf(fp, "#undef APSTUDIO_READONLY_SYMBOLS\n");
    fprintf(fp, "\n\n");
    fprintf(fp, "#ifdef APSTUDIO_INVOKED\n");

    fprintf(fp, "1 TEXTINCLUDE\n");
    fprintf(fp, "BEGIN\n");
    fprintf(fp, "\t\"resource.h\\0\"\n");
    fprintf(fp, "END\n\n");

    fprintf(fp, "2 TEXTINCLUDE\n");
    fprintf(fp, "BEGIN\n");
    fprintf(fp, "\t\"#include \"\"winres.h\"\"\\r\\n\"\n");
    fprintf(fp, "END\n\n");

    fprintf(fp, "3 TEXTINCLUDE\n");
    fprintf(fp, "BEGIN\n");
    fprintf(fp, "\t\"\\r\\n  \\0\"");
    fprintf(fp, "END\n\n");
    fprintf(fp, "#endif\t//APSTUDIO_INVOKED\n\n");

    fprintf(fp, "VS_VERSION_INFO VERSIONINFO\n\n");
    fprintf(fp, "#ifdef   GIT_FILEVERSION\n");
    fprintf(fp, "FILEVERSION  GIT_FILEVERSION\n");
    fprintf(fp, "#else\n");
    fprintf(fp, "FILEVERSION 999, 999, 999, 999\n");
    fprintf(fp, "#endif\n\n");

    fprintf(fp, "#ifdef GIT_PRODUCTVERSION\n");
    fprintf(fp, "PRODUCTVERSION GIT_PRODUCTVERSION\n");

    fprintf(fp, "#else\n");
    fprintf(fp, "PRODUCTVERSION 7, 8, 9, 10\n");
    fprintf(fp, "#endif\n\n");

    fprintf(fp, "FILEFLAGSMASK 0x3fL\n");

    fprintf(fp, "#ifdef _DEBUG\n");
    fprintf(fp, "FILEFLAGS 0x1L\n");

    fprintf(fp, "#else\n");

    fprintf(fp, "FILEFLAGS 0x0L\n");
    fprintf(fp, "#endif\n\n");

    fprintf(fp, "FILEOS 0x40004L\n");
    fprintf(fp, "FILETYPE 0x2L\n");

    fprintf(fp, "FILESUBTYPE 0x0L\n");

    /////////
    fprintf(fp, "BEGIN\n");
    fprintf(fp, "\tBLOCK \"StringFileInfo\"\n");
    fprintf(fp, "\tBEGIN\n");
    fprintf(fp, "\t\tBLOCK \"040904b0\"\n");
    fprintf(fp, "\t\tBEGIN\n");
    fprintf(fp, "\t\t\tVALUE \"CompanyName\", \"%s\"\n", company.c_str());
    fprintf(fp, "\t\t\tVALUE \"FileDescription\", \"%s\"\n", desc.c_str());
    fprintf(fp, "\t\t\tVALUE \"InternalName\", \"%s\"\n", filename.c_str());
    fprintf(fp, "\t\t\tVALUE \"LegalCopyright\", \"%s\"\n", copyright.c_str());
    fprintf(fp, "\t\t\tVALUE \"OriginalFilename\", \"%s\"\n", filename.c_str());
    fprintf(fp, "\t\t\tVALUE \"ProductName\", \"%s\"\n", prod_name.c_str());
    fprintf(fp, "#ifdef GIT_PRODUCTVERSION_STRING\n");
    fprintf(fp, "\t\t\tVALUE  \"ProductVersion\", GIT_PRODUCTVERSION_STRING\n");
    fprintf(fp, "#else\n");
    fprintf(fp, "\t\t\tVALUE \"ProductVersion\", \" - 1. - 1. - 1\"\n");
    fprintf(fp, "#endif\n");
    fprintf(fp, "\t\tEND\n");
    fprintf(fp, "\tEND\n");

    fprintf(fp, "BLOCK \"VarFileInfo\"\n");
    fprintf(fp, "BEGIN\n");
    fprintf(fp, "VALUE \"Translation\", 0x409, 1200\n");

    fprintf(fp, "END\n");
    fprintf(fp, "END\n\n\n");
    fprintf(fp, "////#endif  // // Norwegian, Bokm�l (Norway) resources\n\n");
    fprintf(fp, "/////#ifndef APSTUDIO_INVOKED\n\n");
    fprintf(fp, "/////#endif //not APSTUDIO_INVOKED\n\n");
    fclose(fp);
}
*/



void
scanParameter(int argc, const char **argv, string name, string & par)
{
    for (int i = 0; i < argc ; i++)
    {
        if ( string(argv[i]) == name)
        {
            if (i + 1 >= argc )
            {
                throw(std::runtime_error((string("missing parameter to argument ") + name).c_str()));
            }
            else
            {
                par = string(argv[i + 1] ) ;
                g_string()->Trim(par);
            }
        }
    }
}


bool
is_valid_tag(const string tag)
{
    for ( size_t i = 0; i < g_valid_tags.size(); i++)
    {
        if (g_valid_tags[i] == tag)
        {
            return true;
        }
    }
    return false;
}


bool
is_valid_argument(int argc, const char ** argv, string &invalid)
{
    bool bret = true;

    for (int i = 0; i < argc; i++)
    {
        if (argv[i][0] == '-' && (is_valid_tag(argv[i]) == false))
        {
            invalid = string(argv[i]);
            bret = false;
        }
    }
    return bret;
}


void
print_tags()
{
    for ( size_t i = 0; i < g_valid_tags.size(); i++)
    {
        cout << g_valid_tags[i] << endl;
    }
}





void autoClause(FILE *fp)
{
    fprintf(fp, "\n\n/***** Auto generated file: DO NOT EDIT !!!!!! *****/\n");
  //  struct std::tm timeinfo;
    
 //   std::time_t now = std::time(NULL); 
 //   localtime_s(&timeinfo, &now);
 //   char tmp[1024];
  //  string tmp =  GTime::TimeStamp( "%a %d %B-%Y %H:%M:%S");   
//    std::strftime(tmp, 1024 - 1, "[%a %d %B-%Y %H:%M:%S]", &timeinfo);
  //  fprintf(fp, "/*** Generated at: %s  ***/\n\n", tmp.c_str()  );

}



bool
hasArgument(int argc, const char **argv, const string argument)
{
    for (int i = 0; i < argc; i++)
    {
        if (string(argv[i]) == argument)
        {
            return true;
        }
    }
    return false;
}



string
generateVersionString(const vector<int> num)
{
    char tmp[256];
    int pos = 0;
    for (unsigned int i = 0; i < num.size(); i++)
    {
        int n = 0;
        if (i < (num.size() - 1))
        {
            // snprintf()
            n = snprintf(&tmp[pos], sizeof(tmp), "%d,", num[i]);
        }
        else
        {
            n = snprintf(&tmp[pos], sizeof(tmp), "%d", num[i]);
        }
        pos += n;
    }

    return string(tmp);
}



void
generateVersionFile(const string directory, const string fname, const int rev, const string version, const string branch, const string configuration, const string platform, string &outdir)
{
   COUT << "version = " << version << endl;
   COUT << "branch = " << branch << endl;


#ifdef DEBUG
    CERR << "The revision is " << rev << endl;
#endif

    vector<int> version_numbers(4, 0);
    
    
    FILE *fp_h = 0;
    string filepath = directory + "\\" + fname;
    outdir = directory;

#ifdef _WIN32
    fopen_s(&fp_h, filepath.c_str(), "w");  /// @todo check on return value
#else
    fp_h =   fopen( filepath.c_str(), "w");  
#endif

    if (fp_h == 0)
    {

#ifdef _WIN32
        fopen_s(&fp_h, fname.c_str(), "w"); /// @todo check on return value 
#else
        fp_h = fopen(fname.c_str(), "w"); /// @todo check on return value 
#endif        

    
        if (fp_h == 0)
        {
            cout << "Error opening file " << fname << ", or " << filepath << "aborting" << endl;
            exit(-1);
        }
        else
        {
            outdir = "";
        }
    }

    vector<string> num = g_regexp()->ScanNumber(version, "1,4");

    for (unsigned int i = 0; i < num.size(); i++)
    {
        if (i < 4)
        {
            version_numbers[i] = g_numbers()->ToInteger<int>(num[i]);
        }
    }

    fprintf(fp_h, "#pragma once\n\n");
    autoClause(fp_h);
    fprintf(fp_h, "#define GIT_PRODUCTVERSION %s\n", generateVersionString(version_numbers).c_str()    );

    if (branch != "tags")
    {
        fprintf(fp_h, "#define GIT_PRODUCTVERSION_STRING \"%s-rev%d, %s_%s\"\n", version.c_str(), rev, platform.c_str(), configuration.c_str());
    }
    else
    {
        fprintf(fp_h, "#define GIT_PRODUCTVERSION_STRING \"%s, %s_%s\"\n", version.c_str(), platform.c_str(), configuration.c_str());
    }

//    version_numbers[version_numbers.size() - 1] = rev;
    fprintf(fp_h, "#define GIT_FILEVERSION %s\n", generateVersionString(version_numbers).c_str()   );
    fprintf(fp_h, "#define GIT_FILEVERSION_STRING \"%s\"\n", generateVersionString(version_numbers).c_str() );
    fclose(fp_h);
}



void
printTokens(vector<string>& tokens)
{
    for (unsigned int i = 0; i < tokens.size(); i++)
    {
        CERR << "tokens[" << i << "]  =  " << tokens[i] << endl;
    }
}




void
helpMenu()
{
    cout << "Usage:  svninfo.exe [filepath]  option" << endl;
    cout << "NB ! A file path must be specified before any of the switches, Use \".\" (dot) to refere to the current directory" << endl;
    cout << "**** Command line switches *****" << endl;
    cout << "1)\t--generate-files  Generate the files/classes  SvnData.h and Version.h containing SVN information to be used by other applications (this i done as default if no parameters are given)" << endl;
    cout << "2)\t--repo-name\tPrints out the name of the repository as given in SVN" << endl;
    cout << "3)\t--branch\tPrints out wether you are in the trunk, tags or release branch" << endl;
    cout << "4)\t--version\tPrints out the SW version of this repository. Please note that if you are in the \"trunk\" branch then the version will also be \"trunk\"" << endl;
    cout << "5)\t--rev\tPrints the svn revision number of this subversion repository" << endl;
    cout << "6)\t--lc-rev\tPrints out the last changed revision of the file or directory pointed to by [filepath]" << endl;
    cout << "7)\t--info\tPrint out the SVN info as returned by the \"svn info\" command" << endl;
    cout << "8)\t--help\tPrints out this help menu" << endl;
}


