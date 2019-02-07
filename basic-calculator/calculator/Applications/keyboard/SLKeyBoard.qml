import QtQuick 2.9
import QtQuick.Controls 2.2
import SLKeyCode 1.0

SLKeyBoardForm {
    id: keyboard
    property string state : "normal"
    property bool lockedState : false

    signal activated(int key)

    function switchState(entered) /* string * string -> unit */
    {
        if (keyboard.state == entered) keyboard.state = "normal";
        else keyboard.state = entered;
    }

    function classicalSend(principal, snd, alpha) /* keycode * keycode * keycode -> unit */
    {
        if (keyboard.state == "normal") activated(principal);
        else if (keyboard.state == "snd") activated(snd);
        else if (keyboard.state == "alpha") activated(alpha);

        switchState("normal");
    }


    /* Arrows */
    buttonUp.onActivated: activated(KeyCode.SLK_UP)
    buttonDown.onActivated: activated(KeyCode.SLK_DOWN)
    buttonLeft.onActivated: activated(KeyCode.SLK_LEFT)
    buttonRight.onActivated: activated(KeyCode.SLK_RIGHT)

    /* Special Buttons */
    buttonSND.onActivated: { activated(KeyCode.SLK_SND); switchState("snd"); }
    buttonALPHA.onActivated: { activated(KeyCode.SLK_ALPHA); switchState("alpha") }
    buttonEXE.onActivated: { activated(KeyCode.SLK_EXE); switchState("normal"); }
    buttonDEL.onActivated: { activated(KeyCode.SLK_DEL); switchState("normal"); }
    buttonCLEAR.onActivated: { activated(KeyCode.SLK_CLEAR); switchState("normal"); }

    /* Classical buttons : alphabet */
    buttonA.onActivated: classicalSend(KeyCode.SLK_NULL, KeyCode.SLK_NULL, KeyCode.SLK_A)
    buttonB.onActivated: classicalSend(KeyCode.SLK_NULL, KeyCode.SLK_NULL, KeyCode.SLK_B)
    buttonC.onActivated: classicalSend(KeyCode.SLK_NULL, KeyCode.SLK_NULL, KeyCode.SLK_C)
    buttonD.onActivated: classicalSend(KeyCode.SLK_NULL, KeyCode.SLK_NULL, KeyCode.SLK_D)
    buttonE.onActivated: classicalSend(KeyCode.SLK_NULL, KeyCode.SLK_NULL, KeyCode.SLK_E)
    buttonF.onActivated: classicalSend(KeyCode.SLK_NULL, KeyCode.SLK_NULL, KeyCode.SLK_F)
    buttonG.onActivated: classicalSend(KeyCode.SLK_LPAREN, KeyCode.SLK_NULL, KeyCode.SLK_G)
    buttonH.onActivated: classicalSend(KeyCode.SLK_RPAREN, KeyCode.SLK_NULL, KeyCode.SLK_H)
    buttonI.onActivated: classicalSend(KeyCode.SLK_SQUAREROOT, KeyCode.SLK_NULL, KeyCode.SLK_I)
    buttonJ.onActivated: classicalSend(KeyCode.SLK_NULL, KeyCode.SLK_NULL, KeyCode.SLK_J)
    buttonK.onActivated: classicalSend(KeyCode.SLK_7, KeyCode.SLK_NULL, KeyCode.SLK_K)
    buttonL.onActivated: classicalSend(KeyCode.SLK_8, KeyCode.SLK_NULL, KeyCode.SLK_L)
    buttonM.onActivated: classicalSend(KeyCode.SLK_9, KeyCode.SLK_NULL, KeyCode.SLK_M)
    buttonN.onActivated: classicalSend(KeyCode.SLK_4, KeyCode.SLK_NULL, KeyCode.SLK_N)
    buttonO.onActivated: classicalSend(KeyCode.SLK_5, KeyCode.SLK_NULL, KeyCode.SLK_O)
    buttonP.onActivated: classicalSend(KeyCode.SLK_6, KeyCode.SLK_NULL, KeyCode.SLK_P)
    buttonQ.onActivated: classicalSend(KeyCode.SLK_TIMES, KeyCode.SLK_POWER, KeyCode.SLK_Q)
    buttonR.onActivated: classicalSend(KeyCode.SLK_DIVIDE, KeyCode.SLK_NULL, KeyCode.SLK_R)
    buttonS.onActivated: classicalSend(KeyCode.SLK_1, KeyCode.SLK_NULL, KeyCode.SLK_S)
    buttonT.onActivated: classicalSend(KeyCode.SLK_2, KeyCode.SLK_NULL, KeyCode.SLK_T)
    buttonU.onActivated: classicalSend(KeyCode.SLK_3, KeyCode.SLK_NULL, KeyCode.SLK_U)
    buttonV.onActivated: classicalSend(KeyCode.SLK_PLUS, KeyCode.SLK_NULL, KeyCode.SLK_V)
    buttonW.onActivated: classicalSend(KeyCode.SLK_MINUS, KeyCode.SLK_NULL, KeyCode.SLK_W)
    buttonX.onActivated: classicalSend(KeyCode.SLK_NULL, KeyCode.SLK_NULL, KeyCode.SLK_X)
    buttonY.onActivated: classicalSend(KeyCode.SLK_0, KeyCode.SLK_NULL, KeyCode.SLK_Y)
    buttonZ.onActivated: classicalSend(KeyCode.SLK_NULL, KeyCode.SLK_NULL, KeyCode.SLK_Z)
}
