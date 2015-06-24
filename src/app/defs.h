#ifndef JIL_DEFS_H_
#define JIL_DEFS_H_
#pragma once

#include "wx/defs.h"
#include "wx/string.h"

#define wxLR (wxLEFT|wxRIGHT)
#define wxTB (wxTOP|wxBOTTOM)
#define wxLTR (wxLEFT|wxTOP|wxRIGHT)
#define wxLRB (wxLEFT|wxRIGHT|wxBOTTOM)

namespace jil {

enum FindFlag {
  kFind_UseRegex = 1,
  kFind_CaseSensitive = 2,
  kFind_MatchWord = 4,
  kFind_Reversely = 8,  // TODO: Remove
};

enum FindLocation {
  kCurrentPage = 0,
  kAllPages,
  kSelection,
  kLocationCount,
};

const int kMaxFileTypes = 256;

// Prefix of internal file type ID.
extern const wxString kInternalFtIdPrefix;

// Internal file type IDs.
extern const wxString kFtId_FindResult;

// Top level theme IDs.
enum ThemeId {
  THEME_BOOK_FRAME = 0,
  THEME_TEXT_BOOK,
  THEME_TEXT_PAGE,
  THEME_STATUS_BAR,
  THEME_NAVIGATION_DIALOG,
  THEME_COUNT,
};

// Split parts.
const char* const kSplit_Text = "text";  // Text book
const char* const kSplit_Tool = "tool";  // Tool book

}  // namespace jil

#endif  // JIL_DEFS_H_
