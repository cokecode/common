#!/home/tops/bin/python
# zhangxun@taobao.com, 2015.01
# vim:set smartindent expandtab smarttab shiftwidth=4 tabstop=4:

import sys
import os
import re
import commands
import argparse

def main():
    parser = argparse.ArgumentParser(prog='go')
    parser.add_argument('-z', '--fuzzy', action='store_true', help='fuzzy match hostname, prod, owner, etc.')
    parser.add_argument('keywords', nargs='*')
    args, unknown = parser.parse_known_args()

    if not args.keywords:
        parser.print_usage()
        sys.exit(1)

    keyword = args.keywords[0]
    unknown.extend(args.keywords[1:])
    remain_args = ' '.join(unknown)

    if args.fuzzy:
        pass
    else:
        pass

if __name__ == '__main__':
    main()
