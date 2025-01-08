#include <iostream>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

using namespace std;

int configure_uart(const char* port_name, int baud_rate) {
    int uart_fd = open(port_name, O_RDWR | O_NOCTTY);
    if (uart_fd == -1) {
        cerr << "Error: Unable to open UART. Check port name! Errno: " << strerror(errno) << endl;
        return -1;
    }

    struct termios options;
    tcgetattr(uart_fd, &options);

    cfsetispeed(&options, baud_rate);
    cfsetospeed(&options, baud_rate);

    options.c_cflag &= ~PARENB;         // No parity
    options.c_cflag &= ~CSTOPB;         // 1 stop bit
    options.c_cflag &= ~CSIZE;          // Clear size bits
    options.c_cflag |= CS8;             // 8 data bits
    options.c_cflag |= CREAD | CLOCAL;  // Enable receiver, no modem control

    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    options.c_iflag &= ~(IXON | IXOFF | IXANY);
    options.c_oflag &= ~OPOST;

    fcntl(uart_fd, F_SETFL, 0);         // Blocking mode
    tcsetattr(uart_fd, TCSANOW, &options);

    return uart_fd;
}

void read_uart(int uart_fd) {
    char buffer[256];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytes_read = read(uart_fd, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            cout << "Received: " << buffer << endl;
            // if(buffer[0] == '1'){
            //     cout << "OKE RECEIVED SW3" << endl;
            // }
            // if(buffer[0] == '2'){
            //     cout << "OKE RECEIVED SW1" << endl;
            // }
        } else if (bytes_read < 0) {
            cerr << "Error reading UART: " << strerror(errno) << endl;
            break;
        }
        usleep(100000);
    }
}

int main() {
    const char* port_name = "/dev/ttyACM1"; 
    int baud_rate = B1152000;

    int uart_fd = configure_uart(port_name, baud_rate);
    if (uart_fd == -1) {
        return -1;
    }

    cout << "UART configured successfully. Waiting for data..." << endl;
    read_uart(uart_fd);
    close(uart_fd);
    return 0;
}
