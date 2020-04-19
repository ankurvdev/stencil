// Anything here is not part of the generated code
#include "DataModel.h"

struct ModelDefinition
{
};
const int EnumKeyCount     = 0;
const int EnumCount        = 0;
const int zzFieldType_Idzz = 0;
const int zzStruct_Idzz    = 0;
int       resolve_enum(const char** names, int count, const char* name);
struct zzFieldType_NativeTypezz
{
    int a;
};
struct zzChildFieldType_NativeTypezz
{
    int a;
};
struct RuntimeBindableObject
{
    void StartInitializing() {}
    void EndInitializing() {}
    bool IsValid() { return false; }
};

struct zzComponent_NativeTypezz
{
    public:
    void AddRuntimeBindings(RuntimeBindableObject& obj) const {}
};
struct zzFieldType_NativeHandlerzz
{
    static ITypeHandler* getInstance() { return (ITypeHandler*)nullptr; }
};
namespace zzProgram_Namezz
{
namespace zzDataSource_Namezz
{
void* GetExtensionData()
{
    return nullptr;
}
}    // namespace zzDataSource_Namezz
}    // namespace zzProgram_Namezz

//<Template file="zzFileNamezz.modelgenerator.h"  datasource="modelgenerator">
#include "DataModel.h"
#include "IDL3Generics.h"

namespace zzProgram_Namezz
{
//<Struct>
namespace zzStruct_Namezz
{
struct Data;
}
//</Struct>
//<Typedef>
typedef zzChildFieldType_NativeTypezz zzNamezz;
//</Typedef>
//<Struct>
namespace zzStruct_Namezz
{
extern const ModelDefinition modelDefinition;
struct ConstructorData
{
    //<Relationship>
    zzComponent_NativeTypezz::ConstructorData zzComponent_Namezz_zzIdzz;
    //</Relationship>
    //<Field Filter="zzFieldType_ConstructorDatazz">
    zzFieldType_NativeTypezz::ConstructorData zzNamezz;
    //<Field>
};

struct Data
{
    private:
    struct
    {
    } dummy;

    public:
    void AddRuntimeBindings(RuntimeBindableObject& obj) const;

    private:
    mutable RuntimeBindableObject m_RuntimeBindableObject;

    public:
    operator RuntimeBindableObject&() const;
    //<Relationship>
    private:
    zzComponent_NativeTypezz m_zzComponent_Namezz_zzIdzz;

    public:
    operator const zzComponent_NativeTypezz&() const { return this->m_zzComponent_Namezz_zzIdzz; }
    //</Relationship>
    //<Field>
    private:
    zzFieldType_NativeTypezz m_zzNamezz;

    public:
    StatusCode get_zzNamezz(zzFieldType_NativeTypezz& val)
    {
        val = this->m_zzNamezz;
        return StatusCode::OK;
    }

    public:
    StatusCode set_zzNamezz(zzFieldType_NativeTypezz& val)
    {
        this->m_zzNamezz = val;
        return StatusCode::OK;
    }
    //</Field>
    public:
    Data(ConstructorData& data) :
        dummy()
        //<Relationship>
        ,
        m_zzComponent_Namezz_zzIdzz(data.zzComponent_Namezz_zzIdzz)
        //</Relationship>
        //<Field Filter="zzFieldType_ConstructorDatazz">
        ,
        zzFieldType_NativeTypezz::ConstructorData zzNamezz;
    //<Field>

    {
    }
};

inline Data::operator RuntimeBindableObject&() const
{
    if (!m_RuntimeBindableObject.IsValid())
    {
        m_RuntimeBindableObject.StartInitializing();
        this->AddRuntimeBindings(m_RuntimeBindableObject);
        m_RuntimeBindableObject.EndInitializing();
    }
    return m_RuntimeBindableObject;
}
inline void Data::AddRuntimeBindings(RuntimeBindableObject& obj) const
{
    //<Field>
    obj.AddValueDelegate("zzNamezz", this, [](const Data* ptr, std::string& str, Value* pValue) -> StatusCode {
        DataValue val;
        zzFieldType_NativeHandlerzz::getInstance()->ToBaseType(DataType::String, &ptr->m_zzNamezz, val);
        str = std::string(val);
        return StatusCode::OK;
    });
    //</Field>

    //<Relationship>
    obj.AddRuntimeBindings(m_zzComponent_Namezz_zzIdzz);
    //</Relationship>
}
}    // namespace zzStruct_Namezz
     //</Struct>
}    // namespace zzProgram_Namezz
     //</Template>