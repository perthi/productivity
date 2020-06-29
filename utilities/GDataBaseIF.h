// -*- mode: c++ -*-
#ifndef GDATABASEIF_H
#define GDATABASEIF_H
/**************************************************************************
 * This file is property of and copyright by Embedded Cosnulting  2020    *
 *                                                                        *
 * Author: Per Thomas Hille <pth@embc.no>                                 *
 * Contributors are mentioned in the code where appropriate.              *
 * Please report bugs to pth@embc.no                                      *
 **************************************************************************/


#include  "GLocation.h"

#ifdef HAS_LOGGING
#include  <logging/LEnums.h>
using namespace LOGMASTER;
#endif

#include  <memory>
#include  <string>

using std::string;

struct  sqlite3;
struct  sqlite3_stmt;


namespace LOGMASTER
{
	class  LMessageGenerator;
}



class  GDataBaseIF
{
	public:
	GDataBaseIF();
	virtual ~GDataBaseIF();
    virtual bool   CreateTables() = 0;
	void     API   CloseDatabase();
	bool     API   OpenDatabase( const char *db_path );
	int      API   ReadInteger (  sqlite3_stmt *stmt,  const int idx, const string colname, const int sql_type, const GLocation l);
	double   API   ReadFloat   (  sqlite3_stmt *stmt, const int idx, const string colname, const int sql_type, const GLocation l);
	string   API   ReadText    (  sqlite3_stmt *stmt, const int idx, const string colname, const int sql_type, const GLocation l);

	bool API DeleteEntries( const string tablename );

	protected:
		#ifdef HAS_LOGGING
		virtual void API HandleError( const GLocation l,   eMSGLEVEL lvl,  const bool throw_ex,   const char * fmt, ...);
		#else
		virtual void API HandleError( const GLocation l,  const bool throw_ex,   const char * fmt, ...);
		#endif
		
		string  API SQLType2String( const int sql_type  ) const;	
		string  API LimitString( const int cnt );
		sqlite3       *fDataBase  =  nullptr; 
        sqlite3_stmt  *fStmt     =   nullptr;  // SQLite statmement 

	 private:
	 	GDataBaseIF( const GDataBaseIF &rhs );
		GDataBaseIF &	operator = 	( const GDataBaseIF &rhs); 
		std::shared_ptr< LOGMASTER::LMessageGenerator> fMessageGenerator = nullptr;
};

#endif
