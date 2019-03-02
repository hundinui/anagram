#!/bin/zsh

RUNS=10000
AVGTIME=0

for run in {1..$RUNS}
do
	OUT=$(./anagram lemmad.txt luuk)
	TIME=$(echo $OUT | cut -d',' -f 1)
	AVGTIME=$(($TIME + $AVGTIME))
done

AVGTIME_PERRUN=$(($AVGTIME/$RUNS))
echo "Average runtime $AVGTIME_PERRUN microseconds"
