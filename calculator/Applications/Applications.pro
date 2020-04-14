TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = lib/slmath.pro keyboard funapp calcsheet
keyboard.depends = components
funapp.depends = keyboard
calcsheet.depends = keyboard

