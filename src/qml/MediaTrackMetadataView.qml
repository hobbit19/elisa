/*
 * Copyright 2017 Alexander Stippich <a.stippich@gmx.net>
 * Copyright 2018 Matthieu Gallien <matthieu_gallien@yahoo.fr>
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

import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Window 2.2
import QtQml.Models 2.2
import QtQuick.Layouts 1.2
import QtGraphicalEffects 1.0
import org.kde.elisa 1.0

Window {
    id: trackMetadata

    property int databaseId: 0
    property url fileName

    signal rejected()

    LayoutMirroring.enabled: Qt.application.layoutDirection == Qt.RightToLeft
    LayoutMirroring.childrenInherit: true

    title: i18nc("Window title for track metadata", "View Details")

    TrackMetadataModel {
        id: realModel

        manager: elisa.musicManager
    }

    modality: Qt.NonModal
    flags: Qt.Dialog | Qt.WindowCloseButtonHint

    color: myPalette.window

    minimumHeight: elisaTheme.coverImageSize * 1.8
    minimumWidth: elisaTheme.coverImageSize * 2.8

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: elisaTheme.layoutVerticalMargin

        spacing: elisaTheme.layoutVerticalMargin

        RowLayout {
            id: metadataView

            Layout.fillHeight: true
            Layout.fillWidth: true

            spacing: 0

            Image {
                source: (realModel.coverUrl !== "" ? realModel.coverUrl : elisaTheme.tracksIcon)

                sourceSize.width: elisaTheme.coverImageSize
                sourceSize.height: elisaTheme.coverImageSize

                fillMode: Image.PreserveAspectFit

                Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                Layout.preferredHeight: elisaTheme.coverImageSize
                Layout.preferredWidth: elisaTheme.coverImageSize
                Layout.minimumHeight: elisaTheme.coverImageSize
                Layout.minimumWidth: elisaTheme.coverImageSize
                Layout.maximumHeight: elisaTheme.coverImageSize
                Layout.maximumWidth: elisaTheme.coverImageSize
            }

            ListView {
                id: trackData

                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.leftMargin: 2 * elisaTheme.layoutHorizontalMargin

                focus: true

                ScrollBar.vertical: ScrollBar {
                    id: scrollBar
                }
                boundsBehavior: Flickable.StopAtBounds
                clip: true

                ScrollHelper {
                    id: scrollHelper
                    flickable: trackData
                    anchors.fill: trackData
                }
                model: realModel

                delegate: MetaDataDelegate {
                    width: scrollBar.visible ? (!LayoutMirroring.enabled ? trackData.width - scrollBar.width : trackData.width) : trackData.width
                }
            }
        }

        RowLayout {
            Layout.alignment: Qt.AlignLeft | Qt.AlignBottom
            Layout.topMargin: elisaTheme.layoutVerticalMargin
            Layout.bottomMargin: elisaTheme.layoutVerticalMargin

            spacing: elisaTheme.layoutHorizontalMargin

            Image {
                Layout.preferredWidth: fileNameLabel.height
                Layout.preferredHeight: fileNameLabel.height

                sourceSize.width: fileNameLabel.height
                sourceSize.height: fileNameLabel.height

                source: elisaTheme.folderIcon
            }

            LabelWithToolTip {
                id: fileNameLabel

                Layout.fillWidth: true

                text: realModel.fileUrl

                elide: Text.ElideRight
            }
        }

        DialogButtonBox {
            id: buttons

            Layout.fillWidth: true
            Layout.minimumHeight: implicitHeight

            standardButtons: DialogButtonBox.Close
            alignment: Qt.AlignRight
            onRejected: trackMetadata.rejected()
        }
    }

    Connections {
        target: elisa

        onMusicManagerChanged: {
            if (databaseId !== 0) {
                realModel.initializeByTrackId(databaseId)
            } else {
                realModel.initializeByTrackFileName(fileName)
            }
        }
    }

    Component.onCompleted: {
        if (elisa.musicManager) {
            if (databaseId !== 0) {
                realModel.initializeByTrackId(databaseId)
            } else {
                realModel.initializeByTrackFileName(fileName)
            }
        }
    }
}
