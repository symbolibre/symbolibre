TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = keyboard funapp calcsheet
keyboard.depends = components
funapp.depends = keyboard
calcsheet.depends = keyboard

