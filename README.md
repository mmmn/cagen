CIA Auto Generator
==================

CIA Auto Generator is a non-portable script that was thrown 
together to speed up the generation of `.cia` files from `.3ds` files. It 
requires [Queno138's CIA conversion kit][1], and it **doesn't** come with 
`slot0x25KeyX.bin`.

All that it does is automatically follow all of the steps laid out by Queno138 in his [guide][1].
Even the unneccessary copying. It works out of the box, just put cagen.exe in the
root directory of [Queno138's kit][1].

Usage
=====
* Download and extract [Queno138's CIA conversion kit][1].
* Make sure `slot0x25KeyX.bin` is in both the `Step 1` and `For 3DS SD Card` folders.
* Download [`cagen.exe`][2]. Put it in the root directory of the conversion kit.
    the default name of that directory is `CIA`.
* Copy the `.3ds` file you want to convert to the root directory of the conversion kit.
    You may name it whatever you want.
* Drag the `.3ds` file onto `cagen.exe` to start the process.

[1]: https://gbatemp.net/threads/tutorial-converting-3ds-to-cia-for-dummies.373722/
[2]: https://github.com/mmmn/cagen/releases
