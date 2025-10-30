# Project-core

📦 **STM32F103C8T6 气象仪主控工程**

---

## 🧩 项目简介
本项目基于 **CMake + ARM GNU Toolchain** 构建，使用软件 SPI 驱动 1.8 寸 TFT LCD，用于气象仪数据展示。  
主要外设包括：
- DHT11 温湿度传感器  
- DS18B20 温度探头  
- HW611 气压传感器  
- TLC555CP 模拟时基模块  
- SPI LCD 显示屏（ST77xx / ILI9341）  

---

## 🏗️ 项目结构

Project-core/
├── core/ # 主程序、入口函数、启动文件
│ ├── inc/
│ └── src/
├── drivers/ # 外设驱动与底层库
│ ├── CMSIS/
│ ├── STM32F10x_FWLib/
│ ├── SYSTEM/ # 系统级服务：延时、串口、系统初始化
│ └── HARDWARE/ # 硬件外设驱动：SPI、LCD、传感器等
├── scripts/ # 构建脚本（可选）
├── CMakeLists.txt # 项目构建配置
└── README.md # 本说明文件

---

## ⚙️ 开发环境

| 工具 | 版本 | 说明 |
|------|-------|------|
| CMake | ≥ 3.20 | 项目生成系统 |
| Arm GNU Toolchain | 14.3 rel1 | 编译器 |
| Ninja | 最新版 | 构建工具 |
| VSCode + Cortex-Debug 插件 |  | 调试环境 |

---

## 🚀 快速开始

1. 打开 VSCode → “Open Folder” 选择 `Project-core`  
2. 运行 CMake: Configure → 生成 Ninja 构建文件  
3. 点击 Build → 生成 `Project-core.elf / .bin / .hex`  
4. 使用 ST-Link 烧录程序  
5. 打开串口调试工具，查看 LCD 识别 ID 输出  

---

## 📄 License
MIT License © 2025

---

## 🧠 作者备注
项目仍在早期阶段，目前完成：
- ✅ CMake 工程框架
- ✅ SYSTEM 初始化模块
- ✅ 硬件 SPI 封装
- ✅ LCD 驱动（读屏 ID）
- 🔜 即将添加：传感器驱动、中文字体显示、气象数据融合展示

<p>
  <strong style="color:#b30000; font-size:1.05em;">
    警告（重要）：
  </strong>
  <span style="color:#b30000; font-weight:600;">
    本项目包含部分从其他开源项目移植的代码；未经原作者明确授权，不得用于任何商业目的。
    本项目大量使用生成式 AI 作为创作辅助，可能包含致命缺陷；请在实际部署前自行充分测试并承担相应风险。
  </span>
</p>
<p style="color:#555; font-style:italic; margin-top:6px;">
  （友情提示：AI 很聪明，但并不懂得生活常识——请别把它当成终极 QA。）
</p>

## 版本
-0.1