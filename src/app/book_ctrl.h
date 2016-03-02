#ifndef JIL_BOOK_CTRL_H_
#define JIL_BOOK_CTRL_H_
#pragma once

// A replacement of wxNotebook.

#include <list>
#include <vector>
#include "wx/panel.h"
#include "wx/dcbuffer.h"
#include "editor/theme.h"
#include "app/compile_config.h"

class wxMenu;
class wxSizer;

#ifndef __WXMAC__
class wxMemoryDC;
#endif  // __WXMAC__

namespace jil {

namespace editor {
class TipHandler;
}  // namespace editor

class BookCtrl;
class BookPage;

////////////////////////////////////////////////////////////////////////////////

class BookTabArea : public wxPanel {
  DECLARE_EVENT_TABLE()

public:
  BookTabArea(BookCtrl* book_ctrl, wxWindowID id);

  virtual ~BookTabArea();

  // Page header has no focus.
  virtual bool AcceptsFocus() const override {
    return false;
  }

  void SetToolTipEx(const wxString& tooltip);

protected:
  virtual wxSize DoGetBestSize() const;

  void OnSize(wxSizeEvent& evt);
  void OnPaint(wxPaintEvent& evt);
  void OnMouseEvents(wxMouseEvent& evt);
  void OnMouseCaptureLost(wxMouseCaptureLostEvent& evt);

private:
  BookCtrl* book_ctrl_;
  editor::TipHandler* tip_handler_;
};

////////////////////////////////////////////////////////////////////////////////

class BookCtrl : public wxPanel {
public:
  enum ColorId {
    COLOR_BG = 0,  // The whole book background

    COLOR_TAB_AREA_BG,  // The tab area background

    // Tab item colors:
    COLOR_TAB_FG,
    COLOR_ACTIVE_TAB_FG,
    COLOR_TAB_BG,
    COLOR_ACTIVE_TAB_BG,
    COLOR_TAB_BORDER,
    COLOR_ACTIVE_TAB_BORDER,

    COLORS,
  };

protected:
  class Tab {
  public:
    Tab(BookPage* _page, int _best_size, bool _active)
        : page(_page)
        , best_size(_best_size)
        , size(_best_size)
        , active(_active) {
    }

    BookPage* page;

    // Tab size.
    int best_size;  // Size fitting the label
    int size;  // Display size

    bool active;
  };

  typedef std::list<Tab*> TabList;
  typedef TabList::iterator TabIter;
  typedef TabList::const_iterator TabConstIter;

public:
  BookCtrl();
  virtual ~BookCtrl();

  void set_theme(const editor::SharedTheme& theme) {
    theme_ = theme;
  }

  bool Create(wxWindow* parent, wxWindowID id);

  virtual bool HasFocus() const override;
  virtual void SetFocus() override;

  // NOTE: Call after Create().
  void SetTabFont(const wxFont& tab_font);

  virtual void ReapplyTheme();

  // Set batch flag to avoid unnecessary resizing tabs and refresh.
  // Example:
  //   notebook->StartBatch();
  //   notebook->AddPage(...);
  //   notebook->AddPage(...);
  //   ...
  //   notebook->EndBatch();
  void StartBatch();
  void EndBatch();

  // NOTE: Don't check if the page is already in the book or not.
  virtual void AddPage(BookPage* page, bool active);

  bool RemovePage(const BookPage* page);

  bool RemoveActivePage();

  // The except page, if specified, won't be removed.
  void RemoveAllPages(const BookPage* except_page = NULL);

  size_t PageCount() const {
    return tabs_.size();
  }

  bool IsEmpty() const {
    return PageCount() == 0;
  }

  void ActivatePage(BookPage* page);

  BookPage* ActivePage() const;

  void SwitchToNextPage();
  void SwitchToPrevPage();

  int GetStackIndex(BookPage* page) const;

  void MovePageToStackFront(BookPage* page);

  std::vector<BookPage*> Pages() const;
  std::vector<BookPage*> StackPages() const;

  // Return the page next to the given page.
  BookPage* NextPage(const BookPage* page) const;

  // Recalculate the size for each tab.
  void ResizeTabs();

protected:
  void Init();

  // Update values determined by tab font.
  void UpdateTabFontDetermined();

  //----------------------------------------------------------------------------
  // Tab area event handlers.

  friend class BookTabArea;

  void OnTabSize(wxSizeEvent& evt);

  void OnTabPaint(wxDC& dc, wxPaintEvent& evt);

  void OnTabMouse(wxMouseEvent& evt);
  void OnTabMouseLeftDown(wxMouseEvent& evt);
  void OnTabMouseLeftUp(wxMouseEvent& evt);
  void OnTabMouseMiddleDown(wxMouseEvent& evt);
  void OnTabMouseMiddleUp(wxMouseEvent& evt);
  void OnTabMouseMotion(wxMouseEvent& evt);
  void OnTabMouseRightDown(wxMouseEvent& evt);
  void OnTabMouseRightUp(wxMouseEvent& evt);
  void OnTabMouseLeftDClick(wxMouseEvent& evt);

  virtual void HandleTabMouseLeftDown(wxMouseEvent& evt);
  virtual void HandleTabMouseLeftUp(wxMouseEvent& evt);
  virtual void HandleTabMouseMiddleUp(wxMouseEvent& evt);
  virtual void HandleTabMouseRightDown(wxMouseEvent& evt);
  virtual void HandleTabMouseRightUp(wxMouseEvent& evt) {}
  virtual void HandleTabMouseLeftDClick(wxMouseEvent& evt) {}

  //----------------------------------------------------------------------------

  TabIter TabByPos(int pos_x);
  BookPage* PageByPos(int pos_x);

  //Tab* GetTabByWindow(wxWindow* window, size_t* index = NULL);

  void ActivatePage(TabIter it);
  bool RemovePage(TabIter it);

  virtual void DoActivateTab(Tab* tab, bool active) = 0;
  virtual void DoRemoveTab(Tab* tab) = 0;
  virtual void DoRemoveAll(Tab* tab) = 0;

  void ActivatePageByPos(int pos_x);

  TabIter ActiveTab();
  TabIter TabByPage(const BookPage* page);
  TabConstIter TabByPage(const BookPage* page) const;

  int CalcTabBestSize(const wxString& label) const;

  wxSize CalcTabAreaBestSize() const;

  void PostEvent(wxEventType event_type);

protected:
  editor::SharedTheme theme_;

  int char_width_;
  int ellipsis_width_;  // Size of "...".

  int tab_margin_top_;
  wxSize tab_padding_;

  // The space before/after the first/last tab.
  int tab_area_padding_x_;

  int tab_min_size_;
  int tab_default_size_;

  // Tab area free/available size.
  int free_size_;

  BookTabArea* tab_area_;
  wxPanel* page_area_;

  TabList tabs_;

  // The front tab is the current active tab.
  // A tab is moved to the front when it's activated.
  TabList stack_tabs_;

  bool batch_;
  bool need_resize_tabs_;
};

////////////////////////////////////////////////////////////////////////////////
// Events

BEGIN_DECLARE_EVENT_TYPES()
// Events posted by book ctrl:
DECLARE_EVENT_TYPE(kEvtBookPageChange, 0)  // Page(s) added or removed.
DECLARE_EVENT_TYPE(kEvtBookPageSwitch, 0)  // Page activated.
END_DECLARE_EVENT_TYPES()

}  // namespace jil

#define EVT_BOOK_PAGE_CHANGE(id, func)\
  DECLARE_EVENT_TABLE_ENTRY(::jil::kEvtBookPageChange, id, -1, \
  wxCommandEventHandler(func), (wxObject*)NULL),

#define EVT_BOOK_PAGE_SWITCH(id, func)\
  DECLARE_EVENT_TABLE_ENTRY(::jil::kEvtBookPageSwitch, id, -1, \
  wxCommandEventHandler(func), (wxObject*)NULL),

#endif  // JIL_BOOK_CTRL_H_
