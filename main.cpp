#include <iostream>
#include <vector>
#include <dirent.h>
#include <sys/stat.h>
#include <cstdlib>

// Hàm kiểm tra phần mở rộng của tệp
bool has_extension(const std::string& filename, const std::string& extension) {
    if (filename.size() >= extension.size()) {
        return (filename.compare(filename.size() - extension.size(), extension.size(), extension) == 0);
    }
    return false;
}

// Hàm liệt kê tất cả các USB được gắn kết
std::vector<std::string> list_usb_devices(const std::string& base_path) {
    std::vector<std::string> usb_devices;
    DIR* dir = opendir(base_path.c_str());
    if (!dir) {
        std::cerr << "Error opening base USB path: " << base_path << '\n';
        return usb_devices;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (std::string(entry->d_name) != "." && std::string(entry->d_name) != "..") {
            std::string full_path = base_path + "/" + entry->d_name;
            struct stat sb;
            if (stat(full_path.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode)) {
                usb_devices.push_back(full_path);
            }
        }
    }
    closedir(dir);
    return usb_devices;
}

// Hàm liệt kê tất cả các thư mục trong một đường dẫn
std::vector<std::string> list_folders(const std::string& path) {
    std::vector<std::string> folders;
    DIR* dir = opendir(path.c_str());
    if (!dir) {
        std::cerr << "Error opening directory: " << path << '\n';
        return folders;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (std::string(entry->d_name) != "." && std::string(entry->d_name) != "..") {
            std::string full_path = path + "/" + entry->d_name;
            struct stat sb;
            if (stat(full_path.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode)) {
                folders.push_back(full_path);
            }
        }
    }
    closedir(dir);
    return folders;
}

// Hàm liệt kê các file mp3 và mp4 trong một đường dẫn
void list_media_files(const std::string& path) {
    DIR* dir = opendir(path.c_str());
    if (!dir) {
        std::cerr << "Error opening directory: " << path << '\n';
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (std::string(entry->d_name) != "." && std::string(entry->d_name) != "..") {
            std::string full_path = path + "/" + entry->d_name;
            struct stat sb;
            if (stat(full_path.c_str(), &sb) == 0 && S_ISREG(sb.st_mode)) {
                std::string filename = entry->d_name;
                if (has_extension(filename, ".mp3") || has_extension(filename, ".mp4")) {
                    std::cout << filename << '\n';
                }
            }
        }
    }
    closedir(dir);
}

// Hàm quét tất cả các thư mục và liệt kê file mp3, mp4
void scan_all_folders(const std::string& path) {
    std::vector<std::string> folders = list_folders(path);
    for (const auto& folder : folders) {
        list_media_files(folder);
    }
}

int main() {
    std::string usb_base_path = "/media/" + std::string(std::getenv("USER"));
    std::vector<std::string> usb_devices = list_usb_devices(usb_base_path);

    if (usb_devices.empty()) {
        std::cerr << "No USB devices found.\n";
        return 1;
    }

    // Hiển thị các USB được gắn
    std::cout << "USB devices connected:\n";
    for (size_t i = 0; i < usb_devices.size(); ++i) {
        std::cout << i + 1 << ". " << usb_devices[i] << '\n';
    }

    // Chọn USB
    int usb_choice = 0;
    std::cout << "Enter the number of the USB you want to scan: ";
    std::cin >> usb_choice;

    if (usb_choice < 1 || usb_choice > static_cast<int>(usb_devices.size())) {
        std::cerr << "Invalid choice.\n";
        return 1;
    }

    std::string selected_usb = usb_devices[usb_choice - 1];

    // Lựa chọn hành động
    int action_choice = 0;
    std::cout << "Choose an action:\n";
    std::cout << "1. Scan all folders for MP3 and MP4 files\n";
    std::cout << "2. Choose a specific folder to scan\n";
    std::cin >> action_choice;

    if (action_choice == 1) {
        // Quét tất cả các folder
        scan_all_folders(selected_usb);
    } else if (action_choice == 2) {
        // Hiển thị tất cả các folder
        std::vector<std::string> folders = list_folders(selected_usb);
        if (folders.empty()) {
            std::cerr << "No folders found in the selected USB.\n";
            return 1;
        }

        std::cout << "Folders in USB:\n";
        for (size_t i = 0; i < folders.size(); ++i) {
            std::cout << i + 1 << ". " << folders[i] << '\n';
        }

        // Chọn folder
        int folder_choice = 0;
        std::cout << "Enter the number of the folder you want to scan: ";
        std::cin >> folder_choice;

        if (folder_choice < 1 || folder_choice > static_cast<int>(folders.size())) {
            std::cerr << "Invalid choice.\n";
            return 1;
        }

        std::string selected_folder = folders[folder_choice - 1];
        list_media_files(selected_folder);
    } else {
        std::cerr << "Invalid action choice.\n";
        return 1;
    }

    return 0;
}
