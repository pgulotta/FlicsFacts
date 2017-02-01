TEMPLATE = subdirs
SUBDIRS = FlicsFacts Tests
Tests.depends = FlicsFacts

DEFINES += APP_VER=\\\"$$VERSION\\\"
