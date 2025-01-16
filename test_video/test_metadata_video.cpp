#include <iostream>
#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/audioproperties.h>

int main() {
    std::string filePath = "video/take.mp4"; // Path to the MP4 file

    // Open the file using TagLib::FileRef
    TagLib::FileRef file(filePath.c_str());

    if (file.isNull()) {
        std::cerr << "Could not open file: " << filePath << std::endl;
        return 1;
    }

    // Retrieve audio properties
    TagLib::AudioProperties *audioProps = file.audioProperties();
    if (audioProps) {
        int duration = audioProps->length(); // Duration in seconds
        int minutes = duration / 60;
        int seconds = duration % 60;

        std::cout << "Duration   : " << minutes << "m " << seconds << "s" << std::endl;
        std::cout << "Bitrate    : " << audioProps->bitrate() << " kbps" << std::endl;
        std::cout << "SampleRate : " << audioProps->sampleRate() << " Hz" << std::endl;
        std::cout << "Channels   : " << audioProps->channels() << std::endl;
        std::cout << "Size       : " << audioProps->length() << std::endl;
    } else {
        std::cout << "Audio properties not available for this file." << std::endl;
    }

    return 0;
}
