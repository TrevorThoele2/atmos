#include <boost/test/unit_test.hpp>

#include "PositionalOffsetAdapterTests.h"

#include <Atmos/PositionalOffsetAdapter.h>

PositionalOffsetAdapterTestsFixture::PositionalOffsetAdapterTestsFixture()
{
    typeGroup->Register<DerivedObject>();

    typeRegistration.PushAllTo(savedObjectManager);
}

PositionalOffsetAdapterTestsFixture::DerivedObject::DerivedObject(ObjectManager& manager) :
    AxisAlignedObject(manager)
{}

PositionalOffsetAdapterTestsFixture::DerivedObject::DerivedObject(const ::Inscription::BinaryTableData<DerivedObject>& data) :
    AxisAlignedObject(std::get<0>(data.bases)), number(data.number)
{}

ObjectTypeDescription PositionalOffsetAdapterTestsFixture::DerivedObject::TypeDescription() const
{
    return ObjectTraits<DerivedObject>::TypeDescription();
}

namespace Atmos
{
    const ObjectTypeName ObjectTraits<PositionalOffsetAdapterTestsFixture::DerivedObject>::typeName =
        "DerivedObject";
}

using DerivedObjectOffset = PositionalOffsetAdapter<PositionalOffsetAdapterTestsFixture::DerivedObject>;
using DerivedObjectReference = TypedObjectReference<PositionalOffsetAdapterTestsFixture::DerivedObject>;

BOOST_FIXTURE_TEST_SUITE(PositionalOffsetAdapterTests, PositionalOffsetAdapterTestsFixture)

BOOST_AUTO_TEST_CASE(IsCopyable)
{
    DerivedObject source(savedObjectManager);
    DerivedObjectOffset adapter(source);

    DerivedObjectOffset copyConstructor(adapter);
    DerivedObjectOffset copyAssignment;
    copyAssignment = adapter;

    BOOST_REQUIRE(copyConstructor.source == source);
    BOOST_REQUIRE(copyAssignment.source == source);
}

BOOST_AUTO_TEST_CASE(Serializes)
{
    DerivedObjectReference source = savedObjectManager.CreateObject<DerivedObject>();
    source->number = dataGeneration.Generator<int>().Random();
    DerivedObjectOffset adapter(source);

    {
        auto outputArchive = CreateRegistered<::Inscription::OutputBinaryArchive>();

        outputArchive(savedObjectManager);
        outputArchive(adapter);
    }

    ObjectManager n_objectManager = CreateRegistered<ObjectManager>();
    DerivedObjectOffset newAdapter;

    {
        auto inputArchive = CreateRegistered<::Inscription::InputBinaryArchive>();

        inputArchive(n_objectManager);
        inputArchive(newAdapter);
    }

    BOOST_REQUIRE(newAdapter.source->number == adapter.source->number);
}

BOOST_AUTO_TEST_SUITE_END()

namespace Inscription
{
    Scribe<PositionalOffsetAdapterTestsFixture::DerivedObject, BinaryArchive>::Table::Table()
    {
        AddDataEntry(DataEntry::Auto(&ObjectT::number, &DataT::number));
    };
}