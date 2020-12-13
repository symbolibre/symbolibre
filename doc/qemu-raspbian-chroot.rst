.. Copyright 2018-2020 Symbolibre authors <https://symbolibre.org>
.. SPDX-License-Identifier: CC-BY-SA-4.0
.. SPDX-License-Identifier: CC0-1.0

===================================================
Generating an OS from a static-QEMU Raspbian chroot
===================================================

.. warning::

   This page is a work in progress.

This page describes a setup where a Pi Zero OS image is generated from a
Raspbian system that runs through QEMU on an x86_64 host, allowing for
package installation/compilation/etc outside of the Pi, then installed
on the Pi’s SD card and boot up from the real hardware.

The OS image produced in this tutorial is just the minimum working OS. To
install a desktop environment and Symbolibre applications, follow with
:doc:`setting-up-symbolibre-apps`.

Basic Raspbian chroot
---------------------

First install a statically-linked version of QEMU that can be used to
transparently execute ARM executables on your x86_64 Linux install.

On Arch specifically
`[1] <https://wiki.archlinux.org/index.php/QEMU#Chrooting_into_arm/arm64_environment_from_x86_64>`__,
install ``binfmt-qemu-static`` and ``qemu-user-static`` (alternatively,
``qemu-user-static-bin`` to avoid compiling QEMU).
``binfmt-qemu-static`` will set up executable formats to use the
statically-linked QEMU in the binfmt service; but at the time it is
installed at a dependency, that QEMU is not available so it fails.
Restart the service manually once the static QEMU is installed.

::

   (host)% pacman -S binfmt-qemu-static qemu-user-static-bin
   (host)% systemctl restart systemd-binfmt

On Debian see `[2] <https://wiki.debian.org/QemuUserEmulation>`__. The
QEMU binaries might need to copied or mounted to the chroot (see the
appendix on the wiki page) which can be done elegantly with a bind-mount
(see later).

Create a starter Raspbian install with ``debootstrap`` on a new directory.
Because this is for a foreign architecture, only the first stage can be
executed natively. You can use a mounted separate partition or a plain
directory. ``--variant=minbase`` might provide a smaller image.

::

   (host)% debootstrap --foreign --arch=armhf bullseye symbolibre-os/ https://archive.raspbian.org/raspbian/

The second stage of debootstrap must be executed from the
``/debootstrap/debootstrap`` executable left in the new system. This can
be done through transparent QEMU emulation. However, the shared
``armhf`` libraries are also in the new system, so the root must be
changed for dynamic loading to succeed.

Before chrooting, on some distros the QEMU binaries should be mounted
into the new system.

::

   (host)% mount --bind /usr/bin/qemu-arm-static symbolibre-os/usr/bin

Then log into Raspbian.

::

   (host)% sudo chroot symbolibre-os /bin/env -i TERM=$TERM /bin/bash

``env -i`` clears the host environment variables. The ``TERM`` environment
variable is preserved as it allows the capabilities of your terminal emulator
to be properly identified. If your terminal is not supported by the
newly-installed OS, you can always set ``TERM=linux``.

Once in the Raspbian chroot, complete the system boostrap.

::

   % debootstrap/debootstrap --second-stage

Add a new standard user (``symbolibre`` with password ``symbolibre``).

::

   % adduser symbolibre

That’s it for the base system.

Adding some detail
------------------

Because the guest system is not fully booted up from scratch with
systemd, a few things don’t work out of the box (for instance there is no
``/proc``). On Arch, one can use arch-chroot or manually bind the required
filesystems into the chroot. See
`[3] <https://wiki.archlinux.org/index.php/Chroot#Usage>`__ for both
options. Accessing dbus in this setup will affect your host system, so be
careful.

For now, set the hostname in ``/etc/hostname`` and update ``/etc/hosts``
so that ``sudo`` doesn't complain about not being able to resolve it. Don’t use
the ``hostname`` or ``hostnamectl`` tools as it will affect the host system.
The name should already be set properly when booting from the Pi Zero.

::

   % echo slcalc | tee /etc/hostname

``/etc/hosts`` should look like this.

::

   127.0.0.1   localhost
   127.0.0.1   slcalc
   (...)

I suggest using a script to start the chroot (launch with ``sudo``):

.. code:: bash

  #! /usr/bin/env bash
  # usage: sudo ./symbolibre-os.sh [--mount-qemu] [--arch-chroot]

  mount_qemu=
  use_arch_chroot=

  for arg in "$@"; do
    case "$arg" in
      "--mount-qemu") mount_qemu=1;;
      "--arch-chroot") use_arch_chroot=1;;
      *) echo "unrecognized argument $arg" >&2; exit 1
    esac
  done

  # Use a basic standard terminal
  export TERM=linux
  # Set the proper host name
  export HOSTNAME="slcalc"

  # Mount QEMU executables into the chroot if needed
  if [[ ! -z "$mount_qemu" ]]; then
    mount --bind /usr/bin/qemu-arm-static symbolibre-os/usr/bin
  fi

  # arch-chroot version: best to have the target directory a mount point, so if
  # it's not make it one artifically
  if [[ ! -z "$use_arch_chroot" ]]; then
    mount --bind symbolibre-os/ symbolibre-os/
    env -i TERM="$TERM" HOSTNAME="$HOSTNAME" arch-chroot symbolibre-os/
    umount symbolibre-os/
    exit 0
  fi

  # Normal chroot. Debian will supply a default PATH
  env -i TERM="$TERM" HOSTNAME="$HOSTNAME" chroot symbolibre-os/

Now before we can install packages we need to keep the system up-to-date with
the repositories.

::

  % apt update
  % apt upgrade

If the upgrade step complains of unmet dependencies, run
``apt --fix-broken install`` as advertised before upgrading again.

Kernel and boot from Pi Zero
----------------------------

The system is now mostly complete; we still need to install a kernel, an
fstab, and a couple other files to get it to boot for real instead of
relying on the host’s fully-booted environment. All this section is run
from the host x86_64 Linux.

Option 1: Leeching from Raspberry Pi OS
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This option grabs the kernel and modules of Raspberry Pi OS. Download a
Raspberry Pi OS Lite image `from the official download
page <https://www.raspberrypi.org/downloads/raspberry-pi-os/>`__ and
copy it the SD card so that we can snatch the kernel and some files out
of it.

::

   % sudo dd if=2020-05-27-raspios-buster-lite-armhf.img of=<device> bs=4M

Then copy out the stuff we need. ``rsync`` is used here to keep the file
hierarchy. We save the following files:

-  ``/lib/modules``, which contains the kernel modules;
-  ``/etc/fstab``;
-  ``raspi-config``, which is used as init script during the first boot.

The first include ``*/`` makes sure every directory is traversed, which
is needed to find subdirectories. ``--prune-empty-dirs`` removes unused
directories at the end. Paths with interesting files are included and
everything else is excluded at the end with ``*``.

::

   # Set MOUNTPOINT to the path of the mounted root partition of Raspberry Pi OS
   % MOUNTPOINT="/path/to/rootfs"
   % mkdir rpios-root
   % sudo rsync -a --prune-empty-dirs --include='*/' --include='lib/modules/***' --include=etc/fstab -r --include='usr/lib/raspi-config/***' --include=usr/bin/raspi-config --exclude='*' $MOUNTPOINT/ rpios-root/

It seems that of the subfolders of ``lib/modules``, only the one named
``<x>.<y>.<z>+`` is used. Not copying the others saves about 200 MB.

With that saved, reset the root partition with a copy of the chrooted
Raspbian install and restore the saved files. The ``-K`` option to
``rsync`` ensures that ``/lib/modules`` is copied correctly with respect
to ``/lib`` being a symlink to ``/usr/lib`` in Raspbian (ie. ``modules``
is copied into ``/usr/lib``; the default behavior replaces the symlink
with a new directory).

::

   % sudo rm -rf $MOUNTPOINT/*
   % sudo cp -ra ../symbolibre-os/* $MOUNTPOINT/
   % sudo rsync -avK rpios-root/ $MOUNTPOINT/

Then check that the fstab has correct partition UUIDs and modify it
accordingly if needed (this will be needed in particular if getting the
fstab file from another SD card). Look up the UUIDs with ``lsblk``.

::

   % lsblk -o +PARTUUID <device>
   NAME   MAJ:MIN RM  SIZE RO TYPE MOUNTPOINT      PARTUUID
   sdc      8:32   1  7.4G  0 disk
   ├─sdc1   8:33   1  256M  0 part                 2fed7fee-01
   └─sdc2   8:34   1  1.5G  0 part /path/to/rootfs 2fed7fee-02

Then the fstab should look like this.

::

   proc            /proc           proc    defaults          0       0
   PARTUUID=2fed7fee-01  /boot           vfat    defaults          0       2
   PARTUUID=2fed7fee-02  /               ext4    defaults,noatime  0       1
   # a swapfile is not a swap partition, no line here
   #   use  dphys-swapfile swap[on|off]  for that

Finally, put the SD card into the Raspberry Pi and boot.

Option 2: From official firmware with pre-built kernel
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Start from a blank microSD card and make a new partition table.

.. warning::

   All existing data on the SD card will be lost.

1. A boot partition of at least 100 Mo, type ``W95 FAT32 (LBA)`` (type ID is
   ``0x0c``);
2. A root partition of at least 1.5 Go, type ``Linux`` (type ID is ``0x83``),
   preferably with an ext4 filesystem.

The instructions that follow use the command-line tool ``parted`` for partitioning,
but more user-friendly partitioning tool can also be used: ``gparted``, ``cfdisk``, etc.
The exact partition sizes do not matter.

"""""""""""""""""""""""""""""
Partitioning using ``parted``
"""""""""""""""""""""""""""""

The first partition should not start at sector 0 because space is needed
for the partition table. We start at sector 8192, which is what
Raspberry Pi OS does. The first partition then stops at sector 532479
and the second partition starts at 532480, going all the way to 100% of
the card size.

::

   % sudo parted <device>
   (parted) mklabel msdos
   (parted) mkpart primary fat32 8192s 532479s
   (parted) mkpart primary ext4 532480s 100%

The disk identifier changes when the partition table is changed, so make
sure to update the partition UUIDs if you’re going to reuse a previous
version of ``/etc/fstab``.

Create the file systems, for instance here with ``/dev/mmcblk0`` as a
device, and give useful names at the same time.

::

   % sudo mkfs.fat -F 32 -n slboot /dev/mmcblk0p1
   % sudo mkfs.ext4 -L slroot /dev/mmcblk0p2

Now get the repository with the Raspberry Pi boot files and kernel modules
`from Github <https://github.com/raspberrypi/firmware/archive/master.tar.gz>`__
(~400M once uncompressed).

Mount both partitions and copy the boot folder to /boot. Add in a
``cmdline.txt``. It can be derived from the
Raspberry Pi OS iso, though ``config.txt`` is almost empty. Remember to
change the PARTUUID to the correct ID in ``cmdline.txt``.

::

   % MOUNT_BOOT="/path/to/slboot"
   % MOUNT_ROOT="/path/to/slroot"
   % sudo cp -r firmware/boot/* $MOUNT_BOOT
   % echo "console=serial0,115200 console=tty1 root=PARTUUID=$(sudo blkid -s PARTUUID -o value /dev/mmcblk0p2) rootfstype=ext4 elevator=deadline fsck.repair=yes rootwait" | sudo tee $MOUNT_BOOT/cmdline.txt

The boot partition is now complete. For the root partition, combine the
chrooted Raspbian install with the kernel modules from the ``firmware``
directory and a copy of ``/etc/fstab`` from the previous section.

::

   % sudo cp -ra ../symbolibre-os/* $MOUNT_ROOT/
   % sudo cp -r firmware/modules $MOUNT_ROOT/usr/lib/
   # also copy the fstab

That’s it. Plug the SD card into the Raspberry Pi and boot.

Option 3: From Raspbian packaged firmware and kernel
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

TODO! General roadmap:

* Start ``/boot`` from scratch with
  `raspberrypi/firmware <https://github.com/raspberrypi/firmware/>`_, or use the
  Raspbian package
  `raspberry-pi-firmware-nokernel <http://archive.raspbian.org/raspbian/pool/firmware/r/raspberrypi-firmware-nokernel/>`_.
* Compile the kernel and add the ``kernel.img`` and ``config.txt``.

Updating the OS from now on
---------------------------

The boot partition that we just created is independent from the OS root. The OS
can be updated from the chroot by just synchronizing the contents of the root
partition.

::

   % rsync -a --progress --delete --exclude etc/fstab --exclude usr/lib/modules --exclude var/cache/apt symbolibre-os/ $MOUNT_ROOT/

This method can be used even with different OS variants and chroots as long as
they all agree on the kernel version and boot settings.

