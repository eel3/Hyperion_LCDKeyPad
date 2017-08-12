Hyperion_LCDKeyPad
==================

Arduino library allows communication with Hyperion LCD KeyPad Shield (HP-AMO-LCDSH).

This library allows an Arduino/Genuino board to control Hyperion LCD KeyPad Shield (HP-AMO-LCDSH).

License
-------

zlib License.

Development environments
------------------------

* Arduino IDE 1.8.3.
* Arduino UNO R3.

Set up
------

1. Create a new directory `libraries` in your sketch directory.
    * For example, on Windows 7 or later, it would be `%UserProfile%\Documents\Arduino\libraries`.
2. Clone this repository in the directory above.
```dosbatch
:: Perhaps your OS is Windows 7 or later.
:: Perhaps you are in the `%UserProfile%`.

pushd Documents\Arduino
if not exist libraries mkdir libraries
pushd libraries
git clone git@github.com:eel3/Hyperion_LCDKeyPad.git
```

Usage
-----

See [examples/LCDKeyPadDemo](examples/LCDKeyPadDemo).
