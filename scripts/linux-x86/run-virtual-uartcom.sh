#!/bin/bash


 if [ "$#" -ne 1 ]; then
     echo "usage" $0 " message"
     exit
 fi

message=$1

socat -d -d pty,raw,echo=0 pty,raw,echo=0 1>&  out.txt &
tmp="$(cut -d" " -f7   < out.txt)"

exec_dir=$UCW_HOME/build/x86/bin
exec_name=uart-com

cmd=$exec_dir/$exec_name

set -- $tmp

uart_read=$1
uart_write=$2

read_cmd="$cmd -uart $uart_read -read"
write_cmd="$cmd -uart $uart_write -write $message"

echo  "read cmd =  "  ${read_cmd}
echo  "write cmd = "  ${write_cmd}


## "${write_cmd}"


echo "read_port =" ${uart_read}
echo "write_port =" ${uart_write}

##${cmd} -uart  ${uart_read} -read
## ${write_cmd}
