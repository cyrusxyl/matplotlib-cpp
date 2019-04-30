FROM ubuntu:16.04

# Intall Build and Compiler Tools
RUN apt-get update &&  apt-get install -y --no-install-recommends \
    gcc \
    g++ \
    clang-6.0 \
    clang-tidy-6.0 \
    clang-format-6.0 \
    lldb-6.0 \
    lld-6.0 \
    make \
    sudo \
    lcov \
    git \
    python3-pip \
    python3-setuptools \
    libglib2.0-dev \
    doxygen \
    ; \
    rm -rf /var/lib/apt/lists/*

# required by Klocwork
RUN dpkg --add-architecture i386 && \
    apt-get update && \
    apt-get install -y lsb-core lib32tinfo5 libnuma1 && \
    rm -rf /var/lib/apt/lists/*

# Install Conan Package Manager
ENV CONAN_USER_HOME=/root
RUN pip3 install wheel && \
    pip3 install conan==1.11.2 && \
    conan profile new default --detect && \
    conan profile update settings.compiler.version=5.4 default && \
    conan profile update settings.compiler.libcxx=libstdc++11 default && \
    conan remote add nutonomy https://registry.nutonomy.com/artifactory/api/conan/conan && \
    conan remote remove conan-transit ; \
    conan remote remove conan-center ; \
    conan user -p APAWEMSMr6QCUuZVdHmLYKsYaty -r nutonomy conan && \
    chmod 777 $CONAN_USER_HOME/.conan && rm -f $CONAN_USER_HOME/.conan/registry.*.lock && \
    chmod 666 $CONAN_USER_HOME/.conan/registry.*


# Install CMake
RUN pip3 install cmake==3.13.2 cmake-format==0.4.5;

# setup jenkins user
RUN groupadd -g 1000 jenkins && \
    useradd -d /home/jenkins -u 1000 -g 1000 -m -s /bin/bash jenkins && \
    echo "jenkins ALL=(ALL) NOPASSWD:ALL" > /etc/sudoers.d/sudoers_jenkins

ENTRYPOINT ["/bin/bash"]
