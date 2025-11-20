#! /bin/sh
cd `dirname $0`
set -e
source ./check-pb-home.sh
LD_LIBRARY_PATH=$PBLIB_HOME/lib64 ./bin/bm
