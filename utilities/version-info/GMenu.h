// -*- mode: c++ -*-

#pragma once

/***************************************************************************
***          Author: Per Thomas Hille <pth@embc.no>                     ****
****************************************************************************/

/****************************************************************************
*** Copyright(C) 2018  Per Thomas Hille, pth@embc.no                      ***
*** This file is part of logmaster.logmaster is free software : you can   ***
*** redistribute it and / or modify it under the terms of the Lesser GNU  ***
*** General Public License(LGPL) V3 or later.See.cpp file for details     ***
*****************************************************************************/

#include <string>

using std::string;

#include <version-info/GGenerateVersionInfo.h>
#include <version-info/GVersion.h>
#include <utilities/GDefinitions.h>
#include <utilities/GUtilities.h>


#include <cmdline/GArgument.h>
#include <cmdline/GCommandLineArgument.h>
#include <cmdline/GLogApplication.h>
#include <cmdline/GCmdScan.h>

#define G GVersion

#include <memory>

class GMenu
{
public:
    static inline  GMenu API *  Instance();
    static inline string HelpMenu();
    inline void API  ScanArguments(int argc, const char **argv );
    std::shared_ptr<GArgument>  inline API  GetArgument() { return fArgument; };

private:    
    inline API GMenu ();
    static inline bool CallBack(  const string  cmd, const string args_s, const vector<string>  sub, const vector<string>  par ); 
    std::shared_ptr<GArgument> fArgument = nullptr;
};



void  
GMenu::ScanArguments(int argc, const char **argv)
{
	auto g =  std::make_shared<GLogApplication>();
	g->AddArgument(fArgument);

    bool scan_mode = g_cmdscan()->GetIgnoreStrayArgument();

    g_cmdscan()->SetIgnoreStrayArgument(true);
    g->ScanArguments(argc, argv);
    g_cmdscan()->SetIgnoreStrayArgument( scan_mode);

}



inline 
GMenu::GMenu() : fArgument(nullptr)
{
	fArgument = std::make_shared<GCommandLineArgument<void> > ("-version",
		"-version [\"empty\", --tag, --branch, --gitinfo, --compile-flags, --link-flag]",
		HelpMenu(),
		nullptr,  fgkOPTIONAL,  CallBack);
}


inline GMenu *
GMenu::Instance()
{
	static GMenu *instance = new GMenu();
	return instance;
}


inline bool 
GMenu::CallBack(const string  /*cmd*/, const string /*args_s*/,  const vector<string>  sub, const vector<string>  /*par*/ )
{
 	string msg = "";

    string option = sub.size() == 1 ? sub[0] : "";

    if( option == "--tag" )
    {
        cout << "version(tag)=\t" << G::GetGitTag() << endl; 
    }
    else
    if(  option == "--branch" )
    {
        cout << "branch=\t" << G::GetGitBranch() << endl; 
    }
    else
    if(  option == "--gitinfo" )
    {
        cout << "git-info=\t" << G::GetGitInfo()<< endl; 
    }
    else
    if( option == "--compile-flags" )
    {
        cout << "compile flags=\t" << G::GetCompileFlags() << endl; 
    }
    else
    if( option == "--link-flags" )
    {
        cout << "link-flags=\t" << G::GetLinkFlags() << endl; 
    }
    else
    {
        cout << HelpMenu();
    }
     exit(0);   
    return true;
}



inline string
 GMenu::HelpMenu()
 {  
     std::stringstream buffer;
     buffer << "\tusage: -version  [subcommand]" << endl << endl;
     buffer << "\tsubcommand can be, either empty or one of the following:" << endl;
     buffer <<  g_utilities()->TabAlign("\t\t--branch") << "The GIT branch this code was compiled from" << endl;
     buffer <<  g_utilities()->TabAlign("\t\t--tag")           <<  "The GIT  tag / version. If the version is not a tag then the closest" << endl; 
     buffer <<  g_utilities()->TabAlign( "\t\t")                << "tag is indicated, please refer to the GIT documenation for further details" << endl;
     buffer <<  g_utilities()->TabAlign("\t\t--gitinfo")       <<  "The output of the command \"git-info at the time the code was compiled" << endl;
     buffer <<  g_utilities()->TabAlign("\t\t--compile-flags\t") << "The compilation flags this executabel was compiled with" << endl;
     buffer <<  g_utilities()->TabAlign( "\t\t--link-flags")    << "The compilation flags this executabel was compiled with" << endl;
     return buffer.str();
}	
