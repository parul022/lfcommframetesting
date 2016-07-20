QT += core network
QT -= gui
CONFIG += console qt

TEMPLATE = app
TARGET = TestLFCommFrame

INCLUDEPATH += . $(BOOST_ROOT) /home/lfuser/work/sharedheaders
QMAKE_CXXFLAGS += -std=c++11 -rdynamic
QMAKE_LFLAGS = -Wl,-O1 -Wl,--hash-style=gnu
LIBS = -L/usr/lib/x86_64-linux-gnu -llog4cxx -llfcomm -lunits -lgtest -lpthread

OBJECTS_DIR = .obj
MOC_DIR	= .obj

target.path = /usr/bin 
INSTALLS += target

xml.path = /usr/share/xml
xml.files = ParulTestConfig.xml
INSTALLS += xml

SOURCES = LFCommFrameTestFixture.cpp

HEADERS = 

