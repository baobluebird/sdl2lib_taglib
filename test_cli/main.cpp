#include <ftxui/component/component.hpp> // For interactive components.
#include <ftxui/component/screen_interactive.hpp> // For ScreenInteractive.
#include <iostream>
#include <stdexcept>
#include <cctype> // For isdigit()

using namespace ftxui;

int main() {
    // Tùy chọn trong menu
    std::vector<std::string> menu_entries = {
        "1. MetadataFile Handler",
        "2. MediaFile Manager",
        "3. Playlist Handler",
        "4. Playlist Manager",
        "q. Exit"
    };

    int selected = 0; // Lựa chọn hiện tại
    std::string error_message; // Nơi lưu trữ thông báo lỗi

    // Tạo menu
    auto menu = Menu(&menu_entries, &selected);

    auto screen = ScreenInteractive::TerminalOutput();

    // Tạo giao diện chính
    auto main_component = Renderer(menu, [&] {
        return vbox({
            text("============Main Menu==========") | center,
            separator(),
            menu->Render() | border,
            separator(),
            text("Use UP/DOWN keys, numbers (1-9), or click to navigate. Press ENTER to select.") | dim | center,
            text("Press 'q' to exit.") | dim | center,
            separator(),
            text(error_message) | color(Color::Red) | center // Hiển thị thông báo lỗi (nếu có)
        });
    });

    // Xử lý sự kiện từ bàn phím và chuột
    main_component = CatchEvent(main_component, [&](Event event) {
        // Khi nhấn Enter
        if (event == Event::Return) {
            screen.ExitLoopClosure()(); // Thoát khỏi vòng lặp giao diện
            return true;
        }

        // Khi nhấn một ký tự (số)
        if (event.is_character() && std::isdigit(event.character()[0])) {
            int number = event.character()[0] - '0'; // Chuyển ký tự số thành số nguyên
            if (number >= 1 && number <= (int)menu_entries.size() - 1) { // Bỏ qua mục "q. Exit"
                selected = number - 1; // Giảm 1 để khớp với chỉ số của menu (0-based index)
                screen.ExitLoopClosure()(); // Thoát khỏi giao diện
                return true;
            } else {
                error_message = "Invalid input: number out of range!"; // Hiển thị lỗi
                return true;
            }
        }

        // Khi nhấn phím 'q'
        if (event == Event::Character("q")) {
            selected = (int)menu_entries.size() - 1; // Mục cuối cùng là "q. Exit"
            screen.ExitLoopClosure()(); // Thoát khỏi giao diện
            return true;
        }

        // Xử lý click chuột
        if (event.is_mouse() && event.mouse().button == Mouse::Left && event.mouse().motion == Mouse::Pressed) {
            int clicked_index = event.mouse().y - 3; // Điều chỉnh bù trừ cho đúng (2 dòng tiêu đề và ngăn cách)
            if (clicked_index >= 0 && clicked_index < (int)menu_entries.size()) {
                selected = clicked_index;
                screen.ExitLoopClosure()(); // Thoát khỏi giao diện
                return true;
            } else {
                error_message = "Invalid click: out of menu range!"; // Hiển thị lỗi
                return true;
            }
        }

        return false; // Không xử lý sự kiện nào khác
    });

    // Hiển thị giao diện
    screen.Loop(main_component);

    // Xử lý lựa chọn sau khi giao diện thoát
    try {
        std::cout << "You selected: " << menu_entries[selected] << std::endl;

        switch (selected) {
        case 0:
            std::cout << "Launching MetadataFile Handler..." << std::endl;
            break;
        case 1:
            std::cout << "Launching MediaFile Manager..." << std::endl;
            break;
        case 2:
            std::cout << "Launching Playlist Handler..." << std::endl;
            break;
        case 3:
            std::cout << "Launching Playlist Manager..." << std::endl;
            break;
        case 4:
            std::cout << "Exiting program. Goodbye!" << std::endl;
            break;
        default:
            throw std::invalid_argument("Invalid option selected!");
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
