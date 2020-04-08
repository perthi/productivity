// -*- mode: c++ -*-

// utilities-dll.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "utilities-dll.h"


// This is an example of an exported variable
UTILITIESDLL_API int nutilitiesdll=0;

// This is an example of an exported function.
UTILITIESDLL_API int fnutilitiesdll(void)
{
    return 42;
}

// This is the constructor of a class that has been exported.
// see utilities-dll.h for the class definition
Cutilitiesdll::Cutilitiesdll()
{
    return;
}
