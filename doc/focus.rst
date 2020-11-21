.. Copyright 2018-2020 Symbolibre authors <https://symbolibre.org>
.. SPDX-License-Identifier: CC-BY-SA-4.0

.. highlight:: qml

===============================
Keyboard navigation in Qt Quick
===============================

Official Qt documentation: https://doc.qt.io/qt-5/qtquick-input-focus.html.

As Qt Quick is mostly used on touch devices, information about keyboard focus
tends to be harder to find. This page summarizes a few points to keep in mind
when writing QML code for Symbolibre, with pointers to the relevant Qt documentation pages.

------
Basics
------

Only one item (and its successive parents that are focus scoppes, see next section)
can have the `activeFocus <https://doc.qt.io/qt-5/qml-qtquick-item.html#activeFocus-prop>`_
property set to true at a time, which represents keyboard focus. This property is read-only, but
the `forceActiveFocus() <https://doc.qt.io/qt-5/qml-qtquick-item.html#forceActiveFocus-method>`_
methods can be used to move the focus to another item. However, there are simpler
ways to manage focus, so forceActiveFocus should only be used as a last resort.

`Key events <https://doc.qt.io/qt-5/qml-qtquick-keys.html>`_ are first sent to the
item that has active focus, then propagated to its parents until one accepts the event.

----------
FocusScope
----------

When an item gains active focus, one may want to give focus to one of its children instead.
This is the point of `focus scopes <https://doc.qt.io/qt-5/qml-qtquick-focusscope.html>`_.
When a focus scope gains active focus, it also gives it to its first (possibly indirect) child that
has the `focus <https://doc.qt.io/qt-5/qml-qtquick-item.html#focus-prop>`_ property set to true.
If this child is also a focus scope, it will in turn share the active focus
with one of its children.
Reciprocally, when an item has active focus, all its parent items that are focus scopes get it too,
including the window.

If another child subsequently gains active focus, the focus scope will update the
``focus`` properties accordingly. This ``focus`` property acts as a memory for focus scopes,
so that when it gains focus again, it can share it with the right child.

In Qt Quick, FocusScope (obviously), Window, Loader, ListView and other views are focus scopes.
In Qt Quick Controls 2, ComboBox, MenuBar, Pane, RangeSlider, SpinBox, StackView, SwipeView, TabBar,
and various window content items are also focus scopes.
It is often a good idea to use a FocusScope as a root item of non-trivial view delegates
and non-trivial children of layouts.

-------------
KeyNavigation
-------------

Qt Quick automatically builds a focus chain for the `Tab` key with all items that have
`activeFocusOnTab <https://doc.qt.io/qt-5/qml-qtquick-item.html#activeFocusOnTab-prop>`_ set to true.
The defaults for the ``activeFocusOnTab`` property are usually what you would expect.
There is no focus chain for the arrow keys, with some exceptions such as
`ListView <https://doc.qt.io/qt-5/qml-qtquick-listview.html>`_ when its
`keyNavigationEnabled <https://doc.qt.io/qt-5/qml-qtquick-listview.html#keyNavigationEnabled-prop>`_
property is set to true.

Each item has properties
`KeyNavigation.{backtab,down,left,right,tab,up} <https://doc.qt.io/qt-5/qml-qtquick-keynavigation.html>`_
that can point to another item to customize the focus behavior.
By default, KeyNavigation.* has less priority on key events than
`Keys.* <https://doc.qt.io/qt-5/qml-qtquick-keys.html>`_.
This is convenient when dealing with
`TextField <https://doc.qt.io/qt-5/qml-qtquick-controls2-textfield.html>`_, for instance.

KeyNavigation bindings are two-way: if b is the target of the KeyNavigation.down property of a,
then pressing the up key while b has active focus will move foucs to a.

---------------------
Non-focus scope items
---------------------

Some items, especially positioners and layouts, are not focus scopes, but it could be
useful to have them behave as one at times.
Unfortunately, there is no way to set the ItemIsFocusScope flag to an item from QML or C++
(except for custom types), so the only way to make these items behave as a focus scope is to
use ``forceActiveFocus()``. As a matter of example, here is code to make a StackLayout
behave like a focus scope::

    StackLayout {
        onActiveFocusChanged: if (activeFocus) children[currentIndex].forceActiveFocus()
    }

SLStackLayout implements this behavior in the org.symbolibre.controls module.

------------
Visual focus
------------

Controls have a
`visualFocus <https://doc.qt.io/qt-5/qml-qtquick-controls2-control.html#visualFocus-prop>`_
property that is supposed to represent keyboard focus more precisely than ``activeFocus``.
However we decided against using it in Symbolibre, as it often broke keyboard navigation
during our experiments with it.
