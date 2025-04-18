#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <windows.h>

int main() {
    OPENFILENAMEW ofn;  // 使用宽字符版本的 OPENFILENAME
    wchar_t szFile[260];

    // 初始化OPENFILENAME结构体
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = L'\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = L"JPEG Files (*.jpg)\0*.jpg\0BMP Files (*.bmp)\0*.bmp\0All Files (*.*)\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    // 显示文件选择对话框
    if (GetOpenFileNameW(&ofn) == TRUE) {
        // 将宽字符路径转换为多字节路径
        char mbFile[260];
        WideCharToMultiByte(CP_ACP, 0, ofn.lpstrFile, -1, mbFile, sizeof(mbFile), NULL, NULL);

        // 读取图像
        cv::Mat image = cv::imread(mbFile, cv::IMREAD_COLOR);

        // 检查图像是否成功读取
        if (image.empty()) {
            printf("无法读取图像: %s\n", mbFile);
            return -1;
        }

        // 显示图像，使用 cv::WINDOW_NORMAL 使窗口可自由调整大小
        cv::namedWindow("显示图像", cv::WINDOW_NORMAL);
        cv::imshow("显示图像", image);

        // 等待按键
        cv::waitKey(0);

        // 存储图像，使用保存文件对话框选择保存路径
        wchar_t savePath[260];
        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = NULL;
        ofn.lpstrFile = savePath;
        ofn.lpstrFile[0] = L'\0';
        ofn.nMaxFile = sizeof(savePath);
        ofn.lpstrFilter = L"JPEG Files (*.jpg)\0*.jpg\0BMP Files (*.bmp)\0*.bmp\0All Files (*.*)\0*.*\0";
        ofn.nFilterIndex = 1;
        ofn.lpstrFileTitle = NULL;
        ofn.nMaxFileTitle = 0;
        ofn.lpstrInitialDir = NULL;
        ofn.Flags = OFN_OVERWRITEPROMPT;

        if (GetSaveFileNameW(&ofn) == TRUE) {
            // 将宽字符保存路径转换为多字节路径
            char mbSavePath[260];
            WideCharToMultiByte(CP_ACP, 0, ofn.lpstrFile, -1, mbSavePath, sizeof(mbSavePath), NULL, NULL);

            bool saved = cv::imwrite(mbSavePath, image);

            // 检查图像是否成功保存
            if (saved) {
                printf("图像已成功保存到: %s\n", mbSavePath);
            }
            else {
                printf("无法保存图像到: %s\n", mbSavePath);
            }
        }
        else {
            printf("用户取消了保存文件选择。\n");
        }

        // 关闭所有窗口
        cv::destroyAllWindows();
    }
    else {
        printf("用户取消了文件选择。\n");
    }

    return 0;
}
