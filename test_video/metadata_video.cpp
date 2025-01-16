#include <iostream>
#include <taglib/mp4file.h>
#include <taglib/mp4tag.h>
#include <taglib/tstring.h>

int main() {
    // Đường dẫn đến file MP4
    std::string filePath = "video/take.mp4";

    // Mở file MP4
    TagLib::MP4::File file(filePath.c_str());
    if (!file.isOpen()) {
        std::cerr << "Could not open file: " << filePath << std::endl;
        return 1;
    }

    // Lấy tag từ file
    TagLib::MP4::Tag *tag = file.tag();
    if (!tag) {
        std::cerr << "Could not access tags in file." << std::endl;
        return 1;
    }

    // Thêm key mới vào metadata
    tag->itemListMap().insert("©dir", TagLib::MP4::Item(TagLib::String("Sample Director", TagLib::String::UTF8)));
    tag->itemListMap().insert("©desc", TagLib::MP4::Item(TagLib::String("This is a custom description", TagLib::String::UTF8)));

    // Lưu thay đổi
    if (file.save()) {
        std::cout << "Metadata updated successfully! Added new keys." << std::endl;
    } else {
        std::cerr << "Failed to save metadata." << std::endl;
    }

    // Hiển thị metadata sau khi thêm key
    std::cout << "\n=== Updated Metadata ===" << std::endl;
    for (auto it = tag->itemListMap().begin(); it != tag->itemListMap().end(); ++it) {
        std::cout << it->first.to8Bit(true) << ": " << it->second.toStringList().toString().to8Bit(true) << std::endl;
    }

    return 0;
}
