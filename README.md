edhelind
========

**edhelind** is an ELF file viewer tool. It provides a graphical user interface
to view the internals of an Executable and Linkable Format file, the files used
on a number of operating systems for their executable programs and shared
libraries.

This is a useful tool for developers who need to examine the contents of their
binaries, for casual users who are curious about "what's under the hood" of their
computer, and for anyone in general who just wants to see the inside of a binary
program without memorizing hard-to-find *de facto* standards documents.

**edhelind** can be used to view ELF executable binaries, dynamic sharted objects
(`.so` files), object files (`.o` files), and core dump files.

How is **edhelind** Different?
------------------------------

There are a number of tools for viewing ELF files that are in widespread use.
The goal of **edhelind** was to provide a simple-to-use tool, portable to a
number of popular development hosts, that could be used to view the ELF files
for any arbitrary target regardless of target OS or CPU.

To that end, **edhelind** was developed against the following requirements.

  - standalone definitions of ELF structure (no reliance on host-provided elf.h)
  - no reliance on host libraries
  - builds and runs on Microsoft Windows, Mac OS, and POSIXish systems like Linux

In order to satisfy those requirements, the only dependencies are the Qt
framework for the GUI and the C++17 standard library for non-GUI stuff.

How to Build **edhelind**
-------------------------

To build **edhelind** you will need the following installed in your build
environment (minimum version specified or later).

    On Linux: GCC 9, CMake 5.3, Qt core and widgets 5.5 or
    later.

    On Mac OS: LLVM 10, CMake 5.3, Qt Core and Widgets 5.5

    On Windows: MSVC 2017, CMake 5.3, Qt Core and Widgets 5.5

On Linux you can just check out the sources and build with the following
snippet.

```
$ git clone https://github.com/bregma/edhelind.git
$ cd edhelind
$ mkdir build && cd build
$ cmake ..
$ cmake --build .
```

Similar recipes can be used on Mac OS or Microsoft Windows, depending on your
preferences.

Architectural Notes
-------------------

Edhelind is divided into two parts: `libedhel` containing all the goodness that
opens, reads, and interprets ELF files, and `edhelind` itself, which is a
Qt-based GUI application that display the ELF file by using `libedhel`.  The
library depends only on the C++ standard library and could be used to create
command-line tools with no extra dependencies.

An elf file is strictly a sequence of bytes: those bytes may represent
big-endian or little-endian words, and the structures may be interpreted using
32-bit or 64-bit layouts depending on the target system.  `libedhel` literally treats
the underlying ELF file as a sequence of bytes (using C++'s `std::byte` type). It
determines word endianness and structure layouts by reading the first few bytes
of the ELF file and then relies on that interpretation when retrieving
information on demand.

There are a large number of ABI-, OS- and CPU-specific data that can be stored
in an ELF file.  The intention is to be able to configure various interpretive
personalities to match the target. 

Origins of the Name **edhelind**
--------------------------------

The phrase "edhel ind" is a Sindarin phrase that translates roughly into English as "the
innermost thoughts of an Elf." This is a play on the Extensible Linking Format,
or ELF, being a homonym for J. R. R. Tolkein's Elves, a species of homonid for
which he invented several languages.

A quick search of the internet shows more than a handful of projects called some
variation of "ELF Viewer" so a simple, functional English name is out of the
question.  Reaching out to a Sindarin transation of several useful phrases
related to the inner workings of elves (there are not a large lexicon of Elvish
terms for anatomical structures), I came up with "edhel ind," which can be
mushed together into a pronouncable word, hence *edhelind*.  It may not be valid
Sindarin and may irk some hardcore LotR filk but really, it's just an arbitrary
name to uniquely identify a software program. Don't take made-up stuff so
seriously.

Licensing
---------

**edhelind** is licensed under the GNU General Public License (version 3 or
later) [GPLv3](https://www.gnu.org/licenses/gpl-3.0.en.html).

The Catch2 test framework is licensed under the Boost Software
License 1.0 [BSL-1.0](https://github.com/catchorg/Catch2/blob/master/LICENSE.txt).

The Qt library is licensed under the GNU Lesser General Public License (version
3) [LGPLv3](https://doc.qt.io/qt-5/lgpl.html).
