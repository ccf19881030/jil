#ifndef JIL_FIND_WINDOW_H_
#define JIL_FIND_WINDOW_H_
#pragma once

// Find & Replace window.

#include "wx/minifram.h"
#include "wx/panel.h"
#include "app/defs.h"

class wxButton;
class wxComboBox;

namespace jil {

namespace ui {
class BitmapToggleButton;
}  // namespace ui

class Session;

////////////////////////////////////////////////////////////////////////////////

BEGIN_DECLARE_EVENT_TYPES()
// Check GetInt(), which returns FindWindow::EventType, for the details.
DECLARE_EVENT_TYPE(kFindWindowEvent, 0)
END_DECLARE_EVENT_TYPES()

class FindWindowEvent : public wxCommandEvent {
public:
  FindWindowEvent(int id)
      : wxCommandEvent(kFindWindowEvent, id)
      , flags_(0) {
  }

  FindWindowEvent(const FindWindowEvent& rhs)
      : wxCommandEvent(rhs)
      , flags_(rhs.flags_)
      , find_str_(rhs.find_str_)
      , replace_str_(rhs.replace_str_) {
  }

  virtual wxEvent* Clone() const {
    return new FindWindowEvent(*this);
  }

  int flags() const {
    return flags_;
  }
  void set_flags(int flags) {
    flags_ = flags;
  }

  const std::wstring& find_str() const {
    return find_str_;
  }
  void set_find_str(const std::wstring& find_str) {
    find_str_ = find_str;
  }

  const std::wstring& replace_str() const {
    return replace_str_;
  }
  void set_replace_str(const std::wstring& replace_str) {
    replace_str_ = replace_str;
  }

private:
  int flags_;
  std::wstring find_str_;
  std::wstring replace_str_;
};

typedef void (wxEvtHandler::*FindWindowEventFunction)(FindWindowEvent&);

#define FindWindowEventHandler(func)\
  (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(jil::FindWindowEventFunction, &func)

#define EVT_FIND_WINDOW(id, func)\
  DECLARE_EVENT_TABLE_ENTRY(jil::kFindWindowEvent, id, -1,\
  FindWindowEventHandler(func), (wxObject*)NULL),

////////////////////////////////////////////////////////////////////////////////

class FindWindow : public wxMiniFrame {
  DECLARE_DYNAMIC_CLASS(FindWindow)
  DECLARE_EVENT_TABLE()

public:
  enum Mode {
    kFindMode = 0,
    kReplaceMode
  };

  // Detailed event types of kFindWindowEvent.
  enum EventType {
    kFindStringEvent = 1,
    kReplaceStringEvent,
    kFindEvent,
    kReplaceEvent,
    kFindAllEvent,
    kReplaceAllEvent,
  };

public:
  FindWindow();
  FindWindow(Session* session, int mode);

  bool Create(wxWindow* parent, wxWindowID id);

  virtual ~FindWindow();

  void set_session(Session* session) {
    session_ = session;
  }

  int mode() const {
    return mode_;
  }
  void set_mode(int mode) {
    mode_ = mode;
  }

  int flags() const {
    return flags_;
  }

  void UpdateLayout();

  void SetFindString(const wxString& find_string);

 protected:
  void OnActivate(wxActivateEvent& evt);
  void OnClose(wxCloseEvent& evt);
  void OnKeyDownHook(wxKeyEvent& evt);

  void OnUseRegexToggle(wxCommandEvent& evt);
  void OnCaseSensitiveToggle(wxCommandEvent& evt);
  void OnMatchWholeWordToggle(wxCommandEvent& evt);
  void OnSearchReverselyToggle(wxCommandEvent& evt);

  void OnModeToggle(wxCommandEvent& evt);

  void OnFind(wxCommandEvent& evt);
  void OnFindAll(wxCommandEvent& evt);
  void OnReplace(wxCommandEvent& evt);
  void OnReplaceAll(wxCommandEvent& evt);

 private:
  // Add a string to find history and find combobox.
  void AddFindString(const wxString& string);
  // Add a string to replace history and replace combobox.
  void AddReplaceString(const wxString& string);

  void UpdateSizes();

  void LayoutAsFind();
  void LayoutAsReplace();

  ui::BitmapToggleButton* NewToggleButton(wxWindowID id,
                                          const wxString& bitmap,
                                          const wxString& tooltip);

  wxButton* NewButton(wxWindowID id, const wxString& label);
  
  // \param event_type See enum EventType.
  void PostEvent(int event_type,
                 const wxString& find_str,
                 const wxString& replace_str);

 private:
  Session* session_;

  int mode_;  // enum Mode

  // See enum FindFlag.
  int flags_;

  // For tab traversal.
  wxPanel* panel_;

  ui::BitmapToggleButton* regex_toggle_button_;
  ui::BitmapToggleButton* case_toggle_button_;
  ui::BitmapToggleButton* word_toggle_button_;
  ui::BitmapToggleButton* mode_toggle_button_;

  wxComboBox* find_combo_box_;
  wxComboBox* replace_combo_box_;

  wxButton* find_button_;
  wxButton* find_all_button_;
  wxButton* replace_button_;
  wxButton* replace_all_button_;
};

}  // namespace jil

#endif  // JIL_FIND_WINDOW_H_