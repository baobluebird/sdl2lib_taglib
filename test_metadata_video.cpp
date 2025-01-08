extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <sys/stat.h>
}

#include <iostream>
#include <string>

void showFileInfo(const std::string &filePath);
void editMetadata(const std::string &filePath, const std::string &key, const std::string &value);
void addCustomMetadata(const std::string &filePath, const std::string &key, const std::string &value);

int main() {
    std::string filePath = "video/take.mp4"; // Đường dẫn tệp video

    // 1. Hiển thị thông tin tệp
    std::cout << "=== File Information ===" << std::endl;
    showFileInfo(filePath);

    // 2. Chỉnh sửa metadata
    std::cout << "\n=== Editing Metadata ===" << std::endl;
    editMetadata(filePath, "title", "New Video Title");

    // 3. Thêm key mới
    std::cout << "\n=== Adding Custom Metadata ===" << std::endl;
    addCustomMetadata(filePath, "custom_key", "Custom Value");

    return 0;
}

void showMetadata(AVDictionary *metadata) {
    if (metadata) {
        AVDictionaryEntry *tag = nullptr;
        std::cout << "\n=== Current Metadata ===" << std::endl;
        while ((tag = av_dict_get(metadata, "", tag, AV_DICT_IGNORE_SUFFIX))) {
            std::cout << tag->key << ": " << tag->value << std::endl;
        }
    } else {
        std::cout << "No metadata found." << std::endl;
    }
}

void showFileInfo(const std::string &filePath) {
    AVFormatContext *formatContext = nullptr;

    if (avformat_open_input(&formatContext, filePath.c_str(), nullptr, nullptr) != 0) {
        std::cerr << "Could not open file: " << filePath << std::endl;
        return;
    }

    if (avformat_find_stream_info(formatContext, nullptr) < 0) {
        std::cerr << "Could not find stream info." << std::endl;
        avformat_close_input(&formatContext);
        return;
    }

    std::string fileName = filePath.substr(filePath.find_last_of("/") + 1);
    std::string fileType = filePath.substr(filePath.find_last_of(".") + 1);

    struct stat fileStat;
    double fileSize = 0;
    if (stat(filePath.c_str(), &fileStat) == 0) {
        fileSize = fileStat.st_size / 1024.0 / 1024.0; // MB
    }

    int64_t duration = formatContext->duration / AV_TIME_BASE; // seconds
    int hours = duration / 3600;
    int minutes = (duration % 3600) / 60;
    int seconds = duration % 60;

    AVCodecParameters *codecParams = nullptr;
    int videoStreamIndex = -1;
    for (unsigned int i = 0; i < formatContext->nb_streams; ++i) {
        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            codecParams = formatContext->streams[i]->codecpar;
            videoStreamIndex = i;
            break;
        }
    }

    const AVCodec *codec = codecParams ? avcodec_find_decoder(codecParams->codec_id) : nullptr;
    std::string codecName = codec ? codec->long_name : "Unknown";

    int bitrate = formatContext->bit_rate / 1000; // kbps
    std::string resolution = codecParams ? std::to_string(codecParams->width) + "x" + std::to_string(codecParams->height) : "Unknown";

    std::cout << "File Name  : " << fileName << std::endl;
    std::cout << "Path Name  : " << filePath << std::endl;
    std::cout << "Size       : " << fileSize << " MB" << std::endl;
    std::cout << "Duration   : " << hours << "h " << minutes << "m " << seconds << "s" << std::endl;
    std::cout << "File Type  : " << fileType << std::endl;
    std::cout << "Codec      : " << codecName << std::endl;
    std::cout << "Bitrate    : " << bitrate << " kbps" << std::endl;
    std::cout << "Resolution : " << resolution << std::endl;

    // Hiển thị metadata hiện tại
    showMetadata(formatContext->metadata);

    avformat_close_input(&formatContext);
    avformat_free_context(formatContext);
}

void editMetadata(const std::string &filePath, const std::string &key, const std::string &value) {
    AVFormatContext *formatContext = nullptr;

    if (avformat_open_input(&formatContext, filePath.c_str(), nullptr, nullptr) != 0) {
        std::cerr << "Could not open file: " << filePath << std::endl;
        return;
    }

    AVDictionary *metadata = formatContext->metadata;
    if (metadata) {
        av_dict_set(&metadata, key.c_str(), value.c_str(), 0);
        std::cout << "Metadata '" << key << "' updated to '" << value << "'." << std::endl;
    } else {
        std::cerr << "No metadata found to edit." << std::endl;
    }

    // Hiển thị metadata sau khi chỉnh sửa
    showMetadata(metadata);

    avformat_close_input(&formatContext);
    avformat_free_context(formatContext);
}

void addCustomMetadata(const std::string &filePath, const std::string &key, const std::string &value) {
    AVFormatContext *formatContext = nullptr;

    if (avformat_open_input(&formatContext, filePath.c_str(), nullptr, nullptr) != 0) {
        std::cerr << "Could not open file: " << filePath << std::endl;
        return;
    }

    AVDictionary *metadata = formatContext->metadata;
    if (metadata) {
        av_dict_set(&metadata, key.c_str(), value.c_str(), 0);
        std::cout << "Custom metadata '" << key << "' added with value '" << value << "'." << std::endl;
    } else {
        std::cerr << "No metadata found to add custom key." << std::endl;
    }

    // Hiển thị metadata sau khi thêm key mới
    showMetadata(metadata);

    avformat_close_input(&formatContext);
    avformat_free_context(formatContext);
}
