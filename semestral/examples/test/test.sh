#!/bin/bash
# meant to be executed by 'make test'

cd examples/test

COUNT=`ls ref | wc -l`
if [ $COUNT -ne `ls output | wc -l` ]
then
	echo "The file count in directory output does not match the file count in directory ref!"
	exit 1
fi

for (( i=0 ; ((i-$COUNT)) ; i=(($i+1)) ))
do
	echo -n "Test $i: "
	DIFF_OUTPUT=`diff ref/$i.txt output/$i.txt`
	if [ "$DIFF_OUTPUT" == "" ]
	then
		echo "passed"
	else
		echo "failed: "
		echo "$DIFF_OUTPUT"
	fi
done;