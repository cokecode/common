#!/usr/bin/env python

import os
import sys

pipe = os.popen("less", "w")
sys.stdout = pipe
print "OK"
