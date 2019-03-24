TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = editiontree simulator Applications
../SLL.depends    = editiontree
simulator.depends += editiontree
simulator.depends += SLL_interface
Applications.depends = SLL_interface
