import QtQuick 2.4

EosForm {
    button.onClicked: {
        textT.text = "647.096";
        textP.text = "2206400000";
        textA.text = "0.14266640249924298";
        textB.text = "2.112696413685528e-07";
        state = "";
        labelv1.text = "";
        labelv2.text = "";
        labelv3.text = "";
    }
    button1.onClicked: {
        var T = parseFloat(textT.text);
        var P = parseFloat(textP.text);
        var a = parseFloat(textA.text);
        var b = parseFloat(textB.text);

        var solutions = eoscalculator.calculateV(T, P, a, b);

        labelv1.text = solutions[0].toString();
        state = "state_one_solution";
        if(solutions.length > 1){
            labelv2.text = solutions[1].toString();
            labelv3.text = solutions[2].toString();
            state = "";
        }
    }

    transitions: Transition {
        PropertyAnimation{
            property: "opacity"
            duration: 500 //pola sekunde = 500ms
        }
    }
}
