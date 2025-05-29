#ifndef __IRS_IRS_KNCI_sqLite_h_
#define __IRS_IRS_KNCI_sqLite_h_

// Модуль для подключения sqlite3

#include <plugin/sqlite3/Sqlite3.h>
#define SCHEMADIALECT <plugin/sqlite3/Sqlite3Schema.h>

#ifdef C_UMK
	// **** !!! для компиляции в UMK **** !!!
	#define MODEL <sqLite.sch>
#else
	// **** !!! для компиляции в IDE **** !!!
	#define MODEL <APP_PATH\sqLite.sch>
#endif


Sqlite3Session sqlDb;

#include "Sql/sch_header.h"
#include <Sql/sch_schema.h>
#include <Sql/sch_source.h>


// проверка наличия каких-либо таблиц в sqlDb
int CheckSqLiteTables(){
	Sql sql_0(sqlDb);
	String sqlStr_0;
	sql_0.ClearError();
	sqlStr_0 << "SELECT name from sqlite_master where type = 'table'";
	sql_0.Execute(sqlStr_0);
	int count = 0;
	while(sql_0.Fetch()){
		count++;
	}
	return count;
}
bool GenSqLiteDB(const String& db_path){
#ifndef DB_PGSQL
	SQL;
	sqlDb.Open(db_path);
	if(!CheckSqLiteTables()){
		LOG("Start create: " << db_path);
		SQL = sqlDb;
		SqlSchema sch(SQLITE3);
		StdStatementExecutor se(sqlDb);
		sqlDb.SetTrace();
		All_Tables(sch);
		LOG(sch.Attributes());
		Progress p;
		p.SetText(t_("Генерация файла БД ..."));
		if(sch.ScriptChanged(SqlSchema::UPGRADE, GetExeFolder())){
			Sqlite3PerformScript(sch.Upgrade(), se, p);
		}
		if(sch.ScriptChanged(SqlSchema::ATTRIBUTES, GetExeFolder())){
			Sqlite3PerformScript(sch.Attributes(), se, p);
		}
		if(sch.ScriptChanged(SqlSchema::CONFIG, GetExeFolder())) {
			Sqlite3PerformScript(sch.ConfigDrop(), se, p);
			Sqlite3PerformScript(sch.Config(), se, p);
		}
		sch.SaveNormal(GetExeFolder());
		sqlDb.SetTrace();
		LOG("Finish create: " << db_path);
		#ifdef PLATFORM_WIN32 // !PATH_CASE
			FileDelete(Format("A_%s.sql", GetExeTitle()));
			FileDelete(Format("AD_%s.sql", GetExeTitle()));
			FileDelete(Format("S_%s.sql", GetExeTitle()));
			FileDelete(Format("SD_%s.sql", GetExeTitle()));
			FileDelete(Format("U_%s.sql", GetExeTitle()));
		#else
			FileDelete(Format("A_%s.sql", "script"));
			FileDelete(Format("AD_%s.sql", "script"));
			FileDelete(Format("S_%s.sql", "script"));
			FileDelete(Format("SD_%s.sql", "script"));
			FileDelete(Format("U_%s.sql", "script"));
		#endif


		return true;
	}
#endif
	return false;
}

/*
        SqlSchema sch(SQLITE3);
        DB2::DB2_All_Tables(sch);
        SqlPerformScript(sqlite3, sch.Upgrade());
        SqlPerformScript(sqlite3, sch.Attributes());
        SQL.ClearError();
*/

//	ExecFile(Format("%s.log", APP_SETUP.sExeName));

//		Progress pi("Генерация файла БД ...", 25);

void CheckSqLiteDB(const String& db_path){
	bool isNoDb = false;
	bool isNeedCreateDb = false;
	if(!FileExists(db_path)){
		isNoDb = true;
		String str = Format("Внимание, не найден файл БД: %s. Создать новый?", db_path);
		if(PromptYesNo(DeQtf(str))) isNeedCreateDb = true;
	}
	if(isNeedCreateDb){
		GenSqLiteDB(db_path);
		PromptOK("Для корректной работы, следует перезапустить приложение.");
		return;
	}
    sqlDb.LogErrors(true);
	if(sqlDb.Open(db_path))
		LOG(db_path << " - open");
	else
		LOG(db_path << " - not open");
}


#endif
