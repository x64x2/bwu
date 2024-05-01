FROM golang:latest
LABEL maintainer="Erik"

ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update
RUN apt-get install -y build-essential

WORKDIR /crawler

CMD /bin/bash -c './scripts/run-tests.sh .'
