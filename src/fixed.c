/**
 * fixed.c — Code ĐÃ SỬA (phiên bản an toàn, không có defect)
 *
 * ✅ Đây là phiên bản đúng, có thể dùng trong production.
 *
 * So sánh với vulnerable.c để thấy cách khắc phục từng loại lỗi.
 */

#include "../include/fixed.h"
#include <limits.h>
#include <errno.h>

/* ============================================================
 * FIX 1: Khắc phục Null Pointer Dereference
 * ============================================================ */
char* fixed_null_pointer(int condition) {
    if (condition <= 0) {
        fprintf(stderr, "[ERROR] Condition must be positive\n");
        return NULL;
    }

    char *ptr = (char*)malloc(BUFFER_SIZE);

    /* ✅ FIX: Luôn kiểm tra malloc có thành công không */
    if (ptr == NULL) {
        fprintf(stderr, "[ERROR] Memory allocation failed: %s\n", strerror(errno));
        return NULL;
    }

    strncpy(ptr, "Hello, Coverity!", BUFFER_SIZE - 1);
    ptr[BUFFER_SIZE - 1] = '\0';  /* Đảm bảo null-terminated */

    return ptr;
}

/* ============================================================
 * FIX 2: Khắc phục Buffer Overflow
 * ============================================================ */
void fixed_buffer_overflow(const char *input) {
    if (input == NULL) {
        fprintf(stderr, "[ERROR] Input is NULL\n");
        return;
    }

    char buf[BUFFER_SIZE];
    char result[MAX_RESULT_SIZE];

    /* ✅ FIX: Dùng strncpy thay vì strcpy — giới hạn số ký tự copy */
    strncpy(buf, input, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';

    /* ✅ FIX: Dùng snprintf thay vì sprintf — giới hạn output */
    snprintf(result, sizeof(result), "Result: %s", buf);

    printf("Buffer content: %s\n", buf);
    printf("Result: %s\n", result);
}

/* ============================================================
 * FIX 3: Khắc phục Memory Leak
 * ============================================================ */
int fixed_memory_leak(int n) {
    if (n <= 0) {
        fprintf(stderr, "[ERROR] n must be positive\n");
        return -1;
    }

    char *buffer = (char*)malloc((size_t)n * sizeof(char));
    if (buffer == NULL) {
        fprintf(stderr, "[ERROR] Failed to allocate buffer\n");
        return -1;
    }

    int *array = (int*)malloc((size_t)n * sizeof(int));
    if (array == NULL) {
        fprintf(stderr, "[ERROR] Failed to allocate array\n");
        /* ✅ FIX: Giải phóng buffer trước khi return */
        free(buffer);
        return -1;
    }

    /* Xử lý dữ liệu... */
    for (int i = 0; i < n; i++) {
        array[i] = i * 2;
        buffer[i] = (char)('A' + (i % 26));
    }

    printf("Array[0] = %d, Buffer[0] = %c\n", array[0], buffer[0]);

    /* ✅ FIX: Giải phóng TẤT CẢ bộ nhớ đã cấp phát */
    free(buffer);
    free(array);

    return 0;
}

/* ============================================================
 * FIX 4: Khắc phục Use After Free
 * ============================================================ */
void fixed_use_after_free(void) {
    char *data = (char*)malloc(BUFFER_SIZE);

    if (data == NULL) {
        fprintf(stderr, "[ERROR] Memory allocation failed\n");
        return;
    }

    strncpy(data, "Sensitive data", BUFFER_SIZE - 1);
    data[BUFFER_SIZE - 1] = '\0';

    printf("Before free: %s\n", data);

    free(data);

    /* ✅ FIX: Set con trỏ về NULL ngay sau khi free
     *    Nếu vô tình dùng lại, sẽ gây crash ngay lập tức (dễ debug)
     *    thay vì hành vi không xác định (khó debug hơn nhiều)
     */
    data = NULL;

    /* Giờ không thể vô tình dùng data nữa — sẽ crash rõ ràng nếu thử */
}

/* ============================================================
 * FIX 5: Khắc phục Integer Overflow
 * ============================================================ */
int fixed_integer_overflow(int a, int b) {
    /* ✅ FIX: Kiểm tra overflow trước khi nhân
     *    Nếu a != 0 và b > INT_MAX / a → overflow sẽ xảy ra
     */
    if (a != 0 && b > INT_MAX / a) {
        fprintf(stderr, "[ERROR] Integer overflow detected: %d * %d\n", a, b);
        return -1;  /* Trả về lỗi thay vì cho overflow */
    }

    if (a != 0 && b < INT_MIN / a) {
        fprintf(stderr, "[ERROR] Integer underflow detected: %d * %d\n", a, b);
        return -1;
    }

    int result = a * b;

    /* ✅ FIX: Dùng size_t khi cấp phát bộ nhớ — tránh negative size */
    if (result > 0) {
        char *buf = (char*)malloc((size_t)result);
        if (buf != NULL) {
            /* ... */
            free(buf);
        }
    }

    return result;
}

/* ============================================================
 * FIX 6: Khắc phục Resource Leak
 * ============================================================ */
int fixed_resource_leak(const char *filename) {
    if (filename == NULL) {
        fprintf(stderr, "[ERROR] Filename is NULL\n");
        return -1;
    }

    FILE *fp = fopen(filename, "r");

    if (fp == NULL) {
        perror("Cannot open file");
        return -1;
    }

    char line[256];
    int  return_code = 0;

    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);

        if (line[0] == '#') {
            printf("Comment found, stopping early.\n");
            return_code = 0;
            /* ✅ FIX: Không return ngay — để đến cuối hàm đóng file */
            goto cleanup;
        }
    }

cleanup:
    /* ✅ FIX: Luôn đóng file handle trước khi return */
    fclose(fp);
    return return_code;
}

/* ============================================================
 * FIX 7: Khắc phục Uninitialized Variable
 * ============================================================ */
int fixed_uninitialized_var(int condition) {
    /* ✅ FIX: Khởi tạo biến với giá trị mặc định hợp lý */
    int result = 0;
    int count  = 0;

    if (condition > 0) {
        result = condition * 2;
        count  = 10;
    } else {
        fprintf(stderr, "[WARN] Condition is non-positive, using defaults\n");
    }

    printf("Result: %d, Count: %d\n", result, count);

    return result;
}
