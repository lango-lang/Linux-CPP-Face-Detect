# Linux C++ 实时人脸检测系统 (Linux-CPP-Face-Detect)

本项目是一个基于纯 C++ 和 OpenCV 的实时视觉检测系统，运行于 WSL2 (Ubuntu) 环境。项目针对 USB 摄像头带宽瓶颈与算法计算延迟进行了双重优化，实现了低延迟的视频流处理闭环。

## 🛠️ 技术栈
*   **编程语言**：C++ (C++11及以上)
*   **计算机视觉库**：OpenCV 4.x
*   **系统环境**：Linux (Ubuntu on WSL2)
*   **硬件交互**：V4L2 (Video for Linux 2) 接口、usbipd 硬件穿透

## 🚀 核心优化点
1.  **底层设备直连**：抛弃跨平台的默认调用，强制使用 `cv::CAP_V4L2` 后端直接读取底层视频设备节点（`/dev/video0`）。
2.  **突破 IO 带宽瓶颈**：通过 V4L2 强制摄像头使用 `MJPG` 硬件编码格式输出，大幅降低 USB 传输的带宽压力，解决穿透环境下的 `select() timeout` 读帧超时问题。
3.  **算法延迟极速优化**：在将图像送入 Haar 级联分类器前，采用 `cv::resize` 对灰度图进行 50% 降采样处理，使单帧纯检测耗时从 12ms 降至 **3.5ms**（提升约 3.4 倍）。

## 📊 性能表现
*   **单帧检测延迟**：~ 3.5 ms
*   **画面处理帧率**：稳定在 15 FPS 左右（受限于 USB 穿透协议与网络 IO 瓶颈，纯算法算力仍有大量富余）

## 💻 如何编译与运行
### 依赖安装 (Ubuntu)
```bash
sudo apt update
sudo apt install build-essential libopencv-dev -y
