#include <iostream>
#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/audioproperties.h> // Thêm thư viện AudioProperties

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

        // Lấy thông tin audio properties
        if (file.audioProperties()) {
            TagLib::AudioProperties *audioProps = file.audioProperties();
            int duration = audioProps->length(); // Thời lượng (giây)
            int minutes = duration / 60;
            int seconds = duration % 60;

            std::cout << "Duration: " << minutes << "m " << seconds << "s" << std::endl;
            std::cout << "Bitrate : " << audioProps->bitrate() << " kbps" << std::endl;
            std::cout << "Sample Rate: " << audioProps->sampleRate() << " Hz" << std::endl;
            std::cout << "Channels: " << audioProps->channels() << std::endl;
        } else {
            std::cout << "Audio properties not available for this file." << std::endl;
        }

        // Cập nhật metadata
        tag->setTitle("New Title");
        tag->setArtist("New Artist");
        tag->setAlbum("New Album");
        tag->setYear(2025);
        tag->setGenre("Pop");

        // Lưu thay đổi
        file.save();
        std::cout << "Metadata updated successfully!" << std::endl;

    } else {
        std::cerr << "Could not open file: " << filePath << std::endl;
    }

    return 0;
}
