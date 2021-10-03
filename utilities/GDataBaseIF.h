// -*- mode: c++ -*-
#ifndef GDATABASEIF_H
#define GDATABASEIF_H
/**************************************************************************
 * This file is property of and copyright by Embedded Consulting'  2020    *
 *                                                                        *
 * Author: Per Thomas Hille <pth@embc.no>                                 *
 * Contributors are mentioned in the code where appropriate.              *
 * Please report bugs to pth@embc.no                                      *
 **************************************************************************/


#include  "GLocation.h"
#include  "GFormatting.h"
#include  "GCommon.h"
#ifdef HAS_LOGGING
#include  <logging/LEnums.h>
#include  <logging/LMessage.h>
//#include  <logging/LMessageGenerator.h>
#include  <logging/LPublisher.h>
#include <exception/GException.h>
using namespace LOGMASTER;
#else
#include <exception>
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
                template<typename... Args>
                #ifdef HAS_LOGGING
                void API HandleError( const GLocation l,   eMSGLEVEL lvl,  const bool throw_ex,   const char * fmt, const Args... args);
                #else
                void API HandleError(const GLocation l, const bool throw_ex, const char *fmt, const Args... args);
        #endif
        
        string  API SQLType2String( const int sql_type  ) const;    
        string  API LimitString( const int cnt );
        sqlite3       *fDataBase  =  nullptr; 
        sqlite3_stmt  *fStmt     =   nullptr;  // SQLite statmement 

     private:
         GDataBaseIF( const GDataBaseIF &rhs );
        GDataBaseIF &    operator =     ( const GDataBaseIF &rhs); 
        std::shared_ptr< LOGMASTER::LMessageGenerator> fMessageGenerator = nullptr;
};


template<typename... Args>
void
#ifdef HAS_LOGGING
GDataBaseIF::HandleError(const GLocation  l, eMSGLEVEL  lvl,  const bool  throw_ex, const char *  fmt, const Args... args)
#else
GDataBaseIF::HandleError(const GLocation  l,  const bool  throw_ex, const char *  fmt,  const Args... args)
#endif
{
    auto formatCheck = GFormatting::checkFormat(l.fFileName.c_str(), l.fLineNo, l.fFunctName.c_str(), fmt, args...);
    static char formatted_message[4096] = {0};
    formatted_message[0] = 0;
    if(formatCheck.first == true)
    {
        if(sizeof...(args) > 0)
        {
            snprintf(formatted_message, sizeof(formatted_message) - 1, fmt, args...);
        }
        else
        {
            snprintf(formatted_message, sizeof(formatted_message) - 1, fmt, args...);
        }
    }
    else
    {
        snprintf(formatted_message, sizeof(formatted_message) - 1, "Error while checking format %s: %s", fmt,
                 formatCheck.second.c_str());
    }

#ifdef HAS_LOGGING

    std::shared_ptr<LMessage> msg_ptr = fMessageGenerator->GenerateMsg(eMSGFORMAT::PREFIX_ALL, lvl, eMSGSYSTEM::SYS_DATABASE, l.fFileName.c_str(), l.fLineNo, l.fFunctName.c_str(), fmt, args...);
   // LMessage msg = *msg_ptr;
   // DATABASE_INFO("%s",msg_ptr->  );
  //  LPublisher::Instance()->PublishToConsole(msg_ptr );
    LPublisher::Instance()->PublishToFile("db.log", msg_ptr );
    LPublisher::Instance()->PublishToSubscribers(msg_ptr );
    LPublisher::Instance()->PublishToSubscribers(msg_ptr );

    if( throw_ex == THROW_EXCEPTION )
    {
        DB_EXCEPTION("%s",formatted_message );
    }
#else
    printf("%s::%s[line %d]: %s", l.fFileName.c_str(), l.fFunctName.c_str(), l.fLineNo, formatted_message);

    if( throw_ex == THROW_EXCEPTION )
    {
        throw ( std::invalid_argument( formatted_message ) );
    }
#endif
}

#endif
