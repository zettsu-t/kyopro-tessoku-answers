FROM ubuntu:focal
ENV DEBIAN_FRONTEND noninteractive
RUN apt-get update
RUN apt-get install -y build-essential clang-12 lldb-12 lld-12 cmake curl gdb less make wget
CMD tail -f < /dev/null
