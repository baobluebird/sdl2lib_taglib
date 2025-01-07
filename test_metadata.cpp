#include <iostream>
#include <taglib/fileref.h>
#include <taglib/tag.h>

int main() {
    std::string filePath = "music/confession.mp3"; // Đường dẫn đến file nhạc

    // Mở file nhạc bằng TagLib
    TagLib::FileRef file(filePath.c_str());

    if (!file.isNull() && file.tag()) {
        // Lấy metadata
        TagLib::Tag *tag = file.tag();
        std::cout << "Title  : " << tag->title().toCString(true) << std::endl;
        std::cout << "Artist : " << tag->artist().toCString(true) << std::endl;
        std::cout << "Album  : " << tag->album().toCString(true) << std::endl;
        std::cout << "Year   : " << tag->year() << std::endl;
        std::cout << "Genre  : " << tag->genre().toCString(true) << std::endl;

        TagLib::Tag *tag2 = file.tag();
        tag2->setTitle("New Title");
        tag2->setArtist("New Artist");
        tag2->setAlbum("New Album");
        tag2->setYear(2025);
        tag2->setGenre("Pop");

        // Lấy metadata
        std::cout << "Title  : " << tag->title().toCString(true) << std::endl;
        std::cout << "Artist : " << tag->artist().toCString(true) << std::endl;
        std::cout << "Album  : " << tag->album().toCString(true) << std::endl;
        std::cout << "Year   : " << tag->year() << std::endl;
        std::cout << "Genre  : " << tag->genre().toCString(true) << std::endl;

        // Lưu thay đổi
        file.save();
        std::cout << "Metadata updated successfully!" << std::endl;

    } else {
        std::cerr << "Could not open file: " << filePath << std::endl;
    }

    return 0;
}
