FROM rikorose/gcc-cmake:gcc-8 as builder

COPY /cmake /cmake
COPY /header /header
COPY /source /source

WORKDIR /cmake

RUN cmake CMakeLists.txt
RUN make


FROM ubuntu:latest

RUN mkdir /work
WORKDIR /work

COPY --from=builder /cmake/test.out ./test.out
CMD ["./test.out"]
