ARG BASE=debian:bullseye
FROM $BASE
ARG BASE

ARG JUPYTER=OFF

RUN echo $BASE
RUN echo $JUPYTER

ENV TERM linux

RUN if [ "$BASE" = "debian:bullseye" ]; then \
  apt-get update && \
  apt-get -y install git wget build-essential cmake java-common python3-pip && \
  apt-get -y install clang-format clang-tidy clang-tools clang clangd libc++-dev libc++1 libc++abi-dev libc++abi1 libclang-dev && \
  apt-get -y install libclang1 liblldb-dev libllvm-ocaml-dev libomp-dev libomp5 lld lldb llvm-dev llvm-runtime llvm python-clang &&\
  wget https://d3pxv6yz143wms.cloudfront.net/8.232.09.1/java-1.8.0-amazon-corretto-jdk_8.232.09-1_amd64.deb && \
  dpkg --install java-1.8.0-amazon-corretto-jdk_8.232.09-1_amd64.deb \
  ;fi
RUN if [ "$BASE" = "fedora:32" ]; then \
  dnf check-update && \
  dnf -y install git make gcc gcc-c++ cmake python3-devel python3-pip && \
  rpm --import https://yum.corretto.aws/corretto.key && \
  curl -L -o /etc/yum.repos.d/corretto.repo https://yum.corretto.aws/corretto.repo && \
  yum install -y java-1.8.0-amazon-corretto-devel \
  ;fi

# Replace with pip install -r requirements.txt
# Or a setup.py
RUN if [ "$JUPYTER" = "ON" ]; then \
    pip3 install six \
    && pip3 install numpy \
    && pip3 install pandas \
    && pip3 install matplotlib \
    && pip3 install bqplot \
    && pip3 install jupyter \
    && pip3 install ipywidgets \
    && jupyter nbextension enable --py bqplot \
    ;fi
 
# Pull in local source and build
COPY . /source
RUN export JAVA_HOME=/usr/lib/jvm/java-1.8.0-amazon-corretto && \
    mkdir build && \
    mkdir pulse && \
    cd /build  && \
    cmake -DPulse_PYTHON_API=ON \
          -DCMAKE_INSTALL_PREFIX=/pulse \
          -DCMAKE_BUILD_TYPE:STRING=Release \
          -DCMAKE_C_COMPILER=clang \
          -DCMAKE_CXX_COMPILER=clang++ \
          -DCMAKE_CXX_FLAGS=-stdlib=libc++ \
          /source && \
    cd /build && \
    make -j4 && \
    cd /

# Copy out all needed python files
RUN mkdir pypulse && \
    cp /build/Innerbuild/src/python/[PyPulse]* /pypulse && \
    cp -R /build/Innerbuild/src/python/pulse /pypulse && \
    cp -R /source/src/python/pulse /pypulse && \
    rm -rf /source && \
    rm -rf /build

ENV PYTHONPATH /pypulse
    
RUN if [ "$BASE" = "debian:bullseye" ]; then \
 rm java-1.8.0-amazon-corretto-jdk_8.232.09-1_amd64.deb ;fi

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
