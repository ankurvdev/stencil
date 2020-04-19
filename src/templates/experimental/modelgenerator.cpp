#include "common.struct.h"
//<Template file="zzFileNamezz.cpp"  datasource="modelgenerator">
//<Include>#include "zzFileNamezz.h"</Include>
//<DataSource>
//<Include>#include "zzProgram_FileNamezz.zzDataSource_Namezz.h"</Include>
//</DataSource>

namespace zzProgram_Namezz
{
namespace common
{
void* GetExtensionData()
{
    return nullptr;
}
}    // namespace common
}    // namespace zzProgram_Namezz

namespace zzProgram_Namezz
{
static const ExtensionDataArray extensionData = {
    //<Program:DataSource Join=','>
    {"zzDataSource_Namezz", zzProgram_Namezz::zzDataSource_Namezz::GetExtensionData}
    //</Program:DataSource>
};
}
namespace zzProgram_Namezz
{
//<Struct>
namespace zzStruct_Namezz
{
//<FieldAnnotation>
const char* zzStructAnnotationNamezz[] = {
    //<StructAnnotationFieldValue Join=','>
    "zzValuezz"
    //</StructAnnotationFieldValue>
};

//</FieldAnnotation>

const ModelDefinition modelDefinition = StructHandler::CreateModelDefinition(
    {
        //<Field Join=','>
        StructHandler::StructMember(zzFieldType_NativeHandlerzz::getInstance,
                                    nullptr,
                                    "zzNamezz",
                                    0,
                                    offsetof(zzStruct_Namezz::Data, zzNamezz),
                                    extensionData,
                                    zzFieldType_Idzz,
                                    0)
        //</Field>
    },
    extensionData,
    zzStruct_Idzz,
    0);
}    // namespace zzStruct_Namezz
     //</Struct>
}    // namespace zzProgram_Namezz
     //</Template>