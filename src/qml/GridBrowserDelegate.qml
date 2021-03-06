/*
 * Copyright 2016-2017 Matthieu Gallien <matthieu_gallien@yahoo.fr>
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
import QtQml.Models 2.1
import QtQuick.Layouts 1.2
import QtGraphicalEffects 1.0
import org.kde.kirigami 2.5 as Kirigami

FocusScope {
    id: gridEntry

    property var imageUrl
    property bool shadowForImage
    property alias mainText: mainLabel.text
    property alias secondaryText: secondaryLabel.text
    property var databaseId
    property bool delegateDisplaySecondaryText: true
    property bool isPartial

    signal enqueue(var databaseId, var name)
    signal replaceAndPlay(var databaseId, var name)
    signal open()
    signal selected()

    Keys.onReturnPressed: open()
    Keys.onEnterPressed: open()

    ColumnLayout {
        anchors.fill: parent

        spacing: 0

        MouseArea {
            id: hoverHandle

            hoverEnabled: true
            acceptedButtons: Qt.LeftButton

            Layout.preferredHeight: gridEntry.width * 0.85 + elisaTheme.layoutVerticalMargin * 0.5 +
                                    (mainLabelSize.boundingRect.height - mainLabelSize.boundingRect.y) +
                                    (secondaryLabelSize.boundingRect.height - secondaryLabelSize.boundingRect.y)
            Layout.fillWidth: true

            onClicked: {
                gridEntry.selected()
            }

            onDoubleClicked: open()

            TextMetrics {
                id: mainLabelSize
                font: mainLabel.font
                text: mainLabel.text
            }

            TextMetrics {
                id: secondaryLabelSize
                font: secondaryLabel.font
                text: secondaryLabel.text
            }

            ColumnLayout {
                id: mainData

                spacing: 0
                anchors.fill: parent

                Item {
                    Layout.preferredHeight: gridEntry.width * 0.85
                    Layout.preferredWidth: gridEntry.width * 0.85

                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter


                    Loader {
                        id: hoverLoader
                        active: false

                        anchors {
                            bottom: parent.bottom
                            bottomMargin: 2
                            left: parent.left
                            leftMargin: 2
                        }

                        z: 1

                        opacity: 0

                        sourceComponent: Row {
                            spacing: 2

                            Button {
                                id: replaceAndPlayButton
                                objectName: 'replaceAndPlayButton'

                                icon.name: 'media-playback-start'

                                hoverEnabled: true
                                ToolTip.visible: hovered
                                ToolTip.delay: 1000
                                ToolTip.text: i18nc("Clear play list and add whole container to play list", "Play now, replacing current playlist")

                                onClicked: replaceAndPlay(databaseId, mainText)
                                Keys.onReturnPressed: replaceAndPlay(databaseId, mainText)
                                Keys.onEnterPressed: replaceAndPlay(databaseId, mainText)


                                visible: databaseId !== undefined

                                width: elisaTheme.delegateToolButtonSize
                                height: elisaTheme.delegateToolButtonSize
                            }

                            Button {
                                id: enqueueButton
                                objectName: 'enqueueButton'

                                icon.name: 'media-track-add-amarok'
                                hoverEnabled: true
                                ToolTip.visible: hovered
                                ToolTip.delay: 1000
                                ToolTip.text: i18nc("Add whole container to play list", "Add to playlist")

                                onClicked: enqueue(databaseId, mainText)
                                Keys.onReturnPressed: enqueue(databaseId, mainText)
                                Keys.onEnterPressed: enqueue(databaseId, mainText)

                                visible: databaseId !== undefined

                                width: elisaTheme.delegateToolButtonSize
                                height: elisaTheme.delegateToolButtonSize
                            }

                            Button {
                                id: openButton
                                objectName: 'openButton'

                                icon.name: 'go-next-view-page'
                                hoverEnabled: true
                                ToolTip.visible: hovered
                                ToolTip.delay: 1000
                                ToolTip.text: i18nc("Open view of the container", "Open")

                                onClicked: open()

                                width: elisaTheme.delegateToolButtonSize
                                height: elisaTheme.delegateToolButtonSize
                            }
                        }
                    }

                    Loader {
                        id: coverImageLoader

                        active: !isPartial

                        anchors.fill: parent

                        sourceComponent: Image {
                            id: coverImage

                            anchors.fill: parent

                            sourceSize.width: parent.width
                            sourceSize.height: parent.height
                            fillMode: Image.PreserveAspectFit
                            smooth: true

                            source: (gridEntry.imageUrl !== undefined ? gridEntry.imageUrl : "")

                            asynchronous: true

                            layer.enabled: shadowForImage
                            layer.effect: DropShadow {
                                source: coverImage

                                radius: 10
                                spread: 0.1
                                samples: 21

                                color: myPalette.shadow
                            }
                        }
                    }
                    Loader {
                        active: isPartial

                        anchors.centerIn: parent
                        height: Kirigami.Units.gridUnit * 5
                        width: height


                        sourceComponent: BusyIndicator {
                            anchors.centerIn: parent

                            running: true
                        }
                    }
                }

                LabelWithToolTip {
                    id: mainLabel

                    font.weight: Font.Bold
                    color: myPalette.text

                    // FIXME: Center-aligned text looks better overall, but
                    // sometimes results in font kerning issues
                    // See https://bugreports.qt.io/browse/QTBUG-49646
                    horizontalAlignment: Text.AlignHCenter

                    Layout.topMargin: elisaTheme.layoutVerticalMargin * 0.5
                    Layout.maximumWidth: gridEntry.width * 0.9
                    Layout.minimumWidth: Layout.maximumWidth
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom

                    elide: Text.ElideRight
                }

                LabelWithToolTip {
                    id: secondaryLabel

                    font.weight: Font.Light
                    color: myPalette.text

                    // FIXME: Center-aligned text looks better overall, but
                    // sometimes results in font kerning issues
                    // See https://bugreports.qt.io/browse/QTBUG-49646
                    horizontalAlignment: Text.AlignHCenter

                    Layout.maximumWidth: gridEntry.width * 0.9
                    Layout.minimumWidth: Layout.maximumWidth
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom

                    visible: delegateDisplaySecondaryText

                    elide: Text.ElideRight
                }
            }
        }

        Item {
            Layout.fillHeight: true
        }
    }

    states: [
        State {
            name: 'notSelected'
            when: !gridEntry.activeFocus && !hoverHandle.containsMouse
            PropertyChanges {
                target: hoverLoader
                active: false
            }
            PropertyChanges {
                target: hoverLoader
                opacity: 0.0
            }
        },
        State {
            name: 'hoveredOrSelected'
            when: gridEntry.activeFocus || hoverHandle.containsMouse
            PropertyChanges {
                target: hoverLoader
                active: true
            }
            PropertyChanges {
                target: hoverLoader
                opacity: 1.0
            }
        }
    ]

    transitions: [
        Transition {
            to: 'hoveredOrSelected'
            SequentialAnimation {
                PropertyAction {
                    properties: "active"
                }
                NumberAnimation {
                    properties: "opacity"
                    easing.type: Easing.InOutQuad
                    duration: 100
                }
            }
        },
        Transition {
            to: 'notSelected'
            SequentialAnimation {
                NumberAnimation {
                    properties: "opacity"
                    easing.type: Easing.InOutQuad
                    duration: 100
                }
                PropertyAction {
                    properties: "active"
                }
            }
        }
    ]
}
