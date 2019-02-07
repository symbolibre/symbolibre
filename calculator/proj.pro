TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = editiontree parser SLL_interface simulator
SLL_interface.depends = parser
simulator.depends = editiontree
simulator.depends += SLL_interface
