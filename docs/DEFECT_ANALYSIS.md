# 🐛 Phân tích Defect — Coverity Scan Results

## Tổng quan các Defect trong `vulnerable.c`

| # | Defect Type | CWE | Vị trí | Mức độ |
|---|---|---|---|---|
| 1 | Null Pointer Dereference | CWE-476 | `demo_null_pointer()` | 🔴 High |
| 2 | Buffer Overflow | CWE-120 | `demo_buffer_overflow()` | 🔴 High |
| 3 | Memory Leak | CWE-401 | `demo_memory_leak()` | 🟡 Medium |
| 4 | Use After Free | CWE-416 | `demo_use_after_free()` | 🔴 High |
| 5 | Integer Overflow | CWE-190 | `demo_integer_overflow()` | 🟡 Medium |
| 6 | Resource Leak | CWE-775 | `demo_resource_leak()` | 🟡 Medium |
| 7 | Uninitialized Variable | CWE-457 | `demo_uninitialized_var()` | 🟡 Medium |

---

## Phân tích chi tiết từng Defect

### 1. 🔴 Null Pointer Dereference (CWE-476)

**Coverity báo cáo**: `FORWARD_NULL` hoặc `NULL_RETURNS`

**Code lỗi**:
```c
char *ptr = NULL;
if (condition > 0) {
    ptr = malloc(BUFFER_SIZE);
}
strcpy(ptr, "Hello");  // ← ptr có thể là NULL!
```

**Tại sao nguy hiểm**: Chương trình crash (Segmentation Fault). Kẻ tấn công có thể khai thác để DoS.

**Cách sửa**:
```c
char *ptr = malloc(BUFFER_SIZE);
if (ptr == NULL) return NULL;     // ← Kiểm tra trước khi dùng
strncpy(ptr, "Hello", BUFFER_SIZE - 1);
```

---

### 2. 🔴 Buffer Overflow (CWE-120)

**Coverity báo cáo**: `BUFFER_SIZE_WARNING` hoặc `OVERRUN`

**Code lỗi**:
```c
char buf[64];
strcpy(buf, input);  // ← input có thể > 64 bytes!
```

**Tại sao nguy hiểm**: Stack smashing, có thể dẫn đến Remote Code Execution (RCE).

**Cách sửa**:
```c
char buf[64];
strncpy(buf, input, sizeof(buf) - 1);  // ← Giới hạn số byte copy
buf[sizeof(buf) - 1] = '\0';
```

---

### 3. 🟡 Memory Leak (CWE-401)

**Coverity báo cáo**: `RESOURCE_LEAK` hoặc `MEMORY_LEAK`

**Code lỗi**:
```c
char *buffer = malloc(n);
int  *array  = malloc(n * sizeof(int));
if (array == NULL) {
    return -1;  // ← buffer không được free!
}
```

**Cách sửa**:
```c
if (array == NULL) {
    free(buffer);  // ← Giải phóng buffer trước khi return
    return -1;
}
```

---

### 4. 🔴 Use After Free (CWE-416)

**Coverity báo cáo**: `USE_AFTER_FREE`

**Code lỗi**:
```c
free(data);
printf("%s", data);  // ← Undefined Behavior!
data[0] = 'X';       // ← Write After Free — lỗ hổng bảo mật!
```

**Cách sửa**:
```c
free(data);
data = NULL;  // ← Set NULL ngay sau free
// data[0] = 'X'; → Sẽ crash rõ ràng với NULL pointer
```

---

### 5. 🟡 Integer Overflow (CWE-190)

**Coverity báo cáo**: `OVERFLOW_BEFORE_WIDEN`

**Code lỗi**:
```c
// a = 100000, b = 100000
int result = a * b;  // → 10^10 > INT_MAX (2.1 * 10^9)!
```

**Cách sửa**:
```c
if (a != 0 && b > INT_MAX / a) {
    return -1;  // ← Phát hiện overflow trước khi xảy ra
}
int result = a * b;
```

---

### 6. 🟡 Resource Leak (CWE-775)

**Coverity báo cáo**: `RESOURCE_LEAK`

**Code lỗi**:
```c
FILE *fp = fopen(filename, "r");
// ... xử lý ...
if (line[0] == '#') {
    return 0;  // ← File chưa được đóng!
}
fclose(fp);
```

**Cách sửa**: Dùng `goto cleanup` hoặc cấu trúc `do-while` để đảm bảo `fclose()` luôn được gọi.

---

### 7. 🟡 Uninitialized Variable (CWE-457)

**Coverity báo cáo**: `UNINIT`

**Code lỗi**:
```c
int result;  // ← Chưa khởi tạo
if (condition > 0) {
    result = condition * 2;
}
printf("%d", result);  // ← Nếu condition <= 0, result = ???
```

**Cách sửa**:
```c
int result = 0;  // ← Luôn khởi tạo giá trị mặc định
```

---

## Kết luận

Coverity Scan phát hiện **7 loại defect** trong `vulnerable.c`, trong khi `fixed.c` không có defect nào. Điều này cho thấy hiệu quả của việc tích hợp SAST vào quy trình CI/CD.

**Defect density**: `vulnerable.c` ≈ 7 defects / ~100 LOC = **70 defects/KLOC** (rất cao)
**Mục tiêu**: < 0.35 defects/KLOC (tiêu chuẩn Coverity)
