#include "myloggers.h"

spdlog::level::level_enum MyLoggers::global_level = spdlog::level::info;

spdlog::level::level_enum MyLoggers::getGlobalLevel()
{
    return global_level;
}

std::vector<spdlog::sink_ptr> MyLoggers::createSinks(const std::string &log_file_name)
{
    std::vector<spdlog::sink_ptr> sinks;

    auto sink1 = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    sink1->set_level(MyLoggers::getGlobalLevel());
    sink1->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] [%n]:%v");
    sinks.push_back(sink1);

    auto sink2 = std::make_shared<spdlog::sinks::daily_file_sink_mt>(log_file_name, 4,20);
    sink2->set_level(spdlog::level::trace);

    sink2->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l]:%v");
    sinks.push_back(sink2);
    return sinks;
}

void MyLoggers::createLogger(const std::string &logger_name)
{
    std::string log_file_name = "log_" + logger_name + ".txt";
    auto sinks = MyLoggers::createSinks(log_file_name);

    auto logger = std::make_shared<spdlog::logger>(logger_name, begin(sinks), end(sinks));
    logger->set_level(global_level);
    spdlog::register_logger(logger);
}

std::shared_ptr<spdlog::logger> MyLoggers::getLogger(const std::string &logger_name){
    auto logger = spdlog::get(logger_name);
    if(!logger){//looger指向为空
        spdlog::critical("no such log name {}", logger_name);
    }
    return logger;
}



void MyLoggers::init(const std::string &logger_name)
{
    createLogger(logger_name);
}
