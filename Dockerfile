ARG BASE=debian:buster
FROM $BASE
ARG BASE

RUN echo $BASE

ENV TERM linux

RUN if [ "$BASE" = "debian:buster" ]; then \
  apt-get update && \
  apt-get -y install git wget build-essential cmake java-common python3-pip && \
  wget https://d3pxv6yz143wms.cloudfront.net/8.232.09.1/java-1.8.0-amazon-corretto-jdk_8.232.09-1_amd64.deb && \
  dpkg --install java-1.8.0-amazon-corretto-jdk_8.232.09-1_amd64.deb && \
  export JAVA_HOME=/usr/lib/jvm/java-1.8.0-amazon-corretto \
  ;fi
RUN if [ "$BASE" = "fedora:32" ]; then \
  dnf check-update && \
  dnf -y install git make gcc gcc-c++ cmake python3-devel python3-pip && \
  rpm --import https://yum.corretto.aws/corretto.key && \
  curl -L -o /etc/yum.repos.d/corretto.repo https://yum.corretto.aws/corretto.repo && \
  yum install -y java-1.8.0-amazon-corretto-devel && \
  export JAVA_HOME=/usr/lib/jvm/java-1.8.0-amazon-corretto \
  ;fi

# Replace with pip install -r requirements.txt
# Or a setup.py
RUN pip3 install six \
    && pip3 install numpy \
    && pip3 install pandas \
    && pip3 install matplotlib \
    && pip3 install bqplot \
    && pip3 install jupyter \
    && pip3 install ipywidgets
RUN jupyter nbextension enable --py bqplot
 
# Pull in local source and build
COPY . /source
RUN mkdir build && \
    mkdir pulse  && \
    cd /build  && \
    cmake -DPulse_PYTHON_API=ON -DCMAKE_INSTALL_PREFIX=/pulse /source && \
    cd /build && \
    make -j4 && \
    cd /  && \
    rm -rf /build
    
RUN if [ "$BASE" = "debian:buster" ]; then \
 rm java-1.8.0-amazon-corretto-jdk_8.232.09-1_amd64.deb ;fi

ENV PYTHONPATH /source/src/python:/pulse/bin

# Build-time metadata as defined at http://label-schema.org
ARG BUILD_DATE
ARG IMAGE=pulse
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
