#! /usr/bin/env python
# -*- coding: utf-8 -*-

import inspect

def bar():
	print "bar"

	## show call stack
	for stack in inspect.stack():
		## (<frame object at 0x7fdd22c90c20>, './test-inspect.py', 21, 'foo', ['\tbar()\n'], 0)
		print stack
		## <type 'tuple'>
		print type(stack)

	## the caller frame and module
	frame = inspect.stack()[1][0]
	print type(frame)		## <type 'frame'>
	module = inspect.getmodule(frame)
	print type(module)		## <type 'module'>
	print module			## <module '__main__' from './test-inspect.py'>
	print module.__name__	## "__main__"
	

def foo():
	bar()

def main():
	foo()

main()
