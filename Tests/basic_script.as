string main(Arca::RelicID boundsID)
{
    auto bounds = Atmos::Spatial::Bounds(boundsID);
    return Atmos::ToString(bounds.Size().width) + " " + Atmos::ToString(bounds.Size().height);
}