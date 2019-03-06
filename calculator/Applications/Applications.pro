TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = components keyboard funapp calcsheet
keyboard.depends = components
funapp.depends = keyboard
calcsheet.depends = keyboard

