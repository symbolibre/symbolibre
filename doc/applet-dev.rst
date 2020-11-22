.. Copyright 2018-2020 Symbolibre authors <https://symbolibre.org>
.. SPDX-License-Identifier: CC-BY-SA-4.0
.. SPDX-License-Identifier: CC0-1.0

.. highlight:: qml

==================================
Writing a Symbolibre applet in QML
==================================

.. note::
   We cannot offer any guarantee on the stability of the APIs.

------
Basics
------

The ``SLStandardApplet`` type represents the root of an applet
and implements function bar handling.

Thus, the basic skeleton of an applet is as follows::

    import org.symbolibre.applet 0.2

    SLStandardApplet {
        // your code goes here
    }

Note that you don't have to create a window, the user interface you write
will be directly inserted into the window of the Symbolibre shell.

---------------------
Installing the applet
---------------------

Applets are stored in the apps subdirectory of the static data directory of Symbolibre.
On Unix systems, this may be ``/usr/share/symbolibre/apps`` or ``/usr/local/share/symbolibre/apps``
if the project was installed globally.

To allow the Symbolibre shell to find your applet and display it in the main menu,
you need to write an ``application.json`` file in the directory of the applet.
Its structure is as follows:

.. code-block:: json

   {
     "id": "example",
     "applet": "example/Example.qml",
     "name": "My first applet",
     "icon": "example.png"
   }

You can also provide translated names by specifying ``name_LANG`` strings, e.g.
``"name_fr": "Mon premier applet"``.

The icon should be in the ``icons`` directory next to ``apps``.

The complete directory structure of this example applet would look like this:

.. code-block:: text

   example/
   | application.json
   | Example.qml

-------------
Function keys
-------------

A function key is defined by a ``FunctionKeyModel``, with two attributes
``text`` and ``color`` and one signal ``activated``. Functions can be assigned
to the F1...F5 keys using the ``FunctionBar`` attached property which has six
attributes: ``f1`` to ``f5`` and ``combine``.

Function keys shown on the bar are determined by scanning the tree from the
active focus item up to the parents until an item with a ``FunctionBar``
attached property is found. ::

    import QtQuick.Controls 2.5
    import org.symbolibre.applet 0.2

    SLStandardApplet {
        text: "Button"
        FunctionBar.f1: FunctionKeyModel {
            text: "Foo"
        }
        Button {
            FunctionBar.f1: FunctionKeyModel {
                text: "Bar"
            }
        }
    }

In the above example, if the button has active focus, the F1 key label reads "Bar",
otherwise it reads "Foo".

Normally the search stops once an item that specifies function keys if found.
If the ``FunctionBar.combine`` attribute is set to ``true``, the search will
continue and the function keys will combine with whatever the next parent to
define function keys provides. Remember that since the search is bottom-up, the
``combine`` property should be set on the child.
