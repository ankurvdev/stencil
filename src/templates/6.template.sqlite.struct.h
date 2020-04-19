// Anything here is not part of the generated code
struct ModelDefinition
{
};
const int EnumKeyCount = 0;
const int EnumCount    = 0;
int       resolve_enum(const char** names, int count, const char* name);
struct zzFieldType_NativeTypezz
{
    int a;
};
#include "common.struct.h"

//<Template file="zzFileNamezz.sqlite.h" datasource="sqlite">
#include "SqliteDataWalker.h"
auto CreateSql_struct = "";
auto InsertSql_struct = "";
auto QuerySql_struct  = "";
//<Struct Filter="Variant:struct">
auto CreateSql_zzNamezz = "CREATE TABLE `zzStruct_Namezz` ("
                          //<Field>
                          "`zzField_Namezz` zzFieldType_SqliteDataTypezz,"
                          //</Field>
                          "`ID` INTEGER PRIMARY KEY);";
//</Struct>

//<Struct Filter="Variant:struct">
auto InsertSql_zzNamezz = "INSERT INTO `zzStruct_Namezz` VALUES ("
                          //<Field>
                          "'%zzField_Namezz%',"
                          //</Field>
                          "NULL);";
//</Struct>

//<Struct Filter="Variant:struct">
auto QuerySql_zzNamezz = "SELECT * FROM `zzStruct_Namezz` LIMIT 1";
//</Struct>

namespace zzProgram_Namezz
{
namespace sqlite
{
void* GetExtensionData()
{
    static SqliteTables tables = {
        {"", "", "", ""},
        //<FieldType Join=','>
        {/*zzIdzz*/
         zzSqliteTableNamezz,
         zzSqliteTableCreatezz,
         zzSqliteTableQueryzz,
         zzSqliteTableInsertzz}
        //</FieldType>
    };
    return (void*)&tables;
}
}    // namespace sqlite
}    // namespace zzProgram_Namezz

//</Template>