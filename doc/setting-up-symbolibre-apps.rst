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

To install X with i3:

::

  % apt install xinit
  % apt install i3
  # For safety, install a couple more fonts over xfonts-base
  % apt install xfonts-unifont xfonts-75dpi

i3 can be started from the console by running ``startx``. The logs are stored
in ``/var/log/Xorg*.log``. Copy the default configuration to the user home.

::

  % mkdir -p $HOME/.config/i3
  % cp /etc/i3/config $HOME/.config/i3/

Alternatively, to install Wayland with sway:

::

  % apt install sway
  % apt install xwayland

sway can be started from the console by running ``sway``. A log can be obtained
with ``sway -d 2> sway.log``.

Configuring graphics acceleration
---------------------------------

The Raspberry Pi series come with a GPU of the
`VideoCore family <https://en.wikipedia.org/wiki/VideoCore>`_, and the Pi Zero
GPU in particular is based on VideoCore 4. The Linux open-source driver for VC4
is `maintainted by Eric Anholt <https://github.com/anholt/linux/wiki/VC4>`_ and
integrated into MESA since 2014.

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

The VideoCore GPU, as an embedded architecture, supports the
`OpenGL ES <https://en.wikipedia.org/wiki/GLES>`_ API.
