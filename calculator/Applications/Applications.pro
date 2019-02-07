TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = keyboard funapp calcsheet
funapp.depends = keyboard
calcsheet.depends = keyboard

