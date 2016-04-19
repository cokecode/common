# -*- coding: utf-8 -*-
# vim:smartindent:expandtab:smarttab:shiftwidth=4:tabstop=4
#

import xml.etree.ElementTree as ET

def parse_property(e):
    if e.tag != "property":
        return

    name = None
    value = None

    for child in e:
        if child.tag == "name":
            name = child.text
        elif child.tag == "value":
            value = child.text

    return name, value

def xml_to_keyvalue(fp):
    """parse hadoop xml config file and output key/value dict.
    """
    tree = ET.parse(fp)
    root = tree.getroot()

    confmap = {}

    if root.tag == "configuration":
        for element in root:
            name, value = parse_property(element)
            value = value.strip()

            # value: multiple lines -> one line if possible
            tmp = value.splitlines()
            if len(tmp) > 1:
                tmp = [ x.strip() for x in tmp ]
                tmp = sorted(tmp)
                value = " ".join(tmp)

            confmap[name] = value.strip()

    return confmap

dic = xml_to_keyvalue(file("core-site.xml"))
print dic
