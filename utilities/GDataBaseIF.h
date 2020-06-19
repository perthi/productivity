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

	protected:
		#ifdef HAS_LOGGING
		virtual void HandleError( const GLocation l,   eMSGLEVEL lvl, const char * fmt, ...);
		#else
		virtual void HandleError( const GLocation l, const char * fmt, ...);
		#endif
		
		string SQLType2String( const int sql_type  ) const;	

		sqlite3       *fDataBase  =  nullptr; 
        sqlite3_stmt  *fStmt     =   nullptr;  // SQLite statmement 

	 private:
	 	GDataBaseIF( const GDataBaseIF &rhs );
		GDataBaseIF &	operator = 	( const GDataBaseIF &rhs); 
		std::shared_ptr<LMessageGenerator> fMessageGenerator = nullptr;
};

#endif
