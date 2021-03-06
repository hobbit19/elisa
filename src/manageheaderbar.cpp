/*
 * Copyright 2016 Matthieu Gallien <matthieu_gallien@yahoo.fr>
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "manageheaderbar.h"

#include <QTime>
#include <QTimer>
#include <QDebug>

#include <cstdlib>

ManageHeaderBar::ManageHeaderBar(QObject *parent)
    : QObject(parent)
{
}

void ManageHeaderBar::setArtistRole(int value)
{
    mArtistRole = value;
    Q_EMIT artistRoleChanged();
}

int ManageHeaderBar::artistRole() const
{
    return mArtistRole;
}

void ManageHeaderBar::setTitleRole(int value)
{
    mTitleRole = value;
    Q_EMIT titleRoleChanged();
}

int ManageHeaderBar::titleRole() const
{
    return mTitleRole;
}

void ManageHeaderBar::setAlbumRole(int value)
{
    mAlbumRole = value;
    Q_EMIT albumRoleChanged();
}

void ManageHeaderBar::setAlbumArtistRole(int value)
{
    mAlbumArtistRole = value;
    Q_EMIT albumArtistRoleChanged();
}

void ManageHeaderBar::setFileNameRole(int value)
{
    mFileNameRole = value;
    Q_EMIT fileNameRoleChanged();
}

int ManageHeaderBar::albumRole() const
{
    return mAlbumRole;
}

int ManageHeaderBar::albumArtistRole() const
{
    return mAlbumArtistRole;
}

int ManageHeaderBar::fileNameRole() const
{
    return mFileNameRole;
}

void ManageHeaderBar::setImageRole(int value)
{
    mImageRole = value;
    Q_EMIT imageRoleChanged();
}

void ManageHeaderBar::setDatabaseIdRole(int databaseIdRole)
{
    mDatabaseIdRole = databaseIdRole;
    Q_EMIT databaseIdRoleChanged();
}

int ManageHeaderBar::imageRole() const
{
    return mImageRole;
}

int ManageHeaderBar::databaseIdRole() const
{
    return mDatabaseIdRole;
}

void ManageHeaderBar::setAlbumIdRole(int albumIdRole)
{
    mAlbumIdRole = albumIdRole;
    Q_EMIT albumIdRoleChanged();
}

int ManageHeaderBar::albumIdRole() const
{
    return mAlbumIdRole;
}

QVariant ManageHeaderBar::album() const
{
    if (!mCurrentTrack.isValid()) {
        return QString();
    }

    return mCurrentTrack.data(mAlbumRole);
}

QVariant ManageHeaderBar::albumArtist() const
{
    if (!mCurrentTrack.isValid()) {
        return QString();
    }

    return mCurrentTrack.data(mAlbumArtistRole);
}

QString ManageHeaderBar::fileName() const
{
    QString result;

    if (!mCurrentTrack.isValid()) {
        return result;
    }

    auto fileNameUrl = mCurrentTrack.data(mFileNameRole).toUrl();
    if (fileNameUrl.isLocalFile()) {
        result = fileNameUrl.toLocalFile();
    } else {
        result = fileNameUrl.toString();
    }

    return result;
}

QVariant ManageHeaderBar::title() const
{
    if (!mCurrentTrack.isValid()) {
        return QString();
    }

    return mCurrentTrack.data(mTitleRole);
}

QVariant ManageHeaderBar::artist() const
{
    if (!mCurrentTrack.isValid()) {
        return QString();
    }

    return mCurrentTrack.data(mArtistRole);
}

QUrl ManageHeaderBar::image() const
{
    if (!mCurrentTrack.isValid()) {
        return {};
    }

    return mCurrentTrack.data(mImageRole).toUrl();
}

qulonglong ManageHeaderBar::databaseId() const
{
    if (!mCurrentTrack.isValid()) {
        return 0;
    }

    return mCurrentTrack.data(mDatabaseIdRole).toULongLong();
}

qulonglong ManageHeaderBar::albumId() const
{
    if (!mCurrentTrack.isValid()) {
        return 0;
    }

    return mCurrentTrack.data(mAlbumIdRole).toULongLong();
}

bool ManageHeaderBar::isValid() const
{
    if (!mCurrentTrack.isValid()) {
        return false;
    }

    return mCurrentTrack.data(mIsValidRole).toBool();
}

int ManageHeaderBar::remainingTracks() const
{
    if (!mCurrentTrack.isValid()) {
        return 0;
    }

    return mPlayListModel->rowCount(mCurrentTrack.parent()) - mCurrentTrack.row() - 1;
}

int ManageHeaderBar::isValidRole() const
{
    return mIsValidRole;
}

QPersistentModelIndex ManageHeaderBar::currentTrack() const
{
    return mCurrentTrack;
}

void ManageHeaderBar::playListLayoutChanged(const QList<QPersistentModelIndex> &parents, QAbstractItemModel::LayoutChangeHint hint)
{
    Q_UNUSED(parents);
    Q_UNUSED(hint);

    qDebug() << "ManageHeaderBar::playListLayoutChanged" << "not implemented";
}

void ManageHeaderBar::tracksInserted(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent);
    Q_UNUSED(first);
    Q_UNUSED(last);

    if (!mCurrentTrack.isValid()) {
        return;
    }

    if (mCurrentTrack.row() >= first) {
        return;
    }

    Q_EMIT remainingTracksChanged();
    mOldRemainingTracks = remainingTracks();
}

void ManageHeaderBar::tracksDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
    if (!mCurrentTrack.isValid()) {
        return;
    }

    if (mCurrentTrack.row() > bottomRight.row() || mCurrentTrack.row() < topLeft.row()) {
        return;
    }

    if (mCurrentTrack.column() > bottomRight.column() || mCurrentTrack.column() < topLeft.column()) {
        return;
    }

    if (roles.isEmpty()) {
        notifyArtistProperty();
        notifyTitleProperty();
        notifyAlbumProperty();
        notifyAlbumArtistProperty();
        notifyFileNameProperty();
        notifyImageProperty();
        notifyDatabaseIdProperty();
        notifyAlbumIdProperty();
        notifyIsValidProperty();
    } else {
        for(auto oneRole : roles) {
            if (oneRole == mArtistRole) {
                notifyArtistProperty();
            }
            if (oneRole == mTitleRole) {
                notifyTitleProperty();
            }
            if (oneRole == mAlbumRole) {
                notifyAlbumProperty();
            }
            if (oneRole == mAlbumArtistRole) {
                notifyAlbumArtistProperty();
            }
            if (oneRole == mFileNameRole) {
                notifyFileNameProperty();
            }
            if (oneRole == mImageRole) {
                notifyImageProperty();
            }
            if (oneRole == mDatabaseIdRole) {
                notifyDatabaseIdProperty();
            }
            if (oneRole == mAlbumIdRole) {
                notifyAlbumIdProperty();
            }
            if (oneRole == mIsValidRole) {
                notifyIsValidProperty();
            }
        }
    }
}

void ManageHeaderBar::tracksAboutToBeMoved(const QModelIndex &parent, int start, int end, const QModelIndex &destination, int row)
{
    Q_UNUSED(parent);
    Q_UNUSED(start);
    Q_UNUSED(end);
    Q_UNUSED(destination);
    Q_UNUSED(row);

    mOldRemainingTracks = remainingTracks();
}

void ManageHeaderBar::tracksMoved(const QModelIndex &parent, int start, int end, const QModelIndex &destination, int row)
{
    Q_UNUSED(parent);
    Q_UNUSED(start);
    Q_UNUSED(end);
    Q_UNUSED(destination);
    Q_UNUSED(row);

    auto newRemainingTracks = remainingTracks();
    if (mOldRemainingTracks != newRemainingTracks) {
        Q_EMIT remainingTracksChanged();
    }
}

void ManageHeaderBar::tracksRemoved(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent);
    Q_UNUSED(first);
    Q_UNUSED(last);

    if (!mCurrentTrack.isValid()) {
        notifyArtistProperty();
        notifyTitleProperty();
        notifyAlbumProperty();
        notifyAlbumArtistProperty();
        notifyFileNameProperty();
        notifyImageProperty();
        notifyDatabaseIdProperty();
        notifyAlbumIdProperty();
        notifyIsValidProperty();
        notifyRemainingTracksProperty();

        return;
    }

    notifyRemainingTracksProperty();
}

void ManageHeaderBar::notifyArtistProperty()
{
    auto newArtistValue = mCurrentTrack.data(mArtistRole);
    if (mOldArtist != newArtistValue) {
        Q_EMIT artistChanged();

        mOldArtist = newArtistValue;
    }
}

void ManageHeaderBar::notifyTitleProperty()
{
    auto newTitleValue = mCurrentTrack.data(mTitleRole);
    if (mOldTitle != newTitleValue) {
        Q_EMIT titleChanged();

        mOldTitle = newTitleValue;
    }
}

void ManageHeaderBar::notifyAlbumProperty()
{
    auto newAlbumValue = mCurrentTrack.data(mAlbumRole);
    if (mOldAlbum != newAlbumValue) {
        Q_EMIT albumChanged();

        mOldAlbum = newAlbumValue;
    }
}

void ManageHeaderBar::notifyAlbumArtistProperty()
{
    auto newAlbumArtistValue = mCurrentTrack.data(mAlbumArtistRole);
    if (mOldAlbumArtist != newAlbumArtistValue) {
        Q_EMIT albumArtistChanged();

        mOldAlbumArtist = newAlbumArtistValue;
    }
}

void ManageHeaderBar::notifyFileNameProperty()
{
    auto newFileNameValue = mCurrentTrack.data(mFileNameRole);
    if (mOldFileName != newFileNameValue) {
        Q_EMIT fileNameChanged();

        mOldFileName = newFileNameValue;
    }
}

void ManageHeaderBar::notifyImageProperty()
{
    auto newImageValue = mCurrentTrack.data(mImageRole);
    if (mOldImage != newImageValue) {
        Q_EMIT imageChanged();

        mOldImage = newImageValue;
    }
}

void ManageHeaderBar::notifyDatabaseIdProperty()
{
    bool conversionOk;
    auto newDatabaseIdValue = mCurrentTrack.data(mDatabaseIdRole).toULongLong(&conversionOk);
    if (conversionOk && mOldDatabaseId != newDatabaseIdValue) {
        Q_EMIT databaseIdChanged();

        mOldDatabaseId = newDatabaseIdValue;
    } else if (!conversionOk && mOldDatabaseId != 0) {
        Q_EMIT databaseIdChanged();

        mOldDatabaseId = 0;
    }
}

void ManageHeaderBar::notifyAlbumIdProperty()
{
    bool conversionOk;
    auto newAlbumIdValue = mCurrentTrack.data(mAlbumIdRole).toULongLong(&conversionOk);
    if (conversionOk && mOldAlbumId != newAlbumIdValue) {
        Q_EMIT albumIdChanged();

        mOldAlbumId = newAlbumIdValue;
    } else if (!conversionOk && mOldAlbumId != 0) {
        Q_EMIT albumIdChanged();

        mOldAlbumId = 0;
    }
}

void ManageHeaderBar::notifyIsValidProperty()
{
    auto newIsValidValue = mCurrentTrack.data(mIsValidRole).toBool();
    if (mOldIsValid != newIsValidValue) {
        Q_EMIT isValidChanged();

        mOldIsValid = newIsValidValue;
    }
}

void ManageHeaderBar::notifyRemainingTracksProperty()
{
    auto newRemainingTracksValue = remainingTracks();
    if (mOldRemainingTracks != newRemainingTracksValue) {
        Q_EMIT remainingTracksChanged();

        mOldRemainingTracks = newRemainingTracksValue;
    }
}

void ManageHeaderBar::setIsValidRole(int isValidRole)
{
    mIsValidRole = isValidRole;
    emit isValidRoleChanged();
}

void ManageHeaderBar::setCurrentTrack(const QPersistentModelIndex &currentTrack)
{
    if (mCurrentTrack == currentTrack) {
        return;
    }

    auto oldRemainingTracksCount = remainingTracks();

    mCurrentTrack = currentTrack;
    Q_EMIT currentTrackChanged();

    if (mCurrentTrack.isValid() && oldRemainingTracksCount != remainingTracks()) {
        Q_EMIT remainingTracksChanged();
        mOldRemainingTracks = remainingTracks();
    }

    notifyArtistProperty();
    notifyTitleProperty();
    notifyAlbumProperty();
    notifyAlbumArtistProperty();
    notifyFileNameProperty();
    notifyImageProperty();
    notifyDatabaseIdProperty();
    notifyAlbumIdProperty();
    notifyIsValidProperty();
}

void ManageHeaderBar::setPlayListModel(QAbstractItemModel *aPlayListModel)
{
    if (mPlayListModel) {
        disconnect(mPlayListModel, &QAbstractItemModel::rowsInserted, this, &ManageHeaderBar::tracksInserted);
        disconnect(mPlayListModel, &QAbstractItemModel::rowsAboutToBeMoved, this, &ManageHeaderBar::tracksAboutToBeMoved);
        disconnect(mPlayListModel, &QAbstractItemModel::rowsMoved, this, &ManageHeaderBar::tracksMoved);
        disconnect(mPlayListModel, &QAbstractItemModel::rowsRemoved, this, &ManageHeaderBar::tracksRemoved);
        disconnect(mPlayListModel, &QAbstractItemModel::dataChanged, this, &ManageHeaderBar::tracksDataChanged);
        disconnect(mPlayListModel, &QAbstractItemModel::layoutChanged, this, &ManageHeaderBar::playListLayoutChanged);
    }

    mPlayListModel = aPlayListModel;

    if (mPlayListModel) {
        connect(mPlayListModel, &QAbstractItemModel::rowsInserted, this, &ManageHeaderBar::tracksInserted);
        connect(mPlayListModel, &QAbstractItemModel::rowsAboutToBeMoved, this, &ManageHeaderBar::tracksAboutToBeMoved);
        connect(mPlayListModel, &QAbstractItemModel::rowsMoved, this, &ManageHeaderBar::tracksMoved);
        connect(mPlayListModel, &QAbstractItemModel::rowsRemoved, this, &ManageHeaderBar::tracksRemoved);
        connect(mPlayListModel, &QAbstractItemModel::dataChanged, this, &ManageHeaderBar::tracksDataChanged);
        connect(mPlayListModel, &QAbstractItemModel::layoutChanged, this, &ManageHeaderBar::playListLayoutChanged);
    }

    Q_EMIT playListModelChanged();
}

QAbstractItemModel *ManageHeaderBar::playListModel() const
{
    return mPlayListModel;
}


#include "moc_manageheaderbar.cpp"
