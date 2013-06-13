To build this project, make sure that Qt is installed on your system.
You can download Qt from http://qt.nokia.com/downloads
On Linux, you can also install it using package manager.

Also you need to install or build QBS. Source code can be found at http://gitorious.org/qt-labs.
Instructions for building and using QBS are here http://doc-snapshot.qt-project.org/qbs/

To clone this repository, open terminal and run following commands from
the directory you want to use:
$ git clone git@gitorious.org:andromeda/libs.git
(OR '$ git clone git@github.com:ABBAPOH/libs.git')

To build project run this commands:
$ mkdir libs-build
$ cd libs-build
$ qbs install -f ../libs/libs.qbs release

QBS will create directory with the name of you default QBS profile and install
build libraries to the 'install_root' folder within that directory.
