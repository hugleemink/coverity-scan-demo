#!/usr/bin/env bash
# ============================================================
# run_coverity_local.sh — Chạy Coverity Scan trên máy local
# ============================================================
# Yêu cầu: Đã cài Coverity Analysis Tool
# Tải tại: https://scan.coverity.com/download
# ============================================================

set -euo pipefail

# ── Cấu hình ─────────────────────────────────────────────
COVERITY_DIR="${COVERITY_HOME:-/opt/coverity}"   # Thư mục cài Coverity
PROJECT_NAME="${COVERITY_PROJECT:-coverity-scan-demo}"
COV_RESULTS_DIR="cov-int"
OUTPUT_ARCHIVE="analysis.tgz"

# Màu sắc terminal
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

log_info()    { echo -e "${BLUE}[INFO]${NC}  $1"; }
log_success() { echo -e "${GREEN}[OK]${NC}    $1"; }
log_warn()    { echo -e "${YELLOW}[WARN]${NC}  $1"; }
log_error()   { echo -e "${RED}[ERROR]${NC} $1" >&2; }

# ── Kiểm tra điều kiện ───────────────────────────────────
echo ""
echo "══════════════════════════════════════════════"
echo "  🔍 Coverity Scan — Local Runner"
echo "══════════════════════════════════════════════"
echo ""

# Kiểm tra Coverity đã cài chưa
if [ ! -d "$COVERITY_DIR" ]; then
    log_error "Coverity không tìm thấy tại: $COVERITY_DIR"
    log_warn  "Hãy tải và cài Coverity tại: https://scan.coverity.com/download"
    log_warn  "Sau đó set: export COVERITY_HOME=/path/to/cov-analysis-linux64-xxx"
    exit 1
fi

COV_BUILD="$COVERITY_DIR/bin/cov-build"
COV_ANALYZE="$COVERITY_DIR/bin/cov-analyze"
COV_FORMAT="$COVERITY_DIR/bin/cov-format-errors"

# ── Bước 1: Clean build ───────────────────────────────────
log_info "Bước 1/4: Dọn dẹp build cũ..."
make clean
rm -rf "$COV_RESULTS_DIR" "$OUTPUT_ARCHIVE"
log_success "Đã dọn dẹp"

# ── Bước 2: cov-build (intercept build) ───────────────────
log_info "Bước 2/4: Chạy cov-build để intercept quá trình biên dịch..."
"$COV_BUILD" --dir "$COV_RESULTS_DIR" make all
log_success "cov-build hoàn thành. Kết quả tại: ./$COV_RESULTS_DIR/"

# ── Bước 3: cov-analyze (phân tích tĩnh) ─────────────────
log_info "Bước 3/4: Phân tích code với Coverity..."
"$COV_ANALYZE" \
    --dir "$COV_RESULTS_DIR" \
    --all \
    --disable-default \
    --enable BUFFER_SIZE_WARNING \
    --enable FORWARD_NULL \
    --enable MEMORY_LEAK \
    --enable RESOURCE_LEAK \
    --enable USE_AFTER_FREE \
    --enable INTEGER_OVERFLOW \
    --enable UNINIT
log_success "Phân tích hoàn thành"

# ── Bước 4: Xem kết quả ──────────────────────────────────
log_info "Bước 4/4: Xuất kết quả..."
"$COV_FORMAT" --dir "$COV_RESULTS_DIR" --text-output-style multiline

echo ""
log_success "Scan hoàn tất!"
echo ""
echo "📦 Để submit lên scan.coverity.com:"
echo ""
echo "  tar czvf $OUTPUT_ARCHIVE $COV_RESULTS_DIR"
echo ""
echo "  curl --form token=\$COVERITY_SCAN_TOKEN \\"
echo "       --form email=\$COVERITY_EMAIL \\"
echo "       --form file=@$OUTPUT_ARCHIVE \\"
echo "       --form version=\"v1.0.0\" \\"
echo "       --form description=\"Local build\" \\"
echo "       https://scan.coverity.com/builds?project=$PROJECT_NAME"
echo ""
