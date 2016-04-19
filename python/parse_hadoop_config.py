#!/usr/bin/python
# -*- coding: utf-8 -*-
# vim:smartindent:expandtab:smarttab:shiftwidth=4:tabstop=4
#

import sys
import libxml2
import json

def parse_property(element):
    child = element.children
    d = {}

    while child is not None:
        if child.type == "element":
            name = child.name.strip()
            value = child.content.strip()

            # value: multiple lines -> one line if possible
            tmp = value.splitlines()
            if len(tmp) > 1:
                tmp = [ x.strip() for x in tmp ]
                tmp = sorted(tmp)
                value = " ".join(tmp)

            d[name] = value
        child = child.next

    if "name" in d and "value" in d:
        return { d["name"]: d["value"] }
    else:
        raise Exception("incomplete property: %s" % d)

def parse_xml_config(content):
    cfgmap = {}

    doc = libxml2.parseDoc(content)
    topnode = doc.children
    root = None

    while topnode is not None:
        if topnode.name == "configuration":
            root = topnode
            break

        topnode = topnode.next

    child = root.children
    while child is not None:
        #print child
        if child.type == "element" and child.name == "property":
            #print "==>", child.name
            keyvalue = parse_property(child)
            cfgmap.update(keyvalue)
        child = child.next

    return cfgmap

def print_readable_properties(cfgmap):
    for name in sorted(cfgmap.keys()):
        value = cfgmap[name]
        print "%-40s: %s" % (name, value)

def print_difference(dic1, dic2):
    allnames = set(dic1.keys() + dic2.keys())
    allnames = sorted(list(allnames))

    def _getval(dic, key):
        if key in dic:
            return dic[key]
        return None

    for name in allnames:
        v1 = _getval(dic1, name)
        v2 = _getval(dic2, name)

        if v1 != v2:
            if v1 is not None:
                print "- %s: %s" % (name, v1)
            if v2 is not None:
                print "+ %s: %s" % (name, v2)

def test1():
    filecontent = file("core-site.xml").read()
    cfgmap = parse_xml_config(filecontent)
    print_readable_properties(cfgmap)

if __name__ == "__main__":
    if len(sys.argv) == 1:
        print "Usage:"
        print "  %s hadoop-config-file     parse and print" % sys.argv[0]
        print "  %s file1 file2            compare" % sys.argv[0]
        sys.exit(1)

    if len(sys.argv) == 2:
        cfgfile = sys.argv[1]
        d = parse_xml_config(file(cfgfile).read())
        print_readable_properties(d)
    elif len(sys.argv) == 3:
        file1, file2 = sys.argv[1:]
        d1 = parse_xml_config(file(file1).read())
        d2 = parse_xml_config(file(file2).read())
        print_difference(d1, d2)
