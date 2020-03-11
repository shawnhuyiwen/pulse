FROM debian:buster

ENV TERM linux

RUN apt-get update \
 && apt-get -y install git wget build-essential cmake java-common python3-pip

# Replace with pip install -r requirements.txt
# Or a setup.py
RUN pip3 install six && pip3 install numpy
 
# Pull in local source and build
COPY . /source

RUN wget https://d3pxv6yz143wms.cloudfront.net/8.232.09.1/java-1.8.0-amazon-corretto-jdk_8.232.09-1_amd64.deb
RUN dpkg --install java-1.8.0-amazon-corretto-jdk_8.232.09-1_amd64.deb

RUN export JAVA_HOME=/usr/lib/jvm/java-1.8.0-amazon-corretto/ \
 && mkdir build \
 && mkdir pulse \
 && cd /build \
 && cmake -DPULSE_PYTHON_BINDINGS=ON -DCMAKE_INSTALL_PREFIX=/pulse /source \
 && cd /build \
 && make -j4 \
 && cd / \
 && rm -rf /build \
 && rm java-1.8.0-amazon-corretto-jdk_8.232.09-1_amd64.deb

ENV PYTHONPATH /source/src/python:/pulse/bin

# Build-time metadata as defined at http://label-schema.org
ARG BUILD_DATE
ARG IMAGE=dockcross/linux-armv6
ARG VERSION=latest
ARG VCS_REF
ARG VCS_URL
LABEL org.label-schema.build-date=$BUILD_DATE \
      org.label-schema.name=$IMAGE \
      org.label-schema.version=$VERSION \
      org.label-schema.vcs-ref=$VCS_REF \
      org.label-schema.vcs-url=$VCS_URL \
      org.label-schema.schema-version="1.0"

CMD [ "bash" ]
