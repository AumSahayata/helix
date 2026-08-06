#include "MusicLibrary.h"

MusicLibrary::MusicLibrary(Storage &storage)
    : storage(storage)
{
}

bool MusicLibrary::load()
{
    if (loaded_)
    {
        return true;
    }

    tracks_.clear();

    if (!storage.listFilesRecursive("/Music", tracks_))
    {
        return false;
    }

    loaded_ = true;

    return true;
}

const std::vector<String> &MusicLibrary::tracks() const
{
    return tracks_;
}

bool MusicLibrary::isLoaded() const
{
    return loaded_;
}