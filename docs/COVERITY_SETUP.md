# 📖 Hướng dẫn thiết lập Coverity Scan

## Tổng quan

Tài liệu này hướng dẫn từng bước đăng ký và cấu hình Coverity Scan với GitHub.

---

## Bước 1: Đăng ký tài khoản Coverity Scan

1. Truy cập [https://scan.coverity.com](https://scan.coverity.com)
2. Click **"Sign in with GitHub"**
3. Xác nhận quyền truy cập GitHub

---

## Bước 2: Thêm project vào Coverity Scan

1. Sau khi đăng nhập → click **"Add Project"**
2. Chọn **"GitHub"** làm loại project
3. Nhập tên repo của bạn: `username/repo-name`
4. Click **"Submit"**
5. Vào **"Project Settings"** → copy **Project Token**

> ⚠️ **Lưu ý**: Tên project phân biệt hoa thường và phải khớp chính xác với tên repo GitHub!

---

## Bước 3: Cấu hình GitHub Secrets

Vào repo GitHub của bạn:

```
Settings → Secrets and Variables → Actions → New Repository Secret
```

Thêm 2 secret:

| Secret Name | Giá trị |
|---|---|
| `COVERITY_SCAN_TOKEN` | Token từ Coverity Scan Project Settings |
| `COVERITY_EMAIL` | Email đăng ký tài khoản Coverity |

---

## Bước 4: Cập nhật workflow file

Mở `.github/workflows/coverity.yml`:

```yaml
- name: 🔍 Run Coverity Scan
  uses: vapier/coverity-scan-action@v1
  with:
    # ⬇️ Sửa dòng này thành tên project của bạn
    project: 'YOUR_USERNAME/YOUR_REPO_NAME'
    token: ${{ secrets.COVERITY_SCAN_TOKEN }}
    email: ${{ secrets.COVERITY_EMAIL }}
    build_command: make clean all
```

---

## Bước 5: Push và xem kết quả

```bash
git add .
git commit -m "feat: add Coverity Scan integration"
git push origin main
```

Sau 5–15 phút:
- Vào tab **Actions** trên GitHub để xem build status
- Vào [scan.coverity.com/dashboard](https://scan.coverity.com/dashboard) để xem defect report chi tiết

---

## Troubleshooting

### Lỗi: "Username/Password Authentication Failed"
- Kiểm tra lại `COVERITY_SCAN_TOKEN` — phải khớp chính xác với token trên website
- Kiểm tra tên project — phân biệt hoa thường

### Lỗi: Build không thành công
- Đảm bảo `make clean all` hoạt động bình thường trước khi thêm Coverity
- Kiểm tra Makefile và dependencies

### Không thấy kết quả trên dashboard
- Coverity xử lý mất 10–30 phút sau khi submit
- Kiểm tra email xác nhận từ Coverity

---

## Giới hạn của Coverity Scan Free Tier

| Chỉ số | Giới hạn |
|---|---|
| Loại project | Open source (public repo) |
| Số lần scan/tháng | 3 lần |
| Kích thước code | Không giới hạn |
| Thời gian lưu kết quả | 90 ngày |
