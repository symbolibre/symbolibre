TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = lib components keyboard funapp calcsheet
keyboard.depends = components
funapp.depends = keyboard
calcsheet.depends = keyboard

