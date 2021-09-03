// -*- mode: c++ -*-
#ifndef GVERSION_H
#define GVERSION_H

/***************************************************************************
***          Author: Per Thomas Hille <pth@embc.no>                     ****
****************************************************************************/

/****************************************************************************
*** Copyright(C) 2018  Per Thomas Hille, pth@embc.no                      ***
*** This file is part of logmaster.logmaster is free software : you can   ***
*** redistribute it and / or modify it under the terms of the Lesser GNU  ***
*** General Public License(LGPL) V3 or later.See.cpp file for details     ***
*****************************************************************************/

/**
 * @class GVersion
 * Class to hold version information for a generated executable. This comprises relevant GIT version information
 * such as branch and tag information etc. In addition this class stores compile time information, i.e which 
 * compilation flags ewas used to compile this exe file as well as which libraries was linked in.
*/
#include <string>

using std::string;

class  GVersion
{
	public:
		GVersion();
		virtual ~GVersion();

		static inline string GetGitBranch() {  return fGitBranch; };
		static inline string GetGitTag()  {  return fGitTag; };
		static inline string GetGitInfo() { return fGitInfo; };
		static inline string GetOriginaExeName() { return fOriginalExeName; };
		static inline string GetCompileFlags() { return fCompileFlags; };
		static inline string GetLinkFlags() {return fLinkFlags; };
		static inline void  SetExeName( const string name) { fOriginalExeName  = name; };

    //    private:
		static const string fGitBranch;	       		//!< The GIT branch that this exe was generated from
		static const string fGitTag;          		//!< The GIT tag of this exe file. If its not a tags, it shows the neares tag (see GIT documentation )
		static const string fGitInfo;				//1< The output of the command "git-info"  at the time this exe was compiled	
		static  string fOriginalExeName;   			//!< The original file name of this exe file
		static const string fCompileFlags;			//!< The compile flags that was used when compiling this exe
		static const string fLinkFlags;				//!< The link flags that was used when compiling this exe

};

#endif
