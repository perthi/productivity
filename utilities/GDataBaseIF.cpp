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
using namespace LOGMASTER;

#else
#include <stdio.h>
#endif



#include "sqlite/sqlite3.h"


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



void 
#ifdef HAS_LOGGING
GDataBaseIF::HandleError(const GLocation l, eMSGLEVEL lvl, const char *fmt, ...)
#else
GDataBaseIF::HandleError(const GLocation l, const char *fmt, ...)
#endif
{
    va_list ap;
    va_start(ap, fmt);
    
    va_list ap_l;
    va_copy(ap_l, ap);
    
    char formatted_message[2048] = {0};
    vsnprintf(formatted_message, sizeof(formatted_message) - 1, fmt, ap);
    


#ifdef HAS_LOGGING
    std::shared_ptr<LMessage> msg_ptr = fMessageGenerator->GenerateMsg(eMSGFORMAT::PREFIX_ALL, lvl, eMSGSYSTEM::SYS_DATABASE, l.fFileName.c_str(), l.fLineNo, l.fFunctName.c_str(), fmt, ap_l);
    
    LMessage msg = *msg_ptr;

    LPublisher::Instance()->PublishToConsole(msg );
    LPublisher::Instance()->PublishToFile("db.log", msg );
    LPublisher::Instance()->PublishToSubscribers(msg );
    LPublisher::Instance()->PublishToSubscribers(msg );
    

#else
    printf("%s::%s[line %d]: %s" l.fFileName.c_str(), l.fFunctName.c_str(), l.fLineNo, formatted_message);
#endif

    va_end(ap);
    va_end(ap_l);

}


/**  Opens the database 
 *   @param[in]   db_path The fulle path to hte database
 *   @return      TRUE if the databse was opened sucessfullt, false othervise */
bool 
GDataBaseIF::OpenDatabase(const char *db_path)
{
    int rc;
    rc = sqlite3_open(db_path, &fDataBase);

    if (rc)
    {
        printf("%s line %d, Can't open database: %s\n", __FILE__, __LINE__, sqlite3_errmsg(fDataBase));
        
        #ifdef HAS_LOGGING
            HandleError(GLOCATION, eMSGLEVEL::LOG_FATAL, "Failed to open database \"%s\"",   db_path  );
        #else
            HandleError(GLOCATION,  "Failed to open database \"%s\"",   db_path  );
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
        HandleError(GLOCATION, eMSGLEVEL::LOG_ERROR, "database is busy, cnt =  %d", cnt);
#else
        HandleError(GLOCATION, "database is busy, cnt =  %d", cnt);
#endif

        cnt++;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        if (cnt == 100)
        {
#ifdef HAS_LOGGING
            HandleError(GLOCATION, eMSGLEVEL::LOG_FATAL, "failed to close database after %d attempts, bailing out !!!", cnt);
#else
            HandleError(GLOCATION, eMSGLEVEL::LOG_ERROR, "failed to close database after %d attempts, bailing out !!!", cnt);
#endif
            break;
        }
    }
}
