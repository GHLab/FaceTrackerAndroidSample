#pragma once

#include <android/asset_manager.h>
#include <istream>
#include <streambuf>
#include <vector>

class AssetStreamBuf : public std::streambuf
{
public:
    AssetStreamBuf(AAssetManager* manager, const std::string& filename) : manager(manager)
    {
        asset = AAssetManager_open(manager, filename.c_str(), AASSET_MODE_STREAMING);
        buffer.resize(1024);

        setg(0, 0, 0);
        setp(&buffer.front(), &buffer.front() + buffer.size());
    }

    virtual ~AssetStreamBuf()
    {
        sync();
        AAsset_close(asset);
    }

    std::streambuf::int_type underflow() override
    {
        auto bufferPtr = &buffer.front();
        auto counter = AAsset_read(asset, bufferPtr, buffer.size());

        if(counter == 0)
            return traits_type::eof();
        if(counter < 0) //error, what to do now?
            return traits_type::eof();

        setg(bufferPtr, bufferPtr, bufferPtr + counter);

        return traits_type::to_int_type(*gptr());
    }

    std::streambuf::int_type overflow(std::streambuf::int_type value) override
    {
        return traits_type::eof();
    };

    int sync() override
    {
        std::streambuf::int_type result = overflow(traits_type::eof());

        return traits_type::eq_int_type(result, traits_type::eof()) ? -1 : 0;
    }

private:
    AAssetManager* manager;
    AAsset* asset;
    std::vector<char> buffer;
};

class AssetIstream : public std::istream
{
public:
    AssetIstream(AAssetManager* manager, const std::string& file);

    virtual ~AssetIstream()
    {
        delete rdbuf();
    }
};