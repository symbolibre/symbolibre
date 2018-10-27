TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = editiontree \
		  simulator 
simulator.depends = editiontree