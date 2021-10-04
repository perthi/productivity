
FROM ubuntu:18.04
#FROM dockcross/linux-arm64

MAINTAINER Per Thomas Hille  pth@embc.no
#dockcross/linux-arm64

#USER kts

# Replace dash with bash
RUN rm /bin/sh && ln -s bash /bin/sh

RUN sed -i -e 's|disco|eoan|g' /etc/apt/sources.list
# apt update
RUN apt update --fix-missing


RUN apt-get install -y  apt-utils gawk wget git git-core diffstat unzip \
gcc-multilib build-essential chrpath socat xterm curl parted python python3 \
python3 xz-utils debianutils iputils-ping libsdl1.2-dev \
net-tools cpio  uuid-dev sudo vim libssl-dev  libreadline-dev libxml2-dev

RUN apt-get -y install locate   libc6-armel-cross libc6-dev-armel-cross \
binutils-arm-linux-gnueabi libncurses5-dev \
gcc-arm-linux-gnueabihf g++-arm-linux-gnueabihf

RUN apt update
RUN apt-get -y install autoconf libtool python3-pip python-protobuf
RUN pip3 install setuptools

WORKDIR /tmp
RUN wget https://github.com/protocolbuffers/protobuf/releases/download/v3.13.0/protobuf-all-3.13.0.tar.gz
RUN tar -xzf protobuf-all-3.13.0.tar.gz 
WORKDIR /tmp/protobuf-3.13.0
RUN ./configure --prefix=/usr
RUN make -j8
RUN make install
RUN ldconfig
WORKDIR /tmp/protobuf-3.13.0/python
RUN python3 setup.py build

RUN mkdir -p  /home/root
ENV HOME /home/root


WORKDIR /home/root/

RUN apt-get -y install qemu qemu-system
 
RUN ln -s -f /usr/arm-linux-gnueabihf/lib/ld-linux-armhf.so.3 /lib/ld-linux-armhf.so.3;\
export LD_LIBRARY_PATH=/usr/local/lib/;\
LD_LIBRARY_PATH=/usr/arm-linux-gnueabihf/lib/:$LD_LIBRARY_PATH


WORKDIR /home/root/
