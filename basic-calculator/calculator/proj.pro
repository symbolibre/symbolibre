TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = editiontree \ 
		  parser \
		  simulator 
simulator.depends = editiontree
