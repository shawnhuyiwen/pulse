FROM debian:buster

ENV TERM linux

RUN apt update \
 && apt -y install git wget build-essential cmake java-common

RUN wget https://d3pxv6yz143wms.cloudfront.net/8.232.09.1/java-1.8.0-amazon-corretto-jdk_8.232.09-1_amd64.deb
RUN dpkg --install java-1.8.0-amazon-corretto-jdk_8.232.09-1_amd64.deb

RUN git clone https://gitlab.kitware.com/physiology/engine \
 && export JAVA_HOME=/usr/lib/jvm/java-1.8.0-amazon-corretto/ \
 && mkdir build \
 && mkdir pulse \
 && cd /build \
 && cmake -DPULSE_PYTHON_BINDINGS=ON -DCMAKE_INSTALL_PREFIX=/pulse /engine \
 && cd /build \
 && make -j4 \
 && cd / \
 && rm -rf /build \
 && rm -rf /engine \
 && rm java-1.8.0-amazon-corretto-jdk_8.232.09-1_amd64.deb
 
CMD [ "bash" ]