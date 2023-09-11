FROM ubuntu:22.04
ENV DEBIAN_FRONTEND noninteractive
RUN apt-get update && apt-get install -y g++-12
RUN apt-get install -y build-essential clang-12 lldb-12 lld-12 cmake curl gdb less make wget libboost-all-dev
RUN apt-get install -y python3 python3-pip
RUN pip3 install --no-input autopep8 natsort pylint pycodestyle

COPY .prompt /root/.prompt
RUN cat /root/.prompt >> /root/.bashrc

CMD tail -f < /dev/null
