#include "editor/renderer.h"

#include <algorithm>  // std::min

#ifdef __WXMSW__
// Must be included before "wx/dc.h"!
#include "wx/msw/private.h"
#endif  // __WXMSW__

#include "wx/dc.h"

namespace editor {

Renderer::Renderer(wxDC* dc) : dc_(dc) {
  UpdateCharSize();
}

Renderer::~Renderer() {
}

void Renderer::SetFont(const wxFont& font) {
  dc_->SetFont(font);

  // Font affects char size.
  UpdateCharSize();
}

void Renderer::SetTextForeground(const wxColour& fg) {
  dc_->SetTextForeground(fg);
}

void Renderer::SetTextBackground(const wxColour& bg) {
  dc_->SetTextBackground(bg);
}

const wxBrush& Renderer::GetBrush() const {
  return dc_->GetBrush();
}

void Renderer::SetBrush(const wxBrush& brush, bool backup) {
  if (backup) {
    brush_ = GetBrush();
  }
  dc_->SetBrush(brush);
}

const wxPen& Renderer::GetPen() const {
  return dc_->GetPen();
}

void Renderer::SetPen(const wxPen& pen, bool backup) {
  if (backup) {
    pen_ = GetPen();
  }
  dc_->SetPen(pen);
}

void Renderer::SetStyle(const wxBrush& brush, const wxPen& pen, bool backup) {
  SetBrush(brush, backup);
  SetPen(pen, backup);
}

void Renderer::SetStyle(const wxColour& brush_color, const wxColour& pen_color,
                        bool backup) {
  if (brush_color.IsOk()) {
    SetBrush(wxBrush(brush_color), backup);
  } else {
    SetBrush(*wxTRANSPARENT_BRUSH, backup);
  }

  if (pen_color.IsOk()) {
    SetPen(wxPen(pen_color), backup);
  } else {
    SetPen(*wxTRANSPARENT_PEN, backup);
  }
}

void Renderer::BackupBrush() {
  brush_ = GetBrush();
}

void Renderer::RestoreBrush() {
  dc_->SetBrush(brush_);
}

void Renderer::BackupPen() {
  pen_ = GetPen();
}

void Renderer::RestorePen() {
  dc_->SetPen(pen_);
}

void Renderer::BackupStyle() {
  brush_ = GetBrush();
  pen_ = GetPen();
}

void Renderer::RestoreStyle() {
  dc_->SetBrush(brush_);
  dc_->SetPen(pen_);
}

void Renderer::DrawText(const std::wstring& text, Coord off, Coord len,
                        int x, int y, int* w) {
  assert(len > 0);

#ifdef __WXMSW__
  // Draw text with Win32 API to avoid string copy.

  HDC hdc = (HDC)dc_->GetHDC();

  const wxColour& fg = dc_->GetTextForeground();
  assert(fg.IsOk());
  COLORREF old_fg = ::SetTextColor(hdc, fg.GetPixel());

  // To simplify the usage of renderer, determine bk_mode by text bg
  // instead of dc.GetBackgroundMode().
  int new_bk_mode = TRANSPARENT;
  const wxColour& bg = dc_->GetTextBackground();
  COLORREF old_bg = 0;
  if (bg.IsOk()) {
    new_bk_mode = OPAQUE;
    old_bg = ::SetBkColor(hdc, bg.GetPixel());
  }
  int old_bk_mode = ::SetBkMode(hdc, new_bk_mode);

  ::ExtTextOutW(hdc, x, y, 0, NULL, text.c_str() + off, len, NULL);

  if (w != NULL) {
    SIZE size;
    ::GetTextExtentPoint32W(hdc, text.c_str() + off, len, &size);
    *w = size.cx;
  }

  ::SetTextColor(hdc, old_fg);
  ::SetBkMode(hdc, old_bk_mode);
  if (new_bk_mode == OPAQUE) {
    ::SetBkColor(hdc, old_bg);
  }

#else
  wxString wx_text(text.substr(off, len));
  dc_->DrawText(wx_text, x, y);
  if (w != NULL) {
    dc_->GetTextExtent(wx_text, w, NULL);
  }
#endif  // __WXMSW__
}

void Renderer::DrawLine(int x1, int y1, int x2, int y2) {
  dc_->DrawLine(x1, y1, x2, y2);
}

void Renderer::DrawRectangle(int x, int y, int w, int h) {
  dc_->DrawRectangle(x, y, w, h);
}

void Renderer::DrawRectangle(const wxRect& rect) {
  dc_->DrawRectangle(rect);
}

void Renderer::DrawRoundedRectangle(const wxRect& rect, double radius) {
  dc_->DrawRoundedRectangle(rect, radius);
}

void Renderer::DrawWhiteSpaces(int x, int y, Coord count, int* w) {
  int space_x = x + char_size_.x / 2;
  int space_y = y + char_size_.y / 2;

  for (Coord i = 0; i < count; ++i) {
    dc_->DrawRectangle(space_x, space_y, 2, 2);
    space_x += char_size_.x;
  }

  if (w != NULL) {
    *w = char_size_.x * count;
  }
}

void Renderer::DrawTab(int x, int y, int w, int h) {
  int yc = y + h / 2;
  int x2 = x + w - 1;
  int size = std::min(w, h / 4);
  dc_->DrawLine(x + 1, yc, x2 + 1, yc);
  dc_->DrawLine(x2 - size, yc - size, x2, yc);
  dc_->DrawLine(x2 - size, yc + size, x2, yc);
}

void Renderer::UpdateCharSize() {
  char_size_.x = dc_->GetCharWidth();
  char_size_.y = dc_->GetCharHeight();
}

}  // namespace editor
