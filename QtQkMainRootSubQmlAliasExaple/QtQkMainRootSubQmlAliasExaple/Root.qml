import QtQuick 2.12
import QtQuick.Controls 2.0
import QtQuick.Window 2.12

Item
{
    id: idRoot
    visible: true
    width: 640
    height: 480

    property alias subChecked: idSub.subChecked

    property bool rootB: false

    Sub
    {
        id: idSub

        onTest:
        {
            idRoot.rootB = b
        }
    }
}
