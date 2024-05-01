FROM golang:latest
LABEL maintainer="Erik"

ENV DEBIAN_FRONTEND=noninteractive
ENV PACKAGE main
RUN apt-get update
RUN apt-get install -y build-essential curl git

WORKDIR  /src
CMD build/build.sh $branch $commit $date $version $appname
