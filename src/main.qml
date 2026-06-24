import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ApplicationWindow {
    visible: true
    width: 800
    height: 480
    title: "Laundry Kiosk"

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: HomePage { }
    }

    Component {
        id: HomePage
        Rectangle {
            width: parent.width
            height: parent.height

            ColumnLayout {
                anchors.centerIn: parent
                spacing: 20

                Text {
                    text: "Home Dashboard"
                    font.pointSize: 24
                    Layout.alignment: Qt.AlignCenter
                }

                Button {
                    text: "Go to Machine Detail"
                    onClicked: stackView.push(MachineDetailPage { })
                }
            }
        }
    }

    Component {
        id: MachineDetailPage
        Rectangle {
            width: parent.width
            height: parent.height

            ColumnLayout {
                anchors.centerIn: parent
                spacing: 20

                Text {
                    text: "Machine Detail"
                    font.pointSize: 24
                    Layout.alignment: Qt.AlignCenter
                }

                Button {
                    text: "Go to Payment"
                    onClicked: stackView.push(PaymentPage { })
                }
            }
        }
    }

    Component {
        id: PaymentPage
        Rectangle {
            width: parent.width
            height: parent.height

            ColumnLayout {
                anchors.centerIn: parent
                spacing: 20

                Text {
                    text: "Payment"
                    font.pointSize: 24
                    Layout.alignment: Qt.AlignCenter
                }

                Button {
                    text: "Go to Running Status"
                    onClicked: stackView.push(RunningStatusPage { })
                }
            }
        }
    }

    Component {
        id: RunningStatusPage
        Rectangle {
            width: parent.width
            height: parent.height

            ColumnLayout {
                anchors.centerIn: parent
                spacing: 20

                Text {
                    text: "Running Status"
                    font.pointSize: 24
                    Layout.alignment: Qt.AlignCenter
                }

                Button {
                    text: "Go to Home Dashboard"
                    onClicked: stackView.pop()
                }
            }
        }
    }
}
