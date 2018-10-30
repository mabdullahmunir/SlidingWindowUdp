FROM ubuntu:16.04

RUN apt update && apt install net-tools
RUN apt -y install iproute2

COPY recvfile /tmp/recvfile
COPY sendfile /tmp/sendfile