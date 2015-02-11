#!/bin/sh

INIT=""
PATTERN='s/\"/\\\"/g'

while read LINE
do
	[ -z "$LINE" ] && continue
	LINE2=`echo ${LINE} | sed ${PATTERN}`
	EXPR="gROOT->ProcessLine(\"${LINE2}\");"
	INIT="${INIT}${EXPR} "
done < root/initialize.txt

root -l -e "$INIT"