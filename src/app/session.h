#ifndef JIL_SESSION_H_
#define JIL_SESSION_H_
#pragma once

// Session consists of:
// - Find history
// - Last opened files
// - Last cursor positions
// - Top window states
// - etc.

#include <list>
#include "wx/gdicmn.h"
#include "wx/string.h"
#include "app/defs.h"

namespace jil {

class Setting;
class SplitNode;

class Session {
public:
  Session();
  ~Session();

  bool Load(const wxString& file);
  bool Save(const wxString& file);

  //----------------------------------------------------------------------------
  // Book frame

  wxRect book_frame_rect() const {
    return book_frame_rect_;
  }
  void set_book_frame_rect(const wxRect& rect) {
    book_frame_rect_ = rect;
  }

  bool book_frame_maximized() const {
    return book_frame_maximized_;
  }
  void set_book_frame_maximized(bool maximized) {
    book_frame_maximized_ = maximized;
  }

  //----------------------------------------------------------------------------
  // Find window

  wxRect find_window_rect() const {
    return find_window_rect_;
  }
  void set_find_window_rect(const wxRect& rect) {
    find_window_rect_ = rect;
  }

  void set_find_history_limit_(size_t find_history_limit) {
    find_history_limit_ = find_history_limit;
  }

  const std::list<wxString>& find_strings() const {
    return find_strings_;
  }

  // Add a string to find history.
  // Return true if the find string is new (it didn't exist).
  bool AddFindString(const wxString& find_string) {
    return AddHistoryString(find_strings_, find_string, find_history_limit_);
  }

  const std::list<wxString>& replace_strings() const {
    return replace_strings_;
  }

  // Add a string to replace history.
  // Return true if the replace string is new (it didn't exist).
  bool AddReplaceString(const wxString& replace_string) {
    return AddHistoryString(replace_strings_, replace_string, find_history_limit_);
  }

  int find_flags() const {
    return find_flags_;
  }
  void set_find_flags(int find_flags) {
    find_flags_ = find_flags;
  }

  FindLocation find_location() const {
    return find_location_;
  }
  void set_find_location(FindLocation find_location) {
    find_location_ = find_location;
  }

  bool show_options() const {
    return show_options_;
  }
  void set_show_options(bool show_options) {
    show_options_ = show_options;
  }

  //----------------------------------------------------------------------------
  // Splitter (Layout)

  SplitNode* split_root() const {
    return split_root_;
  }

  void set_split_root(SplitNode* split_root) {
    split_root_ = split_root;
  }

  //----------------------------------------------------------------------------
  // Opened files

  class OpenedFile {
  public:
    wxString file_path;
    int stack_index;
  };

  const std::list<OpenedFile>& opened_files() const {
    return opened_files_;
  }

  void AddOpenedFile(const wxString& file_path, int stack_index) {
    OpenedFile opened_file = { file_path, stack_index };
    opened_files_.push_back(opened_file);
  }

  void ClearOpenedFiles() {
    opened_files_.clear();
  }

  //----------------------------------------------------------------------------
  // Recent files

  const std::list<wxString>& recent_files() const {
    return recent_files_;
  }
  void set_recent_files(const std::list<wxString>& recent_files) {
    recent_files_ = recent_files;
  }

private:
  bool AddHistoryString(std::list<wxString>& strings, const wxString& s, size_t limit);

  void SaveSplitTree(SplitNode* n, Setting* setting);
  SplitNode* RestoreSplitTree(Setting setting);

private:
  wxRect book_frame_rect_;
  bool book_frame_maximized_;

  wxRect find_window_rect_;

  std::list<wxString> find_strings_;
  std::list<wxString> replace_strings_;

  // Max number of find/replace strings to save.
  size_t find_history_limit_;

  // Find flags.
  // See enum FindFlag.
  int find_flags_;

  FindLocation find_location_;

  bool show_options_;

  SplitNode* split_root_;

  // The files opened when last time Jil exits.
  // If option "restore_files" is true, these files will be opened on startup.
  std::list<OpenedFile> opened_files_;

  // Recently opened files.
  // Displayed in the Recent Files menu.
  std::list<wxString> recent_files_;
};

}  // namespace jil

#endif  // JIL_SESSION_H_
