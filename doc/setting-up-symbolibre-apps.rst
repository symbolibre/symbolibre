.. Copyright 2018-2020 Symbolibre authors <https://symbolibre.org>
.. SPDX-License-Identifier: CC-BY-SA-4.0
.. SPDX-License-Identifier: CC0-1.0

=============================================
Setting up the Symbolibre OS and applications
=============================================

This page describes the installation of a desktop environment along with the
Symbolibre applications on a Raspbian image. To generate the base system, see
:doc:`qemu-raspbian-chroot`.

The main parts here are:

* Installing a desktop environment: either X with i3, or Wayland with sway
* Setting up graphics acceleration
* Installing Qt libraries and QML modules
* Compiling and running Symbolibre apps

Quality-of-life settings
------------------------

Adding a user
~~~~~~~~~~~~~

Graphical applications will run under an unprivileged user, so let’s create one.

::

  % adduser symbolibre

You can then go into user mode with ``su - symbolibre``.

``su`` should be sufficient to switch between users but it can be convenient to
set up ``sudo`` if you are used to it.

::

  % apt install sudo
  % usermod -aG sudo symbolibre

Keyboard layout
~~~~~~~~~~~~~~~

If you don't have a QWERTY keyboard, you might want to change the layout of the
TTY console. You can do so in the post-install script of ``console-tools``.

::

  % apt install console-tools
  # ... follow instructions to select a keyboard layout

* For the French AZERTY keyboard, select 22 (Other) then 37 (French) then 1
  (French).

If prompted, you should set UTF-8 as the encoding on the console (27).

Generate locales
~~~~~~~~~~~~~~~~

If you plan on using any other locale than the default C/English, edit the list
in ``/etc/locale.gen`` and run ``locale-gen``. You should always prefer UTF-8
locales when available.

::

  % apt install locales
  # edit /etc/locale.gen to uncomment desired locales
  % locale-gen

Aliases and commodities
~~~~~~~~~~~~~~~~~~~~~~~

If you plan on using applications other than the Symbolibre default or a
terminal, this is a good time to configure core utilities in ``.bashrc``.

.. code:: bash

  alias ls='ls --color=auto --group-directories-first'
  alias ll='ls -lh'

  function mkcd {
    mkdir "$1"
    cd "$1"
  }

Text editor
~~~~~~~~~~~

The default text editor is ``nano``. Any alternative can be installed; don't
forget to set the ``EDITOR`` and ``VISUAL`` variables accordingly. For instance
with vim:

::

  % apt install vim
  % echo "export EDITOR=vim" | tee -a $HOME/.bashrc
  % echo "export VISUAL=vim" | tee -a $HOME/.bashrc

Installing a desktop environment
--------------------------------

With X and i3
~~~~~~~~~~~~~

*Warning: The X server seems to cope poorly with the modesetting driver used
on the Raspberry Pi (the VC4 KMS driver). Clients can be accelerated but
applications that render with X (such as xterm) and the window compositor are
very slow.*

The first option is to install X with `i3 <https://i3wm.org/>`_. The default
configuration file is copied to its standard location in the user home.

::

  % apt install xinit i3
  # For safety, install a couple more fonts over xfonts-base
  % apt install xfonts-unifont xfonts-75dpi
  % mkdir -p $HOME/.config/i3
  % cp /etc/i3/config $HOME/.config/i3/

i3 can be started from the console by running ``startx``. The logs are stored
in ``/var/log/Xorg*.log`` or ``$HOME/.local/share/xorg/Xorg*.log``. The default
terminal is ``xterm``.

With Wayland and sway
~~~~~~~~~~~~~~~~~~~~~

Alternatively, Wayland can be installed with `sway <https://swaywm.org/>`_. Once again
the configuration file is copied. The settings are very close to i3; the
default terminal is a Wayland-native terminal called ``foot``.

::

  % apt install sway xwayland
  % mkdir -p $HOME/.config/sway
  % cp /etc/sway/config $HOME/.config/sway

sway can be started from the console by running ``sway``. A log can be obtained
with ``sway -d 2> sway.log``. If you don't have a mouse on the Pi Zero, you
might want to change the sway exit shortcut to not required cliking on a
confirm button.

::

  bindsym $mod+Shift+E exec swaymsg exit

``foot`` doesn't start if it's no using an Unicode locale, so you should set
one in ``$HOME/.bashrc``. Additionally, sway overrides the keyboard layout so
it should be specified before starting as well.

::

  export LANG=en_US.UTF-8
  # Example for the French AZERTY keyboard
  export XKB_DEFAULT_LAYOUT=fr
  export XKB_DEFAULT_VARIANT=basic

Configuring graphics acceleration
---------------------------------

The Raspberry Pi series come with a GPU of the
`VideoCore family <https://en.wikipedia.org/wiki/VideoCore>`_, and the Pi Zero
GPU in particular is based on VideoCore 4. The Linux open-source driver for VC4
is `maintainted by Eric Anholt <https://github.com/anholt/linux/wiki/VC4>`_ and
integrated into MESA since 2014. It offers a mode-setting interface through
Linux's KMS.

The first step is to enable the VC4 module in the boot configuration and
allocate enough memory to the GPU. The 512 MiB Pi memory is shared between CPU
and GPU so anything that is specified as GPU memory is not available for
applications.

In ``/boot/config.txt``, load the device tree overlay that assigns the VC4
driver to the GPU and add some GPU memory.

::

  dtoverlay=vc4-kms-d3d
  gpu_mem=128

For user processes to be able to access the GPU (which is for instance needed
by ``sway`` since Wayland compositors don't run as root), ``symbolibre`` should
be a member of the ``video`` group.

::

  % usermod -a -G video symbolibre

The group change takes effect at login and the boot configuration at boot, so
now is a good time to shut down and restart the Pi.

When you log in with this new setup, the ``vc4`` driver should have been loaded
automatically; ``lsmod | grep vc4`` should show it along with its dependencies.

Both X and Wayland will automatically use this new driver to perform rendering.
X will select the ``modesetting`` driver and enable acceleration:

* For the server, Glamor will be enabled on the KMS device.
* For clients, DRI2 will be enabled, letting applications perform GPU rendering
  without talking to the server.

However, as mentioned earlier, the Glamor half of that setup performs very
poorly in our test setups. This makes window movement and resizing, as well as
rendering in the server, very slow (even slower than with software rendering as
provided by the ``fbdev`` driver). Applications like ``glxgears`` that use DRI2
run very smoothly but applications that render through the X server like
``xterm`` are extremely slow.

On Wayland, sway will auto-detect the GPU at ``/dev/dri/card0`` (the ``dri``
here has nothing to do with the X server's DRI2 technology and just represents
the GPU) and use KMS with VC4 without any other configuration.

To test client rendering, you can install the ``mesa-utils`` package, which
provides tools to check the configuration and performance of the display, such
as ``glxinfo`` and ``glxgears``.

::

  % apt install mesa-utils

Native Wayland applications or X applications that support DRI through XWayland
will use accelerated rendering. This is for instance the case with ``foot``,
but not with ``xterm``, so not every program will run smoothly even with sway.
