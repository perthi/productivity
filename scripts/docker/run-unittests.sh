#!/bin/bash


##########################################################################
## This bash script is free software: you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program.  If not, see <https://www.gnu.org/licenses/>.
##########################################################################


# Script that runs unit tests. The script atomatically fetches all unit tests from
# the bin directory. Test unit tests can run with 3 different targets
# - x86  Your local PC
# - qemu The arm emulator running under Linux
#
# Author: Per Thomas Hille, Embedded Consulting AS
# email:  pth@embc.no


x86_dir=$PWD/build/x86/bin/
arm_dir=$PWD/build/arm/bin/


run_test ()
{
    testname=$2
    testname=${testname/*\//};


    
    case $1 in
        x86)
            echo "Running tests "$testname "for" $1 
            OLD=$LD_LIBRARY_PATH
            export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PWD/build/x86/lib/ 
            $2 $3 $4 $5
            ret=$?
            export LD_LIBRARY_PATH=$OLD
            ;;
        arm)
            echo "Running arm emulation unit test for "$testname "for" $1
            OLD=$LD_LIBRARY_PATH
             export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/arm-linux-gnueabi/libhf/ 
             export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/arm-linux-gnueabihf/lib/
            #      export LD_LIBRARY_PATH=$PWD/build/arm/lib:$PWD/arm-extras:/usr/arm-linux-gnueabihf/lib/ 
      #      qemu-system-arm -machine virt-2.10 ,format=raw  $arm_dir/$testname $2 $3 $4 $5
             qemu-arm   $arm_dir/$testname  $3 $4 $5
#            qemu-arm   $arm_dir/$testname 
            ret=$?
            export LD_LIBRARY_PATH=$OLD
            ;;
    esac
    return $ret
}



check_params()
{
    if [ "$#" -le 0 ]; then
        echo error, too few parametes "$#"
    else
        if  [ "$1" != "x86" ] &&  [ "$1" != "arm"  ]; then
            usage
        fi
    fi
}



usage()
{
    printf "\n"
    printf "\tUsage: %s [target]\t\t(for target = x86 or arm) \n", $0
    printf "\n"
}


check_params $*



if [ "$1" = "x86" ]; then
    tests=('./build/x86/bin/*unit-test*')
elif [ "$1" = "arm" ]; then
    tests=('./build/arm/bin/*unit-test*')
    

else
    echo "usage: " $0 "[target]  where target is either x86 or arm" 
fi


tests=$PWD/$tests


for test in $tests; do
  #  echo running tets for $test !! argument  = $1
    run_test $1 $test $2 $3 $4
    
    retVal=$?
    
    if [ $retVal -ne 0 ]; then
        echo "TEST FAILED, ret = " $retVal
        exit $retVal
    else
        echo "TEST OK !!!!!!"
    fi
done


exit $retVal

