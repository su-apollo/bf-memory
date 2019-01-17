FROM ubuntu:16.04

RUN apt-get update && apt-get install -y \
    g++ \
    make

COPY /trunk /trunk
WORKDIR /trunk

RUN make