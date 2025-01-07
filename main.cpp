#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cstdlib> // system()
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

// Abstract MediaFile class
class MediaFile {
protected:
    std::string fileName;
    std::string pathName;

public:
    virtual void inputMediaFile() = 0;
    virtual void detailMediaFile() = 0;
    virtual std::string getPathName() const { return pathName; }
    virtual ~MediaFile() {}
};

// MediaFileAudio class
class MediaFileAudio : public MediaFile {
public:
    void inputMediaFile() override {
        pathName = "/home/bluebird/play_music_video_APP/music/confession.mp3";
        fileName = "confession.mp3"; // Extracted manually
    }

    void detailMediaFile() override {
        std::cout << "Audio File: " << fileName << ", Path: " << pathName << "\n";
    }
};

// MediaFileVideo class
class MediaFileVideo : public MediaFile {
public:
    void inputMediaFile() override {
        pathName = "/home/bluebird/play_music_video_APP/video/take.mp4";
        fileName = "take.mp4"; // Extracted manually
    }

    void detailMediaFile() override {
        std::cout << "Video File: " << fileName << ", Path: " << pathName << "\n";
    }
};

// MetadataManager class
class MetadataManager {
    std::vector<std::shared_ptr<MediaFile>> listMediaFile;

public:
    void addMediaFile(std::shared_ptr<MediaFile> file) {
        listMediaFile.push_back(file);
    }

    void displayListMediaFile() {
        for (auto &file : listMediaFile) {
            file->detailMediaFile();
        }
    }
};

// Player class
class Player {
    int volume;

public:
    Player() : volume(50) {
        if (SDL_Init(SDL_INIT_AUDIO) < 0) {
            std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        }
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        }
    }

    ~Player() {
        Mix_CloseAudio();
        SDL_Quit();
    }

    // Play audio file
    void play(const std::string &filePath) {
        Mix_Music *music = Mix_LoadMUS(filePath.c_str());
        if (!music) {
            std::cerr << "Failed to load music: " << Mix_GetError() << std::endl;
            return;
        }

        Mix_PlayMusic(music, 1);  // Play once
        std::cout << "Playing file: " << filePath << "\n";
        SDL_Delay(5000);  // Delay để phát file trong 5 giây

        Mix_FreeMusic(music);
    }

    // Extract audio from video
    std::string extractAudio(const std::string &videoPath) {
        std::string outputAudioPath = "extracted_audio.wav";

        // Sử dụng FFmpeg để trích xuất âm thanh
        std::string command = "ffmpeg -i " + videoPath + " -q:a 0 -map a " + outputAudioPath + " -y";
        int result = system(command.c_str());

        if (result != 0) {
            std::cerr << "Failed to extract audio using FFmpeg.\n";
            return "";
        }

        std::cout << "Audio extracted to: " << outputAudioPath << "\n";
        return outputAudioPath;
    }

    void adjustVolume(int newVolume) {
        volume = newVolume;
        Mix_VolumeMusic(volume);
        std::cout << "Volume set to: " << volume << "\n";
    }
};

// App class
class App {
    MetadataManager metadataManager;
    Player player;

public:
    void viewAllMediaFiles() {
        metadataManager.displayListMediaFile();
    }

    void playMedia(std::shared_ptr<MediaFile> mediaFile) {
        std::string filePath = mediaFile->getPathName();

        if (filePath.find(".mp4") != std::string::npos) { // Nếu là file video
            std::string audioPath = player.extractAudio(filePath);
            if (!audioPath.empty()) {
                player.play(audioPath);
            }
        } else { // Nếu là file audio
            player.play(filePath);
        }
    }

    void addMediaFileToManager(std::shared_ptr<MediaFile> mediaFile) {
        metadataManager.addMediaFile(mediaFile);
    }

    void adjustVolume(int volume) {
        player.adjustVolume(volume);
    }
};

// Main function
int main() {
    App app;

    // Create Audio and Video files
    std::shared_ptr<MediaFileAudio> audioFile = std::make_shared<MediaFileAudio>();
    audioFile->inputMediaFile();

    std::shared_ptr<MediaFileVideo> videoFile = std::make_shared<MediaFileVideo>();
    videoFile->inputMediaFile();

    // Add files to MetadataManager
    app.addMediaFileToManager(audioFile);
    app.addMediaFileToManager(videoFile);

    // Display all media files
    app.viewAllMediaFiles();

    // Play a media file (audio or video)
    app.playMedia(audioFile); // Chơi file audio
    app.playMedia(videoFile); // Chơi file video (sẽ trích xuất âm thanh trước)

    // Adjust volume
    app.adjustVolume(70);

    return 0;
}
