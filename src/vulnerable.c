/**
 * vulnerable.c — Code CÓ LỖI (được cố tình viết để demo Coverity Scan)
 *
 * ⚠️  CẢNH BÁO: File này chứa các lỗi cố ý cho mục đích học tập!
 *               KHÔNG sử dụng bất kỳ đoạn code nào ở đây trong production.
 *
 * Mục đích: Minh họa khả năng phát hiện defect của Coverity Scan.
 *
 * Các lỗi được demo:
 *   1. Null Pointer Dereference     (CWE-476)
 *   2. Buffer Overflow              (CWE-120)
 *   3. Memory Leak                  (CWE-401)
 *   4. Use After Free               (CWE-416)
 *   5. Integer Overflow             (CWE-190)
 *   6. Resource Leak (File Handle)  (CWE-775)
 *   7. Uninitialized Variable       (CWE-457)
 */

#include "../include/vulnerable.h"

/* ============================================================
 * DEFECT 1: Null Pointer Dereference (CWE-476)
 * Coverity sẽ báo: "FORWARD_NULL" hoặc "NULL_RETURNS"
 * ============================================================ */
char* demo_null_pointer(int condition) {
    char *ptr = NULL;

    if (condition > 0) {
        ptr = (char*)malloc(BUFFER_SIZE);
    }

    /* ❌ LỖI: ptr có thể là NULL nếu condition <= 0
     *    Hoặc malloc thất bại không được kiểm tra
     */
    strcpy(ptr, "Hello, Coverity!");  /* NULL dereference! */

    return ptr;
}

/* ============================================================
 * DEFECT 2: Buffer Overflow (CWE-120)
 * Coverity sẽ báo: "BUFFER_SIZE_WARNING" hoặc "OVERRUN"
 * ============================================================ */
void demo_buffer_overflow(const char *input) {
    char buf[BUFFER_SIZE];  /* Buffer nhỏ chỉ 64 bytes */

    /* ❌ LỖI 1: gets() không giới hạn độ dài — deprecated & nguy hiểm */
    /* gets(buf); */

    /* ❌ LỖI 2: strcpy không kiểm tra kích thước nguồn
     *    Nếu input > 64 bytes → tràn bộ đệm!
     */
    strcpy(buf, input);

    /* ❌ LỖI 3: sprintf không giới hạn output */
    char result[10];
    sprintf(result, "Result: %s", input);  /* Tràn nếu input dài */

    printf("Buffer content: %s\n", buf);
    printf("Result: %s\n", result);
}

/* ============================================================
 * DEFECT 3: Memory Leak (CWE-401)
 * Coverity sẽ báo: "RESOURCE_LEAK" hoặc "MEMORY_LEAK"
 * ============================================================ */
int demo_memory_leak(int n) {
    /* ❌ LỖI: Cấp phát bộ nhớ nhưng không giải phóng khi gặp lỗi */
    char *buffer = (char*)malloc(n * sizeof(char));
    int  *array  = (int*)malloc(n * sizeof(int));

    if (buffer == NULL || array == NULL) {
        /* Memory leak: buffer đã được cấp phát nhưng không được free
         * trước khi return -1
         */
        return -1;
    }

    /* Xử lý dữ liệu... */
    for (int i = 0; i < n; i++) {
        array[i] = i * 2;
        buffer[i] = (char)('A' + (i % 26));
    }

    printf("Array[0] = %d, Buffer[0] = %c\n", array[0], buffer[0]);

    /* ❌ LỖI: Quên free(array) trước return trong một nhánh code */
    free(buffer);
    /* free(array);  ← bị quên! */

    return 0;
}

/* ============================================================
 * DEFECT 4: Use After Free (CWE-416)
 * Coverity sẽ báo: "USE_AFTER_FREE"
 * ============================================================ */
void demo_use_after_free(void) {
    char *data = (char*)malloc(BUFFER_SIZE);

    if (data == NULL) return;

    strcpy(data, "Sensitive data");
    printf("Before free: %s\n", data);

    free(data);  /* Giải phóng bộ nhớ */

    /* ❌ LỖI: Tiếp tục sử dụng con trỏ sau khi đã free
     *    Hành vi không xác định (Undefined Behavior)
     *    Kẻ tấn công có thể khai thác lỗ hổng này!
     */
    printf("After free: %s\n", data);   /* Use After Free! */
    data[0] = 'X';                       /* Write After Free! */
}

/* ============================================================
 * DEFECT 5: Integer Overflow (CWE-190)
 * Coverity sẽ báo: "OVERFLOW_BEFORE_WIDEN" hoặc "INTEGER_OVERFLOW"
 * ============================================================ */
int demo_integer_overflow(int a, int b) {
    /* ❌ LỖI: Không kiểm tra overflow trước khi nhân
     *    Ví dụ: a = 100000, b = 100000
     *    a * b = 10,000,000,000 > INT_MAX (2,147,483,647) → overflow!
     */
    int result = a * b;

    /* ❌ LỖI: Dùng kết quả overflow để cấp phát bộ nhớ → nguy hiểm */
    if (result > 0) {
        char *buf = (char*)malloc((size_t)result);
        if (buf) {
            /* ... */
            free(buf);
        }
    }

    return result;
}

/* ============================================================
 * DEFECT 6: Resource Leak — File Handle (CWE-775)
 * Coverity sẽ báo: "RESOURCE_LEAK"
 * ============================================================ */
int demo_resource_leak(const char *filename) {
    FILE *fp = fopen(filename, "r");

    if (fp == NULL) {
        perror("Cannot open file");
        return -1;
    }

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);

        /* ❌ LỖI: Nếu xử lý gặp điều kiện đặc biệt và return sớm,
         *    file handle fp không được đóng → resource leak!
         */
        if (line[0] == '#') {
            printf("Comment found, stopping early.\n");
            return 0;  /* ← fclose(fp) bị quên! */
        }
    }

    fclose(fp);
    return 0;
}

/* ============================================================
 * DEFECT 7: Uninitialized Variable (CWE-457)
 * Coverity sẽ báo: "UNINIT"
 * ============================================================ */
int demo_uninitialized_var(int condition) {
    int result;   /* ❌ LỖI: Không khởi tạo giá trị ban đầu */
    int count;    /* ❌ LỖI: Không khởi tạo */

    if (condition > 0) {
        result = condition * 2;
        count  = 10;
    }

    /* ❌ LỖI: Nếu condition <= 0, result và count chưa được gán giá trị
     *    nhưng vẫn được sử dụng → Undefined Behavior!
     */
    printf("Result: %d, Count: %d\n", result, count);

    return result;
}
