# 🔍 Coverity Scan Demo — Static Analysis with GitHub Actions

[![Coverity Scan Build Status](https://scan.coverity.com/projects/YOUR_PROJECT_NAME/badge.svg)](https://scan.coverity.com/projects/YOUR_PROJECT_NAME)
[![GitHub Actions](https://github.com/YOUR_USERNAME/coverity-scan-demo/actions/workflows/coverity.yml/badge.svg)](https://github.com/YOUR_USERNAME/coverity-scan-demo/actions)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

> Dự án demo tích hợp **Coverity Scan** (SAST — Static Application Security Testing) vào quy trình CI/CD với GitHub Actions, phát hiện lỗi bảo mật và chất lượng code tự động.

---

## 📋 Mục lục

- [Giới thiệu](#-giới-thiệu)
- [Cấu trúc dự án](#-cấu-trúc-dự-án)
- [Các loại lỗi được demo](#-các-loại-lỗi-được-demo)
- [Cách thiết lập](#-cách-thiết-lập)
- [Chạy Coverity Scan](#-chạy-coverity-scan)
- [Hiểu kết quả](#-hiểu-kết-quả)
- [Tài liệu tham khảo](#-tài-liệu-tham-khảo)

---

## 🎯 Giới thiệu

**Coverity Scan** là công cụ phân tích tĩnh (SAST) của Synopsys/Black Duck, giúp phát hiện:

| Loại lỗi | Mô tả |
|---|---|
| **Null Pointer Dereference** | Truy cập con trỏ null gây crash |
| **Buffer Overflow** | Tràn bộ đệm, lỗ hổng bảo mật nghiêm trọng |
| **Memory Leak** | Cấp phát bộ nhớ nhưng không giải phóng |
| **Use After Free** | Sử dụng bộ nhớ đã giải phóng |
| **Integer Overflow** | Tràn số nguyên dẫn đến hành vi không xác định |
| **SQL Injection** | Lỗ hổng bảo mật injection |
| **Resource Leak** | File/socket không được đóng đúng cách |

---

## 📁 Cấu trúc dự án

```
coverity-scan-demo/
├── .github/
│   └── workflows/
│       └── coverity.yml          # GitHub Actions workflow chính
├── src/
│   ├── main.c                    # Entry point
│   ├── vulnerable.c              # Code có lỗi (để demo Coverity phát hiện)
│   ├── fixed.c                   # Code đã sửa (so sánh)
│   └── utils.c                   # Tiện ích
├── include/
│   ├── vulnerable.h
│   ├── fixed.h
│   └── utils.h
├── tests/
│   └── test_basic.c              # Unit tests cơ bản
├── scripts/
│   ├── run_coverity_local.sh     # Chạy Coverity trên máy local
│   └── setup.sh                  # Cài đặt môi trường
├── docs/
│   ├── COVERITY_SETUP.md         # Hướng dẫn đăng ký & cấu hình
│   ├── DEFECT_ANALYSIS.md        # Phân tích các defect tìm thấy
│   └── images/                   # Ảnh minh họa kết quả scan
├── Makefile
├── .coverity_config              # Cấu hình Coverity
└── README.md
```

---

## 🐛 Các loại lỗi được demo

### 1. Null Pointer Dereference
```c
// ❌ Lỗi: không kiểm tra malloc trả về NULL
char *buf = malloc(100);
strcpy(buf, input);  // crash nếu malloc thất bại
```

### 2. Buffer Overflow
```c
// ❌ Lỗi: không kiểm tra độ dài input
char buf[10];
gets(buf);  // nguy hiểm, không giới hạn độ dài
```

### 3. Memory Leak
```c
// ❌ Lỗi: quên free() trước khi return
char *data = malloc(256);
if (error_condition) return -1;  // memory leak!
free(data);
```

### 4. Use After Free
```c
// ❌ Lỗi: sử dụng con trỏ sau khi free
free(ptr);
printf("%s", ptr);  // undefined behavior
```

---

## ⚙️ Cách thiết lập

### Bước 1: Đăng ký tài khoản Coverity Scan
1. Truy cập [scan.coverity.com](https://scan.coverity.com)
2. Đăng nhập bằng GitHub account
3. Thêm project của bạn → lấy **Project Token**

### Bước 2: Thêm Secret vào GitHub Repository
```
GitHub Repo → Settings → Secrets and Variables → Actions → New Secret

COVERITY_SCAN_TOKEN  =  <token từ scan.coverity.com>
COVERITY_EMAIL       =  <email đăng ký Coverity>
```

### Bước 3: Cập nhật workflow
Mở `.github/workflows/coverity.yml` và sửa:
```yaml
project: 'YOUR_GITHUB_USERNAME/YOUR_REPO_NAME'
```

### Bước 4: Push code
```bash
git clone https://github.com/YOUR_USERNAME/coverity-scan-demo
cd coverity-scan-demo
git add .
git commit -m "feat: initial commit with Coverity Scan setup"
git push origin main
```

---

## 🚀 Chạy Coverity Scan

### Tự động qua GitHub Actions
- **Trigger tự động**: mỗi lần push lên `main` hoặc tạo Pull Request
- Xem kết quả tại: `Actions` tab trên GitHub
- Báo cáo chi tiết: [scan.coverity.com/dashboard](https://scan.coverity.com/dashboard)

### Chạy thủ công trên máy local
```bash
# Cài đặt môi trường
chmod +x scripts/setup.sh
./scripts/setup.sh

# Build project
make clean && make

# Chạy Coverity (cần cài Coverity Analysis Tool)
chmod +x scripts/run_coverity_local.sh
./scripts/run_coverity_local.sh
```

---

## 📊 Hiểu kết quả

Sau khi scan, Coverity phân loại defect theo mức độ:

| Mức độ | Ý nghĩa |
|---|---|
| 🔴 **High** | Lỗi nghiêm trọng, cần fix ngay |
| 🟡 **Medium** | Lỗi tiềm ẩn, nên xem xét |
| 🟢 **Low** | Cảnh báo nhỏ, có thể bỏ qua |

**Defect density** (số lỗi/1000 dòng code) là chỉ số quan trọng — Coverity coi < 0.35 là tốt.

---

## 📚 Tài liệu tham khảo

- [Coverity Scan Official Docs](https://scan.coverity.com/faq)
- [GitHub Action: vapier/coverity-scan-action](https://github.com/vapier/coverity-scan-action)
- [Synopsys Coverity Documentation](https://sig-docs.synopsys.com/polaris/topics/c_coverity-overview.html)
- [SAST Best Practices](https://owasp.org/www-community/Source_Code_Analysis_Tools)

---

## 📄 License

MIT License — xem file [LICENSE](LICENSE) để biết thêm chi tiết.

---

*Được tạo bởi SE193994 — CaoVinhPhuc/Lê Minh Hưng | FPT University*
