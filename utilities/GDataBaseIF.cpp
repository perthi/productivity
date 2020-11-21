// -*- mode: c++ -*-
/**************************************************************************
 * This file is property of and copyright by Embedded Cosnulting  2020    *
 *                                                                        *
 * Author: Per Thomas Hille <pth@embc.no>                                 *
 * Contributors are mentioned in the code where appropriate.              *
 * Please report bugs to pth@embc.no                                      *
 **************************************************************************/
#include "GDataBaseIF.h"

#include  <stdarg.h>


#ifdef HAS_LOGGING
#include  <logging/LDatabase.h>
#include  <logging/LMessage.h>
#include  <logging/LLogApi.h>
#include  <logging/LMessageGenerator.h>
#include  <logging/LPublisher.h>
#include  <logging/LEnums.h>
#include <exception/GException.h>
using namespace LOGMASTER;
#else
#include <stdio.h>
#include <exception>
#endif


#include "GDefinitions.h"
#include "GCommon.h"
#include "sqlite/sqlite3.h"
#include  <string.h>
#include  <sstream>
#include <chrono>
#include <thread>



GDataBaseIF::GDataBaseIF()
{
    #ifdef HAS_LOGGING
	fMessageGenerator =  std::make_shared< LMessageGenerator >();
    #endif
}


GDataBaseIF::~GDataBaseIF()
{


}



string
GDataBaseIF::SQLType2String(const int sql_type) const
{
    switch (sql_type)
    {
    case SQLITE_INTEGER:
        return "SQLITE_INTEGER";
        break;
    case SQLITE_FLOAT:
        return "SQLITE_FLOAT";
        break;
    case SQLITE_BLOB:
        return "SQLITE_BLOB (binary large object)";
        break;
    case SQLITE_TEXT:
        return "SQLITE_TEXT";
        break;
    case SQLITE_NULL:
        return "SQLITE_NULL";
        break;
    default:
        std::stringstream buffer;
        buffer << "Unknown SQL type " << sql_type;
        return buffer.str();
        break;
    }

    return "blahh";

}






/**  Opens the database 
 *   @param[in]   db_path The fulle path to hte database
 *   @return      TRUE if the databse was opened sucessfullt, false othervise */
bool 
GDataBaseIF::OpenDatabase(const char *db_path)
{
    #ifdef HAS_LOGGING
          HandleError(GLOCATION, eMSGLEVEL::LOG_INFO, DISABLE_EXCEPTION, "opening database \"%s\"",    db_path  );
    #else
        HandleError(GLOCATION, DISABLE_EXCEPTION, "opening database \"%s\"",    db_path  );
    #endif 

    int rc;
    rc = sqlite3_open(db_path, &fDataBase);

    if (rc)
    {
        #ifdef HAS_LOGGING
            HandleError(GLOCATION, eMSGLEVEL::LOG_FATAL,  THROW_EXCEPTION, "Failed to open database \"%s\"",    db_path  );
        #else
            HandleError(GLOCATION,  THROW_EXCEPTION,  "Failed to open database \"%s\"",   db_path  );
        #endif
        return false;
    }
    else
    {
        return CreateTables();
    }
}


void 
GDataBaseIF::CloseDatabase()
{
    int cnt = 0;
    while (sqlite3_close(fDataBase) == SQLITE_BUSY)
    {
#ifdef HAS_LOGGING
        HandleError(GLOCATION, eMSGLEVEL::LOG_ERROR,  DISABLE_EXCEPTION, "database is busy, cnt =  %d", cnt);
#else
        HandleError(GLOCATION,  DISABLE_EXCEPTION, "database is busy, cnt =  %d", cnt);
#endif

        cnt++;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        if (cnt == 10)
        {
#ifdef HAS_LOGGING
            HandleError( GLOCATION, eMSGLEVEL::LOG_FATAL, DISABLE_EXCEPTION, "failed to close database after %d attempts, bailing out !!!", cnt);
#else
            HandleError( GLOCATION, DISABLE_EXCEPTION, "failed to close database after %d attempts, bailing out !!!", cnt);
#endif
            break;
        }
    }
}

string
GDataBaseIF::ReadText(sqlite3_stmt *stmt, const int idx, const string colname, const int sql_type, const GLocation l)
{
    string tmp = "";
    
    if (strcasecmp(sqlite3_column_name(stmt, idx), colname.c_str() ) == 0)
    {
        if (sqlite3_column_type(stmt, idx) == SQLITE_TEXT)
        {
            tmp = std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, idx)));
        }
        else
        {
#ifdef HAS_LOGGING
            HandleError(l, eMSGLEVEL::LOG_ERROR, THROW_EXCEPTION, "Incorrect Type (%d = %s) for \"%s\", expected SQLITE_TEXT", sql_type, SQLType2String(sql_type).c_str(), colname.c_str());
#else
            HandleError(l, THROW_EXCEPTION, "Incorrect Type (%d = %s) for \"%s\", expected SQLITE_TEXT", sql_type, SQLType2String(sql_type).c_str(), colname.c_str());
#endif // HAS_LOGGING
        }
    }

    return tmp;
}




double
GDataBaseIF::ReadFloat(sqlite3_stmt *stmt, const int idx, const string colname, const int sql_type, const GLocation l)
{
    double tmp = -1;
    if (strcasecmp(sqlite3_column_name(stmt, idx), colname.c_str()) == 0)
    {
        if (sqlite3_column_type(stmt, idx) == SQLITE_FLOAT)
        {
            tmp = (sqlite3_column_double(stmt, idx));
        }
        else
        {
#ifdef HAS_LOGGING
            HandleError(l, eMSGLEVEL::LOG_ERROR, THROW_EXCEPTION, "Incorrect Type  (%d = %s) \"%s\", expcted SQLITE_FLOAT", sql_type, SQLType2String(sql_type).c_str(), colname.c_str());

#else
            HandleError(l, THROW_EXCEPTION, "Incorrect Type  (%d = %s) \"%s\", expcted SQLITE_FLOAT", sql_type, SQLType2String(sql_type).c_str(), colname.c_str());

#endif // DEBUG

        
        }
    }

    return tmp;
}

int GDataBaseIF::ReadInteger(sqlite3_stmt *stmt, const int idx, const string colname, const int sql_type, const GLocation l)
{
    int tmp = -1;
    if (strcasecmp(sqlite3_column_name(stmt, idx), colname.c_str()) == 0)
    {
        if (sqlite3_column_type(stmt, idx) == SQLITE_INTEGER)
        {
            tmp = (sqlite3_column_int(stmt, idx));
        }
        else
        {
#ifdef HAS_LOGGING
            HandleError(l, eMSGLEVEL::LOG_ERROR, THROW_EXCEPTION, "Incorrect Type  (%d = %s) \"%s\", expected SQL_INTEGER", sql_type, SQLType2String(sql_type).c_str(), colname.c_str());

#else
            HandleError(l, THROW_EXCEPTION, "Incorrect Type  (%d = %s) \"%s\", expected SQL_INTEGER", sql_type, SQLType2String(sql_type).c_str(), colname.c_str());

#endif // HAS_LOGGING

        }
    }

    return tmp;
}


string
GDataBaseIF::LimitString(const int limit)
{
    std::stringstream buffer;
    buffer << " LIMIT " << limit;
    return buffer.str();
}



bool 
GDataBaseIF::DeleteEntries( const string tablename )
{
    int rc;
    char sql[200];
    char *zErrMsg = 0;
    snprintf(sql, 200, "DELETE FROM %s;", tablename.c_str() ); 

    rc = sqlite3_exec(fDataBase, sql, NULL, 0, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        CERR <<  "DeleteEntries SQL error: " << zErrMsg << ENDL;
        sqlite3_free(zErrMsg);
        return false;
    }

    sqlite3_free(zErrMsg);
    return true;
}

