#include <iostream>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

// 回调函数，用于接收HTTP响应
size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* buffer) {
    size_t totalSize = size * nmemb;
    buffer->append((char*)contents, totalSize);
    return totalSize;
}

int main() {
    setlocale(LC_ALL, "en_US.UTF-8");
    // 创建一个CURL对象
    CURL* curl = curl_easy_init();
    if (curl) {
        string url = "http://api.tianapi.com/ai/index?key=6a44e40e83eb0f56943aa450a579eb51&num=10&keyword=智能脑&sort=time";
        string response;

        // 设置请求的URL
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        // 设置回调函数，用于接收HTTP响应
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // 发起HTTP请求
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            cerr << "请求失败：" << curl_easy_strerror(res) << endl;
        }
        else {
            // 解析JSON响应
            json jsonResponse = json::parse(response);
            if (jsonResponse.contains("newslist")) {
                // 提取新闻列表
                json articles = jsonResponse["newslist"];
                for (const auto& article : articles) {
                    string title = article["title"];
                    string url = article["url"];
                    string time = article["ctime"];

                    // 打印新闻标题、URL和发布时间
                    cout << "标题：" << title << endl;
                    cout << "URL：" << url << endl;
                    cout << "发布时间：" << time << endl;
                    cout << endl;
                }
            }
            else {
                cerr << "无法解析JSON响应" << endl;
            }
        }

        // 清理CURL对象
        curl_easy_cleanup(curl);
    }
    else {
        cerr << "无法初始化CURL对象" << endl;
    }
    cout << "按下回车键退出程序...";
    cin.ignore();
    return 0;
}