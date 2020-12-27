#!/bin/bash


if [ $# -ne 1 ];then
    echo "Usage:" $0 "[target] wher target is either x86, arm or clean"
    exit -1
fi


export OUTDIR=$PWD
echo "outdir =" $OUTDIR

reopdir=`basename $PWD`


function build()
{
   echo "Compiling for " $1  
   make $1 -j16
   ret=$?;

   if [  $ret -eq  0 ]; then
       echo "build OK"
       exit $ret;
   else
       echo "build failed, retval = "  $ret
       exit $ret
   fi
       
}


if [ $1 = "x86" ] || [ $1 = "arm"  ]; then
    if [ !e /usr/include/gtest ]; then
        cp -R gtest/gtest-kts /usr/include/gtest
    fi

    build $1 
    exit 0;    
elif  [ $1 = "clean" ]; then
    echo "cleaning up, outdir = " $OUTDIR
    #make distclean
    ./clean.sh
    exit 0;  
else
    echo "Unknow target " $1  " aborting.."
    exit 3
fi    


exit 0;



