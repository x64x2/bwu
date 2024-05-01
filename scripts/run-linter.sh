#!/bin/bash

SCRIPTDIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
cd ${SCRIPTDIR}/..

echo -e "\n==== Setup linters ==="
go version
pushd /
go get -u github.com/golangci/golangci-lint/cmd/golangci-lint >/dev/null 2>&1 && \
popd 
${GOPATH}/bin/golangci-lint linters
OLDPWD=$(pwd)
cd $1
pwd
echo -e "\n==== Running linters ==="

RESULTS=$(${GOPATH}/bin/golangci-lint run --enable-all -D gochecknoglobals ./...)

cd $OLDPWD

if [[ "${RESULTS}" != "" ]]; then
  echo "${RESULTS}"
  echo -e "\n\n=== !!! FAILED !!! ===\n"
  exit 1
fi

echo  -e "\n\n=== Passed! ===\n"

