TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = parser editiontree simulator
simulator.depends = editiontree
