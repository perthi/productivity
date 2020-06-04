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

#include "GSystem.h"
#include <sys/stat.h>
#include "GFileIOHandler.h"
#include "GTokenizer.h"
#include "GString.h"
#include "GText.h"
#include "GCommon.h"
#include "GLocation.h"

#ifndef _WIN32
#include <dirent.h>
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

#ifdef _WIN32
#include <direct.h>
#define GetCurrentDir _getcwd
#include <Shlwapi.h>
#include <windows.h>
#include <shellapi.h>
#pragma comment(lib, "shlwapi.lib")
#endif

#include <stdio.h>
#include <memory>
#include <sys/stat.h>
#include <cstdlib>

#ifndef _WIN32
#include <libgen.h>
#endif // !_WIN32




GSystem * g_system()
{
    static GSystem *instance = new GSystem();
    return instance;
}


string
GSystem::getenv(const string  var  )
{
#ifdef _WIN32
    size_t sz = 0;
    char  *tmp = nullptr;
    _dupenv_s(&tmp, &sz,  (char*)var.c_str());
#else
    char *tmp = std::getenv((char*)var.c_str());
#endif // _WIN32
    string ret = "";

    if( tmp != nullptr )
    {
        ret = string(tmp);
#ifdef _WIN32
        free(tmp);
#endif
        return ret;
    }
    else
    {
        return var + ": no such environment variable found";
    }

}



/** mkdir =   Make Directory (that is a folder in Windows terms), Unix/bash style
*   @param    dirname The directory to create
*   @return   true if the directory exists, or if it doesnt allready exists, but was
*   successfully created
*   @return false if the directory doesnt exist, and it also cannot be created (for instance if
*   the program is running under a user that doesnt have write access to the currnt directory)
*   @throw Exception if the directory doesnt exist and it cannot be created.*/
bool
GSystem::mkdir(const string dirname)
{
//    FORCE_DEBUG("creating directory %s", dirname.c_str()  );

#ifdef _WIN32
    int status = ::_mkdir(dirname.c_str() );
#else
    int status = ::mkdir(dirname.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH );
#endif // _WIN32

    if( status == 0)
    {
        return true;
    }
    else
    {
        string errmsg = g_file()->Errno2String(errno, dirname, "");
       g_common()->HandleError(  GText(  "The directory \"%s\" could not be created (%s),.. please check that \
                    you have write + exce permissions to the directory", \
                    dirname.c_str( ), errmsg.c_str() ).str(), GLOCATION, THROW_EXCEPTION  );  
        return false;
    }
}

/**@{*/ 
/** mkdir =   Make Directory (that is a folder in Windows terms), Unix/bash style
*   @param    dirname The directory to create
*   @param    l The location  wher this function was called from
*   @param    opt Acess settings (typically 755)
*   @param    overwrite wheter or not tor overwriet/replace exisiting directory of it allready exists 
*   @return   true if the directory exists, or if it doesnt allready exists, but was
*   successfully created
*   @return false if the directory doesnt exist, and it also cannot be created (for instance if
*   the program is running under a user that doesnt have write access to the currnt directory)
*   @throw Exception if the directory doesnt exist and it cannot be created.*/
bool
#ifndef _WIN32
GSystem::mkdir(const string dirname, GLocation l, const int  opt, bool overwrite)
#else
GSystem::mkdir(const string dirname, GLocation l, bool overwrite)
#endif // !_WIN32
{
//    FORCE_DEBUG("creating directory %s", dirname.c_str()  );

#ifdef _WIN32
    int status = ::_mkdir(dirname.c_str() );
#else
   // int status = ::mkdir(dirname.c_str(), opt);
   // COUT << "opt = " <<  opt << "     !!!!!!!!!!"  << endl;
     int status = ::mkdir(dirname.c_str(),  opt );
#endif // _WIN32

    ///void  HandleError(const string message, const GLocation l, const bool disable_error );
    
    switch (status)
    {
    /// abort if any of the below non recoverable erros are encountered
    case EACCES:       // Search permission denied
    case ELOOP:        // Loop in symbolic link
    case EMLINK:       // Link count too high
    case ENAMETOOLONG: // Filename too long
    case ENOENT:       // Not a path / emtpy string
    case ENOSPC:       // No space left on device
    case ENOTDIR:      // Path is not (or cannot be) a directory
    case EROFS:        // The parent directory is read only
        GCommon().HandleError(GText("non recoverabele erro encountered creating directory %s ( errno %d; %s )",
                                         dirname.c_str(),
                                         errno,
                                         strerror(errno))
                                       .str(),
                                   l);
        return false;
        break;
    case EEXIST:
        if (overwrite == true)
        {
            return true;
        }
        else
        {
            GCommon().HandleError(GText("directory %s allready exists and you are not allowed to overwrite it ( errno %d; %s)",
                                             dirname.c_str(),
                                             errno,
                                             strerror(errno))
                                           .str(),
                                       l);
            return false;
        }
        break;
    }
    return true;
}
/**@}*/

bool
GSystem::Exists(const string filepath)
{
    struct  stat sb;
    int ret = stat( filepath.c_str(), &sb );
    return ret == 0 ? true :false;
}



string
GSystem::GetDirectory(const string filepath)
{
	const char* filepathCp = filepath.c_str();
	const char* charPtr = filepathCp + filepath.length() - 1;
	if ((*charPtr == '/') || (*charPtr == '\\'))
	{
		--charPtr;
	}
	while ((charPtr > filepathCp) && (*charPtr != '/') && (*charPtr != '\\'))
	{
		--charPtr;
	}
	if (charPtr > filepathCp)
	{
		size_t returnLen = charPtr - filepathCp;
		return filepath.substr(0, returnLen);
	}
	return "";
}


string
GSystem::GetProcessID()
{
    std::stringstream buffer;
#ifdef _WIN32
	buffer << GetCurrentProcessId();
#else
    buffer << getpid();
#endif
    return buffer.str();
}


/** @return Returns the name of rootdir */

#ifdef _WIN32
char*
GSystem::GetHomeDir()
{
    static char* buf;
    buf = GetExePath();
    PathRemoveFileSpecA(buf);
    string path = string(buf);
    static string dir;
    static string fout;
    g_tokenizer()->StripPath(path, dir, fout);
    static string homedir = fout + "\\..\\..\\";
    return  (char *)homedir.c_str(); 
   // inline  void API StripPath(const string fin, string &dir, string &fout, const bool keep_trailing_slash = KEEP_TRAILING_SEPARATOR ); 	
    SPRINTF(buf, 1024, "%s\\..\\..\\", buf); // CRAP PTH
	return buf;
}
#endif




/** Executes a system command.
*   @param cmd The command to execute
*   @return  The output of the command, i.e what would have been written on the command line if the
*   the command was executed in a console. */
string
GSystem::exec(const char* cmd)
{
#ifdef _WIN32
    std::shared_ptr<FILE> pipe(_popen(cmd, "r"), _pclose);
#else
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
#endif

    if (!pipe) 
    {
        return "ERROR";
    }
  
    char buffer[256];
    std::string result = "";

    while (!feof(pipe.get()))
    {
    //    FORCE_DEBUG("TP 0 ");
        if (fgets(buffer, 256, pipe.get()) != NULL)
        {
            result += buffer;
        }
    }
    return result;
}




#ifdef _WIN32
/** @return all the command line arguments as a single string*/
string
GSystem::GetCommandLineAll()
{
    return string(GetCommandLineA());
}
#endif


/** @return all the command line arguments, except the firs one (which is the name of the executable), as a single string*/
#ifdef _WIN32
string
GSystem::GetCommandLineArguments()
{
    string ret;

#ifdef _WIN32
    int argc;
    //  string ret;
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

    LPWSTR cmdline = GetCommandLineW();
    LPWSTR* argv = ::CommandLineToArgvW(cmdline, &argc);

    for (int n = 1; n < argc; n++)
    {
        ret += n > 1 ? " \"" : "\"";
        ret += converter.to_bytes(argv[n]);
        ret += "\"";
    }

    return ret;
#else
    int pid = getpid();
    char fname[PATH_MAX];
    char cmdline[ARG_MAX];
    snprintf(fname, sizeof fname, "/proc/%d/cmdline", pid);
    FILE *fp = fopen(fname, "r");
    fgets(cmdline, sizeof cmdline, fp);
    vector<string> tokens = g_tokenizer()->Tokenize(cmdline);

    for (size_t i = 1; i < tokens.size(); i++)
    {
        ret += tokens[i];
    }

    return ret;
#endif    
}
#endif


/** @return the current path, i.e the path of the current location */
char *
GSystem::GetExePath()
{
    const static size_t buflen = 2048;

#ifdef _WIN32 
    static char dir[buflen];
    GetModuleFileNameA(NULL, dir, buflen);
    return dir;
#else
    static char buff[buflen] = { 0 };

    for (size_t i = 0; i < buflen; i++)
    {
        buff[i] = 0;
    }

    if (readlink("/proc/self/exe", buff, buflen) > 0)
    {
        //just shutting up the compiler
    }

    return buff;
#endif

}


/**   @return The path relative to the current location for the current excutable excluding  the name of the exec file  */
#ifdef _WIN32
char *
GSystem::GetExeDir()
{
    static char *buf;
    buf = GetExePath();

    PathRemoveFileSpecA(buf);
    sprintf_s(buf, 1024, "%s\\", buf); // CRAP PTH
    return buf;
}
#else


char *
GSystem::GetExeDir()
{
    #ifdef _WIN32
     static char *buf;
    LPSTR buf;
    buf = GetExePath();
    PathRemoveFileSpecA(buf);
    return buf;
    #else

    static  char buf[ PATH_MAX ] = {0};
    readlink("/proc/self/exe", buf, PATH_MAX);
    dirname(buf);
    return buf;
    #endif
}

#endif


/** @return the name of the current executable */
char *
GSystem::GetExeName()
{
    static string dir;
    static string name;
    string path = string(GetExePath());
    g_tokenizer()->StripPath(path, dir, name);
    return (char *)name.c_str();
    return 0;
}


string
GSystem::GetHostName()
{
#ifdef _WIN32
    static char hostname[1024];
    static unsigned long w = 1024;
    GetComputerNameA(hostname, &w);
    return string(hostname);
#else
    char hostname[1024];
    gethostname(hostname, 1024);
    return string(hostname);
#endif
}


/** pwd = Print Working Directory after the simlar Unix/Linux command
*   @param print Wether or not to print the name of the current directory to stdout
*   @return a string containing the absolute path to the current directory */
string
GSystem::pwd(const bool print)
{
    char current_path[FILENAME_MAX];

    if (!GetCurrentDir(current_path, sizeof(current_path)))
    {
        return "ERROR !!!! could not get directory";
    }

  //  current_path[sizeof(current_path) - 1] = '\0'; /* not really required */

    if (print == true)
    {
        COUT << ": " << current_path << "\n";
    }

    return string(current_path);
}



/** ls  = list content of current directory (Unix/bash)
*  @param dir  The directory to list (default is current directory)
*  @return A vector of files matching the search pattern (default is that all files in "dir" are listed ). */
vector<string>
GSystem::ls(const string dir)
{
    vector<string> tmp;
#ifndef _WIN32
    DIR *dp;

    struct dirent *dirp;
    if ((dp = opendir(dir.c_str())) == 0)
    {
        return tmp;
    }
    while ((dirp = readdir(dp)) != 0)
    {
        tmp.push_back(string(dirp->d_name));
    }
    closedir(dp);
#endif
    return tmp;
}



bool
GSystem::mkfile(const string filepath)
{
    string dir, file;
    g_tokenizer()->StripPath(filepath, dir, file, false);

    if (dir != "")
    {
        mkdir(dir);
    }
    
    FILE *fp = g_file()->OpenFile( filepath, "r", GLOCATION );
    if (fp !=  nullptr)
    {
        g_common()->HandleError( GText("File \"%s\" allready exists, will not be recreated", filepath.c_str()  ).str(), GLOCATION, DISABLE_EXCEPTION ) ;
        fclose(fp);
        return false;
    }
    else
    {
        fp = g_file()->OpenFile(filepath, "w", GLOCATION);
        if (fp !=  nullptr)
        {
            fclose(fp);
            return true;
        }
        else
        {
            g_common()->HandleError(  GText(  "Could not create file \"%s\" Please check your write permissions for this directory", \
                                               filepath.c_str()   ).str(), GLOCATION, THROW_EXCEPTION  );      
        }
    }
   

    return true;
}


void
GSystem::cp(string source, string dest)
{
    mkfile(dest);
    vector<string> content = g_file()->ReadAll(source);
    for (uint16_t i = 0; i < content.size(); i++)
    {
        g_file()->Append(dest, content[i].c_str());
        if (i < content.size() - 1)
        {
            g_file()->Append(dest, "\n");
        }
    }
}


bool
GSystem::rm(const string filename)
{
    return g_file()->Delete(filename);
    //return std::remove(filename.c_str());
}


void
GSystem::mv(const string src, const string dest)
{
    cp(src, dest);
    rm(src);
}
