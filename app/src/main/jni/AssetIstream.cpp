#include "AssetIstream.h"

AssetIstream::AssetIstream(AAssetManager* manager, const std::string& file) : std::istream(new AssetStreamBuf(manager, file))
{
}