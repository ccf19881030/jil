#ifndef EDITOR_RENDERER_H_
#define EDITOR_RENDERER_H_

#include <string>

#include "wx/brush.h"
#include "wx/colour.h"
#include "wx/font.h"
#include "wx/gdicmn.h"
#include "wx/pen.h"

#include "editor/defs.h"

class wxDC;

namespace editor {

class Renderer {
public:
  explicit Renderer(wxDC* dc);
  ~Renderer();

  void SetFont(const wxFont& font);

  void SetTextForeground(const wxColour& fg);
  void SetTextBackground(const wxColour& bg);

  const wxBrush& GetBrush() const;
  void SetBrush(const wxBrush& brush, bool backup);

  const wxPen& GetPen() const;
  void SetPen(const wxPen& pen, bool backup);

  void SetStyle(const wxBrush& brush, const wxPen& pen, bool backup);

  void SetStyle(const wxColour& brush_color, const wxColour& pen_color,
                bool backup);

  void BackupBrush();
  void RestoreBrush();

  void BackupPen();
  void RestorePen();

  void BackupStyle();
  void RestoreStyle();

  void DrawText(const std::wstring& text, Coord off, Coord len, int x, int y,
                int* w = NULL);

  void DrawLine(int x1, int y1, int x2, int y2);

  inline void DrawLineH(int x1, int x2, int y) {
    DrawLine(x1, y, x2, y);
  }

  inline void DrawLineV(int x, int y1, int y2) {
    DrawLine(x, y1, x, y2);
  }

  void DrawRectangle(int x, int y, int w, int h);
  void DrawRectangle(const wxRect& rect);
  void DrawRoundedRectangle(const wxRect& rect, double radius);

  void DrawWhiteSpaces(int x, int y, Coord count = 1, int* w = NULL);
  void DrawTab(int x, int y, int w, int h);

private:
  void UpdateCharSize();

private:
  wxDC* dc_;
  wxSize char_size_;

  // Backups.
  wxBrush brush_;
  wxPen pen_;
};

}  // namespace editor

#endif  // EDITOR_RENDERER_H_
