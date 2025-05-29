#ifndef __HCM4A_CalcFS4A_ctrlReport_h_
#define __HCM4A_CalcFS4A_ctrlReport_h_


class FontSelect : public WithFrmFontSel<TopWindow>{
	typedef FontSelect CLASSNAME;
public:
	Font font;
	Event<Font> WhenAction;
	FontSelect(){
		CtrlLayoutOKCancel(*this, t_("Font"));
		cb_FontName.WhenAction = [=]{SelectFont();};
		cb_FontSize.WhenAction = [=]{SelectFont();};
		ch_FontBold.WhenAction = [=]{SelectFont();};

		cb_FontName.Clear();
		Upp::Index<String> fni;
		for(int i = 0; i < Font::GetFaceCount(); i++){
			if(Font::GetFaceInfo(i)) {
				String n = Font::GetFaceName(i);
				if(fni.Find(n) < 0) {
					fni.Add(n);
					cb_FontName.Add(i, n);
				}
			}
		}
		cb_FontName.SetIndex(0);
		cb_FontSize.ClearList();
		for(int i = 6; i < 64; i++){
			cb_FontSize.Add(i);
		}
		SelectFont();
		font = GetStdFont();
	}
	void SetFont(Font f){
		int fi = f.GetFace();
		if(!cb_FontName.HasKey(fi)){
			fi = cb_FontName.FindValue(f.GetFaceName());
			if(fi < 0)
				fi = Font::COURIER;
			else
				fi = cb_FontName.GetKey(fi);
		}
		cb_FontName.SetData(fi);
		SelectFont();
		cb_FontSize.SetData(f.GetHeight());
		for(int i = 0; i < cb_FontSize.GetCount(); i++){
			int q = cb_FontSize.GetKey(i);
			if(f.GetHeight() <= q) {
				cb_FontSize.SetData(q);
				break;
			}
		}
		font = f;
		ch_FontBold = f.IsBold();
	}
	Font GetFont(){
		Font f(cb_FontName.GetData(), cb_FontSize.GetData());
		f.Bold(ch_FontBold);
		font = f;
		return f;
	}
	void Jsonize(JsonIO& json){
		json
			("font", font)
			;
	}
	void ToJson(){
		StoreAsJsonFile(*this, GetFileDirectory(Format("%s.json", "ini")));
	}
	void FromJson(){
		LoadFromJsonFile(*this, GetFileDirectory(Format("%s.json", "ini")));
		if(font.GetHeight()==0)	 font = GetStdFont().Bold();
		SetFont(font);
	}
private:
	void SelectFont() {WhenAction(GetFont());}
};
//!---------------------------------------------------------------------------------------


class FormReport : public WithFrmReport<TopWindow> {
public:
	typedef FormReport CLASSNAME;
	bool isShow;
	String fileName;
	MenuBar menu;
	void OnFrmClose();
	void Serialize(Stream& s);
	void Save();
	void SetFont();
	void MainMenu(Bar& bar) {
		bar.Sub(t_("File"), [=](Bar& bar) { FileMenu(bar); });
		bar.Sub(t_("Setup"), [=](Bar& bar) { SetupMenu(bar); });
	}
	void FileMenu(Bar& bar) {
		bar.Add(t_("Save"), [=] { Save(); });
		bar.Separator();
		bar.Add(t_("Exit"), [=] { OnFrmClose(); });
	}
	void SetupMenu(Bar& bar) {
		bar.Add(t_("Font"), [=] { SetFont(); });
	}
	String GetReportFileName();
	FormReport();
};
FormReport::FormReport(){
	CtrlLayout(*this, String(t_("")));
	Sizeable().MinimizeBox(false).MaximizeBox(false);
	WhenClose = THISBACK(OnFrmClose);
	AddFrame(menu);
	menu.Set([=](Bar& bar) { MainMenu(bar); });
	fileName = "";
	isShow = false;
}
void FormReport::Serialize(Stream& s){
	int version = 5;
	s / version;
	SerializePlacement(s);
	de_Report.GetFont().Serialize(s);
}
void FormReport::OnFrmClose(){
	isShow = false;
	Close();
}
void FormReport::Save(){
	FileSel fs;
	fs.ReadOnlyOption(true);
	fs.MkDirOption(false);
	fs.ActiveDir(GetExeFolder());
	fs.EditFileName(true);
	fs.Set(GetReportFileName());
	fs.Type("file TXT: [*.txt]", "*.txt");
	if(fs.ExecuteSaveAs()){
		const String fileName = fs;
		PrintToFile(fileName, de_Report.GetData());
	}
}
String FormReport::GetReportFileName(){
	if(!fileName.IsEmpty())
		return Format(t_("Report_%s"), GetFileName(fileName));
	else
		return Format(t_("Report_%s"), ".txt");
}
void FormReport::SetFont(){
	FontSelect fontSelect;
	fontSelect.SetFont(de_Report.GetFont());
	switch(fontSelect.Execute()){
		case IDOK:
			de_Report.SetFont(fontSelect.GetFont());
			break;
		case IDCANCEL:
			return;
	}
}
//!---------------------------------------------------------------------------------------

#endif
