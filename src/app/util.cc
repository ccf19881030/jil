#include "app/util.h"


#include "wx/filename.h"
#include "wx/menu.h"
#include "wx/msgdlg.h"
#include "wx/stdpaths.h"

#include "app/i18n_strings.h"

//#if defined (__WXMAC__)
//#include <Cocoa/Cocoa.h>
//#endif

namespace jil {

size_t NewBufferId() {
  static size_t buffer_id = 1;
  return buffer_id++;
}

namespace path {

wxString ExeDir() {
  wxFileName exe_fn(wxStandardPaths::Get().GetExecutablePath());
  return exe_fn.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR, wxPATH_NATIVE);
}

wxString ResourceDir() {
#if defined (__WXMAC__)
  return wxStandardPaths::Get().GetResourcesDir() + wxFILE_SEP_PATH;
#else
  return ExeDir();
#endif // __WXMAC__
}

} // namespace path

void ErrorMsg(const wxString& msg, wxWindow* parent) {
  wxMessageBox(msg, kTrError, wxOK | wxICON_ERROR | wxCENTRE);
}

void ExploreFile(const wxString& file_path) {
#if defined (__WXMSW__)
  wxString cmd = wxT("explorer /select, \"");
  cmd += file_path;
  cmd += wxT("\"");
  wxExecute(cmd);

#elif defined (__WXMAC__)
//  NSString* nsstr = [[NSString alloc] initWithUTF8String: file_path.ToUTF8().data()];
//  [[NSWorkspace sharedWorkspace] selectFile:(NSString*)nsstr inFileViewerRootedAtPath:nil];
//  [nsstr release];
#endif
}

void ClearMenuItems(wxMenu* menu) {
  while (menu->GetMenuItemCount() > 0) {
    menu->Destroy(menu->FindItemByPosition(0));
  }
}

wxString GetEncodingDisplayName(editor::EncodingId enc_id) {
  static const wxString kDisplayNames[editor::ENCODING_COUNT] = {
    TR_ENCODING_UTF8,
    TR_ENCODING_UTF8_BOM,
    TR_ENCODING_UTF16_BE,
    TR_ENCODING_UTF16_LE,
    TR_ENCODING_GB18030,
    TR_ENCODING_BIG5,
    TR_ENCODING_SHIFT_JIS,
    TR_ENCODING_EUC_JP,
    TR_ENCODING_KOI8_R,
    TR_ENCODING_ISO_8859_1,
    TR_ENCODING_ISO_8859_2,
    TR_ENCODING_ISO_8859_5,
    TR_ENCODING_ISO_8859_7,
    TR_ENCODING_TIS_620,
    TR_ENCODING_WINDOWS_1250,
    TR_ENCODING_WINDOWS_1251,
    TR_ENCODING_WINDOWS_1253,
    TR_ENCODING_MAC_CYRILLIC,
  };

  return kDisplayNames[enc_id];
}

wxString GetFileFormatName(editor::FileFormat ff) {
  static const wxString kFileFormatNames[4] = {
    wxEmptyString,
    wxT("CRLF"),
    wxT("LF"),
    wxT("CR"),
  };
  return kFileFormatNames[ff];
}

} // namespace jil
