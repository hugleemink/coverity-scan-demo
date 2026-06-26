# ============================================================
# Makefile — Coverity Scan Demo Project
# ============================================================

CC      = gcc
CFLAGS  = -Wall -Wextra -Wpedantic -std=c11 -g -Iinclude
TARGET  = coverity_demo
SRCDIR  = src
OBJDIR  = obj

# Thu thập tất cả file .c trong src/
SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# ─── Build target chính ────────────────────────────────────
.PHONY: all clean help

all: $(OBJDIR) $(TARGET)
	@echo "✅ Build thành công: ./$(TARGET)"

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJDIR):
	@mkdir -p $(OBJDIR)

# ─── Build với AddressSanitizer (phát hiện runtime errors) ─
asan:
	$(CC) $(CFLAGS) -fsanitize=address,undefined -o $(TARGET)_asan $(SRCS)
	@echo "✅ ASAN build: ./$(TARGET)_asan"

# ─── Dọn dẹp ───────────────────────────────────────────────
clean:
	@rm -rf $(OBJDIR) $(TARGET) $(TARGET)_asan cov-int/ analysis.tgz
	@echo "🧹 Đã dọn dẹp"

# ─── Chạy chương trình ─────────────────────────────────────
run: all
	@echo "\n🚀 Chạy chương trình:\n"
	./$(TARGET)

# ─── Kiểm tra syntax ───────────────────────────────────────
check:
	$(CC) $(CFLAGS) --analyze $(SRCS) 2>&1 | head -50

# ─── Hướng dẫn ─────────────────────────────────────────────
help:
	@echo "Các lệnh có thể dùng:"
	@echo "  make all    — Build project"
	@echo "  make run    — Build và chạy"
	@echo "  make asan   — Build với AddressSanitizer"
	@echo "  make clean  — Xóa file build"
	@echo "  make check  — Kiểm tra syntax"
