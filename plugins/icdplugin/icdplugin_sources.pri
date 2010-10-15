DEFINES += ICD_LIBRARY

include(icdplugin_dependencies.pri)

HEADERS = $${PWD}/icdplugin.h \
    $${PWD}/icd_exporter.h \
    $${PWD}/icddatabase.h \
    $${PWD}/icddownloader.h \
    $${PWD}/icdwidgetmanager.h \
    $${PWD}/icdcontextualwidget.h \
    $${PWD}/icdcodeselector.h

SOURCES = $${PWD}/icdplugin.cpp \
    $${PWD}/icddatabase.cpp \
    $${PWD}/icddownloader.cpp \
    $${PWD}/icdwidgetmanager.cpp \
    $${PWD}/icdcontextualwidget.cpp \
    $${PWD}/icdcodeselector.cpp

TRANSLATIONS += $${SOURCES_TRANSLATIONS}/icdplugin_fr.ts \
                $${SOURCES_TRANSLATIONS}/icdplugin_de.ts \
                $${SOURCES_TRANSLATIONS}/icdplugin_es.ts

OTHER_FILES += $${SOURCES_RESOURCES_SQL}/icd10.sql

FORMS += \
    $${PWD}/icdcodeselector.ui
