What is Manpager
================

This is a simple application that lets you search for and view system manual pages formatted for normal output.  This application is written in C++ using [Qt](https://www.qt.io).  The manpage viewer also lets you search for content within manual pages using the F3 key.  Probably the closest equivalent is something like xman, but this offers a more complete and modern user interface as well as the ability to actively search for pages.

While Manpager is a cross-platform desktop application, generally this application has much more limited use on Microsoft Windows vs other platforms, because both mac and unix targets have standardized manual pages, whereas Microsoft Windows does not.  However, it could be useful in conjunction with mingw32, cygwin, or the Windows bash shell, to more easily search and view the manual pages in those subsystems on the Windows desktop.

History
=======

Manpager first appeared as "coastal manpager" in coastal-qt in 2011.  Back then I was experimenting with cross-platform Qt desktop applications and cmake. This is a stand-alone derived version of coastal manpager, to make it easier to separately maintain, and to experiment with what I feel are good practices for using Qt and QtCreator for cross-platform development.  Manpager is licensed using the GNU GPL, Version 3 or later, as was it's immediate predecessor, coastal-qt.

Install
=======

On Mac I presume Manpager can be delivered as a fully stand-alone bundled application, complete with required Qt runtime libraries.  This may then be bundled together and distributed with other desktop apps in a common .dmg installer.  This project can produce a stand-alone application that can be used and redistributed directly and separately if so desired.  This package can be setup to build thru macports or homebrew as well.

For Microsoft Windows use I produce a deploy archive directory which includes Qt runtime dll libraries.  The deploy archive directory from this and related applications are then gathered together, removing duplication of runtime dll's, and will normally be delivered in a common .exe installer using inno setup.  The archive directory from this project could also be used to create a stand-alone windows binary distribution of just this application.

For generic Unix (GNU/Linux, BSD systems, etc), the publish target provides a clean source tarball.  **qmake** can be directly ran to configure the build from a source tarball, and **make** to produce binaries, without needing qtcreator.  A generic **make install** can then be used to install the result.  I also may produce a generated .spec file from qmake which can then be used to produce a .rpm package which can then be loaded on a build system like obs.  For BSD systems I recommend creating a "ports" file.

Support
=======

At the moment I do not have infrastructure to offer support.  I have published this, and perhaps as of this writing, other packages, mostly to experiment with and facilitate best practices for future work.  I do maintain an email address for public contact for all similarly published projects as [tychosoft@gmail.com](mailto://tychosoft@gmail.com).
