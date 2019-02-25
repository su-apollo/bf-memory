FROM ubuntu:16.04 as builder

RUN apt-get update && apt-get install -y \
    g++ \
    make

COPY /gcc /gcc
COPY /bf-memory /bf-memory
WORKDIR /gcc

RUN make


FROM ubuntu:16.04

RUN mkdir /work
WORKDIR /work

COPY --from=builder /gcc/test.out ./test.out
CMD ["./test.out"]