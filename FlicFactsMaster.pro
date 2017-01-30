TEMPLATE = subdirs
SUBDIRS = FlicsFacts Tests
Tests.depends = FlicsFacts

VERSION = 1.01
DEFINES += APP_VER=\\\"$$VERSION\\\"
