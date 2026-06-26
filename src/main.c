/**
 * main.c — Entry point của chương trình demo Coverity Scan
 *
 * Chương trình này gọi cả code CÓ LỖI và code ĐÃ SỬA
 * để Coverity có thể phân tích và so sánh.
 */

#include <stdio.h>
#include <stdlib.h>
#include "../include/vulnerable.h"
#include "../include/fixed.h"

void print_separator(const char *title) {
    printf("\n%s\n", title);
    printf("════════════════════════════════════════\n");
}

int main(void) {
    printf("╔══════════════════════════════════════════╗\n");
    printf("║   Coverity Scan Demo — SE193994          ║\n");
    printf("║   CaoVinhPhuc / Lê Minh Hưng             ║\n");
    printf("║   FPT University                         ║\n");
    printf("╚══════════════════════════════════════════╝\n\n");

    printf("Chương trình này minh họa các defect phổ biến\n");
    printf("mà Coverity Scan có thể phát hiện.\n");

    /* ─────────────────────────────────────────────────────
     * Demo phiên bản ĐÃ SỬA (an toàn)
     * ─────────────────────────────────────────────────────
     */
    print_separator("✅ PHIÊN BẢN ĐÃ SỬA (Fixed Version)");

    /* Fix 1: Null pointer check */
    char *ptr = fixed_null_pointer(5);
    if (ptr != NULL) {
        printf("[Fix 1] Null pointer: '%s'\n", ptr);
        free(ptr);
    }

    /* Fix 2: Buffer overflow */
    fixed_buffer_overflow("Hello, World!");

    /* Fix 3: Memory leak */
    fixed_memory_leak(10);

    /* Fix 4: Use after free */
    fixed_use_after_free();

    /* Fix 5: Integer overflow */
    int safe_result = fixed_integer_overflow(100, 200);
    printf("[Fix 5] Integer result: %d\n", safe_result);

    /* Fix 6: Resource leak — dùng file tạm */
    fixed_resource_leak("/tmp/test_coverity_demo.txt");

    /* Fix 7: Uninitialized variable */
    fixed_uninitialized_var(3);

    /* ─────────────────────────────────────────────────────
     * Lưu ý về phiên bản CÓ LỖI
     * ─────────────────────────────────────────────────────
     */
    print_separator("⚠️  PHIÊN BẢN CÓ LỖI (Vulnerable — Chỉ để Coverity phân tích)");
    printf("Các hàm trong vulnerable.c KHÔNG được gọi trực tiếp\n");
    printf("vì chúng sẽ gây crash hoặc hành vi không xác định.\n");
    printf("Coverity phân tích source code tĩnh — không cần chạy code.\n\n");

    printf("Các defect Coverity sẽ tìm thấy trong vulnerable.c:\n");
    printf("  • CWE-476: Null Pointer Dereference\n");
    printf("  • CWE-120: Buffer Copy without Checking Size\n");
    printf("  • CWE-401: Memory Leak\n");
    printf("  • CWE-416: Use After Free\n");
    printf("  • CWE-190: Integer Overflow\n");
    printf("  • CWE-775: File Descriptor Leak\n");
    printf("  • CWE-457: Use of Uninitialized Variable\n");

    print_separator("Hoàn thành");
    printf("Xem kết quả scan tại: https://scan.coverity.com/dashboard\n\n");

    return EXIT_SUCCESS;
}
