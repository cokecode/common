#!/usr/bin/env python

import os

pipe = os.popen("less", "w")
pipe.write("OK")
pipe.close()
