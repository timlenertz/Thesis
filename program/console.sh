#!/bin/sh

INIT=""
PATTERN='s/\"/\\\"/g'
ROOT=../root/dist/bin/root

while read LINE
do
	[ -z "$LINE" ] && continue
	LINE2=`echo ${LINE} | sed ${PATTERN}`
	EXPR="gROOT->ProcessLine(\"${LINE2}\");"
	INIT="${INIT}${EXPR} "
done < root/initialize.txt

${ROOT} -l -e "$INIT"