TARGET = qtgeoservices_osmscout
QT += location-private positioning-private network

PLUGIN_TYPE = geoservices
PLUGIN_CLASS_NAME = QGeoServiceProviderFactoryOsmScout
load(qt_plugin)

OTHER_FILES += \
    osmscout_plugin.json

HEADERS += \
    qgeoserviceproviderfactoryosmscout.h \
    qgeomappingmanagerengineosmscoutraster.h \
    qgeotilefetcherosmscout.h \
    qgeotiledmapreplyosmscoutraster.h \
    qosmscoutrenderer.h

SOURCES += \
    qgeoserviceproviderfactoryosmscout.cpp \
    qgeomappingmanagerengineosmscoutraster.cpp \
    qgeotilefetcherosmscout.cpp \
    qgeotiledmapreplyosmscoutraster.cpp \
    qosmscoutrenderer.cpp

LIBS += -losmscout -losmscoutmap -losmscoutmapqt
