# Tên chương trình đầu ra
TARGET = metadata_video

# Trình biên dịch
CXX = g++

# Các flags của trình biên dịch
CXXFLAGS = -Wall -std=c++17

# Thư viện cần liên kết
LIBS = -ltag

# Tệp nguồn
SRCS = metadata_video.cpp

# Tệp đối tượng
OBJS = $(SRCS:.cpp=.o)

# Quy tắc mặc định
all: $(TARGET)

# Quy tắc để tạo chương trình
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

# Quy tắc để tạo tệp đối tượng
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

# Quy tắc dọn dẹp
clean:
	rm -f $(OBJS) $(TARGET)
