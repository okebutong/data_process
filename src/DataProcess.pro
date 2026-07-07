QT += widgets
QT += serialport
QT += sql
QT += charts


CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


INCLUDEPATH += \
    logger/ \
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
    logger/myLogger/myloggers.h \
    logger/spdlog/async.h \
    logger/spdlog/async_logger-inl.h \
    logger/spdlog/async_logger.h \
    logger/spdlog/cfg/argv.h \
    logger/spdlog/cfg/env.h \
    logger/spdlog/cfg/helpers-inl.h \
    logger/spdlog/cfg/helpers.h \
    logger/spdlog/common-inl.h \
    logger/spdlog/common.h \
    logger/spdlog/details/backtracer-inl.h \
    logger/spdlog/details/backtracer.h \
    logger/spdlog/details/circular_q.h \
    logger/spdlog/details/console_globals.h \
    logger/spdlog/details/file_helper-inl.h \
    logger/spdlog/details/file_helper.h \
    logger/spdlog/details/fmt_helper.h \
    logger/spdlog/details/log_msg-inl.h \
    logger/spdlog/details/log_msg.h \
    logger/spdlog/details/log_msg_buffer-inl.h \
    logger/spdlog/details/log_msg_buffer.h \
    logger/spdlog/details/mpmc_blocking_q.h \
    logger/spdlog/details/null_mutex.h \
    logger/spdlog/details/os-inl.h \
    logger/spdlog/details/os.h \
    logger/spdlog/details/periodic_worker-inl.h \
    logger/spdlog/details/periodic_worker.h \
    logger/spdlog/details/registry-inl.h \
    logger/spdlog/details/registry.h \
    logger/spdlog/details/synchronous_factory.h \
    logger/spdlog/details/tcp_client-windows.h \
    logger/spdlog/details/tcp_client.h \
    logger/spdlog/details/thread_pool-inl.h \
    logger/spdlog/details/thread_pool.h \
    logger/spdlog/details/udp_client-windows.h \
    logger/spdlog/details/udp_client.h \
    logger/spdlog/details/windows_include.h \
    logger/spdlog/fmt/bin_to_hex.h \
    logger/spdlog/fmt/bundled/args.h \
    logger/spdlog/fmt/bundled/base.h \
    logger/spdlog/fmt/bundled/chrono.h \
    logger/spdlog/fmt/bundled/color.h \
    logger/spdlog/fmt/bundled/compile.h \
    logger/spdlog/fmt/bundled/core.h \
    logger/spdlog/fmt/bundled/format-inl.h \
    logger/spdlog/fmt/bundled/format.h \
    logger/spdlog/fmt/bundled/os.h \
    logger/spdlog/fmt/bundled/ostream.h \
    logger/spdlog/fmt/bundled/printf.h \
    logger/spdlog/fmt/bundled/ranges.h \
    logger/spdlog/fmt/bundled/std.h \
    logger/spdlog/fmt/bundled/xchar.h \
    logger/spdlog/fmt/chrono.h \
    logger/spdlog/fmt/compile.h \
    logger/spdlog/fmt/fmt.h \
    logger/spdlog/fmt/ostr.h \
    logger/spdlog/fmt/ranges.h \
    logger/spdlog/fmt/std.h \
    logger/spdlog/fmt/xchar.h \
    logger/spdlog/formatter.h \
    logger/spdlog/fwd.h \
    logger/spdlog/logger-inl.h \
    logger/spdlog/logger.h \
    logger/spdlog/mdc.h \
    logger/spdlog/namespace.h \
    logger/spdlog/pattern_formatter-inl.h \
    logger/spdlog/pattern_formatter.h \
    logger/spdlog/sinks/android_sink.h \
    logger/spdlog/sinks/ansicolor_sink-inl.h \
    logger/spdlog/sinks/ansicolor_sink.h \
    logger/spdlog/sinks/base_sink-inl.h \
    logger/spdlog/sinks/base_sink.h \
    logger/spdlog/sinks/basic_file_sink-inl.h \
    logger/spdlog/sinks/basic_file_sink.h \
    logger/spdlog/sinks/callback_sink.h \
    logger/spdlog/sinks/daily_file_sink.h \
    logger/spdlog/sinks/dist_sink.h \
    logger/spdlog/sinks/dup_filter_sink.h \
    logger/spdlog/sinks/hourly_file_sink.h \
    logger/spdlog/sinks/kafka_sink.h \
    logger/spdlog/sinks/loki_sink.h \
    logger/spdlog/sinks/mongo_sink.h \
    logger/spdlog/sinks/msvc_sink.h \
    logger/spdlog/sinks/null_sink.h \
    logger/spdlog/sinks/ostream_sink.h \
    logger/spdlog/sinks/qt_sinks.h \
    logger/spdlog/sinks/ringbuffer_sink.h \
    logger/spdlog/sinks/rotating_file_sink-inl.h \
    logger/spdlog/sinks/rotating_file_sink.h \
    logger/spdlog/sinks/sink-inl.h \
    logger/spdlog/sinks/sink.h \
    logger/spdlog/sinks/stdout_color_sinks-inl.h \
    logger/spdlog/sinks/stdout_color_sinks.h \
    logger/spdlog/sinks/stdout_sinks-inl.h \
    logger/spdlog/sinks/stdout_sinks.h \
    logger/spdlog/sinks/syslog_sink.h \
    logger/spdlog/sinks/systemd_namespace_sink.h \
    logger/spdlog/sinks/systemd_sink.h \
    logger/spdlog/sinks/tcp_sink.h \
    logger/spdlog/sinks/udp_sink.h \
    logger/spdlog/sinks/win_eventlog_sink.h \
    logger/spdlog/sinks/wincolor_sink-inl.h \
    logger/spdlog/sinks/wincolor_sink.h \
    logger/spdlog/spdlog-inl.h \
    logger/spdlog/spdlog.h \
    logger/spdlog/stopwatch.h \
    logger/spdlog/tweakme.h \
    logger/spdlog/version.h \
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


