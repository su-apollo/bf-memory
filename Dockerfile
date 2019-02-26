FROM ubuntu:16.04 as builder

RUN apt-get update && apt-get install -y \
    g++ \
    make

COPY /g++ /g++
COPY /bf-memory /bf-memory
WORKDIR /g++

RUN make


FROM ubuntu:16.04

RUN mkdir /work
WORKDIR /work

COPY --from=builder /g++/test.out ./test.out
CMD ["./test.out"]