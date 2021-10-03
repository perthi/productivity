// -*- mode: c++ -*-

#ifndef GSEMAPHORE_H
#define GSEMAPHORE_H

/***************************************************************************
***          Author: Per Thomas Hille <pth@embc.no>                     ****
****************************************************************************/

/****************************************************************************
*** Copyright(C) 2018  Per Thomas Hille, pth@embc.no                      ***
*** This file is part of logmaster.logmaster is free software : you can   ***
*** redistribute it and / or modify it under the terms of the Lesser GNU  ***
*** General Public License(LGPL) V3 or later.See.cpp file for details     ***
*****************************************************************************/


#include <stdio.h>          /* printf()                 */
#include <stdlib.h>         /* exit(), malloc(), free() */
#include <sys/types.h>      /* key_t, sem_t, pid_t      */
#include <errno.h>          /* errno, ECHILD            */

#ifndef _WIN32
#include <unistd.h>
#include <semaphore.h>      /* sem_open(), sem_destroy(), sem_wait().. */
#include <sys/wait.h>
#include <sys/shm.h>        /* shmat(), IPC_RMID        */
#endif

#include <fcntl.h> /* O_CREAT, O_EXEC          */




#include <utilities/GDefinitions.h>

class GSemaphore;


GSemaphore  API * g_semaphore();

/** @brief Simplified interface for using semaphores for inter process communication under Linux */
class  GSemaphore
{
public:
    static GSemaphore API * Instance();
    void HandleSemaphoreError( const int ret, const double time = 0 ) const;
    int Wait(sem_t *s);
    int TimedWait(sem_t *s,  const double timetowait_sec = 1 );
    int Post(sem_t *s); 
private:    
    GSemaphore();
    virtual ~GSemaphore();
};

#endif
