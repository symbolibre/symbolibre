TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = editiontree parser SLL_interface simulator Applications
SLL_interface.depends = parser
../SLL.depends    = editiontree
simulator.depends += editiontree
simulator.depends += SLL_interface
Applications.depends = SLL_interface
