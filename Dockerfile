FROM centos:7
# Выбираем рабочую папку
WORKDIR /root
COPY . /root
RUN yum install zip unzip -y
#WORKDIR /root/tmp/data
#RUN unzip data.zip -d ../../
#WORKDIR /root
#RUN ls -la
RUN yum install centos-release-scl -y
RUN yum install devtoolset-6-gcc-c++ -y
#RUN yum install devtoolset-6-toolchain -y
RUN scl enable devtoolset-6 bash
RUN scl enable devtoolset-6 'g++ -Irapidjson main.cpp sockshare.cpp master.cpp worker.cpp misc.cpp cycle.cpp handler.cpp countAvg.cpp fetchVisits.cpp getparams.cpp parsedata.cpp -std=c++14 -lpthread -lstdc++fs  -Ofast'


EXPOSE 80


CMD ["./a.out"]

