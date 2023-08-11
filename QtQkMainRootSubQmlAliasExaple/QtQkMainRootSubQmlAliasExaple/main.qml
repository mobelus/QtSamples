import QtQuick 2.12
import QtQuick.Controls 2.0
import QtQuick.Window 2.12

Window
{
    id: root
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    signal write()

    property bool check: false

    Component.onCompleted: {
        idRoot.subChecked = root.check;
        console.log("COMPLETE CHECK     = " + root.check)
        console.log("COMPLETE SUB_CHECK = " + idRoot.subChecked)
    }

    Root
    {
        id: idRoot
        onSubCheckedChanged: {
            root.write();
        }
    }

    onWrite:
    {
        root.check = idRoot.subChecked
        console.log("-------- CHECK     = " + root.check)
        console.log("-------- SUB_CHECK = " + idRoot.subChecked)

        console.log("-------- ROOT_BBBB = " + idRoot.rootB)
    }

}



























