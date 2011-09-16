#!/usr/bin/env python
import subprocess
import re

def runProcess(exe):    
    p = subprocess.Popen(exe, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    while(True):
      retcode = p.poll() #returns None while subprocess is running
      line = p.stdout.readline()
      yield line
      if(retcode is not None):
        break

for line in runProcess(['git','rev-parse','--short','HEAD']):
    if line:
        gitsha = line

gitsha = gitsha.strip();

print gitsha


#include "version.h"
#const char * build_date = __DATE__ " at " __TIME__;
#const char * build_git_sha = e45a430;

with open("version.c","r+w") as cfile:
    content = []
    cfile = open("version.c","r+b")
    for line in cfile:
        line = re.sub(r'(build_git_sha = ")\w*(";)',r"\1"+gitsha+r"\2",line)
        content.append(line)
    cfile.seek(0)
    cfile.writelines(content)
