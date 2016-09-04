/*
 * Copyright 2016 Matthieu Gallien <matthieu_gallien@yahoo.fr>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "musicalbum.h"

class MusicAlbumPrivate
{
public:

    qulonglong mDatabaseId = 0;

    QString mId;

    QString mParentId;

    QString mTitle;

    QString mArtist;

    QUrl mAlbumArtURI;

    QUrl mResourceURI;

    QMap<qulonglong, MusicAudioTrack> mTracks;

    QList<qulonglong> mTrackIds;

    int mTracksCount = 0;

    bool mIsValid = false;

};

MusicAlbum::MusicAlbum() : d(new MusicAlbumPrivate)
{
}

MusicAlbum::MusicAlbum(MusicAlbum &&other) : d(other.d)
{
    other.d = nullptr;
}

MusicAlbum::MusicAlbum(const MusicAlbum &other) : d(new MusicAlbumPrivate(*other.d))
{
}

MusicAlbum& MusicAlbum::operator=(MusicAlbum &&other)
{
    if (&other != this) {
        delete d;
        d = other.d;
        other.d = nullptr;
    }

    return *this;
}

MusicAlbum& MusicAlbum::operator=(const MusicAlbum &other)
{
    if (&other != this) {
        (*d) = (*other.d);
    }

    return *this;
}

MusicAlbum::~MusicAlbum()
{
    delete d;
}

void MusicAlbum::setValid(bool value)
{
    d->mIsValid = value;
}

bool MusicAlbum::isValid() const
{
    return d->mIsValid;
}

void MusicAlbum::setDatabaseId(qulonglong value)
{
    d->mDatabaseId = value;
}

qulonglong MusicAlbum::databaseId() const
{
    return d->mDatabaseId;
}

void MusicAlbum::setId(const QString &value)
{
    d->mId = value;
}

QString MusicAlbum::id() const
{
    return d->mId;
}

void MusicAlbum::setParentId(const QString &value)
{
    d->mParentId = value;
}

QString MusicAlbum::parentId() const
{
    return d->mParentId;
}

void MusicAlbum::setTracksCount(int count)
{
    d->mTracksCount = count;
}

int MusicAlbum::tracksCount() const
{
    const auto realTracksCount = d->mTracks.size();
    return (realTracksCount == 0 ? d->mTracksCount : realTracksCount);
}

void MusicAlbum::setTitle(const QString &value)
{
    d->mTitle = value;
}

QString MusicAlbum::title() const
{
    return d->mTitle;
}

void MusicAlbum::setArtist(const QString &value)
{
    d->mArtist = value;
}

QString MusicAlbum::artist() const
{
    return d->mArtist;
}

void MusicAlbum::setAlbumArtURI(const QUrl &value)
{
    d->mAlbumArtURI = value;
}

QUrl MusicAlbum::albumArtURI() const
{
    return d->mAlbumArtURI;
}

void MusicAlbum::setResourceURI(const QUrl &value)
{
    d->mResourceURI = value;
}

QUrl MusicAlbum::resourceURI() const
{
    return d->mResourceURI;
}

void MusicAlbum::setTracks(const QMap<qulonglong, MusicAudioTrack> &allTracks)
{
    d->mTracks = allTracks;
}

QList<qulonglong> MusicAlbum::tracksKeys() const
{
    return d->mTracks.keys();
}

MusicAudioTrack MusicAlbum::trackFromIndex(int index) const
{
    return d->mTracks[d->mTrackIds[index]];
}

void MusicAlbum::setTrackIds(const QList<qulonglong> &allTracksIds)
{
    d->mTrackIds = allTracksIds;
}

qulonglong MusicAlbum::trackIdFromIndex(int index) const
{
    return d->mTrackIds[index];
}

