#include <iostream>
#include <taglib/mpegfile.h>
#include <taglib/id3v2tag.h>
#include <taglib/textidentificationframe.h>
#include <taglib/audioproperties.h>

void showAudioInfo(const std::string &filePath);
void editMetadata(TagLib::MPEG::File &file, const std::string &key, const std::string &value);
void addCustomMetadata(TagLib::MPEG::File &file, const std::string &key, const std::string &value);

int main() {
    // Đường dẫn đến file nhạc
    std::string filePath = "music/confession.mp3";

    // Mở file MP3 bằng TagLib::MPEG::File
    TagLib::MPEG::File file(filePath.c_str());

    if (file.isOpen()) {
        // 1. Hiển thị thông tin tệp audio
        std::cout << "=== File Information ===" << std::endl;
        showAudioInfo(filePath);

        // 2. Chỉnh sửa metadata
        std::cout << "\n=== Editing Metadata ===" << std::endl;
        editMetadata(file, "TIT2", "Updated Publisher Name");

        // 3. Thêm key mới
        std::cout << "\n=== Adding Custom Metadata ===" << std::endl;
        addCustomMetadata(file, "TXXX", "Custom Value");
    } else {
        std::cerr << "Failed to open audio file: " << filePath << std::endl;
    }

    return 0;
}

void showAudioInfo(const std::string &filePath) {
    TagLib::MPEG::File file(filePath.c_str());

    if (!file.isOpen() || file.audioProperties() == nullptr) {
        std::cerr << "Failed to access audio properties for: " << filePath << std::endl;
        return;
    }

    TagLib::AudioProperties *audioProps = file.audioProperties();

    // Lấy metadata
    TagLib::ID3v2::Tag *tag = file.ID3v2Tag();
    if (tag) {
        std::cout << "Title      : " << tag->title().to8Bit(true) << std::endl;
        std::cout << "Artist     : " << tag->artist().to8Bit(true) << std::endl;
        std::cout << "Album      : " << tag->album().to8Bit(true) << std::endl;
        std::cout << "Year       : " << tag->year() << std::endl;
        std::cout << "Genre      : " << tag->genre().to8Bit(true) << std::endl;
    }

    // Lấy audio properties
    int duration = audioProps->length();
    int minutes = duration / 60;
    int seconds = duration % 60;
    std::cout << "Duration   : " << minutes << "m " << seconds << "s" << std::endl;
    std::cout << "Bitrate    : " << audioProps->bitrate() << " kbps" << std::endl;
    std::cout << "SampleRate : " << audioProps->sampleRate() << " Hz" << std::endl;
    std::cout << "Channels   : " << audioProps->channels() << std::endl;

    // Hiển thị metadata hiện tại
    std::cout << "\n=== Current Metadata ===" << std::endl;
    TagLib::ID3v2::FrameList frameList = tag->frameList();
    for (auto it = frameList.begin(); it != frameList.end(); ++it) {
        auto *frame = dynamic_cast<TagLib::ID3v2::TextIdentificationFrame *>(*it);
        if (frame) {
            std::cout << frame->frameID().data() << ": " << frame->toString().to8Bit(true) << std::endl;
        }
    }
}

void editMetadata(TagLib::MPEG::File &file, const std::string &key, const std::string &value) {
    TagLib::ID3v2::Tag *tag = file.ID3v2Tag(true);

    if (tag) {
        TagLib::ByteVector byteKey(key.c_str());
        TagLib::ID3v2::FrameList frameList = tag->frameListMap()[byteKey];
        if (!frameList.isEmpty()) {
            // Cập nhật giá trị của frame nếu đã tồn tại
            auto *frame = dynamic_cast<TagLib::ID3v2::TextIdentificationFrame *>(frameList.front());
            if (frame) {
                frame->setText(value);
                std::cout << "Metadata '" << key << "' updated to '" << value << "'." << std::endl;
            }
        } else {
            // Thêm mới nếu chưa tồn tại
            TagLib::ID3v2::TextIdentificationFrame *frame =
                new TagLib::ID3v2::TextIdentificationFrame(TagLib::ByteVector(key.c_str()), TagLib::String::UTF8);
            frame->setText(value);
            tag->addFrame(frame);
            std::cout << "Added new key '" << key << "' with value '" << value << "'." << std::endl;
        }

        if (file.save()) {
            std::cout << "Changes saved successfully!" << std::endl;
        } else {
            std::cerr << "Failed to save changes." << std::endl;
        }
    } else {
        std::cerr << "Failed to access ID3v2 tag for editing." << std::endl;
    }
}

void addCustomMetadata(TagLib::MPEG::File &file, const std::string &key, const std::string &value) {
    TagLib::ID3v2::Tag *tag = file.ID3v2Tag(true);

    if (tag) {
        TagLib::ByteVector byteKey(key.c_str());
        TagLib::ID3v2::TextIdentificationFrame *frame =
            new TagLib::ID3v2::TextIdentificationFrame(byteKey, TagLib::String::UTF8);
        frame->setText(value);
        tag->addFrame(frame);
        std::cout << "Custom metadata '" << key << "' added with value '" << value << "'." << std::endl;

        if (file.save()) {
            std::cout << "Changes saved successfully!" << std::endl;
        } else {
            std::cerr << "Failed to save changes." << std::endl;
        }
    } else {
        std::cerr << "Failed to access ID3v2 tag for adding custom key." << std::endl;
    }
}
