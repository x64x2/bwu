#!/bin/bash
test_type="unit"
echo -e "\n=== Running tests ===="

echo "Starting $test_type tests in $1"


GOCOVMERGE=$GOPATH/bin/gocovmerge

workdir=.cover
test -d $workdir || mkdir -p $workdir
rm -rf $workdir/*

OLDPWD=$(pwd)
cd $1
pwd
export workdir=$OLDPWD/$workdir

find_packages() {
  tag=$1
  go list -tags=$tag ./... | grep -v vendor
}

run_coverage() {
  local package=$1
  tag=$2

  packname=$(echo "$package" | tr / -)
  covprofile=$workdir/$tag.$packname.coverage
  outfile=$workdir/$tag.$packname.txt

  go test -v -tags="$tag" -covermode=count -coverpkg=$coverpkg -coverprofile=$covprofile $package 2> >(grep -v 'warning: no packages being tested') | tee ${outfile};
}
export -f run_coverage

coverage_report() {
  tag=$1

  $GOCOVMERGE $workdir/$tag.*.coverage > $workdir/$tag.profile
  go tool cover -html=$workdir/$tag.profile -o $workdir/$tag.html

  go tool cover -func=$workdir/$tag.profile
}

# ensure of test tools presence
test -f $GOCOVMERGE || pushd / && go get github.com/wadey/gocovmerge && popd

tout=$(mktemp) || exit 1
trap "rm -f -- '$tout'" EXIT

find_packages $test_type | tr '\n' '\0' | xargs -0 -P4 -n1 -I {} bash -c 'run_coverage "$@"' _ {} $test_type | tee ${tout}

coverage_report $test_type

cd ${OLDPWD}

if grep -q FAIL ${tout}
then (echo; echo "*** found FAIL in output; exiting with error code 1 ***"; grep FAIL ${tout}; echo) 2>&1
     exit 1
fi

echo -e "=== Done! ===\n"
