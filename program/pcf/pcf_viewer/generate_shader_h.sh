#!/bin/sh

rm shaders/*.h

for s in shaders/*;
do
	h="$s.h"
	xxd -i "$s" > "$h"
done