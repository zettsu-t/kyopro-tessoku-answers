FROM ubuntu:24.04
ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update && apt install -y software-properties-common && add-apt-repository -y ppa:ubuntu-toolchain-r/test
RUN apt-get update && apt-get install -y build-essential g++-14 man cmake curl gdb less make wget libboost-all-dev unattended-upgrades && dpkg-reconfigure -plow unattended-upgrades
RUN apt-get install -y libstdc++-11-doc
RUN apt-get install -y python3 python3-pip ruby-full && pip3 install --break-system-packages --no-input autopep8 natsort pylint pycodestyle
RUN yes | unminimize

COPY .prompt /root/.prompt
RUN cat /root/.prompt >> /root/.bashrc

CMD tail -f < /dev/null
