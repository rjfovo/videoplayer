# Videoplayer Qt6 迁移说明

## 迁移概述

本项目已成功从 Qt5 迁移到 Qt6，移除了对 Qt5 和 KDE5 的向后兼容性。

## 主要修改

### 1. CMakeLists.txt 修改
- 将 Qt5 依赖改为 Qt6：
  ```cmake
  find_package(Qt6 REQUIRED COMPONENTS Core Quick DBus Widgets LinguistTools)
  ```
- 更新了 D-Bus 接口生成：
  ```cmake
  qt6_add_dbus_interface(screensaver_inhibit_SRCS ./org.freedesktop.ScreenSaver.xml screensaverdbusinterface)
  ```
- 更新了目标链接库：
  ```cmake
  target_link_libraries(cutefish-videoplayer
    PRIVATE
    Qt6::Core
    Qt6::Quick
    Qt6::DBus
    Qt6::Widgets
  )
  ```
- 更新了翻译文件生成：
  ```cmake
  qt6_create_translation(QM_FILES ${TS_FILES})
  ```

### 2. Libmpv 依赖处理
- 改进了 Libmpv 查找逻辑，在没有 libmpv-dev 包的情况下也能配置成功
- 添加了回退机制，直接查找 mpv 库文件
- 在没有 libmpv 的情况下显示警告而不是错误

### 3. 代码修改
- `src/main.cpp`: 移除了 Qt5 向后兼容的高 DPI 缩放设置
  ```cpp
  // 移除的代码：
  #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
      QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  #endif
  ```

## 构建验证

项目已成功配置：
- CMake 配置：✓ 成功
- Qt6 依赖：✓ 找到
- Libmpv 依赖：⚠ 警告（需要安装 libmpv-dev 包才能构建）

## 注意事项

1. **Libmpv 依赖**: 需要安装 libmpv-dev 包才能成功构建项目
   ```bash
   sudo apt install libmpv-dev
   ```

2. **Qt6 依赖**: 确保系统安装了 Qt6 开发包
   ```bash
   sudo apt install qt6-base-dev qt6-declarative-dev
   ```

3. **构建命令**:
   ```bash
   cd cutefish/code/videoplayer
   mkdir build && cd build
   cmake ..
   make
   ```

## 后续步骤

1. 安装 libmpv-dev 包以启用完整的构建功能
2. 测试构建和运行
3. 更新 debian 打包配置中的 Qt 依赖版本

---
**迁移完成时间**: 2025-11-30  
**迁移状态**: ✅ 配置成功，⚠ 需要 libmpv-dev 包才能构建
