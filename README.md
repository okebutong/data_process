# DataProcess — 嵌入式设备数据采集与管理系统

基于 **Qt 6 / C++17** 的上位机软件：通过串口接收嵌入式设备发送的 TLV 格式数据，实时解析、绘图展示，并持久化到 SQLite 数据库，支持历史数据查询。

## 功能特性

- **串口通信**：枚举 / 打开 / 关闭串口，可配置波特率（9600 / 57600 / 115200 等）
- **TLV 协议解析**：状态机逐字节解析，支持半包、粘包与校验错误检测
- **实时监控**：温度、湿度、电压、电流等数据的实时曲线与数值显示（Qt Charts）
- **数据存储**：解析结果批量写入 SQLite（`SensorData` 表）
- **历史查询**：按时间区间查询历史数据并绘制曲线
- **日志系统**：基于 spdlog 的异步日志（`MyLoggers` 封装）

## 技术栈

| 项目 | 说明 |
|------|------|
| 框架 | Qt 6（Widgets / SerialPort / Sql / Charts） |
| 语言 | C++17 |
| 构建 | qmake（`src/DataProcess.pro`） |
| 数据库 | SQLite |
| 日志 | spdlog（源码内置于 `src/logger/spdlog`） |

## 架构设计

采用 **UI → Service → Core / Repository → Infrastructure** 分层架构，配合独立的 Model（数据结构）与 Logger（日志）横切模块：

```
┌──────────────────────────────────────────────┐
│ UI        MainWindow / MonitorWidget /       │  界面与用户交互
│           HistoryWidget / SerialConfigPage   │
├──────────────────────────────────────────────┤
│ Service  Acquisition(总调度) / CommService / │  业务流程编排
│           Storage                            │
├───────────────────────┬──────────────────────┤
│ Core                  │ Repository           │
│ TlvParserWorker       │ ManagerStorage       │  核心算法 / 数据仓储
├───────────────────────┴──────────────────────┤
│ Infrastructure  SerialComm / SqlManager      │  外部设施封装（串口、SQLite）
└──────────────────────────────────────────────┘
```

### 线程模型

`Acquisition` 通过 `moveToThread` 将 **通信（CommService）**、**解析（TlvParserWorker）**、**存储（Storage）** 分别放入独立线程，跨线程通信全部走 Qt 信号槽（队列连接），主线程只负责 UI。

### 采集数据流

```
串口收到字节流 (SerialComm)
    → TlvParserWorker 状态机解析出 SensorData
        ├─→ Storage / ManagerStorage 批量入库 (SQLite)
        └─→ MonitorWidget 实时曲线刷新
```

## 目录结构

```
DataProcess/
    ├── core/tlvparser/         # TLV 协议解析（状态机）
    ├── infrastructure/
    │   ├── communication/serial/   # QSerialPort 封装（SerialComm）
    │   └── sql_manager/            # QSqlDatabase 连接封装（SqlManager）
    ├── logger/                 # MyLoggers 封装 + spdlog 源码
    ├── model/                  # SensorData / SerialConfig 数据结构
    ├── repository/manager_storage/ # 传感器数据批量入库与按时间查询
    ├── service/
    │   ├── acquisition/        # 总调度：创建管理三条工作线程
    │   ├── communication/      # 通信服务
    │   └── storage/            # 存储服务
    ├── ui/                     # 监控页、历史查询页、串口配置页
    ├── main.cpp / mainwindow.* # 程序入口与主窗口
    └── DataProcess.pro         # qmake 工程文件
```

## TLV 协议

完整帧格式：

```
+--------+--------+------+--------+---------+----------+
| header | Dev id | Type | Length | Value   | CheckNum |
+--------+--------+------+--------+---------+----------+
|   2    |   1    |   1  |   1    | N Byte  |    2     |
+--------+--------+------+--------+---------+----------+
```

- 帧头固定为 `AA 55`
- Type 为数据类型（0-10），Length 为 Value 字段字节数（1-0xFF）
- 校验方式为 CRC-16/MODBUS（查表法）

示例：`AA 55 02 03 02 03 07 76 BD` → 帧头 `AA 55`，设备 ID `02`，数据类型 `03`，Value 长度 2 字节，数据值 `03 07`，校验位 `76 BD`。



## 构建与运行

### 环境要求

- Qt 6.x（含 SerialPort、Sql、Charts 模块），项目开发使用 Qt 6.10.3 + MSVC 2022 64bit
- 支持 C++17 的编译器（MSVC 2022 / GCC 等）

### Qt Creator（推荐）

1. 打开 `src/DataProcess.pro`
2. 选择 Qt 6 构建套件（Kit）
3. 构建并运行

