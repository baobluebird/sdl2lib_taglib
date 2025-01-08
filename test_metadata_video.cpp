extern "C" {
#include <libavformat/avformat.h>
}

#include <iostream>
#include <string>

int main() {
    std::string filePath = "video/take.mp4"; // Đường dẫn tệp video

    AVFormatContext *formatContext = nullptr;

    // Mở file video
    if (avformat_open_input(&formatContext, filePath.c_str(), nullptr, nullptr) != 0) {
        std::cerr << "Could not open file: " << filePath << std::endl;
        return 1;
    }

    // Đọc metadata
    AVDictionary *metadata = formatContext->metadata;
    if (metadata) {
        AVDictionaryEntry *tag = nullptr;
        while ((tag = av_dict_get(metadata, "", tag, AV_DICT_IGNORE_SUFFIX))) {
            std::cout << tag->key << ": " << tag->value << std::endl;
        }
    } else {
        std::cout << "No metadata found." << std::endl;
    }

    // Đọc thông tin cơ bản của tệp
    std::cout << "Format: " << formatContext->iformat->long_name << std::endl;
    std::cout << "Duration: " << formatContext->duration / AV_TIME_BASE << " seconds" << std::endl;

    // Giải phóng tài nguyên
    avformat_close_input(&formatContext);
    avformat_free_context(formatContext);

    return 0;
}
