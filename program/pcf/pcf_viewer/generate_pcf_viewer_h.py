#!/usr/bin/python

import os
import glob

headers = []
for root, dirnames, filenames in os.walk("."):
	directory_headers = glob.glob(root + "/*.h")
	for h in directory_headers:
		headers.append(h[2:])

f = open("pcf_viewer.h", "w")
f.writelines([
	"// File automatically generated by generate_pcf_viewer_h.py\n",
	"\n",
	"#ifndef PCF_PCF_VIEWER_H_\n",
	"#define PCF_PCF_VIEWER_H_\n",
	"\n"
])

for h in headers:
	if(h != "pcf_viewer.h"):
		f.write("#include \"" + h + "\"\n")

f.writelines([
	"\n",
	"#endif\n"
])
