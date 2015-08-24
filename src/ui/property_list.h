#ifndef JIL_UI_PROPERTY_LIST_H
#define JIL_UI_PROPERTY_LIST_H
#pragma once

// A replacement of wxPropertyGrid.

#include <vector>
#include "wx/scrolwin.h"
#include "wx/panel.h"

class wxTextCtrl;

namespace jil {
namespace ui {

class PropertyList;

////////////////////////////////////////////////////////////////////////////////

class HeadPanel : public wxPanel {
  DECLARE_EVENT_TABLE()

public:
  HeadPanel(PropertyList* property_ctrl, wxWindowID id);
  virtual ~HeadPanel();

  // Let body panel have the focus.
  virtual bool AcceptsFocus() const override {
    return false;
  }

protected:
  void OnPaint(wxPaintEvent& evt);
  void OnMouseEvents(wxMouseEvent& evt);
  void OnMouseCaptureLost(wxMouseCaptureLostEvent& evt);

private:
  PropertyList* property_ctrl_;
};

////////////////////////////////////////////////////////////////////////////////

class BodyPanel : public wxPanel {
  DECLARE_EVENT_TABLE()

public:
  BodyPanel(PropertyList* property_ctrl, wxWindowID id);
  virtual ~BodyPanel();

protected:
  void OnPaint(wxPaintEvent& evt);
  void OnMouseLeftDown(wxMouseEvent& evt);
  void OnSetFocus(wxFocusEvent& evt);
  void OnKillFocus(wxFocusEvent& evt);

private:
  PropertyList* property_ctrl_;
};

////////////////////////////////////////////////////////////////////////////////

class PropertyList : public wxScrolledWindow {
  DECLARE_EVENT_TABLE()

public:
  enum ColorId {
    COLOR_HEAD_FG = 0,
    COLOR_HEAD_BG,
    COLOR_HEAD_BAR,
    COLOR_BODY_BG,
    COLOR_BODY_BG_SELECT,
    COLOR_BODY_FG,
    COLOR_BODY_FG_SELECT,
    COLOR_BODY_BAR,
    COLOR_COUNT
  };

private:
  struct Property {
    wxString key;
    wxString value;
  };

public:
  PropertyList();
  virtual ~PropertyList();

  bool Create(wxWindow* parent,
              wxWindowID id,
              const wxSize& size = wxDefaultSize,
              long style = wxScrolledWindowStyle);

  const wxColour& GetColor(ColorId id) const {
    return colors_[id];
  }
  void SetColor(ColorId id, const wxColour& color) {
    colors_[id] = color;
  }

  void StartBatch();
  void EndBatch();

  void AddProperty(const wxString& key, const wxString& value);

  int GetCount() const {
    return static_cast<int>(properties_.size());
  }

protected:
  void Init();

  void CheckColors();

  Property* GetPropertyByRow(int row);

  void OnSize(wxSizeEvent& evt);

  void OnEditingDone(wxCommandEvent& evt);

  friend class HeadPanel;
  void OnHeadPaint(wxDC& dc);
  void OnHeadMouse(wxMouseEvent& evt);

  friend class BodyPanel;
  void OnBodyPaint(wxDC& dc);
  void OnBodyMouseLeftDown(wxMouseEvent& evt);
  void OnBodySetFocus(wxFocusEvent& evt);
  void OnBodyKillFocus(wxFocusEvent& evt);

  bool IsEditing() const;
  void StartEditing(int row);
  void StopEditing();

  wxRect GetRowClientRect(int row) const;
  void RefreshRow(int row);

  void UpdateLayout();
  void UpdateVirtualSize();

  int GetKeyWidth() const;
  int GetValueWidth() const;

  wxRect GetKeyRect(int row) const;
  wxRect GetValueRect(int row) const;

  int GetScrolledX(int unscrolled_x) const;
  int GetScrolledY(int unscrolled_y) const;

private:
  bool created_;
  bool batch_;

  wxColour colors_[COLOR_COUNT];

  HeadPanel* head_panel_;
  BodyPanel* body_panel_;

  wxSize row_padding_;
  int row_height_;

  std::vector<Property*> properties_;
  int selected_row_;

  wxTextCtrl* text_ctrl_;
};

}  // namespace ui
}  // namespace jil

#endif  // JIL_UI_PROPERTY_LIST_H
