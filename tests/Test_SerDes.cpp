#include "TestUtils.h"
#include "Transactions.pidl.h"

#include "stencil/jsonserdes.h"
#include "stencil/serdes.h"

template <typename InputIterator1, typename InputIterator2>
bool range_equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2)
{
    while (first1 != last1 && first2 != last2)
    {
        if (*first1 != *first2) return false;
        ++first1;
        ++first2;
    }
    return (first1 == last1) && (first2 == last2);
}

template <typename SerDes> struct TestSerDes
{
    TestSerDes() = default;

    CLASS_DELETE_COPY_AND_MOVE(TestSerDes);

    void Test(Transactions::Object::Data const& obj)
    {
        {
            Stencil::Visitor<const Transactions::Object::Data> visitor(obj);
            SerDes::Serialize(visitor, _accumulated);
        }
        {
            Transactions::Object::Data obj1;
            std::string                serialized1;
            std::string                serialized2;
            {
                std::ostringstream                                 osstrm;
                Stencil::Visitor<const Transactions::Object::Data> visitor(obj);
                SerDes::Serialize(visitor, osstrm);
                serialized1 = osstrm.str();
            }
            {
                Stencil::Visitor<const Transactions::Object::Data> visitor(obj);
                SerDes::Serialize(visitor, _accumulated);
                _accumulated << "\n";
            }
            {
                std::istringstream                           isstrm(serialized1);
                Stencil::Visitor<Transactions::Object::Data> visitor(obj1);
                SerDes::Deserialize(visitor, isstrm);
            }
            {
                std::ostringstream                                 osstrm;
                Stencil::Visitor<const Transactions::Object::Data> visitor(obj1);
                SerDes::Serialize(visitor, osstrm);
                serialized2 = osstrm.str();
            }
            REQUIRE(serialized1 == serialized2);
        }
    }

    std::ostringstream _accumulated;
};

TEST_CASE("SerDes", "[serdes]")
{
    TestSerDes<Stencil::JsonSerDes>   tester1;
    TestSerDes<Stencil::BinarySerDes> tester2;

    Transactions::Object::Data obj;
    tester1.Test(obj);
    tester2.Test(obj);

    obj.obj1().set_val1(1);
    obj.obj1().set_val2(2);
    obj.obj1().set_val3(3);
    obj.obj1().set_val4("4");
    obj.obj1().set_val5(5.0);
    obj.obj2().set_val1(true);
    obj.obj2().set_val2(6.0);
    obj.obj3().obj1().set_val1(7);
    obj.obj3().obj1().set_val2(8);
    obj.obj3().obj1().set_val3(9);
    obj.obj3().obj1().set_val4("10");
    obj.obj3().obj1().set_val5(11.0);

    tester1.Test(obj);
    tester2.Test(obj);

    Transactions::ListObject::Data lobj;
    lobj.obj1().set_val1(12);
    lobj.obj1().set_val2(13);
    lobj.obj1().set_val3(14);
    lobj.obj1().set_val4("15");
    lobj.obj1().set_val5(16);
    obj.list1().listobj().push_back(std::move(lobj));

    tester1.Test(obj);
    tester2.Test(obj);

    obj.list1().listobj().push_back(Transactions::ListObject::Data());

    tester1.Test(obj);
    tester2.Test(obj);
}
