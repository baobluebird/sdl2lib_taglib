#include <iostream>
#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/mpegfile.h>
#include <taglib/id3v2tag.h>
#include <taglib/textidentificationframe.h>
#include <taglib/tbytevector.h>

void viewMetadata(TagLib::MPEG::File &file) {
    // Lấy tag ID3v2
    TagLib::ID3v2::Tag *tag = file.ID3v2Tag();
    if (tag) {
        std::cout << "\n=== Current Metadata ===" << std::endl;
        for (auto it = tag->frameList().begin(); it != tag->frameList().end(); ++it) {
            auto *frame = dynamic_cast<TagLib::ID3v2::TextIdentificationFrame *>(*it);
            if (frame) {
                std::cout << frame->frameID().data() << ": " << frame->toString().to8Bit() << std::endl;
            }
        }
    } else {
        std::cout << "No ID3v2 tags found in the file." << std::endl;
    }
}

void editMetadata(TagLib::MPEG::File &file, const std::string &key, const std::string &newValue) {
    // Lấy tag ID3v2
    TagLib::ID3v2::Tag *tag = file.ID3v2Tag(true); // `true` để tạo tag nếu chưa tồn tại
    if (tag) {
        TagLib::ByteVector keyByteVector(key.c_str()); // Chuyển std::string sang TagLib::ByteVector
        auto frameList = tag->frameListMap()[keyByteVector];
        if (!frameList.isEmpty()) {
            auto *frame = dynamic_cast<TagLib::ID3v2::TextIdentificationFrame *>(frameList.front());
            if (frame) {
                frame->setText(TagLib::String(newValue, TagLib::String::UTF8));
                std::cout << "Updated key: " << key << " with new value: " << newValue << std::endl;
            }
        } else {
            std::cout << "Key not found: " << key << std::endl;
        }
    }
}

void addMetadataKey(TagLib::MPEG::File &file, const std::string &key, const std::string &value) {
    // Lấy tag ID3v2
    TagLib::ID3v2::Tag *tag = file.ID3v2Tag(true); // `true` để tạo tag nếu chưa tồn tại
    if (tag) {
        TagLib::ByteVector keyByteVector(key.c_str()); // Chuyển std::string sang TagLib::ByteVector
        TagLib::ID3v2::TextIdentificationFrame *frame =
            new TagLib::ID3v2::TextIdentificationFrame(keyByteVector, TagLib::String::UTF8);
        frame->setText(TagLib::String(value, TagLib::String::UTF8));
        tag->addFrame(frame);
        std::cout << "Added new key: " << key << " with value: " << value << std::endl;
    }
}

int main() {
    std::string filePath = "music/confession.mp3"; // Đường dẫn đến file MP3

    // Mở file MP3
    TagLib::MPEG::File file(filePath.c_str());
    if (!file.isOpen()) {
        std::cerr << "Could not open file: " << filePath << std::endl;
        return 1;
    }

    // Hiển thị metadata hiện tại
    viewMetadata(file);

    // Chỉnh sửa metadata
    editMetadata(file, "TALB", "Updated Album"); // "TALB" là Frame ID cho Album

    // Lưu thay đổi
    if (file.save()) {
        std::cout << "Metadata updated successfully!" << std::endl;
    } else {
        std::cerr << "Failed to save metadata." << std::endl;
    }

    // Thêm một key mới vào metadata
    addMetadataKey(file, "TPUB", "New Publisher"); // "TPUB" là Frame ID cho Publisher

    // Lưu thay đổi
    if (file.save()) {
        std::cout << "New key added and changes saved successfully!" << std::endl;
    } else {
        std::cerr << "Failed to save new key." << std::endl;
    }

    // Hiển thị lại metadata
    viewMetadata(file);

    return 0;
}
