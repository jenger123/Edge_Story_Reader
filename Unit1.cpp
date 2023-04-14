//---------------------------------------------------------------------------
#include "sqlite3.h"
#include <string>
#include <windows.h>
#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "VirtualTrees"
#pragma resource "*.dfm"
TForm1 *Form1;

typedef struct
{
int id;
String name;
UnicodeString surname;

} TreeData;


sqlite3 * DB;
char * errmsg;
sqlite3_stmt * State;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
VirtualStringTree1->NodeDataSize = sizeof(TreeData);

}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
   auto dbopen=sqlite3_open16(L"History",&DB);
     auto sql = "select * from urls;";
   auto result = sqlite3_prepare_v2(DB,sql,-1,&State,NULL);
   if (result!=SQLITE_OK) {
	ShowMessage("Не удалось подготовить запрос");
	ExitProcess(0);
   }
   VirtualStringTree1->BeginUpdate();
   while(true)
{
	result = sqlite3_step(State);
	if(result != SQLITE_ROW) break;
	PVirtualNode Node = VirtualStringTree1->AddChild(NULL);
		TreeData* treeData = (TreeData*)VirtualStringTree1->GetNodeData(Node);
	treeData->id= sqlite3_column_int(State, 0);
	  treeData->name = (char*)sqlite3_column_text(State,1);
	  treeData->surname = (WCHAR*)sqlite3_column_text16(State,2);
}
VirtualStringTree1->EndUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormDestroy(TObject *Sender)
{
  sqlite3_finalize(State);
auto dbclose = sqlite3_close(DB);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::VirtualStringTree1GetText(TBaseVirtualTree *Sender, PVirtualNode Node,
          TColumnIndex Column, TVSTTextType TextType, UnicodeString &CellText)

{
   TreeData* treeData = (TreeData*)Sender->GetNodeData(Node);
	switch (Column) {
        case 0:
			CellText = treeData->id;
			break;
		case 1:
			CellText =treeData->name;
			break;
		case 2:
			CellText = treeData->surname;
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::VirtualStringTree1NodeClick(TBaseVirtualTree *Sender, const THitInfo &HitInfo)
          
{
	auto node = Sender->GetFirstSelected();
	TreeData * treeData = (TreeData*)Sender->GetNodeData(node);
	std::string sql = "SELECT datetime((last_visit_time/1000000)-11644473600,'unixepoch', 'localtime') AS time FROM urls where id ="+ std::to_string(treeData->id) + ";";
	auto result = sqlite3_prepare_v2(DB,sql.c_str(),-1,&State,NULL);
	result = sqlite3_step(State);
	std::string s = (char*)sqlite3_column_text(State,0);
	s = "Дата и время последнего визита: "+s;
    Label1->Caption=s.c_str();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject *Sender)
{
auto node = VirtualStringTree1->GetFirstSelected();
for(int i=0;i<VirtualStringTree1->SelectedCount	;++i)
{
TreeData * treeData =(TreeData*)VirtualStringTree1->GetNodeData(node);
std::string sql = "delete from urls where id =" + std::to_string(treeData->id) + ";";
auto del = sqlite3_exec(DB,sql.c_str(),NULL,NULL,&errmsg);
node = VirtualStringTree1->GetNextSelected(node);
}
VirtualStringTree1->DeleteSelectedNodes();

}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button3Click(TObject *Sender)
{
VirtualStringTree1->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button4Click(TObject *Sender)
{
auto sql = "delete from urls";
auto ClearTabDB = sqlite3_exec(DB,sql,NULL,NULL,&errmsg);
 VirtualStringTree1->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender)
{
VirtualStringTree1->Clear();
auto sql = "select * from urls;";
   auto result = sqlite3_prepare_v2(DB,sql,-1,&State,NULL);
   if (result!=SQLITE_OK) {
	ShowMessage("Не удалось подготовить запрос");
	ExitProcess(0);
   }
   VirtualStringTree1->BeginUpdate();
   while(true)
{
	result = sqlite3_step(State);
	if(result != SQLITE_ROW) break;
	PVirtualNode Node = VirtualStringTree1->AddChild(NULL);
		TreeData* treeData = (TreeData*)VirtualStringTree1->GetNodeData(Node);
	treeData->id= sqlite3_column_int(State, 0);
	  treeData->name = (char*)sqlite3_column_text(State,1);
	  treeData->surname = (WCHAR*)sqlite3_column_text16(State,2);
}
VirtualStringTree1->EndUpdate();
}
//---------------------------------------------------------------------------

