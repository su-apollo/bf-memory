FROM ubuntu:16.04 as builder

RUN apt-get update && apt-get install -y \
    g++ \
    make

COPY /trunk /trunk
WORKDIR /trunk

RUN make


FROM ubuntu:16.04

RUN mkdir /work
WORKDIR /work

COPY --from=builder /trunk/output .
CMD ["./test.out"]