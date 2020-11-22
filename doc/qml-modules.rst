.. Copyright 2018-2020 Symbolibre authors <https://symbolibre.org>
.. SPDX-License-Identifier: CC-BY-SA-4.0

======================
Symbolibre QML modules
======================

.. note::
   This documentation does not cover all the Symbolibre QML modules.
   We plan to replace it with doxygen-based documentation.

   We cannot offer any guarantee on the stability of the APIs.

-----------------------
``org.symbolibre.expr``
-----------------------

This module provides items that can display and edit math expressions.

^^^^^^^^
``Expr``
^^^^^^^^

``Expr`` is a selectable, static math expression.

* ``text`` (read-only string) is the Giac formula for the expression.
* ``json`` (string) is the serialized form of the expression.
* ``font`` (font) is the font used to render the expression.
* ``color`` (color) is the text and line color.
* ``fillColor`` (color) is the background color. Transparent backgrounds are
  not supported. This property is inherited from ``QQuickPaintedItem``.
* ``horizontalAlignment`` is the horizontal alignment of the contents within
  the item. Valid values are ``Expr.AlignLeft``, ``Expr.AlignHCenter`` and
  ``Expr.AlignRight``.
* ``verticalAlignment`` is the vertical alignment of the contents within the
  item. Valid values are ``Expr.AlignTop``, ``Expr.AlignVCenter`` and
  ``Expr.AlignBottom``.

Two methods are provided:

* ``void insertJson(string)`` inserts a subexpression's serialized JSON at the
  cursor's current position.
* ``bool clear()`` clears the expression and returns ``true``.

^^^^^^^^^^^^^
``ExprField``
^^^^^^^^^^^^^

``ExprField`` is a wrapper around ``Expr`` that provides interactive edition
capabilities. When focused, it shows the cursor and accepts input events from
the keyboard.

* The ``expr`` property exposes the underlying ``Expr`` item.
* Aliases to the underlying expression's ``text``, ``json``, ``insertJson`` and
  ``clear`` are provided.

-----------------------
``org.symbolibre.util``
-----------------------

Utility functions and objects.

The ``Fs`` singleton provides some filesystem access for Javascript code in
QML modules. Most notably, it provides the program's install path and data
storage path.

* ``string Fs.staticDataDir()`` returns the folder in which read-only
  application-specific data can be found. This folder is populated at install
  time and should be not written to.
* ``string Fs.readWriteDataDir()`` returns the read-write application data
  folder. The directory is created if it does not exist. This is the place to
  store user files and values to remember between executions.

These paths are the same for all applications, so typically you should access a
subfolder (unless the data being used is shared across several applications).

.. code-block:: javascript

  // An example program from the IDE application
  const example_py = Fs.staticDataDir() + "/ide/programs/example.py"

The ``Fs`` singleton also provides functions for reading and writing to a file,
although complex I/O should be left to C++ code.

* ``string Fs.readFile(string path)`` reads a file and returns its contents,
  or an empty string in case of an error.
* ``void Fs.writeFile(string path, string content)`` writes the given string
  to the given file, overwriting its previous content.

The complete list can be found in the source code at
``lib/symbolibre/util``.

---------------------------
``org.symbolibre.controls``
---------------------------

This module wraps some Qt Quick and Qt Quick Controls 2 items, mostly to ease keyboard
navigation or integrate with Symbolibre applets. Generally they should be
preferred to their Qt Quick original counterparts.

The following items are defined:

* ``SLStackLayout`` is a StackLayout that forward active focus to its children
  like a ``FocusScope``.
* ``SLWindow`` is an ApplicationWindow that provides an additional
  ``void showError(string text[, string title])`` function to open a popup dialog.
  Symbolibre applets can assume they are run in an SLWindow, see :doc:`applet-dev`.
