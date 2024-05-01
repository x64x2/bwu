#!/bin/bash
echo "$@"
branch=${1:-0}
commit=${2:-0}
date=${3:-0}
binary_version=${4:-0}
binary_name=${5:-"main"}

echo "Building ${binary_name} binaries!" 
pwd
GOOS=linux GOARCH=386 go build -v -o ./bin/${binary_name}.i386 -ldflags "-X $PACKAGE.Version=$binary_version -X $PACKAGE.Branch=$branch -X $PACKAGE.Commit=$commit -X $PACKAGE.Date=$date"  ./cmd/main.go
GOOS=linux GOARCH=amd64 go build -v -o ./bin/${binary_name}.x86_64 -ldflags "-X $PACKAGE.Version=$binary_version -X $PACKAGE.Branch=$branch -X $PACKAGE.Commit=$commit -X $PACKAGE.Date=$date"  ./cmd/main.go
GOOS=linux GOARCH=arm GOARM=7 go build -v -o ./bin/${binary_name}.armv7 -ldflags "-X $PACKAGE.Version=$binary_version -X $PACKAGE.Branch=$branch -X $PACKAGE.Commit=$commit -X $PACKAGE.Date=$date"  ./cmd/main.go
GOOS=darwin go build -v -o ./bin/${binary_name}.macos -ldflags "-X $PACKAGE.Version=$binary_version -X $PACKAGE.Branch=$branch -X $PACKAGE.Commit=$commit -X $PACKAGE.Date=$date"  ./cmd/main.go
GOOS=windows GOARCH=386 go build -v -o ./bin/${binary_name}_32bit.exe -ldflags "-X $PACKAGE.Version=$binary_version -X $PACKAGE.Branch=$branch -X $PACKAGE.Commit=$commit -X $PACKAGE.Date=$date"  ./cmd/main.go
GOOS=windows GOARCH=amd64 go build -v -o ./bin/${binary_name}_64bit.exe -ldflags "-X $PACKAGE.Version=$binary_version -X $PACKAGE.Branch=$branch -X $PACKAGE.Commit=$commit -X $PACKAGE.Date=$date"  ./cmd/main.go
