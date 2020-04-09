#!/bin/bash
chmod a+rwx /dev/ttyUSB*
stty 115200 raw -echo -F /dev/ttyUSB0
stty 115200 raw -echo -F /dev/ttyUSB1
stty 115200 raw -echo -F /dev/ttyUSB2
