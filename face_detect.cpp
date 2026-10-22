#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono>

int main() {
    // 1. 加载 XML 分类器
    cv::CascadeClassifier face_cascade;
    if (!face_cascade.load("haarcascade_frontalface_default.xml")) {
        std::cerr << "❌ 错误：找不到 XML 文件，请确保它和代码在同一目录！" << std::endl;
        return -1;
    }

    // 2. 打开摄像头
    cv::VideoCapture cap(0, cv::CAP_V4L2);
    if (!cap.isOpened()) {
        std::cerr << "❌ 无法打开摄像头" << std::endl;
        return -1;
    }
    cap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);

    cv::Mat frame, gray, small_gray;
    std::vector<cv::Rect> faces;
    
    auto prev_time = std::chrono::steady_clock::now();

    while (true) {
        cap >> frame;
        if (frame.empty()) break;

        // 3. 转灰度图
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

        // 4. 性能优化：缩小一半
        cv::resize(gray, small_gray, cv::Size(), 0.5, 0.5);

        // 5. 测速开始
        auto t1 = std::chrono::steady_clock::now();
        
        // 检测人脸
        face_cascade.detectMultiScale(small_gray, faces, 1.1, 5);

        // 测速结束
        auto t2 = std::chrono::steady_clock::now();
        double detect_time = std::chrono::duration<double, std::milli>(t2 - t1).count();

        // 6. 画框（坐标乘2映射回原图）
        for (const auto& face : faces) {
            cv::rectangle(frame, cv::Rect(face.x * 2, face.y * 2, face.width * 2, face.height * 2), cv::Scalar(255, 0, 0), 2);
        }

        // 7. 计算 FPS
        auto curr_time = std::chrono::steady_clock::now();
        double elapsed = std::chrono::duration<double>(curr_time - prev_time).count();
        prev_time = curr_time;
        double fps = 1.0 / elapsed;

        // 8. 在屏幕上打字
        std::string fps_text = "FPS: " + std::to_string(int(fps));
        std::string time_text = "Detect: " + std::to_string(detect_time).substr(0, 4) + " ms";
        cv::putText(frame, fps_text, cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 255, 0), 2);
        cv::putText(frame, time_text, cv::Point(10, 60), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 0, 255), 2);

        cv::imshow("Linux C++ Face Detect", frame);

        if (cv::waitKey(1) == 'q') break;
    }

    cap.release();
    cv::destroyAllWindows();
    return 0;
}
