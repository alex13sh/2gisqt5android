CONFIG += testcase
testcase.timeout = 1100 # this test can be slow
TARGET = tst_qquickgridview
macx:CONFIG -= app_bundle

SOURCES += tst_qquickgridview.cpp

include (../../shared/util.pri)
include (../shared/util.pri)

TESTDATA = data/*

QT += core-private gui-private  qml-private quick-private testlib

win32:CONFIG += insignificant_test # QTBUG-33017
macx:CONFIG += insignificant_test # QTBUG-33017

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0