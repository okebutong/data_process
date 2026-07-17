QT += widgets
QT += serialport
QT += sql
QT += charts


CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


INCLUDEPATH += \
    logger/spdlog \
    logger/myLogger \
    .


SOURCES += \
    core/tlvparser/tlvparser.cpp \
    infrastructure/communication/serial/infra_serial.cpp \
    infrastructure/sql_manager/sql_manager.cpp \
    logger/myLogger/myloggers.cpp \
    main.cpp \
    mainwindow.cpp \
    repository/manager_storage/manager_storage.cpp \
    service/acquisition/service_acquisition.cpp \
    service/communication/service_comm.cpp \
    service/storage/service_storage.cpp \
    ui/comm_config/serial/page_serial_config.cpp \
    ui/history/page_history.cpp \
    ui/monitor/page_monitor.cpp


HEADERS += \
    core/tlvparser/tlvparser.h \
    infrastructure/communication/serial/infra_serial.h \
    infrastructure/sql_manager/sql_manager.h \
    mainwindow.h \
    model/config_serial.h \
    model/data_sensor.h \
    repository/manager_storage/manager_storage.h \
    service/acquisition/service_acquisition.h \
    service/communication/service_comm.h \
    service/storage/service_storage.h \
    ui/comm_config/serial/page_serial_config.h \
    ui/history/page_history.h \
    ui/monitor/page_monitor.h

FORMS += \
    mainwindow.ui \
    ui/comm_config/serial/page_serial_config.ui \
    ui/history/page_history.ui \
    ui/monitor/page_monitor.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    ../README.md \
    ../resources/image/cha.png \
    ../resources/image/dian.png


