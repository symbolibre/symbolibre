TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = editiontree parser ../SLL SLL_interface simulator 
SLL_interface.depends = parser
../SLL.depends    = editiontree
simulator.depends += editiontree
simulator.depends += SLL_interface
simulator.depends += ../SLL/
