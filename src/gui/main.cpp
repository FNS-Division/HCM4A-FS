#include "CalcFs4A.h"

GUI_APP_MAIN
{
	#ifdef _DEBUG
		LOG("Debug mode!");
	#endif
	StdLogSetup(LOG_FILE);
	SetLanguage(LNGC_('E','N','E','N', CHARSET_UNICODE));
	SetDefaultCharset(CHARSET_UTF8);
	// First is year, second is month and third is day.
	SetDateFormat("%3:02d.%2:02d.%1:4d");
	SetDateScan("dmy");
	
	// CheckSqLiteDB(Format("%s.db", GetExeTitle()));
	CalcFs4A calcFs4A;
	calcFs4A.Icon(Images::ICON_64());
	calcFs4A.BeforeFrmShow();
	calcFs4A.Run();
	calcFs4A.BeforeFrmClose();
}
