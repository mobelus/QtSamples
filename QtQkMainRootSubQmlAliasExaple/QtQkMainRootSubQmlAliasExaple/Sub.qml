import QtQuick 2.12
import QtQuick.Controls 2.0
import QtQuick.Window 2.12

Item
{
    id: idSub
    visible: true
    width: 640
    height: 480

    signal test(bool b)

    property alias subChecked: idCheck.checked

    CheckBox
    {
        id: idCheck

        onCheckedChanged:
        {
            //idSub.test(false);
            idSub.test(!idCheck.checked);
        }
    }
}
